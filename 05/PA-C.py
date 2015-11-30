"""
# Group member (Name, Student ID, E-Mail) 
	- Omar Trinidad Gutierrez Mendez, 2850441, omar.vpa@gmail.com 
	- Shinho Kang, 2890169, wis.shinho.kang@gmail.com

# PA-C
	- Generate P random vectors in d dimensional unit hypercube. 
	  each component of vectors equally distributed from the interval [0,1]
	- Calculate expected value and the distribution of the Euclidean length L of these random vectors
	- Calculate expected value and the distribution of the angle a between these random vectors and the space diagonal vector.
	- Vectors as points in the hypercube, calculate expected value and the distribution of the Euclidean length E between these points.

# Instruction
    - to execute this program
		` python PA-C.py `
	- python version : 2.7.10
	
	- setting P = 10000 makes the program work too long. (more than 30 min. and couldn't find a solution with native python libraries)
	- setting P = 1000, D = 1000 takes about 13 sec. to calculate
	- setting P = 2000, D = 1000 takes about 43 sec. to calculate
		
	- export output files
	 => vectorLength.dat : length of vectors
	 => vectorAngle.dat : angle between vector and space diagonal vector 
	 => distance.dat : distance between vectors
"""
import time;
import math
from random import shuffle
import numpy as np
start_time = time.time()

#====================================================
# P: number of random vectors
# d: dimension of the hypercube
P = 2000 # 10000
D = 1000 # 1000

# list for vectors
w_V = np.zeros((P,D), dtype=np.float64)  # list of vectors
w_L = np.zeros(P, dtype=np.float64) # list of vector's length
w_A = np.zeros(P, dtype=np.float64) # list of vector's angle (between v and space diagonal vector)
w_E = []
diagonal_Vector = np.full((D), 1, dtype=np.float64)

#====================================================
# generate P vectors in d-dimensional hypercube
# each component of vectors should be equally distributed from the interval [0,1] 
def get_equally_distributed_rand_nums():
    frame = range(0,P,1)
    shuffle(frame)
    return frame

def generate_random_vectors():
	for d in range(0, D):
		numList = get_equally_distributed_rand_nums()
		for p in range(0, P):
			 w_V[p][d] = float(numList[p]) / float(P-1)	
	return 0

generate_random_vectors()

#====================================================
# functions for calculation
# calculate Euclidean length of a vector
def euclidean_len(v):
	# sqrt(sum(p^2)), where p are components of vector
	return math.sqrt(sum(v*v))

# calculate Euclidean distance between 2 vectors(points)
def euclidean_dist(v1, v2):
	# sqrt(sum(p-q)**2), where p are components of vector1, q are components of vector2
	# to speed up, we used numpy.linalg.norm function
	return  np.linalg.norm(v1-v2) #  math.sqrt( sum( ((a-b)**2 ) for a,b in zip(v1, v2) ))
	
# calculate angle between vector1 and vector2 
len_diagonal = euclidean_len(diagonal_Vector)
def angle(v1, v2):
	# cos(angle) = v1*v2 / ( len(v1)*len(v2) )
  	# return math.acos( sum(v1*v2) / (euclidean_len(v1) * euclidean_len(v2)))
	# to speed up - modified code
	return math.acos( sum(v1) / (euclidean_len(v1) * len_diagonal))

#====================================================
# calculate values 
f_L = open('vectorLength.dat','w')
f_A = open('vectorAngle.dat','w')
f_E = open('distance.dat','w')

for p1 in range(0, P):
	# calculate length of each vector
	w_L[p1] = euclidean_len(w_V[p1])
	f_L.write(str(w_L[p1]))
	f_L.write("\n")

	# calculate angle between each vector and space diagonal vector
	w_A[p1] = angle(w_V[p1], diagonal_Vector)
	f_A.write(str(w_A[p1]))
	f_A.write("\n")
	
	# calculate distance between vectors
	for p2 in range(p1+1, P):
		dist = euclidean_dist(w_V[p1], w_V[p2])
		w_E.append(dist)
		f_E.write(str(dist))
		f_E.write("\n")

f_L.close()
f_A.close()
f_E.close()
#====================================================
# calculate expected values from the calculated values above  
print "calculate expected values"

ev_L = sum(w_L)/len(w_L)
ev_A = sum(w_A)/len(w_A)

tmp = sum(val for val in w_E )
ev_E = tmp / len(w_E)

print "Expected Length : ", ev_L
print "Expected Angle : ", ev_A
print "Expected Euclidean distance : ", ev_E

#====================================================
# calculate standard deviation
# standard_deviation = sqrt (1/N * sum(x-mean))
print "calculate standard deviation"
tmp_L = 0
tmp_A = 0
tmp_E = 0
for val in w_L:
	tmp_L += (val - ev_L)**2
for val in w_A:	
	tmp_A += (val - ev_A)**2
for val in w_E:
	tmp_E += (val - ev_E)**2
	
sd_L = math.sqrt(tmp_L / len(w_L))
sd_A = math.sqrt(tmp_A / len(w_A))
sd_E = math.sqrt(tmp_E / len(w_E))# w_E.size)

print "Standard Deviation of L : ", sd_L
print "Standard Deviation of A : ", sd_A
print "Standard Deviation of E : ", sd_E

#====================================================
print("\n--- %s seconds ---" % (time.time() - start_time))
