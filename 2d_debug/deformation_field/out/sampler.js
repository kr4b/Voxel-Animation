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
        var index = (this.size - 1) * (Math.floor(samplePos.y * this.size) + Math.floor(samplePos.x));
        if (index < 0 || index >= this.data.length || !this.sampler[index]) {
            return null;
        }
        return this.make_spline(ray, this.data[index]);
    };
    Sampler.prototype.draw = function (ctx) {
        var width = (this.aabbMax.x - this.aabbMin.x) / this.size;
        var height = (this.aabbMax.y - this.aabbMin.y) / this.size;
        for (var i = 0; i < this.size; i++) {
            for (var j = 0; j < this.size; j++) {
                var color = this.colors[i * this.size + j];
                ctx.fillStyle = "rgb(" + color[0] + "," + color[1] + "," + color[2] + ")";
                ctx.fillRect(this.aabbMin.x + width * j, this.aabbMin.y + height * i, width, height);
                ctx.fillStyle = "black";
            }
        }
    };
    return Sampler;
}());
export { Sampler };
