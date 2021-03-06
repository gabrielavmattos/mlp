#include "NeuralNet.h"

NeuralNet::NeuralNet()
{
	layers = new Layer[3];
	this->numberLayers = 3;
}

NeuralNet::NeuralNet(int numberLayers)
{
	this->numberLayers = numberLayers;
	layers = new Layer[numberLayers];

}


NeuralNet::~NeuralNet()
{
	delete []layers;

}

/*Name method: definesNumberNeuronsH
Resume: It defines the number neurons of the hidden layer. 

http://www.decom.ufop.br/imobilis/wp-content/uploads/2012/06/03_Feedforward-e-Backpropagation1.pdf
*/

int NeuralNet::definesNumberNeuronsH(int numberInputs, int numberOutputs)
{
	//return 50;
	//return (2*numberInputs/3.0 + numberOutputs);
	return (numberInputs+numberOutputs)/2.0;
}

/*
Name method: Training;
Resume:

*/

//void NeuralNet::training(int numberImages, int numberRows, int numberCols, float *data, int numberNeuronsInput, int numberNeuronsOutput, int *numberNeuronsHidden)
// a última coluna do data é o resultado esperado (VO2)
void NeuralNet::training(int numRowsTrain, int numRowsCross, int numCols, int numNeuronsInput, int numNeuronsOutput, int *numNeuronsHidden, float *dataTrain, float *dataCross) 
{
	

	int i = 0, j;
	
	cout << "Number of Layers: " << numberLayers << endl;
	cout << "Gain term: " << gain << endl;
	//create input layer
	layers[0].setLayer(numNeuronsInput, numNeuronsHidden[0], false); 	
	//create input layer
	for (j=1; j<numberLayers-2; j++)
		layers[j].setLayer(numNeuronsHidden[j-1], numNeuronsHidden[j], false); 
	//create last hidden layer before of the output layer
	layers[numberLayers-2].setLayer(numNeuronsHidden[j-1], numNeuronsOutput, false); 
	//create output layer
	layers[numberLayers-1].setLayer(numNeuronsOutput, 0, true);
	
	int valor = 0;
	while (valor < 700)
	{
		for (i=0; i < numRowsTrain; i++)
		{
		//for (i=0; i<5; i++)
		//{
			layers[0].presentsInput(dataTrain, i, numNeuronsInput);
			//cout << "Aw" << dataTrain[i*numCols+numCols-1] << endl;
			layers[numberLayers-1].presentDesiredOutputs(dataTrain[i*numCols+numCols-1], numNeuronsOutput);
			//cout << i << endl;
			//layers[0].getOutputs();
			fowardPropagation();
			//cout << "CAMADA DE ENTRADA" << endl;
			//layers[numberLayers-1].getOutputs();
			//cout << "CAMADA ESCONDIDA" << endl;
			//layers[1].getOutputs();
			//cout << "CAMADA SAÍDA" << endl;
			//layers[numberLayers-1].getOutputs();

			backPropagation();	
		
			//cout << "CAMADA DE ENTRADA back" << endl;
			//layers[0].getOutputs();
			//cout << "CAMADA ESCONDIDA back" << endl;
			//layers[1].getOutputs();
			//cout << "CAMADA SAÍDA back" << endl;
			//layers[numberLayers-1].getOutputs();
		}
		valor++;

		for (i=0; i < numRowsCross; i++)
		{
		//for (i=0; i<5; i++)
		//{
			layers[0].presentsInput(dataCross, i, numNeuronsInput);
			//cout << "Aw" << dataTrain[i*numCols+numCols-1] << endl;
			layers[numberLayers-1].presentDesiredOutputs(dataCross[i*numCols+numCols-1], numNeuronsOutput);
			//cout << i << endl;
			//layers[0].getOutputs();
			fowardPropagation();
			//cout << "CAMADA DE ENTRADA" << endl;
			//layers[numberLayers-1].getOutputs();
			//cout << "CAMADA ESCONDIDA" << endl;
			//layers[1].getOutputs();
			//cout << "CAMADA SAÍDA" << endl;
			//layers[numberLayers-1].getOutputs();

			backPropagation();	
		
			//cout << "CAMADA DE ENTRADA back" << endl;
			//layers[0].getOutputs();
			//cout << "CAMADA ESCONDIDA back" << endl;
			//layers[1].getOutputs();
			//cout << "CAMADA SAÍDA back" << endl;
			//layers[numberLayers-1].getOutputs();
		}
	

			
	}
}


