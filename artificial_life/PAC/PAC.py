

class F:
    def __init__(self, params, x0, y0, steps):
        self.params = params
        self.x[0] = x0
        self.y[0] = y0
        self.steps = steps

    def do_function(self, step=0):

        if step < self.steps:
            self.x[step+1] = self.x[step] + self.params['a'] * self.x[step] + self.params['b'] * self.y[step] + \
                             self.params['e'] * self.x[step] * self.x[step] + self.params['g'] * self.x[step] * self.y[step]
            self.y[step+1] = self.y[step] + self.params['c'] * self.x[step] + self.params['d'] * self.y[step] + \
                             self.params['f'] * self.y[step] * self.y[step] + self.params['h'] * self.x[step] * self.y[step]
            self.do_function(step + 1)

    def __repr__(self):
        s = "<parameters a:%s b:%s c:%s d:%s e:%s f:%s g:%>" % (self.a, self.b)
        for p in self.params:
            s += p.name
        return text


parameters = {'a': 0.1, 'b':0.1, 'c':0.1, 'd':0.1, 'e':0.1, 'f':0.1, 'g':0, 'h':0}
x0 = 1
y0 = 1
steps = 10
X = F(params=parameters, x0=x0, y0=y0, steps=steps)
X.do_function(0)

print (X)