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
#define MAX_TRAINING_PATTERN 1000 // number of training patterns up to 1000  												 
//=====================

//=====================
// helper functions - split str
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        //if (!item.empty()) {
			elems.push_back(item);
		//}
    }
    return elems;
}
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
//=====================


//=====================
void constructMLP();	// construct MLP
void initWeights();		// initialize weights
void verifyDataFile(); 	// verify given training.dat file and test.dat file 
void training();		// training using training.dat file 
void validation();		// validate performance using test.dat file 

/////============================
// Declare MLP components class

string trainingFileName;
string testFileName;
float trainingPatters_Input[MAX_TRAINING_PATTERN][N_INPUT];
float trainingPatters_Teacher[MAX_TRAINING_PATTERN][M_OUTPUT];
float testPatters_Input[MAX_TRAINING_PATTERN][N_INPUT];
float testPatters_Teacher[MAX_TRAINING_PATTERN][M_OUTPUT];

/////============================
int main(int argc, char** argv) {
    try {	
        trainingFileName = argv[1];
        testFileName = argv[2];
        //constructMLP();
        //initWeights(); 
        
        verifyDataFile();
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
	
	// -- Omar will work on this
	
}

void initWeights() {
	// Initialize weights
	// Weight values should be -2.0 < w < +2.0 random numbers using RND_SEED
	 
	// -- Omar will work on this
}

void verifyDataFile() {
	// // verify given training.dat file and test.dat file
	// - ignore header lines (commented lines)
	// - check the number of input and teacher values each line
	// - check if there is no blank line
	// - check if the number of pattern is more than defined number
	
	ifstream trainingFile(trainingFileName);   // read training data file
    
    if ( !trainingFile.eof() ) {
        int p_cnt = 0;
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
			for (int i =0; i < values.size(); ++i) {
				if (values[i].empty()){
					
				}  
				cout << values[i] << ",";
			}	
			cout << endl;
			p_cnt++;
        }
	}
        //testPatters[][]
    	//float testPatters_Input[MAX_TRAINING_PATTERN][N_INPUT];
		//float testPatters_Teacher[MAX_TRAINING_PATTERN][M_OUTPUT];
        
	// -- Shinho will work on this 
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