void NeuralNet::fowardPropagation()
{
	int i, j;
	for (i=1; i<numberLayers; i++)
	{
		for (j=0; j<layers[i].getNumberNeuron(); j++)
		{
			//cout <<"AQUI: " << layers[i-1].calculatesV(j) << endl;
			layers[i].adaptValues(layers[i-1].calculatesV(j), j);
		}
	}

}

void NeuralNet::backPropagation()
{
	int i, j, k;
	double sum;

	//Calculate error of each neuron
	for (i=numberLayers-1; i>=0; i--)
	{	
	//	cout << "Backpropagation. Layer " << i << endl;
		for(j=0; j<layers[i].getNumberNeuron(); j++)
		{
		
			sum = 0;
	//		cout << "Somatorio "<< j << endl;
			if (i == numberLayers-1)
				layers[i].back(true, true,  0, j);
			else
			{	
				for (k=0; k<layers[i].neuron[j].numberSynapses; k++)
				{
		//			cout << sum << endl;	
					sum += layers[i].neuron[j].weight[k] * layers[i+1].neuron[k].error;
				}
				
				layers[i].neuron[j].error = layers[i].neuron[j].value * (1 - layers[i].neuron[j].value) * sum;				
			}
		}
	}

	

	//cout << "-------Ajuarw peso: " << endl;
	for (i=0; i<numberLayers-1; i++)
		for(j=0; j<layers[i].getNumberNeuron(); j++)
			for(k=0; k<layers[i].neuron[j].numberSynapses; k++)
				layers[i].neuron[j].weight[k] += gain*layers[i+1].neuron[k].error*layers[i].neuron[j].value;
		
}

void NeuralNet::testing(float *data, int numberImages, int numNeuronsInput, int numCols)
{

	int i = layers[numberLayers-1].getNumberNeuron();
	int matriz[i][i], j, k; //linha = saída desejada; coluna = saída encontrada
	int approximateValue = 0;
	double error;
	int cont[i];

	//inicializa os contadores
	for(i=0; i<layers[numberLayers-1].getNumberNeuron(); i++)
		cont[i]=0;

	//inicializa a matriz 
	for (i=0; i<layers[numberLayers-1].getNumberNeuron(); i++)
		for (j=0; j<layers[numberLayers-1].getNumberNeuron(); j++)
			matriz[i][j] = 0;

	for (i=0; i<numberImages; i++)
	{	
		k = data[i*numCols+numCols-1];
	
		layers[0].presentsInput(data, i, numNeuronsInput);
		//cout << "Antes" << endl;
		fowardPropagation();

		//cout << "Depois" << endl;
		//layers[numberLayers-1].getOutputs();
		
		j = layers[numberLayers-1].getPositionGreaterValue();
		cont[k]++;
		cout << ":::::" << j << endl;
		matriz[k][j]++;
		cout << ":" << matriz[k][j] << endl;
		cout << "Image " << i << " - Desired Output " << data[i*numCols+numCols-1] << " - " << j << endl; 

	}


	cout << "Numero de neuronios de teste: " << endl;
	for (i=0; i<layers[numberLayers-1].getNumberNeuron(); i++)
		cout << "\t" << cont[i];
	cout << endl;	
	cout << endl;	
	cout << endl;


	for (i=0; i<layers[numberLayers-1].getNumberNeuron(); i++)
		cout << "\t" << i;
	cout << endl;



	for (i=0; i<layers[numberLayers-1].getNumberNeuron(); i++)
	{
		cout << i ;
		for (j=0; j<layers[numberLayers-1].getNumberNeuron(); j++)
		{
			cout << "\t" << matriz[i][j];
			if ( i == j)
				approximateValue += matriz [i][j];		
		}		
		cout << endl;
	}	
		
	error = abs(approximateValue - numberImages) / (numberImages * 1.0);
	error = error * 100;
	cout << "Quantidade de acertos:" << approximateValue << endl;
	cout << "Percent error: " << error << "%." << endl;

}
