import Tkinter as tk
import random

class App(tk.Tk):
    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)
        self.canvas = tk.Canvas(self, width=820, height=1010, borderwidth=0, highlightthickness=0)
        self.canvas.pack(side="top", fill="both", expand="true")
        self.rows = 101
        self.columns = 82
        self.cellwidth = 10
        self.cellheight = 10

        self.rect = {}
        self.oval = {}
        for column in range(self.columns):
            for row in range(self.rows):
                x1 = column*self.cellwidth
                y1 = row * self.cellheight
                x2 = x1 + self.cellwidth
                y2 = y1 + self.cellheight
                self.rect[row,column] = self.canvas.create_rectangle(x1,y1,x2,y2, fill="white", tags="rect")
                self.oval[row,column] = self.canvas.create_oval(x1+2,y1+2,x2-2,y2-2, fill="white", tags="oval")

        self.redraw(1000)

    def redraw(self, delay):
        self.canvas.itemconfig("rect", fill="white")
        self.canvas.itemconfig("oval", fill="white")
        for i in range(10):
            row = random.randint(0,19)
            col = random.randint(0,19)
            item_id = self.oval[row,col]
            self.canvas.itemconfig(item_id, fill="red")
        self.after(delay, lambda: self.redraw(delay))

if __name__ == "__main__":
    app = App()
    app.mainloop()