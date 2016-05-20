class F:
    def __init__(self, params, x0, y0, steps):
        self.params = params
        self.x = [x0]
        self.y = [y0]
        self.steps = steps

    def do_function(self, step=0):

        if step < self.steps:
            x = self.x[step] + self.params['a'] * self.x[step] +\
                self.params['b'] * self.y[step] +\
                self.params['e'] * self.x[step] * self.x[step]
                # + self.params['g'] * self.x[step] * self.y[step]
            self.x.append(x)

            y = self.y[step] + self.params['c'] * self.x[step] +\
                self.params['d'] * self.y[step] +\
                self.params['f'] * self.y[step] * self.y[step]
                # + self.params['h'] * self.x[step] * self.y[step]
            self.y.append(y)

            self.do_function(step + 1)

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


parameters = {
              'e': -0.015, 'f': 0.015,
              'a': 0.16, 'd': -0.16,
              'b': -0.59, 'c': -0.59,
              #'g': 0.0, 'h': 0.0
             }
x0 = 10
y0 = 3
steps = 200

X = F(params=parameters, x0=x0, y0=y0, steps=steps)
X.do_function(0)
X.printResult()
X.save_on_file()
