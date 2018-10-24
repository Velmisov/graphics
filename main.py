import tkinter as tk
import numpy as np

WIDTH = 1400
HEIGHT = 800


class MainWindow(tk.Frame):
    def __init__(self, root):
        super().__init__()

        self.root = root

        self.canvas = tk.Canvas(width=WIDTH, height=HEIGHT, bg='white')
        self.canvas.pack()

        self.canvas.delete(tk.ALL)
        self.canvas.bind("<B1-Motion>", self.make_point)
        self.canvas.bind("<Button-1>", self.make_point)

        self.points = []

    def draw_function(self):
        self.canvas.delete(tk.ALL)
        for p in self.points:
            self.canvas.create_oval(p[0] - 1, p[1] - 1, p[0] + 1, p[1] + 1)
        self.quick_hull(self.points)

    def quick_hull(self, points):
        def find_left_point():
            x_left, y_down = WIDTH, HEIGHT
            for x, y in points:
                if x < x_left or x == x_left and y > y_down:
                    x_left, y_down = x, y
            return x_left, y_down

        left_point = find_left_point()

        def find_right_point():
            x_right, y_up = 0, 0
            for x, y in points:
                if x > x_right or x == x_right and y < y_up:
                    x_right, y_up = x, y
            return x_right, y_up

        right_point = find_right_point()

        # print(left_point)
        # print(right_point)
        # self.canvas.create_line(*left_point, *right_point, width=2)

        def help_hull(left_point, right_point, points):
            left_3d = np.append(np.array(left_point), [0])
            right_3d = np.append(np.array(right_point), [0])
            uppers = [p for p in points if np.cross(right_3d - left_3d, np.append(np.array(p), [0]) - left_3d)[2] < 0]
            # print(uppers)
            if len(uppers) == 0:
                return False

            farest = -1
            tmp = 0
            for i in range(len(uppers)):
                p = uppers[i]
                if tmp < np.linalg.norm(np.cross(right_3d - left_3d, np.append(np.array(p), [0]) - left_3d)) \
                        / np.linalg.norm(right_3d - left_3d):
                    tmp = np.linalg.norm(np.cross(right_3d - left_3d, np.append(np.array(p), [0]) - left_3d)) \
                          / np.linalg.norm(right_3d - left_3d)
                    farest = uppers[i]

            # print(farest)

            # help_hull(left_point, farest, uppers)
            # help_hull(farest, right_point, uppers)
            # self.canvas.create_line(*left_point, *farest, width=2)
            # self.canvas.create_line(*farest, *right_point, width=2)
            if not help_hull(left_point, farest, uppers):
                self.canvas.create_line(*left_point, *farest, width=2)
            if not help_hull(farest, right_point, uppers):
                self.canvas.create_line(*farest, *right_point, width=2)
            return True

        if not help_hull(left_point, right_point, points):
            self.canvas.create_line(*left_point, *right_point, width=2)
        if not help_hull(right_point, left_point, points):
            self.canvas.create_line(*left_point, *right_point, width=2)

    def make_point(self, event):
        self.points.append((event.x, event.y))
        self.draw_function()


if __name__ == '__main__':
    root = tk.Tk()
    MainWindow(root)
    root.mainloop()
