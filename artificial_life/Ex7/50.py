"""
# Assignment 50

    - Generate P random vectors in d dimensional unit hypercube. each component
      of vectors distributed in the interval [0,1]

    - Vectors are points in the hypercube

    - Calculate average and the distribution of the angle a between
      these random vectors and one space diagonal vector.

# Instruction
    - to execute this program
        ` python 50.py `
    - python version : 2.7.10

# Result:
# Average Euclidean distance :  12.9141304763
# Distribution is normal
# Average of angle:  0.52357074435

"""

import time, math
from random import shuffle
import numpy as np

#====================================================
# P: number of random vectors (each vector is a point in the unit-hypercube)
P = 3000 # 10000
# N: dimension of the hypercube
N = 1000 # 1000

# list of vectors
w_V = np.zeros((P, N), dtype=np.float64)
# list of vector's length
w_L = np.zeros(P, dtype=np.float64)
# list of vector's angle (between v and space diagonal vector)
w_A = np.zeros(P, dtype=np.float64)

w_E = []
diagonal_vector = np.full((N), 1, dtype=np.float64)

#====================================================
# generate P vectors in n-dimensional hypercube
# each component of vectors should be equally distributed in the interval [0,1]

def get_rand_nums():
    """
    Equally distributed random numbers
    """
    frame = range(0,P,1)
    shuffle(frame)
    return frame

def generate_random_vectors():
    for d in range(0, N):
        numList = get_rand_nums()
        for p in range(0, P):
             w_V[p][d] = float(numList[p]) / float(P-1)

generate_random_vectors()

#====================================================
# functions for calculation
# calculate Euclidean length of a vector
def euclidean_len(v):
    # sqrt(sum(p^2)), where p are components of vector
    return math.sqrt(sum(v*v))

# calculate Euclidean distance between 2 vectors(points)
def euclidean_dist(v1, v2):

    # sqrt(sum(p-q)**2), where p are components of vector1, q are components of
    # vector2
    # to speed up, we used numpy.linalg.norm function
    # math.sqrt( sum( ((a-b)**2 ) for a,b in zip(v1, v2) ))

    return  np.linalg.norm(v1-v2)

# calculate angle between vector1 and vector2
len_diagonal = euclidean_len(diagonal_vector)
def angle(v1, v2):
    # cos(angle) = v1*v2 / ( len(v1)*len(v2) )
    # return math.acos( sum(v1*v2) / (euclidean_len(v1) * euclidean_len(v2)))
    # to speed up - modified code
    return math.acos( sum(v1) / (euclidean_len(v1) * len_diagonal))

#====================================================
# calculate values

for p1 in range(0, P):

    # calculate length of each vector
    w_L[p1] = euclidean_len(w_V[p1])

    # calculate angle between each vector and space diagonal vector
    w_A[p1] = angle(w_V[p1], diagonal_vector)

    # calculate distance between vectors
    for p2 in range(p1+1, P):
        dist = euclidean_dist(w_V[p1], w_V[p2])
        w_E.append(dist)

ev_L = sum(w_L)/len(w_L)
ev_A = sum(w_A)/len(w_A)

tmp = sum(val for val in w_E)
ev_E = tmp / len(w_E)

print "Average angle: ", ev_A
print "Average Euclidean distance : ", ev_E
