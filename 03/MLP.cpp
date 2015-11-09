/*=========================================
# Group member (Name, Student ID, E-Mail) 
	- Omar Trinidad Gutierrez Mendez, 2850441, omar.vpa@gmail.com 
	- Shinho Kang, 2890169, wis.shinho.kang@gmail.com

# PA-B
    - To implement a Multi Layer Perzeptron
    - Train MLP with training data

# Instruction
    - Please read the "README.txt" file to compile and execute this source code.    
=========================================*/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <math.h>

using namespace std;

#define N_INPUT 4       // Number of neurons in input layer ( n > 1)
#define M_OUTPUT 2      // Number of neurons in output layer ( m > 1)
#define H1_HIDDEN 30   	// Number of neurons in first hidden layer ( h1 > 1)
#define H2_HIDDEN 30   	// Number of neurons in second hidden layer 
						// ( h2 >= 0, if h2 = 0 no second hidden layer)

#define RND_SEED 5      // seed for Random number generator (which is for initialize weights)




enum Transferfunction { TANH, LOGISTIC, IDENTITY };

public class Neuron {
	float net_val;		// weighted sum of inputs to this neuron
	float output;		// output value of the neuron which indicates f(net_val)  
	float delta;
}
class Layer {
	Transferfunction tf;	// transfer function of neurons in the layer 
	Neuron *neurons;		// 
	int num_neurons;		// number of neurons in the layer 
} 
class Weights {
	float **weights;
}

void constructMLP();

int main(int argc, char** argv){
	
	Layer L_In;
	Layer L_H1;
	Layer L_H2;
	Layer L_Out;
	Weights W_X_H1;
	//Weights W_
	
	constructMLP(L_In, L_H1, L_H2, L_Out);
	initweights() 
	return 0;
} 

void constructMLP(Layer& L_In, Layer& L_H1, Layer& L_H2, Layer& L_Out) {
		// Initialize neurons in each layer
		// .....
		
		// 
		// .....
}