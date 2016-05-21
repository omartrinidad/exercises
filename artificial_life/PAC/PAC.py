class F:
    def __init__(self, params, x0, y0, steps):
        self.params = params
        self.x = [x0]
        self.y = [y0]
        self.steps = steps

    def do_function(self):

        for step in range(0, self.steps):
            x = self.x[step] + self.params['a'] * self.x[step] + \
                self.params['b'] * self.y[step] + \
                self.params['e'] * self.x[step] * self.x[step] + \
                self.params['g'] * self.x[step] * self.y[step]
            self.x.append(x)

            y = self.y[step] + self.params['c'] * self.x[step] + \
                self.params['d'] * self.y[step] + \
                self.params['f'] * self.y[step] * self.y[step] + \
                self.params['h'] * self.x[step] * self.y[step]
            self.y.append(y)

    def __repr__(self):
        s = "<parameters a:%s b:%s c:%s d:%s e:%s f:%s g:%s h:%s>" % \
                (self.params['a'], self.params['b'], self.params['c'], \
                self.params['d'], self.params['e'], self.params['f'], \
                self.params['g'], self.params['h'])
        return s

    def printResult(self):
        print "X: "
        print [i for i in self.x]
        print "Y:"
        print [i for i in self.y]

    def save_on_file(self):
        results = open("results.csv", "w+")
        for i, (x, y) in enumerate(zip(self.x, self.y)):
            results.write("{}, {}, {}\n".format(i, x, y))
        results.close()



# TEST 1 : source http://www.ssc.wisc.edu/~jmontgom/predatorprey.pdf
parameters = {'a': 1.3,
              'b': 0.0,
              'e': -1.3,
              'g': -0.5,

              'c': 0.0,
              'd': -0.7,
              'f': 0.0,
              'h': 1.6
             }

x0 = 1
y0 = 1
steps = 200

X = F(params=parameters, x0=x0, y0=y0, steps=steps)
X.do_function()
X.printResult()
X.save_on_file()


# TEST 2 : source http://www.ssc.wisc.edu/~jmontgom/predatorprey.pdf
# parameters = {'a': 0.013,
#               'b': 0.0,
#               'e': -0.013,
#               'g': -0.005,
#
#               'c': 0.0,
#               'd': -0.007,
#               'f': 0.0,
#               'h': 0.016
#               }
#
# x0 = 1
# y0 = 1
# steps = 10000
#
# X = F(params=parameters, x0=x0, y0=y0, steps=steps)
# X.do_function()
# X.printResult()
# X.save_on_file()
