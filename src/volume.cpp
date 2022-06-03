#include "volume.hpp"

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>

#include <limits>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <stack>

#include <glm/vec3.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "miniz.h"

namespace {
    enum class MHDType {
        unknown,
        u8,
        s16
    };
    
    struct MHDInfo {
        bool typeIsImage     = false;
        bool dataIsBinary    = false;
        bool dataCompressed  = false;

        int ndims            = -1;
        int x = -1, y = -1, z = -1;

        MHDType elementType  = MHDType::unknown;

        std::string dataFile;
    };


    template<typename tElementType>
    Volume load_data_raw_(FILE*);
    template<typename tElementType>
    Volume load_data_compressed_(FILE*);


    // This is a helper class + function for ON_SCOPE_EXIT().
    template<class tFunc>
    struct OnScopeExit_ {
        OnScopeExit_(tFunc&& aFunc) : func(std::move(aFunc)) {}
        ~OnScopeExit_() { func(); }
        private: tFunc func;
    };

    template<class tFunc>
    OnScopeExit_<tFunc> make_scope_exit(tFunc&& aFunc) {
        return OnScopeExit_<tFunc>{ std::forward<tFunc>(aFunc) };
    };

    // ON_SCOPE_EXIT(): helper to safetly clean up stuff.
#    define ON_SCOPE_EXIT(fn) auto CONCAT_(scopeExitGuard,__LINE__) = make_scope_exit(fn)

#    define CONCAT_(a,b) CONCAT0_(a,b)
#    define CONCAT0_(a,b) a##b
}

namespace {
    template<typename tType> inline
    Volume load_data_raw_(FILE* aFile, MHDInfo const& aInfo) {
        std::vector<tType> buffer(aInfo.x*aInfo.y*aInfo.z);
        auto rd = std::fread(buffer.data(), sizeof(tType), buffer.size(), aFile);
        if(rd != buffer.size())
            throw std::runtime_error("Could not read volume data");
    
        Volume ret(aInfo.x, aInfo.y, aInfo.z);

        float minf = float(std::numeric_limits<tType>::max());
        float maxf = float(std::numeric_limits<tType>::min());
        for(auto const elem : buffer) {
            float const elemf = float(elem);
            if(elemf > maxf) maxf = elemf;
            else if(elemf < minf) minf = elemf;
        }
        
        std::transform( buffer.begin(), buffer.end(), ret.data(), [&] (tType aValue) {
            return (aValue-minf) / (maxf-minf);
        } );

        ret.initialize();
        return ret;
    }

    template<typename tType> inline
    Volume load_data_compressed_(FILE* aFile, MHDInfo const& aInfo) {
        // Figure out size of compressed data
        std::fseek(aFile, 0, SEEK_END);
        auto const size = std::ftell(aFile);
        std::fseek(aFile, 0, SEEK_SET);

        // Read compressed data
        std::vector<std::uint8_t> compressed(size);
        auto rd = std::fread(compressed.data(), 1, size, aFile);
        if(long(rd) != size)
            throw std::runtime_error("Could not read compressed volume data");
        
        // Decompress using the "miniz" library.
        std::vector<tType> buffer(aInfo.x*aInfo.y*aInfo.z);

        mz_stream mzs{};
        mzs.avail_in = compressed.size();
        mzs.next_in = compressed.data();
        mzs.avail_out = buffer.size()*sizeof(tType);
        mzs.next_out = reinterpret_cast<std::uint8_t*>(buffer.data());

        auto iret = mz_inflateInit(&mzs);
        if(MZ_OK != iret) {
            std::ostringstream oss;
            oss << "miniz: could not initialize decompressor: " << mz_error(iret);
            throw std::runtime_error(oss.str());
        }

        auto jret = mz_inflate(&mzs, MZ_FINISH);

        mz_inflateEnd(&mzs);
        
        if(MZ_OK != jret) {
            if(MZ_STREAM_END == jret && mzs.total_out == buffer.size()*sizeof(tType)) {
                /* OK, seems like the some of the compressed streams aren't
                 * properly finalized or something. So, if the error is
                 * MZ_STREAM_END but we've got all our data, we're going to
                 * YOLO it and ignore the error.
                 */
            }
            else {
                std::ostringstream oss;
                oss << "miniz: decompression failed: " << mz_error(jret);
                throw std::runtime_error(oss.str());
            }
        }

        Volume ret(aInfo.x, aInfo.y, aInfo.z);

        float minf = float(std::numeric_limits<tType>::max());
        float maxf = float(std::numeric_limits<tType>::min());
        for(auto const elem : buffer) {
            float const elemf = float(elem);
            if(elemf > maxf) maxf = elemf;
            else if(elemf < minf) minf = elemf;
        }

        std::transform( buffer.begin(), buffer.end(), ret.data(), [&] (tType aValue) {
            return (aValue-minf) / (maxf-minf);
        } );

        ret.initialize();
        return ret;
    }
}


