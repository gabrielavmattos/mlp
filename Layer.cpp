#include "Layer.h"


Layer::Layer()
{}

Layer::~Layer()
{
	delete []neuron;
}

void Layer::setLayer(int numberNeuron, int numberSynapses, bool isOutput)
{
	int i;
	this->numberNeuron = numberNeuron;
	neuron = new Neuron[numberNeuron];
	
	cout << "Camada criada com "<< numberNeuron << " neuronios e " << numberSynapses << " Sinapses." << endl;
	if(!isOutput)
	{
		for(i=0; i<numberNeuron; i++)
			neuron[i].initializesWeightThreshold(numberSynapses);
	}
	
}

int Layer::getNumberNeuron()
{
	return (this->numberNeuron);

}

Neuron Layer::getNeuron(int indice)
{

	return (this->neuron[indice]);
}


// Precisa ser pensado como trabalhar essa input
void Layer::presentsInput(float *datas, int numberImages, int numNeuronsInput)
{
	int i, j=0;
	int begin = numberImages*numNeuronsInput + numberImages;
	
	//if(begin != 0)
	//	begin+=numberImages;

	for (i = begin; i<(begin+numNeuronsInput); i++)
	{	
		//cout << "image " << j << "   -   " << datas[i] << endl; 
		//adaptValues(datas[i]/255, j++);
		//cout << "image " << j << "   -   " << datas[i] << endl; 
		//cout << datas[i]/255 << endl;
		neuron[j++].setValue(datas[i]);
	
	}

	
}

int Layer::getPositionOutput(float label)	
{
	int i;
	cout << "\n\n" << label << endl;

	if (label>=0 && label<0.1)
		i=0;
	else
		if (label>=0.1 && label<0.2)
			i=1;	
	else
		if (label>=0.2 && label<0.3)
			i=2;

	else
		if (label>=0.3 && label<0.4)
			i=3;
	else
		if (label>=0.4 && label<0.5)
			i=4;

	else
		if (label>=0.5 && label<0.6)
			i=5;

	else
		if (label>=0.6 && label<0.7)
			i=6;

	else
		if (label>=0.7 && label<0.8)
			i=7;

	else
		if (label>=0.8 && label<0.9)
			i=8;

	else
		i=9;


	cout << "\n\n" << i << endl;
	return i;
}

void Layer::presentDesiredOutputs(float label, int numberOutputs)
{

	float *DesiredOutput = new float[numberOutputs];
	int i, j = (int) label;

	for (i=0; i<numberOutputs; i++)
		DesiredOutput[i] = 0;

	DesiredOutput[j] = 1;

	for (i = 0; i<numberOutputs; i++)
		neuron[i].setD(DesiredOutput[i]);
	//cout << label << "Escolhido " << j << "-"<< neuron[i].getD << endl;

}

float Layer::calculatesV(int numberNeuron)
{

	int i;
	float value = 0;

	//cout << "Numero de neuronios nessa camada: " << getNumberNeuron() << endl;
	for (i=0; i<getNumberNeuron(); i++)
	{
	//	cout << "Valor antes " << i << " : " <<  value << "   "<< neuron[i].getValue() << "*" << neuron[i].getWeight(numberNeuron)<< " = " << neuron[i].getValue()*neuron[i].getWeight(numberNeuron) << endl; 
		value += neuron[i].getValue() * neuron[i].getWeight(numberNeuron);
	//	cout << "Valor depois " << i << " : " <<  value << "   "<< neuron[i].getValue() << "*" << " = " << neuron[i].getValue()*neuron[i].getWeight(numberNeuron) << endl; 
	}


	//cout << "Valor de " << numberNeuron << "w: " <<  neuron[0].getWeight(numberNeuron) << endl; 
	//cout << "Valor final " << numberNeuron << " : " <<  value << endl; 
	return value;

}

void Layer::adaptValues(float value, int index)
{	
	//float aux;
	//cout << "Index: "<< index << endl;	
	//cout << "Value antes: " << value << endl;
	value = (1.0)/(1.0 + exp(-value+neuron[index].getThreshold()));
	//cout << "Value depois: " << value << endl;
	neuron[index].setValue(value);
}


void Layer::getOutputs()
{

	int i;
	
	for (i=0; i<numberNeuron; i++)
		cout << "Neuron [" << i << "]: " << neuron[i].getValue() << "." << endl;

}

float Layer::getNeuronOfWeight(int j, int k)
{
	return (neuron[k].getWeight(j));

}

float Layer::getNeuronOfError(int k)
{

	return (neuron[k].getError());
}

int Layer::getPositionGreaterValue()
{
	int i, posMax=0;
	float max = 0;
	for (i=0; i<getNumberNeuron(); i++)
	{
		if (max < neuron[i].getValue())
		{	
			max = neuron[i].getValue();
			posMax = i;
			cout <<  neuron[i].getValue() << " - I max: " << i << endl;
		}
	}

	return posMax;
}

void Layer::back(bool isError, bool isOutput, float sum, int indice)
{
	if (isError)
		neuron[indice].calculatesError(isOutput, sum);
	else
		neuron[indice].adjustWeight();
}
	
