'''
Shinho
@brief get parameter (matrix instance), and draw it to the grid.
'''
from Tkinter import Tk, Canvas, Frame, BOTH
#from Grid import Langton

import numpy as np
import random

class Langton(object):

    def __init__(self, grid, start, orientation='up', conf='white'):
        """
        Initialize parameters
        """
        self.rows = grid[0]
        self.cols = grid[1]
        self.orientation = orientation
        self.conf = conf
        self.current = start
        self.grid = np.zeros((self.rows, self.cols), dtype=int)

    def findNextCell(self):
        pass

    def currentVal(self):
        return self.grid[self.current]

    def next(self):

        print self.current, self.orientation

        # scan and turn
        print self.currentVal()
        print self.currentVal() == 1

        if self.currentVal() == 1:
            # black
            print 'white'
            if self.orientation == 'up':
                self.orientation = 'left'
            elif self.orientation == 'left':
                self.orientation = 'down'
            elif self.orientation == 'down':
                self.orientation = 'right'
            elif self.orientation == 'right':
                self.orientation = 'up'

        else:
            # white
            print 'white'
            if self.orientation == 'up':
                self.orientation = 'right'
            elif self.orientation == 'right':
                self.orientation = 'down'
            elif self.orientation == 'down':
                self.orientation = 'left'
            elif self.orientation == 'left':
                self.orientation = 'up'

        # flip
        self.grid[self.current] = 1 if self.grid[self.current] == 0 else 0

        # move
        if self.orientation == 'up':
            self.current = (self.current[0] - 1, self.current[1])
        elif self.orientation == 'down':
            self.current = (self.current[0] + 1, self.current[1])
        elif self.orientation == 'right':
            self.current = (self.current[0], self.current[1] + 1)
        elif self.orientation == 'left':
            self.current = (self.current[0], self.current[1] - 1)


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


#if __name__ == "__main__":

my_torus = Langton((101, 82), (50, 40), 'left', 'white')

for i in range(11):
    my_torus.next()
    test = np.matrix(my_torus.grid)
    draw(test)