Volume load_mhd_volume(char const* aFileName) try {
    // Load the .mhd, which gives us the meta data for the volume.
    FILE* mhd = std::fopen(aFileName, "rb");
    if(!mhd)
        throw std::runtime_error("Could not open source file");

    ON_SCOPE_EXIT([&] { std::fclose(mhd); });

    MHDInfo info;

    int line = 1;
    char name[64] = "";
    char value[64] = "";

    while(2 == std::fscanf(mhd, "%63s = %63[^\n\r]", name, value)) {
        if(0 == std::strcmp("ObjectType", name))
            info.typeIsImage = (0 == std::strcmp("Image", value));
        else if(0 == std::strcmp("BinaryData", name))
            info.dataIsBinary = (0 == std::strcmp( "True", value));
        else if(0 == std::strcmp("CompressedData", name))
            info.dataCompressed = (0 == std::strcmp( "True", value));
        else if(0 == std::strcmp("NDims", name)) {
            char dummy;
            if(1 != std::sscanf(value, "%d%c", &info.ndims, &dummy))
                throw std::runtime_error("MHD: NDims should be a single integer");
        }
        else if(0 == std::strcmp("DimSize", name)) {
            char dummy;
            if(3 != std::sscanf(value, "%d %d %d%c", &info.x, &info.y, &info.z, &dummy))
                throw std::runtime_error("MHD: DimSize should be three integers");
        }
        else if(0 == std::strcmp("ElementType", name)) {
            if(0 == std::strcmp("MET_UCHAR", value))
                info.elementType = MHDType::u8;
            else if(0 == std::strcmp("MET_SHORT", value))
                info.elementType = MHDType::s16;
            else {
                std::ostringstream oss;
                oss << "MHD: ElementType '" << value << "' unknown";
                throw std::runtime_error(oss.str());
            }
        }
        else if(0 == std::strcmp("ElementDataFile", name)) {
            info.dataFile = value;
        }
        else {
#            if 0
            std::fprintf(stderr, "Line %d: unused '%s' = '%s'\n", line, name, value);
#            endif
        }
        
        ++line;
    }

    if(!std::feof(mhd)) {
        std::fprintf(stderr, "MHD (%s): ignored garbage on line %d\n", aFileName, line);
    }

    // Verify that this makes sense
    if(!info.typeIsImage)
        throw std::runtime_error("Only support ObjectType = Image.");

    if(!info.dataIsBinary)
        throw std::runtime_error("Only support binary data (BinaryData = True)");

    if(MHDType::unknown == info.elementType)
        throw std::runtime_error("Element type of binary data is not recognized");

    /* Note: this is unlikely to trigger, since we explicitly try to parse
     * DimSize with three integers.
     */
    if(3 != info.ndims)
        throw std::runtime_error("Only support 3D volumes");

    if(info.x <= 0 || info.y <= 0 || info.z <= 0)
        throw std::runtime_error("Volume size is invalid (DimSize should be three positive integers");

    // Try to read the volume data
    std::ostringstream oss;
    
    if(char const* lastSlash = std::strrchr(aFileName, '/'))
        oss << std::string(aFileName, lastSlash) << "/";

    oss << info.dataFile; 

    FILE* data = std::fopen(oss.str().c_str(), "rb");
    if(!data) {
        std::ostringstream emsg;
        emsg << "Could not open volume data '" << oss.str() << "' for reading";
        throw std::runtime_error(emsg.str());
    }

    ON_SCOPE_EXIT([&] { std::fclose(data); });

    if(!info.dataCompressed) {
        switch(info.elementType) {
            case MHDType::u8: return load_data_raw_<std::uint8_t>(data, info);
            case MHDType::s16: return load_data_raw_<std::int16_t>(data, info);
            case MHDType::unknown: assert(!"Unkown element type"); break;
        }
    }
    else {
        switch(info.elementType) {
            case MHDType::u8: return load_data_compressed_<std::uint8_t>(data, info);
            case MHDType::s16: return load_data_compressed_<std::int16_t>(data, info);
            case MHDType::unknown: assert(!"Unkown element type"); break;
        }
    }

    // Should be unreachable.
    return Volume(0,0,0);
}
catch(std::exception const& eError) {
    std::fprintf(stderr, "Error while loading MHD file \"%s\":\n", aFileName),
    std::fprintf(stderr, "  - %s\n", eError.what());
    return Volume(0, 0, 0);
}

