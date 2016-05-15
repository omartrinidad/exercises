class F:
    def __init__(self, params, x0, y0, steps):
        self.params = params
        self.x = [x0]
        self.y = [y0]
        self.steps = steps

    def do_function(self, step=0):

        if step < self.steps:
            x = self.x[step] + self.params['a'] * self.x[step] + self.params['b'] * self.y[step] + \
                             self.params['e'] * self.x[step] * self.x[step] + self.params['g'] * self.x[step] * self.y[step]
            self.x.append(x)
            y = self.y[step] + self.params['c'] * self.x[step] + self.params['d'] * self.y[step] + \
                             self.params['f'] * self.y[step] * self.y[step] + self.params['h'] * self.x[step] * self.y[step]
            self.y.append(y)
            self.do_function(step + 1)

    def __repr__(self):
        # s = "a:%s b:%s" % (self.params['a'], self.params['b'])
        s = "<parameters a:%s b:%s c:%s d:%s e:%s f:%s g:%s h:%s>" % (self.params['a'], self.params['b'], self.params['c'], \
            self.params['d'], self.params['e'], self.params['f'], \
            self.params['g'], self.params['h'])
        return s

    def printResult(self):
        print "X"
        for i in self.x:
            print "%s " % i
        print "Y"
        for i in self.y:
            print "%s " % i

parameters = {'a': 0.1, 'b': -0.1, 'c': 0.1, 'd': -0.1, 'e': 0.1, 'f': 0.1, 'g': 0.0, 'h': 0.0}
x0 = 1.0
y0 = 1.0
steps = 10
X = F(params=parameters, x0=x0, y0=y0, steps=steps)

print (X)
X.do_function(0)
X.printResult()
