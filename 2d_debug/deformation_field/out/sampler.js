var Sampler = /** @class */ (function () {
    function Sampler(samplerAABB, realAABB, sampler, data, colors, make_spline) {
        console.assert(data.length == sampler.length, "Check if the sampler length is correct");
        console.assert(data.length == colors.length, "Check if the colors length is correct");
        var size = Math.floor(Math.sqrt(data.length));
        console.assert(data.length / size == size, "Check if the length of the data is a square");
        this.samplerAABB = samplerAABB;
        this.realAABB = realAABB;
        this.sampler = sampler;
        this.data = data;
        this.colors = colors;
        this.size = size;
        this.make_spline = make_spline;
    }
    Sampler.prototype.get = function (ray, samplePos) {
        var x = Math.round((this.size - 1) * samplePos.x);
        var y = Math.round((this.size - 1) * samplePos.y);
        var index = y * this.size + x;
        if (index < 0 || index >= this.data.length || !this.sampler[index]) {
            return null;
        }
        return this.make_spline(ray, this.data[index], this.colors[index]);
    };
    Sampler.prototype.draw = function (ctx) {
        var size = this.samplerAABB.size();
        var width = size.x / this.size;
        var height = size.y / this.size;
        for (var i = 0; i < this.size; i++) {
            for (var j = 0; j < this.size; j++) {
                var index = i * this.size + j;
                if (!this.sampler[index])
                    continue;
                var color = this.colors[index];
                ctx.fillStyle = "rgb(" + color[0] + "," + color[1] + "," + color[2] + ")";
                ctx.fillRect(this.samplerAABB.min.x + width * j, this.samplerAABB.min.y + height * i, width, height);
                ctx.fillStyle = "black";
            }
        }
    };
    return Sampler;
}());
export { Sampler };