Volume load_fld_volume(char const* fileName) {
    FLDInfo info;
    std::ifstream file(fileName, std::ios::in | std::ios::binary);

    std::string line = "";

    while (!file.eof()) {
        char byte = file.get();
        if (byte == 12) {
            byte = file.get();
            if (byte == 12) break;
        } else if (byte == '\n') {
            int index, value;
            char data[16] = "";

            if (std::sscanf(line.c_str(), "ndim = %d", &value) == 1) {
                info.ndims = value;
            } else if (std::sscanf(line.c_str(), "veclen = %d", &value) == 1) {
                info.vecLen = value;
            } else if (std::sscanf(line.c_str(), "dim%d = %d", &index, &value) == 2) {
                info.dims[index - 1] = value;
            } else if (std::sscanf(line.c_str(), "data = %16s", &data) == 1) {
                if (std::strcmp("byte", data) == 0) {
                    info.bytes = 1;
                } else if (std::strcmp("short", data) == 0) {
                    info.bytes = 2;
                } else if (std::strcmp("int", data) == 0) {
                    info.bytes = 4;
                }
            }

            line = "";
        } else {
            line += byte;
        }
    }

    assert(info.ndims == 3 && "Must have 3 dimensions");
    Volume volume(info.dims[0], info.dims[1], info.dims[2], info.vecLen);

    size_t x = 0;
    size_t y = 0;
    size_t z = 0;
    size_t vecIndex = 0;
    size_t dataSize = 0;
    unsigned int data = 0;

    float minv = std::numeric_limits<float>::max();
    float maxv = std::numeric_limits<float>::min();

    while (!file.eof()) {
        unsigned char byte = file.get();
        data = (data << 8) + (unsigned int) byte;
        if (++dataSize == info.bytes) {
            const float value = float(data);
            const size_t index = (z * info.dims[0] * info.dims[1] + y * info.dims[0] + x) * info.vecLen + vecIndex;
            volume.data()[index] = value;
            if (value > maxv) maxv = value;
            if (value < minv) minv = value;
            data = 0;
            dataSize = 0;
            vecIndex = (vecIndex + 1) % info.vecLen;

            if (++x == info.dims[0]) {
                x = 0;
                if (++y == info.dims[1]) {
                    y = 0;
                    if (++z == info.dims[2]) {
                        break;
                    }
                }
            }
        }
    }

    for (size_t i = 0; i < info.dims[0] * info.dims[1] * info.dims[2] * info.vecLen; i++) {
        volume.data()[i] = (volume.data()[i] - minv) / maxv;
    }

    volume.initialize();
    return volume;
}

Volume load_cube() {
    const int size = 4;
    Volume cube(size, size, size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                int index = i * size * size + j * size + k;
                // if (j % 10 == 1 && k % 10 == 1 && i % 10 == 1) {
                    cube.data()[index] = 1.0f;
                // }
            }
        }
    }

    cube.initialize();
    return cube;
}

