import numpy as np
from functools import reduce
import numbers


def extract(cond, x):
    if isinstance(x, numbers.Number):
        return x
    else:
        return np.extract(cond, x)


class Vector:
    def __init__(self, x, y, z):
        self.x, self.y, self.z = x, y, z

    def __mul__(self, other):
        return Vector(self.x * other, self.y * other, self.z * other)

    def __add__(self, other):
        return Vector(self.x + other.x, self.y + other.y, self.z + other.z)

    def __sub__(self, other):
        return Vector(self.x - other.x, self.y - other.y, self.z - other.z)

    def dot(self, other):
        return (self.x * other.x) + (self.y * other.y) + (self.z * other.z)

    def __abs__(self):
        return self.dot(self)

    def norm(self):
        norma = np.sqrt(abs(self))
        return self * (1.0 / np.where(norma == 0, 1, norma))

    def components(self):
        return self.x, self.y, self.z

    def extract(self, cond):
        return Vector(extract(cond, self.x), extract(cond, self.y), extract(cond, self.z))

    def place(self, cond):
        res = Vector(np.zeros(cond.shape), np.zeros(cond.shape), np.zeros(cond.shape))
        np.place(res.x, cond, self.x)
        np.place(res.y, cond, self.y)
        np.place(res.z, cond, self.z)
        return res


rgb = Vector


class Figure:
    def intersect(self, ray_origin, direction):
        pass


width, height = 400, 300
light_pos = Vector(5, 5., -10)
eye_pos = Vector(0., 0.35, -1.)


class Sphere(Figure):
    def __init__(self, center, radius, diffuse, mirror=0.5):
        self.center = center
        self.radius = radius
        self.diffuse = diffuse
        self.mirror = mirror

    def intersect(self, ray_origin, direction):
        b = 2 * direction.dot(ray_origin - self.center)
        c = abs(self.center) + abs(ray_origin) - 2 * self.center.dot(ray_origin) - (self.radius * self.radius)
        discriminant = (b ** 2) - (4 * c)
        sqrt_disc = np.sqrt(np.maximum(0, discriminant))
        r1 = (-b - sqrt_disc) / 2
        r2 = (-b + sqrt_disc) / 2
        root = np.where((r1 > 0) & (r1 < r2), r1, r2)

        return np.where((discriminant > 0) & (root > 0), root, np.inf)

    def light(self, ray_origin, direction, dist, scene, bounce):
        M = (ray_origin + direction * dist)  # intersection point
        N = (M - self.center) * (1. / self.radius)  # normal
        toL = (light_pos - M).norm()  # direction to light
        toO = (eye_pos - M).norm()  # direction to ray origin
        nudged = M + N * .0001  # M nudged to avoid itself

        # Shadow: find if the point is shadowed or not.
        # This amounts to finding out if M can see the light
        light_distances = [s.intersect(nudged, toL) for s in scene]
        light_nearest = reduce(np.minimum, light_distances)
        seelight = light_distances[scene.index(self)] == light_nearest

        # Ambient
        color = rgb(0.05, 0.05, 0.05)

        # Lambert shading (diffuse)
        lv = np.maximum(N.dot(toL), 0)
        color += self.diffuse * lv * seelight

        # Reflection
        if bounce > 0:
            rayD = (direction - N * 2 * direction.dot(N)).norm()
            color += ray_trace(nudged, rayD, scene, bounce - 1) * self.mirror

        # Blinn-Phong shading (specular)
        phong = N.dot((toL + toO).norm())
        color += rgb(1, 1, 1) * np.power(np.clip(phong, 0, 1), 50) * seelight
        return color


class Cube(Figure):
    def __init__(self, center, size, diffuse, mirror=0.5):
        self.center = center
        self.size = size
        self.diffuse = diffuse
        self.mirror = mirror

    def intersect(self, ray_origin, direction):
        pass


def ray_trace(ray_origin, direction, scene, bounce=0):
    distances = [s.intersect(ray_origin, direction) for s in scene]
    nearest = reduce(np.minimum, distances)
    color = rgb(0, 0, 0)
    for figure, dist in zip(scene, distances):
        hit = (nearest != np.inf) & (dist == nearest)
        if np.any(hit):
            dc = extract(hit, dist)
            Oc = ray_origin.extract(hit)
            Dc = direction.extract(hit)
            cc = figure.light(Oc, Dc, dc, scene, bounce)
            color += cc.place(hit)
    return color
