#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <math.h>

#define N_MAX_SIZE 100  	// Max number of dimension of input layer
#define M_MAX_SIZE 29       // Max number of dimension of output layer
#define P_MAX_SIZE 199      // Max number of patterns
#define LAYER_SIZE 2        // 2-layers perzeptron

using namespace std;

/*=========================================
# Group member (Name, Student ID, E-Mail) 
	- Omar Trinidad Gutierrez Mendez, 2850441, omar.vpa@gmail.com 
	- Shinho Kang, 2890169, wis.shinho.kang@gmail.com

# PA-A
    - To implement a 2-layer Perzeptron

# Instruction
    - Please read the "README.txt" file to compile and execute this source code.    
=========================================*/

/*
# evaluateDataFile()
 Evaluate and verify data file, and Decide dimensions of each layer
 
 PARAMETERS
    - vector<int>& layerStructure : each value in layerStructure is the number of neurons of each layer
    - char* fileName : name of a data file to evaluate
*/
void evaluateDataFile(vector<int>& layerStructure, const char* fileName) {
    ifstream inputFile(fileName);   // read file
    int p_cnt = 0;
    
    if ( !inputFile.eof() ) {
        string line;
        int index = 0;
    	
        while ( getline(inputFile, line) ) {    	// loop for lines of file
            p_cnt++;
            if (p_cnt == 1) {   
                // In first line: store the dimensions of each layer
                layerStructure.push_back(0);    // first layer (input)

                // Parse a line
                index = 0;
                while (index < line.size() ) {
                    // layer change -> \t
                    if (line[index] == '\t') {
                        layerStructure.push_back(0);    // add layer
                    } else if ( line[index] != ' ') {
                        layerStructure[layerStructure.size() - 1]++; // count the dimension of layer
                    }
                    index++;
                }
            } else {
                // From second line: check the data file by comparing with the layerStructure
                vector<int> layers_t;   // to compare with the layer structure which is derived from the first line.
                layers_t.push_back(0);
                // Parse a line
                index = 0;
                while (index < line.size() ) {
                    // layer change -> \t
                    if (line[index] == '\t') {
                        layers_t.push_back(0);  // add layer
                    } else if ( line[index] != ' ') {
                        layers_t[layers_t.size() - 1]++; // count the dimension of layer
                    }
                    index++;
                }

                // comparison
                bool isCorrect = true;

                if (layerStructure.size() == layers_t.size()) { 	// compare number of layers
                    isCorrect = equal(layerStructure.begin(), layerStructure.end(), layers_t.begin()) ; // compare the dimensions of each layer
                } else {
                    isCorrect = false;
                }

                // throw exception 
                if ( !isCorrect ) {
                    stringstream ess;
                    ess << "Given data file has an error on line " << p_cnt;
                    throw(ess.str());
                }                    
            }
        }
        inputFile.close();
        
        // Check the constraints
        if (layerStructure.size() != LAYER_SIZE) {
            stringstream ess;
            ess << "Given data file has different number of layers from the requirement." << endl;
            ess << "Number of layers should be " << LAYER_SIZE;
            throw(ess.str());
        }
        if (layerStructure[0] > N_MAX_SIZE) {
            stringstream ess;
            ess << "Number of inputs over. MAX: " << N_MAX_SIZE;
            throw(ess.str());
        }
        if (layerStructure[1] > M_MAX_SIZE) {
            stringstream ess;
            ess << "Number of outputs over. MAX: " << M_MAX_SIZE;
            throw(ess.str());
        }
        if (p_cnt > P_MAX_SIZE) {
            stringstream ess;
            ess << "Number of patterns over. MAX: " << P_MAX_SIZE;
            throw(ess.str());
        }            
    }
    else {  // when the file is empty or null
        stringstream ess;
        ess << "The given data file is empty or null.";
        throw(ess.str());        
    }
    cout << "==========" << endl;
    cout << " result of evaluateDataFile()" << endl;
    cout << " layer number: " << layerStructure.size() << endl;
    cout << " input size: " << layerStructure[0] << endl;
    cout << " output size: " << layerStructure[1] << endl;
    cout << " pattern size: " << p_cnt << endl;
}



/* constructLayers()
 Construct perzeptron layers and weights and initialize them.
 assign random weight values
 
 PARAMETERS
    - vector<int>& x : input layer
    - vector<int>& y : output layer
    - vector<int>& y_t : teacher values of each output neuron
    - vector<float*>& weights : weight values for pair of inputs-outputs
    - const vector<int>& layerStructure : layer Structure info
*/
void constructLayers(vector<int>& x, vector<int>& y, vector<int>& y_t, vector<float*>& weights, const vector<int>& layerStructure) {
    // construct perzeptron layers and assign 0 into each neuron
    x.resize(layerStructure[0] + 1, 0);  // allocate N-input layers (include BIAS)
    y.resize(layerStructure[1], 0);  // allocate M-output layers
    y_t.resize(layerStructure[1], 0);  // allocate M-teacher values
    
    // create the weights
    srand(time(0)); // random seed

    for (int n = 0; n < x.size(); ++n) {
        float *weight = new float[y.size()];    // allocate m-array
        for (int m = 0; m < y.size(); ++m) {    // assign random number into m-array
            weight[m] = (float) rand()/RAND_MAX - 0.5;  // to make the number range as -0.5 <= n <= +0.5
        }
        weights.push_back(weight);      // push the array into weights vector
    }
    
    cout << "==========" << endl;
    cout << " result of constructLayers()" << endl;
    cout << " input layer: " << x.size() << endl;
    cout << " output layer: " << y.size() << endl;
    cout << " teacher values: " << y_t.size() << endl;
    cout << " weight values: ";

    for (int i=0; i < x.size(); ++i) {
        cout << endl << "\t";
        for (int j=0; j < y.size(); ++j) {
            cout << weights[i][j] << " ";
        }
    }
    
}








