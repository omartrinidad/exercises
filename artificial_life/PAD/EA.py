import numpy as np
import math

"""
# Group members (Name, Student ID, E-Mail):
    - Shinho Kang, 2890169, wis.shinho.kang@gmail.com
    - Omar Trinidad Gutiérrez Méndez, 2850441, omar.vpa@gmail.com
# Instructions to execute this program:
    - Run in the command line: `python EA.py`
    - Be careful to have numpy library installed
    - The program was tested in Python version : 2.7.10
"""


class Individual:
    """
    class Individual consists of genome and fitness value
    """
    def __init__(self, L):
        """
        initialize Individual class
        :param L: length of genome vector
        """
        self.L = L
        self.genome = np.zeros(L)
        self.fitness = 0
        pass

    def initialize_genome(self):
        """
        initialize genome: assign random real values to genome
        """
        self.genome = np.random.random_sample(self.L)

    def inherit_genome(self, genome):
        """
        just copy the parent genome to the newly generated genome
        :param genome: parent genome
        """
        self.genome = np.array(genome)

    def mutation(self, bound):
        """
        mutation adding uniform distribution r to the genome
        :param bound: bound
        :return:
        """
        r = np.random.uniform(-1*bound, bound, self.L)
        self.genome = self.genome + r

    def fitness_evaluation(self):
        """
        here we use Hosaki function - with only 2 dim.
        :return: result of Hosaki function
        """
        x1 = self.genome[0]
        x2 = self.genome[1]
        self.fitness = (1 - 8*x1 + 7*(x1**2) - 7.0/3*(x1**3) + 1.0/4*(x1**4)) * (x2**2) * math.exp(-1*x2)
        return self.fitness

    def print_ind(self, lable = ""):
        """
        print information about the individual
        """
        print lable, "    Genome: ", self.genome, "Fitness Value: ", self.fitness
        pass


class EA:
    def __init__(self):
        """
        setting environments.
        L should be 2 because the fitness function (hosaki's function) is valid only for 2 dimension.
        """
        self.P = 2
        self.L = 2
        self.mutation_bound = 1

    def getKey(self, item):
        """
        function for sorting
        """
        return item.fitness

    def process(self, descending):
        """
        process EA steps.
        :param descending: Finding Maximum of function, if True
                            Finding Minimum of function, if False
        """
        # Initialize the first individuals
        population = [None] * self.P
        for i in range(self.P):
            population[i] = Individual(self.L)  # generate individual
            population[i].initialize_genome()   # initialize genome by random assigning
            population[i].fitness_evaluation()  # fitness evaluation of each individual
            population[i].print_ind("Initial Individual")

        # first External Selection - sorting populations by their fitness values
        # The first individual of sorted population is the fittest
        sorted(population, key=self.getKey, reverse=descending)

        step, stop_count = 0, 0
        """
        iterate EA generations till it satisfy the stop criteria
        """
        while True:
            step += 1
            # parent selection - first individual of sorted population
            parent = population[0]
            population[0].print_ind("Parent".format(step))

            prev_fit = population[0].fitness    # will use for stop criteria
            # inheritance, mutation and fitness evaluation for each of child
            for i in range(1, self.P):
                population[i] = Individual(self.L)  # generate individual
                population[i].inherit_genome(parent.genome)     # inheritance of genome
                population[i].mutation(self.mutation_bound)     # mutation
                population[i].fitness_evaluation()      # fitness evaluation
                population[i].print_ind("Child")

            # external selection - sorting populations by their fitness values
            population = sorted(population, key=self.getKey, reverse=descending)
            after_fit = population[0].fitness

            # stop criteria:
            #  - if the fittest doesn't make progress for 100 sequential generations
            #  - if the total generation is over 1000
            if prev_fit == after_fit:
                stop_count += 1
            else:
                stop_count = 0
            if stop_count > 100 or step > 1000:
                break

        population[0].print_ind("FINAL Generation {}".format(step))
        pass

ea = EA()
print "=========================="
print "Finding Maximum Optima"
print "=========================="
ea.process(descending=True)

# Finding maximum of the function looks non-sense.
# So I implemented both - finding maximum and minimum

print "Finding Minimum Optima"
ea.process(descending=False)
print "=========================="
