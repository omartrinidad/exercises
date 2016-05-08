'''
Shinho
@brief get parameter (matrix instance), and draw it to the grid.
'''
from Tkinter import Tk, Canvas, Frame, BOTH

import numpy as np
import random


class DrawGrid(Frame):
    def __init__(self, parent, mat):
        Frame.__init__(self, parent)
        self.parent = parent
        self.pack(fill=BOTH, expand=1)

        self.rows = 101
        self.columns = 82
        self.cell_width = 6
        self.cell_height = 6

        self.canvas = Canvas(self, width=self.cell_width * self.columns, height=self.cell_height * self.rows,
                             borderwidth=0, highlightthickness=0)
        self.canvas.pack(side="top", fill="both", expand="true")

        self.rect = {}
        for column in range(self.columns):
            for row in range(self.rows):
                x1 = column * self.cell_width
                y1 = row * self.cell_height
                x2 = x1 + self.cell_width
                y2 = y1 + self.cell_height
                self.rect[row, column] = self.canvas.create_rectangle(x1, y1, x2, y2, fill="white", tags="rect",
                                                                      outline="black")

        self.data = mat
        self.draw_result(500)

    def draw_result(self, delay):
        self.canvas.itemconfig("rect", fill="white")

        nonzero_idx = self.data.nonzero()
        for row,col in zip(nonzero_idx[0], nonzero_idx[1]):
            item_id = self.rect[row, col]
            self.canvas.itemconfig(item_id, fill="red")

        # self.after(delay, lambda: self.redraw(delay))


def draw(mat):

    root = Tk()
    root.title("test abc")
    ex = DrawGrid(root, mat)
    root.mainloop()


if __name__ == "__main__":
    k = np.zeros((101, 82), dtype=np.int)
    k[0, 0] = 1
    k[100, 81] = 1
    test = np.matrix(k)

    draw(test)
