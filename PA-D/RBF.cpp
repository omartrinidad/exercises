/*=========================================
# Group member (Name, Student ID, E-Mail) 
	- Omar Trinidad Gutierrez Mendez, 2850441, omar.vpa@gmail.com 
	- Shinho Kang, 2890169, wis.shinho.kang@gmail.com

# PA-D
    - To implement a RBF Network
    - 

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

//=====================
// Setting Area
#define N_INPUT 2       // Number of neurons in input layer
#define K_RBF 2   	// Number of neurons in RBF layer
#define M_OUTPUT 1      // Number of neurons in output layer
#define RND_SEED 5      // seed for Random number generator 
						// Weight values are initialized refer to the seed  												 
//=====================

//=====================
// Helper functions - split str
std::vector<std::string> split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

//=====================
// Input Layer & Neuron
class InputLayer {
    public:
        float *input;
        InputLayer();
};
InputLayer::InputLayer() {
    input = new float[N_INPUT];
}

//=====================
// RBF Layer & Neuron
class RBFNeuron {
    public:
        float *center;
        float size;
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

//=====================
// Output Layer & Neuron
class OutputNeuron {
    public:
        float netVal;
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
        OutputLayer();
};
OutputLayer::OutputLayer() {
    outNeurons = new OutputNeuron[M_OUTPUT+1];  // [0] will not be used.
}


//=====================
// Weights
class Weights {
    public:
        float **weights;
        float **weight_changes;
        Weights();
        
        void initWeights();
};
Weights::Weights() {
    srand(RND_SEED); // random seed
    int X = K_RBF + 1;
    int Y = M_OUTPUT + 1;
    weights = new float*[X];
    weight_changes = new float*[X];
    
    for (int x = 0; x < X; x++) {
        weights[x] = new float[Y];
        weight_changes[x] = new float[Y];
    }    
}

void Weights::initWeights() {
    // cout << "============================" << endl;
    // cout << "Weight Initiate" << endl;
    int X = K_RBF + 1;
    int Y = M_OUTPUT + 1;    
    for (int x = 0; x < X; x++) {
        for (int y = 1; y < Y; y++) {
            weights[x][y] = (float(rand()) / float(RAND_MAX)) - 0.5;
            // cout << weights[x][y] << " ";
            weight_changes[x][y] = 0;
        }
        // cout << "" << endl;
    }
    // cout << "============================" << endl;

}

/////============================
// Declare RBF components



void verifyDataFile();
void constructRBF();
void trainRBF();
void validate();

string trainingFileName;
string testFileName;
// float trainingPatterns_Input[ MAX_TRAINING_PATTERN ][ N_INPUT ];
// float trainingPatterns_Teacher[ MAX_TRAINING_PATTERN ][ M_OUTPUT ];
// float testPatterns_Input[ MAX_TRAINING_PATTERN ][ N_INPUT ];
// float testPatterns_Teacher[ MAX_TRAINING_PATTERN ][ M_OUTPUT ];

/////============================
int main(int argc, char** argv) {
    try {
        // arguments as filenames 
        trainingFileName = argv[1];
        testFileName = argv[2];
        
        verifyDataFile();   // verify data files and load into memory
        constructRBF();     // construct RBF and setting init data
                            // include weights 
        trainRBF();
        validate();
    } catch(const exception& e) {
        cerr << e.what();
    } catch(const string& msg) {
        cerr << endl << "Error: " << msg << endl;
    }
	return 0;    
}






/////============================
// Verify datafiles and load the data
void verifyDataFile() {
	// verify given files
	// - ignore header lines (commented lines)
	// - check the number of input and teacher values each line
	// - check if the number of pattern is more than defined number
/*	
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
			// parse a line
			vector<std::string> values = split(line, ' ');
			
			// store data into array
			bool b_input = true;
            int index = 0;
			for (int i =0; i < values.size(); ++i) {
				if (values[i].empty()){
					if (b_input) index = 0;
                    b_input = false;
                    continue;
				}
                // cout << values[i]<< " ";
                if (b_input && index >= N_INPUT) {
                    stringstream ess;
                    ess << "Too many input values in pattern " << p_cnt;
                    throw(ess.str()); 
                }
                if (!b_input && index >= M_OUTPUT) {
                    stringstream ess;
                    ess << "Too many teacher values in pattern " << p_cnt;
                    throw(ess.str()); 
                }
                
                if (b_input)  
                    trainingPatterns_Input[p_cnt][index++] = stof(values[i]);
                else trainingPatterns_Teacher[p_cnt][index++] = stof(values[i]);
			}	
            //cout << endl;
			p_cnt++;
        }
	}
    trainingFile.close();
    // cout << "============================" << endl;
    // cout << "training patterns" << endl;
    // for (int i=0; i < p_cnt; i++) {
    //     for (int j=0; j < N_INPUT; j++) {
    //         cout << trainingPatterns_Input[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;

    // for (int i=0; i < p_cnt; i++) {
    //     for (int j=0; j < M_OUTPUT; j++) {
    //         cout << trainingPatterns_Teacher[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl << endl;


	ifstream testFile(testFileName);   // read test data file    
    if ( !testFile.eof() ) {
        int comment_cnt = 0;
    	string line;
        
        while(getline(testFile, line)) {
			if (line[0] == '#') {
				// ingnore comment line
				comment_cnt++;
				continue;
			}
			// parse a line
			vector<std::string> values = split(line, ' ');
			
			// store data into array
			bool b_input = true;
            int index = 0;
			for (int i =0; i < values.size(); ++i) {
				if (values[i].empty()){
					if (b_input) index = 0;
                    b_input = false;
                    continue;
				}
                // cout << values[i] << " ";
                if (b_input && index >= N_INPUT) {
                    stringstream ess;
                    ess << "Too many input values in pattern " << p_cnt;
                    throw(ess.str()); 
                }
                if (!b_input && index >= M_OUTPUT) {
                    stringstream ess;
                    ess << "Too many teacher values in pattern " << p_cnt;
                    throw(ess.str()); 
                }                
                if (b_input)  
                    testPatterns_Input[p_test_cnt][index++] = stof(values[i]);
                else testPatterns_Teacher[p_test_cnt][index++] = stof(values[i]);
			}	
			p_test_cnt++;
            // cout << endl;
        }
	}
    testFile.close();

    // cout << "============================" << endl;
    // cout << "test patterns" << endl;
    // for (int i=0; i < p_test_cnt; i++) {
    //     for (int j=0; j < N_INPUT; j++) {
    //         cout << testPatterns_Input[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl << endl;
    // for (int i=0; i < p_test_cnt; i++) {
    //     for (int j=0; j < M_OUTPUT; j++) {
    //         cout << testPatterns_Teacher[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl << endl;
*/
}