/* calculateAndLearning()
 calcualte weighted sum for each output and compare with threshold.
 adapt learning rule to adjust weight values.
   
 PARAMETERS
    - const vector<int>& x : input layer
    - vector<int>& y : output layer
    - const vector<int>& y_t : teacher values of each output neuron
    - vector<float*>& weights : weight values for pair of inputs-outputs
*/
void calculateAndLearning(const vector<int>& x, vector<int>& y, const vector<int>& y_t, vector<float*>& weights) {
    // calcualte weighted sum for each output and compare with threshold.
    for (int m = 0; m < y.size(); ++m) {
        float weightedSum = 0.0;
        for (int n = 0; n < x.size(); ++n) {
            weightedSum += x[n] * weights[n][m];
        }
        int output = weightedSum > 0 ? 1 : 0;
        y[m] = output;
    }
    
    // comparing the ouputs and teacher values - incomplete
    float e = 0; // pattern error
    for (int m = 0; m < y.size(); ++m) {
        e += pow(y_t[m] - y[m], 2);
    }
    e = e/2;
    
    cout << " output values: ";
    for (int i = 0; i < y.size(); i++) {
        cout << y[i] << ", ";
    }
    cout << endl << " pattern error: " << e << endl;
    cout << "----" << endl;
    
    // .....
    // We can adapt adjusting weights algorithms here
    // .....
}




/* perzeptron_process()
 read data and put into perzeptron input layer.
 calcualte weighted sum for each output and compare with threshold.
 adapt learning rule to adjust weight values.
   
 PARAMETERS
    - vector<int>& x : input layer
    - vector<int>& y : output layer
    - vector<int>& y_t : teacher values of each output neuron
    - vector<float*>& weights : weight values for pair of inputs-outputs
    - char* fileName : given data file name
*/
void perzeptron_process(vector<int>& x, vector<int>& y, vector<int>& y_t, vector<float*> weights, char* fileName){
    ifstream inputFile(fileName);   // read data file
    
    // parse data line by line
    if ( !inputFile.eof() ) {
        string line;
    	int p_cnt = 0;
        
        cout << endl << "==========" << endl;
        while ( getline(inputFile, line) ) {
            bool isInput = true;
            int index = 0;
            p_cnt++;
            
            int neuron = 0;
            x[neuron++] = 1;  // assign the BIAS
            
            while (index < line.size() ) {
                if (line[index] == '\t') {
                    // layer change -> \t
                    isInput = false;
                    neuron = 0; 	// initialize
                } else if ( line[index] != ' ') {
                    int value = (int) line[index] - '0';    // trick for casting
                    if (isInput) {
                        x[neuron++] = value;    // assign input value
                    } else {
                        y_t[neuron++] = value;  // assign teacher value
                    }
                }
                index++;
            }
            
            cout << "Pattern " << p_cnt << endl;
            cout << " input values: ";
            for (int i = 0; i < x.size(); i++) {
                cout << x[i] << ", ";
            }
            cout << endl << " teacher values: ";
            for (int i = 0; i < y_t.size(); i++) {
                cout << y_t[i] << ", ";
            }
            cout << endl;
            
            // calculate weighted sum for each ouput, adjust weights by learning
            calculateAndLearning(x, y, y_t, weights);

        }
        inputFile.close();
    }

}




/*
# main()
 - int argc: number of parameters from execution command 
 - char** argv: parameters from execution command          
*/
int main(int argc, char** argv){
    // Declare of variables   
    vector<int> layerStructure;    // vector for storing dimensions of each layer
    vector<int> x;        // vector for input layer (max length = 100)
    vector<int> y;        // vector for output layer (max length = 29)
    vector<int> y_t;      // vector for teacher values (for comparison with output values) (max length = 29)
    vector< float* > weights; // vector for weight values of each pair of input-output. will be assigned randomly.
        
    try {
        // Evaluate and verify data file
        // Decide dimensions of each layer
        evaluateDataFile(layerStructure, argv[1]);
        
        // Construct perzeptron layers  
        constructLayers(x, y, y_t, weights, layerStructure);
        
        // Process perzeptron
        perzeptron_process(x, y, y_t, weights, argv[1]);
        
    } catch(const exception& e) {
        cerr << e.what();
    } catch(const string& msg) {
        cerr << endl << "Error: " << msg << endl;
    }
    return 0;
}
