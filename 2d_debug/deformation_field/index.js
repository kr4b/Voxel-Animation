onload = () => {
  const ctx = canvas.getContext("2d");
  canvas.width = 400;
  canvas.height = 400;

  ctx.scale(canvas.width / 2, canvas.height / 2);
  ctx.translate(1, 1);
}