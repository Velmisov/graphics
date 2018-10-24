import tkinter as tk
import numpy as np

WIDTH = 1200
HEIGHT = 700


class MainWindow(tk.Frame):
    def __init__(self, root):
        super().__init__()

        self.root = root

        self.canvas = tk.Canvas(width=WIDTH, height=HEIGHT, bg='white')
        self.canvas.pack()

        self.canvas.delete(tk.ALL)
        self.root.bind('<Configure>', self.resize)

        self.points = [(100, 50), (60, 75), (50, 100), (60, 125), (100, 150), (140, 125), (150, 100), (140, 75), (100, 100), (170, 100)]

        self.draw_function()

    def draw_function(self):
        self.canvas.delete(tk.ALL)
        for p in self.points:
            self.canvas.create_oval(p[0] - 1, p[1] - 1, p[0] + 1, p[1] + 1)
        self.quick_hull(self.points)
        # number_of_elems = self.canvas.winfo_width()
        # if number_of_elems < 2:
        #     return
        #
        # self.canvas.create_rectangle(1, 1, self.canvas.winfo_width() - 2, self.canvas.winfo_height() - 2, outline='red')
        #
        # xs = np.linspace(self.range[0], self.range[1], number_of_elems)
        # points = self.func_now(xs)
        # mi, ma = points.min(), points.max()
        # points = (points - mi) / (ma - mi) * (self.canvas.winfo_height() - 3)
        # points = self.canvas.winfo_height() - points - 3
        # for elem in range(number_of_elems-1):
        #     self.canvas.create_line(elem + 1, points[elem], elem + 2, points[elem + 1])

    def quick_hull(self, points):
        def find_left_point():
            x_left, y_down = WIDTH, HEIGHT
            for x, y in points:
                if x < x_left or x == x_left and y < y_down:
                    x_left, y_down = x, y
            return x_left, y_down

        left_point = find_left_point()

        def find_right_point():
            x_right, y_up = 0, 0
            for x, y in points:
                if x > x_right or x == x_right and y > y_up:
                    x_right, y_up = x, y
            return x_right, y_up

        right_point = find_right_point()

        print(left_point)
        print(right_point)
        self.canvas.create_line(*left_point, *right_point, width=2)

        def help_hull(left_point, right_point, points):
            left_3d = np.append(np.array(left_point), [0])
            right_3d = np.append(np.array(right_point), [0])
            uppers = [p for p in points if np.cross(right_3d - left_3d, np.append(np.array(p), [0]) - left_3d)[2] > 0]
            print(uppers)
            if len(uppers) == 0:
                return False

            farest = uppers[np.argmax(np.linalg.norm(np.cross(right_3d - left_3d,
                                                              np.append(np.array(p), [0]) - left_3d))
                                      / np.linalg.norm(right_3d - left_3d) for p in uppers)]

            print(farest)

            help_hull(left_point, farest, uppers)
            help_hull(farest, right_point, uppers)
            self.canvas.create_line(*left_point, *farest, width=2)
            self.canvas.create_line(*farest, *right_point, width=2)
            # if not help_hull(left_point, farest):
            #     self.canvas.create_line(*left_point, *farest, width=2)
            # if not help_hull(farest, right_point):
            #     self.canvas.create_line(*farest, *right_point, width=2)
            return True

        def help_hull2(left_point, right_point, points):
            left_3d = np.append(np.array(left_point), [0])
            right_3d = np.append(np.array(right_point), [0])
            downs = [p for p in points if np.cross(right_3d - left_3d, np.append(np.array(p), [0]) - left_3d)[2] < 0]
            print(downs)
            if len(downs) == 0:
                return False

            farest = downs[np.argmax(np.linalg.norm(np.cross(right_3d - left_3d,
                                                             np.append(np.array(p), [0]) - left_3d))
                                     / np.linalg.norm(right_3d - left_3d) for p in downs)]

            print("HERE")
            print(farest)

            help_hull2(left_point, farest, downs)
            help_hull2(farest, right_point, downs)
            self.canvas.create_line(*left_point, *farest, width=2)
            self.canvas.create_line(*farest, *right_point, width=2)
            # if not help_hull(left_point, farest):
            #     self.canvas.create_line(*left_point, *farest, width=2)
            # if not help_hull(farest, right_point):
            #     self.canvas.create_line(*farest, *right_point, width=2)
            return True

        # help_hull(left_point, right_point)
        # help_hull(right_point, left_point)
        help_hull(left_point, right_point, points)
        help_hull2(left_point, right_point, points)

    def resize(self, _):
        self.draw_function()


if __name__ == '__main__':
    root = tk.Tk()
    MainWindow(root)
    root.mainloop()
    # quick_hull([(0, 0), (10, 0), (5, 0), (5, 5), (5, -2), (5, -1), (3, 1)])
