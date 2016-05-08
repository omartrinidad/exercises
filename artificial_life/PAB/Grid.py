'''
'''

import numpy as np

class Langton(object):

    def __init__(self, grid, start, orientation='up', conf='white'):
        """
        Initialize parameters
        """
        self.__rows = grid[0]
        self.__cols = grid[1]
        self.__orientation = orientation
        self.__conf = conf
        self.__current = start
        self.grid = np.zeros((self.__rows, self.__cols))

    def findNextCell(self):
        pass

    def current(self):

        return self.grid[self.__current]

    def next(self):

        # black
        print self.current():
        if self.current():
            pass
        # white
        else:
            pass
