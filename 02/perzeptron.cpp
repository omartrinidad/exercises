/* Perzeptron */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

void calculateAndLearning(vector< vector<int> >& network, vector<float**>& weights ) {

    for (int i = 0; i < network.size() - 1; ++i ) {
        for (int j = 0; j < network[i+1].size(); ++j) {
            float sum = 0;

            cout << "<->" << endl;

            for (int k = 0; k < network[i].size(); ++k ) {

                cout << "<->" << weights[0] << endl;
                cout << "<->" << weights[i][0][1] << endl;
                cout << "<->" << weights[i][0][2] << endl;
                cout << "<->" << weights[i][k][j] << endl;
                sum += network[i][k] * weights[i][k][j];
                cout << "<->" << endl;

            }

            /* comparison with the teacher value */
            int output = sum > 0 ? 1 : 0;
            cout << network[i+1][j] << "<->" << output << endl;
        }
    }

}

void evaluateFile(vector<int>& layers, char* fileName) {

    string line;
    ifstream inputFile(fileName);
    int index;

    /* each value in layers is the number of neurons of each layer */
    int p = 0;

    if ( !inputFile.eof() ) {

        while ( getline(inputFile, line) ) {

            p++;
            if (p == 1) {
                /* first line */
                layers.push_back(0);

                /* Parse a line */
                index = 0;
                while (index < line.size() ) {

                    /* layer change -> \t */
                    if (line[index] == '\t') {
                        layers.push_back(0);
                    } else if ( line[index] != ' ') {
                        layers[layers.size() - 1] += 1;
                    }
                    index += 1;
                }

            } else {
                /* from second line */

                vector<int> layers_t;
                layers_t.push_back(0);
                index = 0;
                while (index < line.size() ) {

                    /* layer change -> \t */
                    if (line[index] == '\t') {
                        layers_t.push_back(0);
                    } else if ( line[index] != ' ') {
                        layers_t[layers_t.size() - 1] += 1;
                    }
                    index += 1;
                }

                /* comparison */
                bool isCorrect = true;

                if (layers.size() == layers_t.size()) {
                    isCorrect = equal(layers.begin(), layers.end(), layers_t.begin()) ;
                } else {
                    cout << "different size" << "";
                    isCorrect = false;
                }

                if ( !isCorrect )
                    cout << "Error on line " << p << endl;
            }
        }
        inputFile.close();
    }
}


/* This function initialize a network */
void initializeNetwork(vector< vector<int> > & network, vector<int>& layersStructure, vector<float**> weights){

    /* create the network */
    for(int i = 0; i < layersStructure.size(); ++i ) {

        int layerSize = layersStructure[i];

        /* Add a BIAS neuron if is not the last layer */
        int bias = i == layersStructure.size() - 1 ? 0 : 1;
        network.push_back(vector<int> (layerSize + bias));
    }

    /* create the weights */

    srand(time(0));

    for (int i = 0; i < network.size() - 1; ++i) {

        int n = network[i].size();
        int m = network[i+1].size();

        float **weight = new float *[n];
        for (int j = 0; j < n; ++j ) {
            weight[j] = new float[m];
            for (int k = 0; k < m; ++k) {
                weight[j][k] = (float) rand()/RAND_MAX - 0.5;
            }
        }

        weights.push_back(weight);
    }

}

/* Compute the network */
void computeNetwork(vector< vector<int> > & network, vector<float**> weights, char* fileName){

    string line;
    ifstream inputFile(fileName);
    int index;
    int layer = 0, neuron = 0;

    /* each value in layers is the number of neurons of each layer */
    if ( !inputFile.eof() ) {

        while ( getline(inputFile, line) ) {

            /* assign the BIAS*/
            network[0][0] = 1;
            index = 0;

            while (index < line.size() ) {

                /* layer change -> \t */
                if (line[index] == '\t') {
                    layer++;
                } else if ( line[index] != ' ') {
                    int value = (int) line[index] - '0';
                    network[layer][++neuron] = value;
                }
                index += 1;
            }

            calculateAndLearning(network, weights);

        }
        inputFile.close();
    }

}

int main(int argc, char** argv){

    vector< vector<int> > network;
    vector< float** > weights;
    vector<int> layersStructure;

    /* evaluate file and get number of neurons for layer, apply constraints */
    evaluateFile(layersStructure, argv[1]);
    initializeNetwork(network, layersStructure, weights);
    computeNetwork(network, weights, argv[1]);

    return 0;
}
