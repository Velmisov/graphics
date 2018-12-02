import tkinter as tk
from PIL import Image
import numpy as np
from lib import Vector, rgb, Sphere, ray_trace


WIDTH, HEIGHT = 400, 300


class MainWindow(tk.Frame):
    def __init__(self, root):
        super().__init__()
        self.root = root


scene = [
    Sphere(Vector(.75, .1, 1.), .6, rgb(0, 0, 1)),
    Sphere(Vector(-.75, .1, 2.25), .6, rgb(.5, .223, .5)),
    Sphere(Vector(-2.75, .1, 3.5), .6, rgb(1., .572, .184)),
    Sphere(Vector(0, -99999.5, 0), 99999, rgb(.75, .75, .75), 0.25),
]

r = float(WIDTH) / HEIGHT
# Screen coordinates: x0, y0, x1, y1.
S = (-1., 1. / r + .25, 1., -1. / r + .25)
xs = np.tile(np.linspace(S[0], S[2], WIDTH), HEIGHT)
ys = np.repeat(np.linspace(S[1], S[3], HEIGHT), WIDTH)

Q = Vector(xs, ys, 0)
eye_pos = Vector(0., 0.35, -1.)
color = ray_trace(eye_pos, (Q - eye_pos).norm(), scene, 2)

rgb = [Image.fromarray((255 * np.clip(c, 0, 1).reshape((HEIGHT, WIDTH))).astype(np.uint8), "L") for c in color.components()]
Image.merge("RGB", rgb).save("fig.png")
