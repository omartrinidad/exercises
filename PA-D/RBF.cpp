/*=========================================
# Group member (Name, Student ID, E-Mail) 
	- Omar Trinidad Gutierrez Mendez, 2850441, omar.vpa@gmail.com 
	- Shinho Kang, 2890169, wis.shinho.kang@gmail.com

# PA-D
    - To implement a RBF Network

# Instruction
    - 
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

//======================================================
// Setting Area
#define N_INPUT 2       // Number of neurons in input layer
#define K_RBF 25  	    // Number of neurons in RBF layer
                        // (K ^ N_INPUT) for uniform distribution
                        // e.g. when N_INPUT = 3, K_RBF = X ^ 3 (1,8,27, ...) 
#define M_OUTPUT 1      // Number of neurons in output layer
#define RND_SEED 5      // seed for Random number generator
						// Weight values are initialized refer to the seed
#define MAX_PATTERN 999
//======================================================


//======================================================
// Helper functions - split str
std::vector<std::string> split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if (!item.empty())
            elems.push_back(item);
    }
    return elems;
}
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
//======================================================

//======================================================
// Declaring Functions
void loadDataFile();
void constructRBF();
void setRBFCenterSize();

void trainRBF();
void calcRBFLayer();
void calcOutput();
void calcError();
//======================================================



//======================================================
// CLASSES
//================================
// class: Input Layer & Neuron
class InputLayer {
    public:
        float *input;
        InputLayer();
};
InputLayer::InputLayer() {
    input = new float[N_INPUT];
}

//================================
// class: RBF Layer & Neuron
class RBFNeuron {
    public:
        float *center;
        float size;
        float output;
        
        RBFNeuron();
};
RBFNeuron::RBFNeuron() {
    center = new float[N_INPUT];
}

enum RBFTransferfunction { GAUSSIANBELL };
class RBFLayer {
    public:
        RBFNeuron *rbfNeurons;
        RBFTransferfunction tf;
        RBFLayer();
};
RBFLayer::RBFLayer() {
    rbfNeurons = new RBFNeuron[K_RBF+1];    // include BIAS
    tf = GAUSSIANBELL;
}

//================================
// class: Output Layer & Neuron
class OutputNeuron {
    public:
        float output;
        float teacher;
        float delta;
        
        OutputNeuron();
};
OutputNeuron::OutputNeuron() {
}

class OutputLayer {
    public:
        OutputNeuron *outNeurons;
        float learningRate;
        
        OutputLayer();
        void setLearningRate(float _lr);
};
OutputLayer::OutputLayer() {
    outNeurons = new OutputNeuron[M_OUTPUT+1];  // [0] will not be used.
}
void OutputLayer::setLearningRate(float _lr) {
    learningRate = _lr;
}


//================================
// class: Weights
class Weights {
    public:
        float **weight;
        float **weight_change;
        
        Weights();
        void initWeights();
};
Weights::Weights() {
    // class constructor
    int X = K_RBF + 1;
    int Y = M_OUTPUT + 1;
    weight = new float*[X];
    weight_change = new float*[X];
    
    for (int x = 0; x < X; x++) {
        weight[x] = new float[Y];
        weight_change[x] = new float[Y];
    }    
}
void Weights::initWeights() {
    // initialize weights - random value between -0.5 and +0.5
    srand(RND_SEED); // random seed
    int X = K_RBF + 1;
    int Y = M_OUTPUT + 1;    
    for (int x = 0; x < X; x++) {
        for (int y = 1; y < Y; y++) {
            weight[x][y] = (float(rand()) / float(RAND_MAX)) - 0.5;
            weight_change[x][y] = 0;
        }
    }
}
//======================================================




//======================================================
// Declaring RBF components
InputLayer L_X;     // input layer
RBFLayer L_RBF;     // rbf layer
OutputLayer L_Y;    // output layer

Weights W;  // weights between rbf layer and output layer

// Training data
string trainingFileName;
float trainData_Input[ MAX_PATTERN ][ N_INPUT ];
float trainData_Teacher[ MAX_PATTERN ][ M_OUTPUT ];
int p_cnt = 0;      // start from 0
//======================================================


//======================================================
// main():
//      - parse arguments
//      - load data file
//      - construct RBF network
//          . set layers and neurons
//          . set RBF centers and sizes
//          . assign random values to weights 
//      - train RBF network
//          . calculate rbf neuron output
//          . calculate output (weighted sum)
//          . calculate error and print into a output file "learningcurve.dat"
//          . calculate weight changes and update weight 
int main(int argc, char** argv) {

    try {
        // arguments as filenames 
        trainingFileName = argv[1];
        
        loadDataFile();   // verify data files and load into memory
        constructRBF();   // construct RBF and setting init data
                          // include weights 
        //trainRBF();
        
    } catch(const exception& e) {
        cerr << e.what();
    } catch(const string& msg) {
        cerr << endl << "Error: " << msg << endl;
    }
	return 0;    
}


//======================================================
// loadDataFile(): load given data
// - ignore header lines (commented lines)
// - check the number of input and teacher values each line
void loadDataFile() {
	ifstream trainingFile(trainingFileName);   // read training data file
    if ( !trainingFile.eof() ) {
        int comment_cnt = 0;
        string line;
        
        while(getline(trainingFile, line)) {
            if (line[0] == '#') {
				// ingnore comment line
				comment_cnt++;
				continue;
			}
			p_cnt++;
            if (p_cnt > MAX_PATTERN) {
                break;
            }
            
            // parse a line
			vector<std::string> values = split(line, ' ');
			
            // verify number of data in a line
			if (values.size() != N_INPUT + M_OUTPUT) {
                    stringstream ess;
                    ess << "The number of data in pattern number " << p_cnt << " is not match with defined number.";
                    throw(ess.str());
            }
            
            // store data into array
            for (int i = 0; i < values.size(); i++) {
                if (i < N_INPUT) {
                    trainData_Input[p_cnt-1][i] = stof(values[i]);
                }
                else {
                    trainData_Teacher[p_cnt-1][i-N_INPUT] = stof(values[i]);
                }
            }
        }
	}
    trainingFile.close();
}

//======================================================
// Construct RBF
void constructRBF() {

    // set Layers and neurons 
    L_X = InputLayer();
    L_RBF = RBFLayer();
    L_Y = OutputLayer();
    
    // set RBF centers and sizes
    setRBFCenterSize();

    /*
    for (int k = 1; k <= K_RBF; k++) {
        cout << L_RBF.rbfNeurons[k].size << " --> ";
        cout << L_RBF.rbfNeurons[k].center[0] << " ";
        cout << L_RBF.rbfNeurons[k].center[1] << endl;
    }
    */
    
    // initialize weights and assign random values 
    W = Weights();
    W.initWeights();

}    

