#pragma once

// Copied from the TI1806 final project.

#include <vector>

#include <cassert>
#include <cstddef>

#include <glad/glad.h>

struct FLDInfo {
    size_t ndims;
    std::vector<size_t> dims;
    size_t vecLen;
    size_t bytes;
};

/* A 3D volume
 *
 * The `Volume` holds a scalar 3D volume, i.e., a 3D volume where each point
 * has a certain density. The size of the volume is given by the
 *   - `width()`
 *   - `height()`
 *   - `depth()'
 * member functions. Important: the volume is not necessarily cubic!
 *
 * Individual elements of the volume can be accessed via the operator(). For
 * example:
 *
 *   Volume myVolume = ...;
 *   float element = myVolume( 1, 3, 8 );
 *
 * `element` contains the density at the coordinate (1,3,8). Note: you must not
 * attempt to read the value of an element outside of the defined volume. So
 *
 *   std::size_t i = ..., j = ..., k = ...;
 *   float elem = myVolume( i, j, k );
 *
 * requires that
 *   - i >= 0 and i < myVolume.width()
 *   - j >= 0 and j < myVolume.height()
 *   - k >= 0 and k < myVolume.depth()
 *
 * The total number of elements in the volume is given by
 * `total_element_count()`, which is equal to `width()*height()*depth()`.
 */
class Volume
{
    public:
        Volume(GLsizei aWidth, GLsizei aHeight, GLsizei aDepth);
        Volume(GLsizei aWidth, GLsizei aHeight, GLsizei aDepth, GLsizei vecLen);

        // Not copyable, but movable.
        // Rationale: don't accidentally make copies of large data.
        Volume( Volume const& ) = delete;
        Volume& operator= (Volume const&) = delete;

        Volume( Volume&& ) = default;
        Volume& operator= (Volume&&) = default;

    public:
        float& operator() (std::size_t aI, std::size_t aJ, std::size_t aK);
        const float& operator() (std::size_t aI, std::size_t aJ, std::size_t aK) const;
        void initialize();

    public:
        GLsizei width() const;
        GLsizei height() const;
        GLsizei depth() const;
        GLsizei total_element_count() const;

        float* data();
        const float* data() const;

        void bind() const;

        std::size_t to_linear_index( std::size_t, std::size_t, std::size_t ) const;

    private:
        std::vector<float> mData;
        GLsizei mWidth, mHeight, mDepth;
        GLuint texture;
};

/* Use this method to load a Volume from a file. We've included two example
 * volumes in the `data/` subdirectory.
 */
Volume load_mhd_volume(char const* aFileName);

Volume load_fld_volume(char const* fileName, FLDInfo info);

Volume load_cube();

// Implementation:
inline
Volume::Volume( GLsizei aWidth, GLsizei aHeight, GLsizei aDepth )
    : mData(aWidth * aHeight * aDepth)
    , mWidth(aWidth)
    , mHeight(aHeight)
    , mDepth(aDepth)
{}

inline
Volume::Volume(GLsizei aWidth, GLsizei aHeight, GLsizei aDepth, GLsizei vecLen)
    : mData(aWidth * aHeight * aDepth * vecLen)
    , mWidth(aWidth)
    , mHeight(aHeight)
    , mDepth(aDepth)
{}

inline
float& Volume::operator() (std::size_t aI, std::size_t aJ, std::size_t aK) {
    assert(aI < mWidth && aJ < mHeight && aK < mDepth);
    return mData[to_linear_index(aI, aJ, aK)];
}
inline
const float& Volume::operator() (std::size_t aI, std::size_t aJ, std::size_t aK) const {
    assert(aI < mWidth && aJ < mHeight && aK < mDepth);
    return mData[to_linear_index(aI, aJ, aK)];
}

inline 
GLsizei Volume::width() const {
    return mWidth;
}
inline 
GLsizei Volume::height() const {
    return mHeight;
}
inline
GLsizei Volume::depth() const {
    return mDepth;
}
inline
GLsizei Volume::total_element_count() const {
    return mWidth * mHeight * mDepth;
}

inline
float* Volume::data() {
    return mData.data();
}
inline
const float* Volume::data() const {
    return mData.data();
}

inline
std::size_t Volume::to_linear_index(std::size_t aI, std::size_t aJ, std::size_t aK) const {
    return aK * mWidth * mHeight + aJ * mWidth + aI;
}

inline void Volume::bind() const {
    glBindTextureUnit(0, this->texture);
}
