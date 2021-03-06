#include <fstream>
#include <iostream>
#include <cstdlib>
#include <random>
#include <string>
#include "NeuralNet.h"

#define num_linhas 5862
#define num_cols 11

#define num_linhas_conv 5862
#define num_colunas_conv 11

using namespace std;

string label[num_linhas][num_cols];

float *dataTrain;
float *dataCross;
float *dataTest;

void readData(string file)
{
	ifstream infile_data;
	infile_data.open(file, ios::binary);
	int i = 0, j = 0, k = 0;
	string aux;

	while (!infile_data.eof())
	{
		//cout << "." << endl;
		getline(infile_data, aux);

		i=0;
		j=0;

		int tamanho = aux.size();

		while (i<tamanho and j<num_cols)
		{
			if(aux[i] == '\t' || aux[i] == '\n')
			{
				j++;
			}		
			else
			{		
				label[k][j] += aux[i];
			//	cout << label[k][j] << endl;
			}
			i++;
		}		
		k++;
	}		

	infile_data.close();

}

void conversao(int &numRowsTrain, int &numRowsTest, int &numRowsCross)
{

	int i, j, k=0, l=0, m = 0;
	

	for (i=0; i<num_linhas_conv;i++)
	{
		if (atof(label[i][num_colunas_conv-1].c_str()) == 0)
			numRowsTrain++;
		else if (atof(label[i][num_colunas_conv-1].c_str()) == 1)
			numRowsCross++;
		else
			numRowsTest++;
		

	}

	cout << "\n\n******Values******\n" << endl;
	cout << "Numer train: " << numRowsTrain << endl;
	cout << "Numer cross: " << numRowsCross << endl;
	cout << "Numer test : " << numRowsTest << endl;
	cout << "Total: " << numRowsTest+numRowsTrain+numRowsCross << endl;
	cout << "\n******************\n" << endl;
	
	dataTest = new float[numRowsTest*(num_colunas_conv-1)];
	dataCross = new float[numRowsCross*(num_colunas_conv-1)];
	dataTrain = new float[numRowsTrain*(num_colunas_conv-1)];
	
	int isNull = 0;	
	
	for(i=0; i<num_linhas_conv; i++)
	{


		if (atof(label[i][num_colunas_conv-1].c_str()) == 0)
		{
			//cout << "\nTrain: " << endl;
			for(j=0; j < num_colunas_conv-1; j++)
			{
			
				dataTrain[k+j] = atof(label[i][j].c_str());
				//cout << k+j << " : " <<  dataTrain[k+j] << "\t" ;
		
			}		
			
			k += j;			
		}
		else if (atof(label[i][num_colunas_conv-1].c_str()) == 1)
		{	
			//cout << "\nCross: " << endl;
			for(j = 0; j < num_colunas_conv-1; j++)
			{
			
				dataCross[l+j] = atof(label[i][j].c_str());	
				//cout << l+j << " : " <<  dataCross[l+j] << "\t" ;	
			}		
			
			l += j;			
		}
		else
		{	
			//cout << "\nTest: " << endl;
			for(j = 0; j < num_colunas_conv-1; j++)
			{
			
				dataTest[m+j] = atof(label[i][j].c_str());	
				//cout << m+j << " : " <<  dataTest[m+j] << "\t" ;	
			}		
			
			m += j;			
		}

		//cout << endl;
	}	

	//cout << k << "\t " << l << "\t " << m << endl;
}

void divisaoTeste()
{
	int i, j;
	int max, min;

	//min = data[i][num_colunas-1];	
		
	//for (i=2; i<num_linhas; i++)
	//	for (j=0; j<num_colunas; j++)
	//		if((int(data[i][num_colunas-1])<min+1))
			//	cont++;



}

int main(int argc, char ** argv)
{

	int numberLayersHidden = atoi(argv[1]);
	int numberNeuronsHidden [atoi(argv[1])];
	int numRowsTrain = 0, numRowsTest = 0, numRowsCross = 0;
	string nameFile = argv[3];
	int i;

	for (i = 0; i < numberLayersHidden; i++)
		numberNeuronsHidden[i] = atoi(argv[2]);
	

	cout << "Number of layer hidden: "<< argv[1] << endl;
	cout << "Number of neurons layer hidden: "<< argv[2] << endl;
	cout << "Name input file: " << nameFile << endl;

	//Etapas de leitura do arquivo e separação entre train, cross e teste
	readData(nameFile);
	conversao(numRowsTrain, numRowsTest, numRowsCross);

	//O número de neuronios na camada de saída é determinada pelo que se espera prever como resultado, ou seja, o que a rede neural deve aprender.
	int numberNeuronsOutput = 10;


	//Criação da rede neural com a quantidade de camadas informadas, mais 
	NeuralNet mlp(numberLayersHidden+2);


	// num_cols - 2, já que as duas ultimas colunas é categoria e tipo de entrada
	mlp.training(numRowsTrain, numRowsCross, num_cols-1, num_cols-2, numberNeuronsOutput, numberNeuronsHidden, dataTrain, dataCross);
	//mlp.training(numRowsTrain, numRowsTrain, num_colunas_conv-1, dataTrain, num_colunas_conv-1, numberNeuronsOutput, &numberNeuronsHidden);
	mlp.testing(dataTest, numRowsTest,  num_cols-2, num_cols-1);

	cout << "S U C E S S O" << endl;

	return(0);
}
