#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

int main( int argc, char *argv[] ){
	const string arq1=argv[1], arq2=argv[2];
	//Input da matriz 1 e 2 em um vetor de vetores
	fstream M1, M2;
	string linha, elemento;
	M1.open(arq1+".txt", ios::in);
  	getline(M1, linha);
  	int n1=stoi(linha);
  	int m1=stoi(linha.substr(linha.find(" ")+1, linha.size()-linha.find(" ")));
  	std::vector<std::vector<int>> vetm1;
  	for(int i=0; i<n1; i++){
		vetm1.push_back(vector<int>());
		for(int j=0; j<m1; j++){
			getline(M1, linha);
			elemento=linha.back();
			if(elemento=="0")
				elemento="10";
			vetm1[i].push_back(stoi(elemento));
		}
	}
	M1.close();
    
	M2.open(arq2+".txt", ios::in);
	getline(M2, linha);
  	int n2=stoi(linha);
  	int m2=stoi(linha.substr(linha.find(" ")+1, linha.size()-linha.find(" ")));
	std::vector<std::vector<int>> vetm2;
	for(int i=0; i<n2; i++){
		vetm2.push_back(vector<int>());
		for(int j=0; j<m2; j++){
			getline(M2, linha);
			elemento=linha.back();
			if(elemento=="0")
				elemento="10";
			vetm2[i].push_back(stoi(elemento));
		}
	}
    M2.close();
  	
  	//tempo de cálculo  da matriz resultado
  	fstream arq_res;
  	stringstream arq_name;
  	arq_name<<n1<<"_"<<m2<<" Sequencial.txt";
  	arq_res.open(arq_name.str(), ios::out); 
	arq_res<< n1<<" "<< m2<<std::endl;
	chrono::steady_clock::time_point begin = chrono::steady_clock::now();
  	for(int i=0; i<n1; i++){
		for(int j=0; j<m2; j++){
     		int soma=0;
     		for(int k=0; k<m1; k++){
     	       soma+=(vetm1[i][k]*vetm2[k][j]);
     	    }
		arq_res<< "Res["<< i<<"]["<< j<<"] "<< soma<<std::endl;
     	    }
        }
	chrono::steady_clock::time_point end = chrono::steady_clock::now();
	arq_res<<chrono::duration_cast<chrono::milliseconds>(end - begin).count()<<endl;
	arq_res.close();
	fstream arq_tempo;
	arq_tempo.open("Tempo.txt", ios_base::app);
	arq_tempo<<chrono::duration_cast<chrono::milliseconds>(end - begin).count()<<endl;
	arq_tempo.close();
  return 0;  
}