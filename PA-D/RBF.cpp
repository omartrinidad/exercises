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
#define LAYER_NUM 4		// 3: N-H-M , 4: N-H1-H2-M
#define N_INPUT 5       // Number of neurons in input layer (1 < n <=1000)
#define M_OUTPUT 31      // Number of neurons in output layer (1 < m <= 1000)
#define H1_HIDDEN 30   	// Number of neurons in first hidden layer (1 < h1 <=1000)
#define H2_HIDDEN 30   	// Number of neurons in second hidden layer (1 < h2 <= 1000) 
						// this value is valid only when LAYER_NUM is 4.
#define RND_SEED 7      // seed for Random number generator 
						// Weight values are initialized refer to the seed
#define MAX_TRAINING_PATTERN 1000 // number of training patterns up to 1000  												 
//=====================


//=====================
// helper functions - split str
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

enum Transferfunction { TANH, LOGISTIC, IDENTITY };
enum Layer_Category{INPUT, HIDDEN, OUTPUT};
// Layer class
class Layer {
    public:
        Neuron *neurons;		// neurons in the layer 
        int num_neurons;		// number of neurons in the layer
        Transferfunction tf;	// transfer function of neurons in the layer 
        float learningRate; 	// Learning Rate - BP
        Layer_Category lc;
        
        /* constructors declaration */
        Layer();
        Layer(int num_neurons);

        /* methods definition */
        void setTF(Transferfunction _tf){
            tf = _tf;  
        };
        void setLearningRate(float _lr){
            learningRate = _lr;
        };
        void setLayerCategory(Layer_Category _lc) {
            lc = _lc;
        }
};


/* constructors definition */
Layer::Layer() { } 

Layer::Layer(int init) {
    cout << "init layer: " << init << endl;
    num_neurons = init + 1;     // include BIAS
    neurons = new Neuron[init + 1]; // include BIAS
    neurons[0].output = 1;
}


// Weights class
class Weights {

    public:
        float **weights;
        float **weight_changes;

        /* constructors definition */
        Weights(){ }

        Weights(int X, int Y) {

            X = X + 1;      // add BIAS
            Y = Y + 1;      // add blank space 
            srand(RND_SEED); // random seed
            
            weights = new float*[X];
            weight_changes = new float*[X];

            for (int x = 0; x < X; x++) {
                weights[x] = new float[Y];
                weight_changes[x] = new float[Y];
            }
            
            // cout << "============================" << endl;
            // cout << "Weight Initiate" << endl;
            for (int x = 0; x < X; x++) {
                for (int y = 1; y < Y; y++) {
                    weights[x][y] = ((float(rand()) / float(RAND_MAX)) * (2 - -2)) + -2;
                    // cout << weights[x][y] << " ";
                    weight_changes[x][y] = 0;
                }
                // cout << "" << endl;
            }
            // cout << "============================" << endl;
        }
};

//=====================
void constructMLP();	// construct MLP
void initWeights();		// initialize weights
void verifyDataFile(); 	// verify given training.dat file and test.dat file 
void training();		// training using training.dat file 
void validation();		// validate performance using test.dat file 

void initNeuron(Layer l, float* pattern);
void initTeacher(Layer l, float* pattern);
void calcOutput(Layer l_Pre, Layer l_Next, Weights w);    // calculate weighted sum and ouput in terms of neurons in the layer
float calcTransferFunction(float net, Transferfunction tf); // calculation of the result of transfer function

float calcError(Layer l); // calculate error of the output layer using quadratic differences
void printGNUPlotForm(string fileName, int patternNum, float error); // print error into GNUplot form

void calcDelta_Hidden(Layer l_Pre, Layer l_Next, Weights w);
void calcDelta_OutputLayer(Layer l);

void calcWeightchanges(Weights w, Layer l_Pre, Layer l_Next);
void updateWeights(Weights w);
/////============================
// Declare MLP components class

