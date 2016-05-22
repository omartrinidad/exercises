class F:
    def __init__(self, params, x0, y0, steps):
        self.params = params
        self.x = [x0]
        self.y = [y0]
        self.steps = steps
        self.stable_x = 0
        self.stable_y = 0

    def do_function(self):

        counter_x, counter_y = 0, 0

        for step in range(0, self.steps):

            x = self.x[step] + self.params['a'] * self.x[step] + \
                self.params['b'] * self.y[step] + \
                self.params['e'] * self.x[step] * self.x[step] + \
                self.params['g'] * self.x[step] * self.y[step]

            self.x.append(x)

            """
            When the changes in the values are less than 0.01, the stable
            oscillation has been reached
            """
            if counter_x < 10:
                if abs(self.x[-1] - self.x[-2]) < 0.01:
                    counter_x += 1
                else:
                    counter_x = 0
            elif counter_x == 10:
                self.stable_x = step
                print "Stable oscillation reached on step %i" % (step - 10)
                counter_x += 1

            y = self.y[step] + self.params['c'] * self.x[step] + \
                self.params['d'] * self.y[step] + \
                self.params['f'] * self.y[step] * self.y[step] + \
                self.params['h'] * self.x[step] * self.y[step]

            self.y.append(y)

            if counter_y < 10:
                if abs(self.y[-1] - self.y[-2]) < 0.01:
                    counter_y += 1
                else:
                    counter_y = 0
            elif counter_y == 10:
                self.stable_y = step
                print "Stable oscillation reached on step %i" % (step - 10)
                counter_y += 1



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

    def gnuplot(self):
        """
        """
        from os import system, remove

        gnuplot_string = """
            set terminal jpeg
            set output 'plot.jpg'
            set grid
            set title "Predator-Prey Model"
            set xlabel "Steps"
            set ylabel "Population"

            set arrow from {0},0.2 to {0},2.2 nohead lc rgb "red"
            set arrow from {1},0.2 to {1},2.2 nohead lc rgb "green"

            plot 'results.csv' u 1:2 with lines t 'prey',\
                 'results.csv' u 1:3 with lines t 'predator'
            pause -1
            """.format(self.stable_x, self.stable_y)

        gnuplot_file = open("plot.gp", "w+")
        gnuplot_file.write(gnuplot_string)
        gnuplot_file.close()

        system('gnuplot plot.gp')
        remove('plot.gp')


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
X.gnuplot()


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
#X.mean_population()
# X.gnuplot()