Volume load_seaweed() {
    struct Segment {
        glm::vec3 from;
        glm::vec3 to;
        float phi;
        float theta;
        float length;
        float width;
    };

    std::vector<Segment> segments;
    srand(123456);

    const int size = 64;
    const int minSegments = 3;
    const int maxSegments = 6;
    const int minLeaves = 1;
    const int maxLeaves = 3;
    const float minLength = size / 10.0f;
    const float maxLength = size / 4.0f;
    const float minLeafLength = size / 20.0f;
    const float maxLeafLength = size / 16.0f;
    const float leafOffset = minLength;
    const float minWidth = 1.0f;
    const float maxWidth = 3.0f;
    const float angleFactor = glm::radians(15.0f);
    const float leafAngleFactor = glm::radians(50.0f);

    const int segmentCount = rand() % (maxSegments - minSegments + 1) + minSegments;
    float totalLength = 0.0f;
    float prevPhi = glm::radians(90.0f);
    float prevTheta = glm::radians(90.0f);
    glm::vec3 from(size / 2.0f, size - 1.0f, size / 2.0f);

    for (int i = 0; i < segmentCount; i++) {
        const float width = (1.0f - float(i) / float(segmentCount)) * (maxWidth - minWidth) + minWidth;
        const float phi = prevPhi - angleFactor + float(rand()) / float(RAND_MAX) * angleFactor * 2.0f;
        const float theta = prevTheta - angleFactor + float(rand()) / float(RAND_MAX) * angleFactor * 2.0f;
        const float length = float(rand()) / float(RAND_MAX) * (maxLength - minLength + 1) + minLength;
        const glm::vec3 to = from + glm::vec3(length * cos(phi) * sin(theta), -length * sin(phi) * sin(theta), length * cos(theta));
        const Segment segment { from, to, phi, theta, length, width };
        segments.push_back(segment);
        from = to;
        prevPhi = phi;
        prevTheta = theta;
        totalLength += length;
    }

    const int leafCount = (rand() % (maxLeaves - minLeaves + 1) + minLeaves) * 2 * segmentCount;
    float currentLength = 0.0f;
    int segmentIndex = 0;

    for (int i = 0; i < leafCount; i++) {
        const float dist = float(i) / float(leafCount) * (totalLength - leafOffset) + leafOffset;
        if (dist > currentLength + segments[segmentIndex].length) {
            currentLength += segments[segmentIndex].length;
            segmentIndex += 1;
        }

        const float factor = 1.0f - float(segmentIndex) / float(segmentCount) * 0.5f;
        const Segment& segment = segments[segmentIndex];
        const glm::vec3 dir = glm::normalize(segment.to - segment.from);
        const float t = dist - currentLength;
        const float sign = float(i % 2) * 2.0f - 1.0f;
        glm::vec3 from = segment.from + dir * t + glm::vec3(0.0f, 0.0f, sign * segment.width / 2.0f);
        prevPhi = segment.phi - glm::radians(90.0f);
        prevTheta = segment.theta - glm::radians(90.0f);

        for (int j = 0; j < 2; j++) {
            const float phi = prevPhi - leafAngleFactor + float(rand()) / float(RAND_MAX) * leafAngleFactor * 2.0f;
            const float theta = prevTheta - leafAngleFactor + float(rand()) / float(RAND_MAX) * leafAngleFactor * 2.0f;
            const float length = (float(rand()) / float(RAND_MAX) * (maxLeafLength - minLeafLength + 1) + minLeafLength) * factor;
            const float width = 2.0f - float(j) * 0.5f;
            const glm::vec3 to = from + glm::vec3(length * cos(phi) * sin(theta), -length * sin(phi) * sin(theta), sign * length * cos(theta));
            const Segment leaf { from, to, phi, theta, length, width };
            segments.push_back(leaf);
            from = to;
            prevPhi = phi;
            prevTheta = theta;
        }
    }

    Volume seaweed(size, size, size);

    for (Segment& segment : segments) {
        const glm::vec3 span1 = glm::vec3(
            cos(segment.phi + glm::radians(90.0f)) * sin(segment.theta),
            sin(segment.phi + glm::radians(90.0f)) * sin(segment.theta),
            cos(segment.theta)
        );
        const glm::vec3 span2 = glm::vec3(
            cos(segment.phi) * sin(segment.theta + glm::radians(90.0f)),
            sin(segment.phi) * sin(segment.theta + glm::radians(90.0f)),
            cos(segment.theta + glm::radians(90.0f))
        );

        const glm::vec3 dir = segment.to - segment.from;
        float t = 0.0f;
        while (t <= 1.0f) {
            const glm::vec3 pos = segment.from + dir * t;

            const float dt = glm::min(
                glm::min(
                    ((glm::min(glm::sign(dir.x), 0.0f) + 1.0f) - glm::fract(pos.x)) / dir.x,
                    ((glm::min(glm::sign(dir.y), 0.0f) + 1.0f) - glm::fract(pos.y)) / dir.y
                ),
                ((glm::min(glm::sign(dir.z), 0.0f) + 1.0f) - glm::fract(pos.z)) / dir.z
            );

            t += dt + glm::epsilon<float>();
            for (int i = 0; i < ceil(segment.width); i++) {
                for (int j = 0; j < ceil(segment.width); j++) {
                    const float fi = float(i) / ceil(segment.width);
                    const float fj = float(j) / ceil(segment.width);
                    const glm::vec3 off = (fi - 0.5f) * span1 * segment.width + (fj - 0.5f) * span2 * segment.width;
                    const glm::ivec3 ipos = glm::floor(pos + off);
                    if (ipos.x < 0 || ipos.y < 0 || ipos.z < 0 || ipos.x >= size || ipos.y >= size || ipos.z >= size) {
                        continue;
                    }
                    int index = ipos.z * size * size + ipos.y * size + ipos.x;
                    seaweed.data()[index] = 1.0f;
                }
            }

        }
    }

    seaweed.initialize();
    return seaweed;
}

