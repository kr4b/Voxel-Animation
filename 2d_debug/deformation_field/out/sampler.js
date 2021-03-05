var Sampler = /** @class */ (function () {
    function Sampler(aabbMin, aabbMax, sampler, data, colors, size, make_spline) {
        this.aabbMin = aabbMin;
        this.aabbMax = aabbMax;
        this.sampler = sampler;
        this.data = data;
        this.colors = colors;
        this.size = size;
        this.make_spline = make_spline;
    }
    Sampler.prototype.get = function (ray, samplePos) {
        var index = (this.size - 1) * (Math.floor(samplePos.y * (this.size - 1)) + Math.floor(samplePos.x));
        if (index < 0 || index >= this.data.length || this.sampler[index]) {
            return null;
        }
        return this.make_spline(ray, this.data[index]);
    };
    Sampler.prototype.draw = function (ctx) {
        var image = ctx.createImageData(this.size, this.size);
        var data = image.data;
        for (var i = 0; i < this.size; i++) {
            for (var j = 0; j < this.size; j++) {
                var color = this.colors[i * this.size + j];
                for (var k = 0; k < 3; k++) {
                    data[(i * this.size + j) * 3 + k] = color[k];
                }
            }
        }
        ctx.putImageData(image, this.aabbMin.x, this.aabbMin.y);
    };
    return Sampler;
}());
export { Sampler };
