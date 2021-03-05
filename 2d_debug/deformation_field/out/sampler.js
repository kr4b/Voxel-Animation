var Sampler = /** @class */ (function () {
    function Sampler(aabbMin, aabbMax, sampler, data, size) {
        this.aabbMin = aabbMin;
        this.aabbMax = aabbMax;
        this.sampler = sampler;
        this.data = data;
        this.size = size;
    }
    Sampler.prototype.get = function (samplePos) {
        var index = Math.round(samplePos.y * this.size) + Math.round(samplePos.x);
        if (this.sampler[index]) {
            return null;
        }
        return this.data[index];
    };
    return Sampler;
}());
export { Sampler };