Layer L_X;	
Layer L_H1;
Layer L_H2;
Layer L_Y;

Weights W_XtoH;
Weights W_HtoH;
Weights W_HtoY;

string trainingFileName;
string testFileName;
float trainingPatterns_Input[ MAX_TRAINING_PATTERN ][ N_INPUT ];
float trainingPatterns_Teacher[ MAX_TRAINING_PATTERN ][ M_OUTPUT ];
float testPatterns_Input[ MAX_TRAINING_PATTERN ][ N_INPUT ];
float testPatterns_Teacher[ MAX_TRAINING_PATTERN ][ M_OUTPUT ];
int p_cnt = 0;
int p_test_cnt = 0;

/////============================
int main(int argc, char** argv) {
    try {	
        // arguments as filenames 
        trainingFileName = argv[1];
        testFileName = argv[2];
        
        verifyDataFile(); // verify data files
        constructMLP(); // construct MLP structure
        initWeights(); // initiate weight values
        training();     // training
        validation();   // test
        
    } catch(const exception& e) {
        cerr << e.what();
    } catch(const string& msg) {
        cerr << endl << "Error: " << msg << endl;
    }
	return 0;
} 

void constructMLP() {
	// Initialize neurons in each layer using classes
    // the number of layers and the number of neurons 
    // should be set as the defined numbers
    
    L_X = Layer(N_INPUT);
    L_H1 = Layer(H1_HIDDEN);
    L_H1.setTF(TANH);           // Transfer Function
    L_H1.setLearningRate(0.3);  // Learning Rate
    
	if (LAYER_NUM == 4){
		L_H2 = Layer(H2_HIDDEN);
        L_H2.setTF(TANH);           // Transfer Function
        L_H2.setLearningRate(0.4);  // Learning Rate
	}
    L_Y = Layer(M_OUTPUT);
    L_Y.setTF(LOGISTIC);                // Transfer Function
    L_Y.setLearningRate(0.5);       // Learning Rate
}

void initWeights() {
	// Initialize weights
	// Weight values should be -2.0 < w < +2.0 random numbers using RND_SEED
    W_XtoH = Weights(N_INPUT, H1_HIDDEN);
	if (LAYER_NUM == 4) {
		W_HtoH = Weights(H1_HIDDEN, H2_HIDDEN);
		W_HtoY = Weights(H2_HIDDEN, M_OUTPUT);	
	}
	if (LAYER_NUM == 3) {
		W_HtoY = Weights(H1_HIDDEN, M_OUTPUT);
	}    
}

void verifyDataFile() {
	// // verify given training.dat file and test.dat file
	// - ignore header lines (commented lines)
	// - check the number of input and teacher values each line
	// - check if the number of pattern is more than defined number
	
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
}

