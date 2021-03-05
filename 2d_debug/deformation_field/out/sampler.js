var Sampler = /** @class */ (function () {
    function Sampler(aabbMin, aabbMax, sampler, data, size, make_spline) {
        this.aabbMin = aabbMin;
        this.aabbMax = aabbMax;
        this.sampler = sampler;
        this.data = data;
        this.size = size;
        this.make_spline = make_spline;
    }
    Sampler.prototype.get = function (ray, samplePos) {
        var index = Math.round(samplePos.y * this.size) + Math.round(samplePos.x);
        if (this.sampler[index]) {
            return null;
        }
        return this.make_spline(ray, this.data[index]);
    };
    return Sampler;
}());
export { Sampler };
