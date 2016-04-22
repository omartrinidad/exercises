import numpy as np
import sys

# setting the parameters by the user

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
    notation = raw_input("Enter the rule in Wolfram Notation (e.g. 250): ")
    try:
        notation = int(notation)
        if notation < 1 or notation > 255:
            print("The notation must in the range 1 or 255")
        else:
            break
    except ValueError as e:
        print("The notation must beetwen 1 or 255")

neighborhood = 2 * radius + 1
states_neighborhood = 2 ** neighborhood

rule_binary = "{0:b}".format(notation)
rule_binary = rule_binary.zfill(states_neighborhood)
# inverse the string
rule_binary = rule_binary[::-1]

def calculate_new_line(lattice):

    updated_positions = { 0:[], 1:[] }
    imprimir_linea(lattice)

    for p, l in enumerate(lattice[0][2:-2]):
        window = "".join( str(int(x)) for x in lattice[0][p:p+neighborhood] )
        value = int(window, 2)

        if value:
            # enter here it means an update
            if rule_binary[value] == '1':
                updated_positions[1].append(p+1)
            else:
                updated_positions[0].append(p+1)

    # update line
    for i in updated_positions[1]:
        lattice[0][i] = 1

    for i in updated_positions[0]:
        lattice[0][i] = 0

def imprimir_linea(lattice):
    sys.stdout.write('\n')
    for t in lattice[0]:
        if t == 0.0:
           sys.stdout.write(' ')
        else:
           #sys.stdout.write('#')
           sys.stdout.write(u"\u2588")

for i in range(39):
    calculate_new_line(lattice)