void training() {
	// read training data
	// do the process training line by line
	
	// Use the sum over quadratic differences as error function
	// Calculate the error in every training step, 
	// and print it as a learning curve into a file learning.curve during the training process. 
	// Choose a format that can easily be depicted using the freely available program gnuplot.
	
    // 1. pick a pattern
    //string resultFileName = "";
    std::remove("result_training.dat"); // delete file
    
    for (int p = 0 ; p < p_cnt; ++p) {
    // for (int p = 0 ; p < 1; ++p) {
        // 1. put training pattern input and teacher values into neurons
        //trainingPatterns_Input[p]
        initNeuron(L_X, trainingPatterns_Input[p]);
        initTeacher(L_Y, trainingPatterns_Teacher[p]);
        
        // 2. calculate weighted sum and output of each neuron
        // using weighted sum
        // using transfer function
        // save result into neuron object
        calcOutput(L_X, L_H1, W_XtoH);
        if (LAYER_NUM == 3) {
            calcOutput(L_H1, L_Y, W_HtoY);
        } else if (LAYER_NUM == 4) {
            calcOutput(L_H1, L_H2, W_HtoH);
            calcOutput(L_H2, L_Y, W_HtoY);
        }
        
        // 4. calculate error - using error function "sum of quadratic differences" and print it as a learning curve 
        float error = calcError(L_Y);
        printGNUPlotForm("result_training.dat", p, error);
        
        
        // 5. calculate delta value - output
        calcDelta_OutputLayer(L_Y);
        // 6. calculate delta value - H2
        // 7. calculate delta value - H1
        if (LAYER_NUM == 3) {
            calcDelta_Hidden(L_H1, L_Y, W_HtoY);
        } else if (LAYER_NUM == 4) {
            calcDelta_Hidden(L_H2, L_Y, W_HtoY);
            calcDelta_Hidden(L_H1, L_H2, W_HtoH);
        }
        //    calculate weight changes
        if (LAYER_NUM == 3) {
            calcWeightchanges(W_HtoY, L_H1, L_Y);
            calcWeightchanges(W_XtoH, L_X, L_H1);
        } else if (LAYER_NUM == 4) {
            calcWeightchanges(W_HtoY, L_H2, L_Y);
            calcWeightchanges(W_HtoH, L_H1, L_H2);
            calcWeightchanges(W_XtoH, L_X, L_H1);
        }
    }
    
}

void validation() {
	// read test data
	// write the performance result to file
    // 1. pick a pattern
    // for (....)
    cout << "validation " << p_test_cnt << endl;
    std::remove("result_test.dat"); // delete file
    for (int p = 0 ; p < p_test_cnt; ++p) {
    // for (int p = 0 ; p < 1; ++p) {
        // 1. put training pattern input and teacher values into neurons
        //trainingPatterns_Input[p]
        initNeuron(L_X, trainingPatterns_Input[p]);
        initTeacher(L_Y, trainingPatterns_Teacher[p]);
        
        // 2. calculate weighted sum and output of each neuron
        // using weighted sum
        // using transfer function
        // save result into neuron object
        calcOutput(L_X, L_H1, W_XtoH);
        if (LAYER_NUM == 3) {
            calcOutput(L_H1, L_Y, W_HtoY);
        } else if (LAYER_NUM == 4) {
            calcOutput(L_H1, L_H2, W_HtoH);
            calcOutput(L_H2, L_Y, W_HtoY);
        }
        
        // 4. calculate error - using error function "sum of quadratic differences" and print it as a learning curve 
        float error = calcError(L_Y);
        printGNUPlotForm("result_test.dat", p, error);
    }
    
}

// to init input neurons with the values of pattern
void initNeuron(Layer l, float* pattern){
    // cout << "============================" << endl;
    // cout << "initNeuron" << endl;
    for (int i = 1; i < l.num_neurons; ++i) {
        // cout << l.neurons[i].output << " ";        
        l.neurons[i].output = pattern[i-1];
        // cout << l.neurons[i].output << endl;;
    }
}

void initTeacher(Layer l, float* pattern){
    // cout << "============================" << endl;
    // cout << "initTeacher" << endl;
    for (int i = 1; i < l.num_neurons; ++i) {
        // cout << l.neurons[i].teacher << " ";        
        l.neurons[i].teacher = pattern[i-1];
        // cout << l.neurons[i].teacher << endl;;
    }
}

void calcOutput(Layer pre, Layer next, Weights w) {
    // cout << "============================" << endl;    
    // cout << "calcOutput" << endl;
    
    for (int j = 1; j < next.num_neurons; j++) {
        next.neurons[j].net_val = 0;
        next.neurons[j].output = 0;
        next.neurons[j].delta = 0;
    }

    for (int i = 0; i < pre.num_neurons; i++) {
        // cout << " -------------> " << i << endl;
        for (int j = 1; j < next.num_neurons; j++) {
            // cout << "(" << i << " " << j << ") " << w.weights[i][j] << " * " << pre.neurons[i].output << "  ";
            next.neurons[j].net_val += w.weights[i][j] * pre.neurons[i].output;
            // cout << next.neurons[j].net_val << endl;
        }
        // cout << " " << "\n";
    }

    // cout << "-----------------------------------> " << endl;
    // calculation each of output of neuron - transfer function 
    for (int j = 1; j < next.num_neurons; j++) {
        next.neurons[j].output =\
                calcTransferFunction(next.neurons[j].net_val, next.tf);
    }
}

