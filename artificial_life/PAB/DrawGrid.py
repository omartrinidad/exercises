#/usr/env/bin python
# encoding: utf8

"""
# Group members (Name, Student ID, E-Mail):
    - Shinho Kang, 2890169, wis.shinho.kang@gmail.com
    - Omar Trinidad Gutiérrez Méndez, 2850441, omar.vpa@gmail.com

# Instructions to execute this program:

    - Run in the command line: `python PAB.py `
    - Be careful to have numpy library installed
    - The program was tested in Python version : 2.7.10
    - The user will be asked for the parameters:
        - grid,
        - orientation,
        - coordinate X and,
        - coordinate Y

"""

from Tkinter import Tk, Canvas, Frame, BOTH

import numpy as np
import sys

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

        if conf == 'white':
            self.grid = np.zeros((self.rows, self.cols), dtype=int)

        elif conf == 'black':
            self.grid = np.ones((self.rows, self.cols), dtype=int)

        elif conf == 'stripes':
            re = np.r_[ 41 * [0, 0] ]
            ro = np.r_[ 41 * [1, 1] ]
            self.grid = np.row_stack(51 * (re, ro))[:-1]

        elif conf == 'checker':
            re = np.r_[ 41 * [0, 1] ]
            ro = np.r_[ 41 * [1, 0] ]
            self.grid = np.row_stack(51 * (re, ro))[:-1]

        elif conf == 'random':
            a = np.random.random((self.rows, self.cols)) + 0.5
            self.grid = a.astype(int)

    def findNextCell(self):
        pass

    def currentVal(self):
        return self.grid[self.current]

    def next(self):

        if self.currentVal():
            # black
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
            if self.current[0] == -1:
                self.current = (self.rows-1, self.current[1])

        elif self.orientation == 'down':
            self.current = (self.current[0] + 1, self.current[1])
            if self.current[0] == self.rows:
                self.current = (0, self.current[1])

        elif self.orientation == 'right':
            self.current = (self.current[0], self.current[1] + 1)
            if self.current[1] == self.cols:
                self.current = (self.current[0], 0)

        elif self.orientation == 'left':
            self.current = (self.current[0], self.current[1] - 1)
            if self.current[1] == -1:
                self.current = (self.current[0], self.cols - 1)


class DrawGrid(Frame):
    def __init__(self, parent, langton_ant):
        Frame.__init__(self, parent)
        self.parent = parent
        self.pack(fill=BOTH, expand=1)

        self.title = self.parent.title()

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

        self.langton_ant = langton_ant
        self.draw_result(500)

    def draw_result(self, delay, i=0):
        self.parent.title(self.title+" step: " + str(i))
        self.canvas.itemconfig("rect", fill="white")

        data = np.matrix(self.langton_ant.grid)
        nonzero_idx = data.nonzero()
        for row,col in zip(nonzero_idx[0], nonzero_idx[1]):
            item_id = self.rect[row, col]
            self.canvas.itemconfig(item_id, fill="black")

        self.langton_ant.next()
        self.after(delay, lambda: self.draw_result(delay, i+1))


if __name__ == "__main__":

    print("Langton's Ant")
    print("Configuration of grid")
    while True:
        print("1. All white")
        print("2. All black")
        print("3. Checker board")
        print("4. Horizontal stripes")
        print("5. Random")

        conf = raw_input("Enter: ")
        try:
            conf = int(conf)
            if conf < 1 or conf > 5:
                print("Choose a number beetwen 1 and 5")
            else:
                dict = {
                        1: 'white', 2: 'black', 3: 'checker',
                        4: 'stripes', 5: 'random'
                        }
                conf = dict[conf]
                break

        except ValueError as e:
            print("Choose a number beetwen 1 and 5")

    print("Configuration of orientation")
    while True:
        options = ['up', 'down', 'left', 'right']
        orientation = raw_input("Choose between 'up', 'down', 'left', or "\
                "'right': ")

        try:
            if orientation not in options:
                print("Choose between 'up', 'down', 'left', or 'right': ")
            else:
                break

        except ValueError as e:
            print("Choose between 'up', 'down', 'left', or 'right': ")

    while True:
        x = raw_input("Enter the position X: ")
        try:
            x = int(x)
            if x < 1 or x > 82:
                print("The position X must be in the range 1 and 82")
            else:
                x = x - 1
                break
        except ValueError as e:
            print("The position X must be in the range 1 and 82")

    while True:
        y = raw_input("Enter the position Y: ")
        try:
            y = int(y)
            if y < 1 or y > 101:
                print("The position Y must be in the range 1 and 101")
            else:
                y = y - 1
                break
        except ValueError as e:
            print("The position Y must be in the range 1 and 101")


    langton_ant = Langton((101, 82), (y, x), orientation, conf)

    root = Tk()
    root.title("Langton's Ant: "+conf+", "+orientation+", ("+str(y+1)+", "+str(x+1)+")")
    ex = DrawGrid(root, langton_ant)
    root.mainloop()


    # for i in range(11):
    #     my_torus.next()
    #     test = np.matrix(my_torus.grid)
        # draw(test)

