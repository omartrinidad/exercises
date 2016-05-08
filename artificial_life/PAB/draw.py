#!/usr/bin/python
# encoding: utf8

import numpy as np
from Tkinter import Tk, Canvas, Frame, BOTH

def clusterize_data():
    """
    Create new samples from base data
    """

    dataset = np.array([[-2, 1], [-1, -2], [-1,2], [1,1]])
    new_dataset = np.array([[-2, 1], [-1, -2], [-1,2], [1,1]])
    size = len(dataset)

    for line in dataset:
        for i in xrange(5):
            noise = line + np.random.uniform(-0.35, 0.35, 2)
            new_dataset = np.concatenate((new_dataset, [noise]), axis=0)

    return new_dataset


class Example(Frame):

    def __init__(self, parent):
        Frame.__init__(self, parent)
        self.parent = parent
        self.initUI()

    # def draw(self):
    #     # draw the board according to the current state
    #     # arrange for the next frame to draw in 4 seconds
    #     self.after(4000, self.draw)

    def initUI(self):
        self.parent.title("TNN visualization")
        self.pack(fill=BOTH, expand=1)

        canvas = Canvas(self, bg="#000")

        width, height = 500, 500
        canvas.config(width=width, height=height)
        canvas.create_line(400, 0, 0, 400, width=1, fill="#fff")

        data = clusterize_data()

        #for i in range(30):
        #    x, y = randint(1, height), randint(1, height)
        #    canvas.create_oval(x, y, x+4, y+4, outline="#0f0", fill="#0f0")

        min, max = -3, 3
        range_data = max - min
        step = 500.0/range_data

        for d in data:
            x, y = (max - d[0])*step , (max - d[1])*step
            x, y = 500 - x, 500 - y
            canvas.create_oval(x, y, x+4, y+4, outline="#0f0", fill="#0f0")

        canvas.pack(fill=BOTH, expand=1)


def main():
    root = Tk()
    # print (root)
    ex = Example(root)
    root.mainloop()


if __name__ == '__main__':
    main()

