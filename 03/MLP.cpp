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

//=====================
// Setting Area
#define LAYER_NUM 4		// 3: N-H-M , 4: N-H1-H2-M
#define N_INPUT 4       // Number of neurons in input layer (1 < n <=1000)
#define M_OUTPUT 2      // Number of neurons in output layer (1 < m <= 1000)
#define H1_HIDDEN 30   	// Number of neurons in first hidden layer (1 < h1 <=1000)
#define H2_HIDDEN 30   	// Number of neurons in second hidden layer (1 < h2 <= 1000) 
						// this value is valid only when LAYER_NUM is 4.
#define RND_SEED 5      // seed for Random number generator 
						// Weight values are initialized refer to the seed
#define TRAINING_PATTERN 1000 // number of training patterns up to 1000  												 
//=====================

enum Transferfunction { TANH, LOGISTIC, IDENTITY };

// Neuron class
class Neuron {
    public:
        float net_val;		// weighted sum of inputs to this neuron
        float output;		// output value of the neuron which indicates f(net_val)
        float teacher;		//
        float delta;		//
        
        /* constructor  definition */
        Neuron() {
            net_val = 0.0;
            delta = 0.0;
            output = 0.0;
            teacher = 0.0;
        }
};


// Layer class
class Layer {
    public:
        Neuron *neurons;		// neurons in the layer 
        int num_neurons;		// number of neurons in the layer
        Transferfunction tf;	// transfer function of neurons in the layer 
        float learningRate; 	// Learning Rate - BP 

        /* constructors declaration */
        Layer();
        Layer(int num_neurons);
};


/* constructors definition */
Layer::Layer() { } 

Layer::Layer(int init) {

    Neuron *neurons[init];
    num_neurons = init;
    // Transferfunction = new Transferfunction();
    // learningRate = 0.0;

    for (int i = 0; i < init; i++) {
        neurons[i] = new Neuron();
    }
}


// Weights class
class Weights {

    public:
        float **weights;
        float **weight_changes;

        /* constructors definition */
        Weights(){ }

        Weights(int H, int M) {

            srand(RND_SEED);
            weights = new float*[H];
            weight_changes = new float*[H];

            for (int h = 0; h < H; h++) {
                weights[h] = new float[M];
                weight_changes[h] = new float[M];
            }

            for (int h = 0; h < H; h++) {
                for (int m = 0; m < M; m++) {
                    weights[h][m] = (float) rand()/RAND_MAX - 2.0;
                    weight_changes[h][m] = 0;
                    //cout << weights[h][m] << "\t";
                }
                //cout << "\n------------" << "";
                //cout << "" << endl;
            }
        }
};


void constructMLP();	// construct MLP
void initWeights();		// initialize weights
void verifyDataFile(); 	// verify given training.dat file and test.dat file 
void training();		// training using training.dat file 
void validation();		// validate performance using test.dat file 

/////============================
// Declare MLP components class
Layer L_X;	
Layer L_H1;
Layer L_H2;
Layer L_Y;
Weights W_XtoH;
Weights W_HtoH;
Weights W_HtoY;

/////============================
int main(int argc, char** argv) {
	constructMLP();
	initWeights(); 
	return 0;
} 

void constructMLP() {
	// Initialize neurons in each layer using classes
    // the number of layers and the number of neurons 
    // should be set as the defined numbers
    
    L_X = Layer(N_INPUT);
    L_H1 = Layer(H1_HIDDEN);
    L_H2 = Layer(H2_HIDDEN);
    L_Y = Layer(M_OUTPUT);

}

void initWeights() {

	// Initialize weights
	// Weight values should be -2.0 < w < +2.0 random numbers using RND_SEED
    Weights W_XtoH = Weights(N_INPUT, H1_HIDDEN);
    Weights W_HtoH = Weights(H1_HIDDEN, H2_HIDDEN);
    Weights W_HtoY = Weights(H2_HIDDEN, M_OUTPUT);
	 
}

void verifyDataFile() {
	// // verify given training.dat file and test.dat file
	// - check the number of input and teach values each line
	// - check if there is no blank line
	// - check if the number of pattern is more than defined number
	// - check header line (2 lines) 
}

void training() {
	// read training data
	// do the process training line by line
	
	// Use the sum over quadratic differences as error function
	// Calculate the error in every training step, 
	// and print it as a learning curve into a file learning.curve during the training process. 
	// Choose a format that can easily be depicted using the freely available program gnuplot.
	  
}

void validation() {
	// read test data
	// write the performance result to file
	
}
