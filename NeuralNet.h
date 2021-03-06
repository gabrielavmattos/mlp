/*

File: NeuralNet.cpp
Author: Gabriela Mattos
Resume: Implementation of neural network multi-layer perceptrons.

"Multi-layer perceptron are feed-forward nets with one or more layers of nodes between the input and output nodes. These additional layers contain hidden units or nodes that are not directly conect to both the input and output nodes."

*/

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Layer.h"


using namespace std;

#define gain 0.1

class NeuralNet
{

	public:
		Layer *layers; //input layer
		int numberLayers;
		//Layer *hidden; //hidden layers, is used pointer because of the uncentainty of the number of layers
	//	Layer output; //output layer


		NeuralNet();
		NeuralNet(int);
		~NeuralNet();

		int definesNumberNeuronsH(int, int);
		
		void training(int, int, int, int, int, int*, float*, float*);
		void testing(float*, int, int, int);


		void fowardPropagation();
		void backPropagation();
		
		
		void PresentsInput(double*, int, int, int, float*, int);
		int DefinesNumberNeuronsH(); 
		
};
