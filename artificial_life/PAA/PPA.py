#/usr/env/bin python
# encoding: utf8

"""
# Group members (Name, Student ID, E-Mail):
    - Shinho Kang, 2890169, wis.shinho.kang@gmail.com
    - Omar Trinidad Gutiérrez Méndez, 2850441, omar.vpa@gmail.com

# Instructions to execute this program:

    - Run in the command line: `python PAA.py `
    - Be careful to have numpy library installed
    - The program was tested in Python version : 2.7.10
    - The user will be asked for the parameters:
        - radius,
        - starting condition, and
        - Wolfram number

"""

import numpy as np
import sys

# setting of the parameters by the user
lattice = np.zeros([1, 84])

while True:
    radius = raw_input("Enter the neighborhood radius 1 or 2: ")
    try:
        radius = int(radius)
        if radius != 1 and radius != 2:
            print("The radius must be 1 or 2")
        else:
            break
    except ValueError as e:
        print("The radius must be 1 or 2")

while True:
    starting_condition = raw_input("Enter the starting condition S or R: ")
    if starting_condition != 'S' and starting_condition != 'R':
        print("The starting condition must be S or R")
    else:
        if starting_condition == 'S':
            lattice[0][41] = 1
        # starting condition R
        else:
            for i in xrange(84):
                if np.random.rand() <= 0.5:
                    lattice[0][i] = 1
        break

while True:
    neighborhood = 2 * radius + 1
    states_neighborhood = 2 ** neighborhood
    possible_rules = 2 ** states_neighborhood
    maximum = possible_rules - 1
    notation = raw_input("Enter the rule in Wolfram Notation, in the range "\
            "0-%s: " % maximum)
    try:
        notation = int(notation)
        if notation < 0 or notation > maximum:
            print("The notation must be in the range 0 or %s" % maximum)
        else:
            break
    except ValueError as e:
        print("The notation must beetwen 0 and %s" % maximum)


rule_binary = "{0:b}".format(notation)
rule_binary = rule_binary.zfill(states_neighborhood)
# inverse the string
rule_binary = rule_binary[::-1]


def calculate_new_line(lattice):
    """
    In this function we calculate the values for the next iteration
    """
    updated_positions = { 0:[], 1:[] }
    print_line(lattice)

    for p, l in enumerate(lattice[0][2:-2]):
        window = "".join( str(int(x)) for x in lattice[0][p:p+neighborhood] )
        value = int(window, 2)

        if value:
            # enter here it means that an update is needed
            if rule_binary[value] == '1':
                updated_positions[1].append(p+1)
            else:
                updated_positions[0].append(p+1)

    # update line
    for i in updated_positions[1]:
        lattice[0][i] = 1

    for i in updated_positions[0]:
        lattice[0][i] = 0


def print_line(lattice):
    """
    Function to print the line in each iteration
    """
    for t in lattice[0]:
        if t == 0.0:
           sys.stdout.write(' ')
           output.write(' ')
        else:
           e = u"\u2588"
           sys.stdout.write(e.encode('utf-8'))
           output.write(e.encode('utf-8'))

    sys.stdout.write('\n')
    output.write('\n')


# opening file to put the output
output = open(str(radius) + starting_condition + str(notation) + ".txt", 'w')

for i in range(39):
    calculate_new_line(lattice)

output.close()