void Volume::initialize() {
    glCreateTextures(GL_TEXTURE_3D, 1, &this->dataTexture);
    glBindTextureUnit(0, this->dataTexture);

    glTextureStorage3D(this->dataTexture, 1, GL_R32F, this->width(), this->height(), this->depth());
    glTextureSubImage3D(
        this->dataTexture, 0, 0, 0, 0, this->width(), this->height(), this->depth(), GL_RED, GL_FLOAT, this->data());
    glBindTexture(GL_TEXTURE_3D, this->dataTexture);
    glTexParameteri(GL_TEXTURE_3D,  GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_3D,  GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_3D,  GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);

    glCreateTextures(GL_TEXTURE_3D, 1, &this->distanceTexture);
    glBindTextureUnit(1, this->distanceTexture);

    glTextureStorage3D(this->distanceTexture, 1, GL_R32F, this->width(), this->height(), this->depth());

    glCreateTextures(GL_TEXTURE_3D, 1, &this->gradientTexture);
    glBindTextureUnit(2, this->gradientTexture);

    glTextureStorage3D(this->gradientTexture, 1, GL_RGB32F, this->width(), this->height(), this->depth());
}

/// Create distance field by starting at non-empty voxels (0 distance)
/// Then iteratively go outwards by 1 voxel until all values in the distance field are filled correctly
void Volume::create_distance_field(float threshold) {
    hasDistance = true;
    struct Unit {
        glm::vec3 voxel;
        glm::vec3 distance;

        float get_distance() const {
            return sqrtf(distance.x * distance.x + distance.y * distance.y + distance.z * distance.z);
        }
    };

    std::stack<Unit> stack, oldStack;
    const float maxDistance =
        sqrtf(this->depth() * this->depth() + this->height() * this->height() + this->width() * this->width()) + 1;

    // Initialize non-empty voxels as 0, all other voxels to the maximum distance
    // For each non-empty voxel a new unit is added to the initial stack
    for (int z = 0; z < this->depth(); z++) {
        for (int y = 0; y < this->mHeight; y++) {
            for (int x = 0; x < this->width(); x++) {
                const int index = z * this->width() * this->height() + y * this->width() + x;
                if (this->mData[index] >= threshold) {
                    this->mDistanceField[index] = 0;
                    oldStack.push(Unit { glm::vec3(x, y, z), glm::vec3(0, 0, 0) });
                    continue;
                } else {
                    this->mDistanceField[index] = maxDistance;
                }
            }
        }
    }

    // Try to add the unit to the new stack if its distance is less than the current distance
    const auto tryAdd = [this](std::stack<Unit>& stack, int x, int y, int z, glm::vec3 distance) {
        const int index = this->to_linear_index(x, y, z);
        const Unit newUnit = {
            glm::vec3(x, y, z),
            distance,
        };
        const float newDistance = newUnit.get_distance();
        if (this->mDistanceField[index] > newDistance) {
            this->mDistanceField[index] = newDistance;
            stack.push(newUnit);
        }
    };

    // Iteratively go over each element in the stack, trying to add new units going outwards by 1 voxel to the new stack
    // When the old stack is empty, replace it with the new stack
    // Do this until no new units are added to the new stack
    while (true) {
        const Unit unit = oldStack.top();
        for (int i = -1; i <= 1; i += 2) {
            const int x = unit.voxel.x + i;
            const int y = unit.voxel.y + i;
            const int z = unit.voxel.z + i;
            if (x >= 0 && x < this->width()) {
                tryAdd(stack, x, unit.voxel.y, unit.voxel.z, unit.distance + glm::vec3(1, 0, 0));
            }
            if (y >= 0 && y < this->mHeight) {
                tryAdd(stack, unit.voxel.x, y, unit.voxel.z, unit.distance + glm::vec3(0, 1, 0));
            }
            if (z >= 0 && z < this->depth()) {
                tryAdd(stack, unit.voxel.x, unit.voxel.y, z, unit.distance + glm::vec3(0, 0, 1));
            }
        }

        oldStack.pop();
        if (oldStack.empty()) {
            if (stack.empty()) break;
            oldStack.swap(stack);
        }
    }

    glBindTextureUnit(1, this->distanceTexture);
    glTextureSubImage3D(
        this->distanceTexture, 0, 0, 0, 0,
        this->width(), this->height(), this->depth(),
        GL_RED, GL_FLOAT, this->mDistanceField.data()
    );
    glBindTexture(GL_TEXTURE_3D, this->distanceTexture);
    glTexParameteri(GL_TEXTURE_3D,  GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_3D,  GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_3D,  GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Volume::create_gradient_field(float threshold) {
    hasGradient = true;
    int x = 0,
        y = 0,
        z = 0;
    
    for (int i = 0; i < this->total_element_count(); i++) {
        glm::vec3 normal(0.0f);

        if (x >= 1 && x <= this->width() - 2)
            normal.x = this->mDistanceField[this->to_linear_index(x - 1, y, z)] - this->mDistanceField[this->to_linear_index(x + 1, y, z)];
        else
            normal.x = x == 0 ? 1.0f : -1.0f;

        if (y >= 1 && y <= this->height() - 2)
            normal.y = this->mDistanceField[this->to_linear_index(x, y - 1, z)] - this->mDistanceField[this->to_linear_index(x, y + 1, z)];
        else
            normal.y = y == 0 ? 1.0f : -1.0f;

        if (z >= 1 && z <= this->depth() - 2)
            normal.z = this->mDistanceField[this->to_linear_index(x, y, z - 1)] - this->mDistanceField[this->to_linear_index(x, y, z + 1)];
        else
            normal.z = z == 0 ? 1.0f : -1.0f;

        size_t index = this->to_linear_index(x, y, z);
        this->mGradientField[index] = glm::normalize(normal);
        this->mGradientField[index] = this->mGradientField[index] * 0.5f + 0.5f;

        x = (x + 1) % this->width();
        if (x == 0) {
            y = (y + 1) % this->height();
            if (y == 0) {
                z = (z + 1) % this->depth();
            }
        }
    }

    glBindTextureUnit(2, this->gradientTexture);
    glTextureSubImage3D(
        this->gradientTexture, 0, 0, 0, 0,
        this->width(), this->height(), this->depth(),
        GL_RGB, GL_FLOAT, this->mGradientField.data()
    );
    glBindTexture(GL_TEXTURE_3D, this->gradientTexture);
    glTexParameteri(GL_TEXTURE_3D,  GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_3D,  GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_3D,  GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
