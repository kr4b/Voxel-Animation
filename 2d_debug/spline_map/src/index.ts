let canvas: HTMLCanvasElement;
let ctx: CanvasRenderingContext2D;

onload = () => {
    canvas = <HTMLCanvasElement> document.getElementById("canvas");
    ctx = <CanvasRenderingContext2D> canvas.getContext("2d")

    canvas.width = 400;
    canvas.height = 400;

    ctx.scale(canvas.width / 2, canvas.height / 2);
    ctx.translate(1, 1);
    ctx.lineWidth = 0.0075;

    ctx.fillRect(-0.5, -0.5, 1, 1);
}