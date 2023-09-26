#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <sstream>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;

int main(int argc, char *argv[]){
	const string arq1=argv[1], arq2=argv[2];
	int P=stoi(argv[3]);
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
	stringstream arq_name;
	fstream arq_res;
	int n_ele=(n1*m2)/P;
	pid_t pid[P-1];
	chrono::steady_clock::time_point begin = chrono::steady_clock::now();
	//Uso do multiprocessamento
	for(int h=0; h<P-1; h++){
		int contador=0;
		pid[h]=fork();
		chrono::steady_clock::time_point begin = chrono::steady_clock::now();
		if(pid[h]==0){
				arq_name<<n1<<"_"<<m2<<"_P="<<P<<"_proc"<<h<<".txt";
			arq_res.open(arq_name.str(), ios::out); 
			arq_res<< n1<<" "<< m2<<std::endl;
			for(int i=0; i<n1; i++){
				for(int j=0; j<m2; j++){
     				int soma=0;
     				if(contador>=h*n_ele && contador< (h+1)*n_ele){
     					for(int k=0; k<m1; k++)
     						soma+=(vetm1[i][k]*vetm2[k][j]);
     					arq_res<< "Res["<< i<<"]["<< j<<"] "<< soma<<std::endl;
     					}
     	    		contador++;					
     	    	}
        }
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        arq_res<<chrono::duration_cast<chrono::milliseconds>(end - begin).count()<<endl;
        arq_res.close();
        exit(0);
		}
	}
	//O pai também tem que trabalhar
	int contador=0;
	arq_name<<n1<<"_"<<m2<<"_P="<<P<<"_proc"<<P-1<<".txt";
	arq_res.open(arq_name.str(), ios::out); 
	arq_res<< n1<<" "<< m2<<std::endl;
	for(int i=0; i<n1; i++){
		for(int j=0; j<m2; j++){
     		int soma=0;
     		if(contador>=(P-1)*n_ele && contador< P*n_ele){
     			for(int k=0; k<m1; k++)
     				soma+=(vetm1[i][k]*vetm2[k][j]);
     			arq_res<< "Res["<< i<<"]["<< j<<"] "<< soma<<std::endl;
     			}
     	    contador++;					
     	   }
      }
        arq_res.close();
   //O pai espera o filhos acabarem
	while (wait(NULL)>0);
	chrono::steady_clock::time_point end = chrono::steady_clock::now();
	//Arq para salvar o tempo que levou do começo das criações dos processos ao fim de todas eles.
	fstream arq_tempo;
	arq_tempo.open("Tempo.txt", ios_base::app);
	arq_tempo<<chrono::duration_cast<chrono::milliseconds>(end - begin).count()<<endl;
	arq_tempo.close();
	return 0;
}