// set RBF centers and sizes
void setRBFCenterSize() {
    // TODO: implement setting centers and sizes for RBF neurons.
    // option 1. uniform distribution - easiest
    // option 2. statistical analysis
    // option 3. subset of data - not recommended
    
    float lower = -2;
    float upper = 2;

    int axis_x = sqrt(K_RBF);
    int axis_y = sqrt(K_RBF);

    float stepSize = (upper - lower)/axis_x;
    float sigma = stepSize/2;

    // avoid BIAS
    int k = 1;

    for (float x = lower + sigma; x <= upper; x += stepSize)  {
        for (float y = lower + sigma; y <= upper; y += stepSize) {
            L_RBF.rbfNeurons[k].center[0] = x;
            L_RBF.rbfNeurons[k].center[1] = y;
            L_RBF.rbfNeurons[k].size = sigma;
            k++;
        }
    }
}


//======================================================
// Train RBF
void trainRBF() {
    // TODO: calculate rbf neuron output
    // TODO: calculate output (weighted sum)
    // TODO: calculate error and print into a output file "learningcurve.dat"
    // TODO: calculate weight changes and update weight
    // TODO: implement stopping criterion
    calcRBFLayer();
    calcOutput();
    calcError();
}

void calcRBFLayer() {
    // calculate gaussian bell function 
    L_RBF.rbfNeurons[0].output = 1; // BIAS
    for (int k = 1; k < K_RBF+1; k++ ) {
        float dist = 0;
        float size = L_RBF.rbfNeurons[k].size; 
        for (int n = 0; n < N_INPUT; n++) {
            dist += pow(L_X.input[n] - L_RBF.rbfNeurons[k].center[n], 2);
        }
        float r = exp(-dist/(2*pow(size,2)));
        L_RBF.rbfNeurons[k].output = r;
    }
}
void calcOutput() {
    // calculate output (weighted sum)
    for (int m = 1; m < M_OUTPUT+1; m++) {
        float weightedSum = 0;
        for (int k = 0; k < K_RBF+1; k++) {
            weightedSum += L_RBF.rbfNeurons[k].output * W.weight[k][m];
        }
        L_Y.outNeurons[m].output = weightedSum;
    }
}
void calcError() {
    
}