// calculation each of output of neuron - transfer function 
float calcTransferFunction(float net, Transferfunction tf){
    // cout << "Transfer function : " << tf << endl;
    // cout << "net  " << net << endl;
    float result = 0.0;
    if (tf == TANH) {
        result = (exp(net)-exp(-net)) /(exp(net) + exp(-net));
    }
    if (tf == LOGISTIC) {
        result = 1 /(1 + exp(-net));
    }
    if (tf == IDENTITY) {
        result = net;
    }
    
    // cout << result << endl;
    return result;

}

// pattern error calculation
float calcError(Layer l) {
    float sum = 0;
    float error = 0;

    for (int i = 1; i < l.num_neurons; ++i) {
        sum += pow(l.neurons[i].teacher - l.neurons[i].output, 4);
    }
    error = 0.5 * sum; //pow(sum, 4);

    // cout << error << endl;
    return error;
} 


void printGNUPlotForm(string fileName, int patternNum, float error){
    ofstream fout;
    fout.open(fileName, ios::app);    // open file for appending
    //assert (!fout.fail( ));     
    
    fout << patternNum << "\t" << error << endl;
    
    fout.close( );       //close file
    // assert(!fout.fail( ));
};

// calculate delta value of neurons of output layer 
void calcDelta_OutputLayer(Layer l){
    // cout << "==========================" << endl;
    // cout << "calcDelta_OutputLayer" << endl;
    for (int i = 1 ; i < l.num_neurons; i ++) {
        float delta = 2 * pow(l.neurons[i].teacher-l.neurons[i].output,3);
        if (l.tf == TANH) {
            delta = delta * (1.0 - pow(l.neurons[i].output, 2));  
        }
        else if (l.tf == LOGISTIC) {
            delta = delta * (1.0 - l.neurons[i].output);
        }
        
        l.neurons[i].delta = delta;
        // cout << i << " : " << delta << endl;
    }
};
// calculate delta value of neurons of hidden layer
void calcDelta_Hidden(Layer l_Pre, Layer l_Next, Weights w){
    // cout << "==========================" << endl;
    // cout << "calcDelta_Hidden" << endl;
    for (int i = 1 ; i < l_Pre.num_neurons; i ++) {
        float delta = 0.0;
        for (int j = 1; j < l_Next.num_neurons; j++) {
            delta += l_Next.neurons[j].delta * w.weights[i][j];
        }
        if (l_Pre.tf == TANH) {
            delta = delta * (1.0 - pow(l_Pre.neurons[i].output, 2));  
        }
        else if (l_Pre.tf == LOGISTIC) {
            delta = delta * (1.0 - l_Pre.neurons[i].output);
        }
        
        l_Pre.neurons[i].delta = delta;
        // cout << i << " : " << delta << endl;
    }
};

//
void calcWeightchanges(Weights w, Layer l_Pre, Layer l_Next){
    for (int i = 0; i < l_Pre.num_neurons; i ++) {
        for (int j = 1; j < l_Next.num_neurons; j++) {
            w.weight_changes[i][j] = l_Next.learningRate * l_Next.neurons[j].delta * l_Pre.neurons[i].output;
        }
    }
    for (int i = 0; i < l_Pre.num_neurons; i ++) {
        for (int j = 1; j < l_Next.num_neurons; j++) {
            w.weights[i][j] += w.weight_changes[i][j];
        }
    }
};
