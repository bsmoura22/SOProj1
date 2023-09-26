#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <sstream>
#include <thread>
using namespace std;

// Variáveis globais
vector<vector<int>> vetm1;
vector<vector<int>> vetm2;
int thread_i = 0;
int n1;
int m1;
int n2;
int m2;
int n_ele;
int P;

void* multiplicacao(void* arg) {
    int thread_n = (*(int*)arg);
    fstream arq_res;
    stringstream arq_nome;
    int contador = thread_n * n_ele;
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    arq_nome << n1 << "_" << m2 << "_P=" << P << "_thread" << thread_n << ".txt";
    arq_res.open(arq_nome.str(), ios::out);
    arq_res << n1 << " " << m2 << endl;

    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++) {
            int soma = 0;
            if (contador < (thread_n + 1) * n_ele) {
                for (int k = 0; k < m1; k++)
                    soma += (vetm1[i][k] * vetm2[k][j]);
                arq_res << "Res[" << i << "][" << j << "] " << soma << endl;
            }
            contador++;
        }
    }

    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    arq_res << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
    arq_res.close();
    delete (int*)arg;  // Liberar o ponteiro passado como argumento
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    const string arq1 = argv[1], arq2 = argv[2];
    P = stoi(argv[3]);

    // Input da matriz 1 e 2 em um vetor de vetores
    fstream M1, M2;
    string linha, elemento;

    M1.open(arq1 + ".txt", ios::in);
    getline(M1, linha);
    n1 = stoi(linha);
    m1 = stoi(linha.substr(linha.find(" ") + 1, linha.size() - linha.find(" ")));

    for (int i = 0; i < n1; i++) {
        vetm1.push_back(vector<int>());
        for (int j = 0; j < m1; j++) {
            getline(M1, linha);
            elemento = linha.back();
            if (elemento == "0")
                elemento = "10";
            vetm1[i].push_back(stoi(elemento));
        }
    }
    M1.close();

    M2.open(arq2 + ".txt", ios::in);
    getline(M2, linha);
    n2 = stoi(linha);
    m2 = stoi(linha.substr(linha.find(" ") + 1, linha.size() - linha.find(" ")));

    for (int i = 0; i < n2; i++) {
        vetm2.push_back(vector<int>());
        for (int j = 0; j < m2; j++) {
            getline(M2, linha);
            elemento = linha.back();
            if (elemento == "0")
                elemento = "10";
            vetm2[i].push_back(stoi(elemento));
        }
    }
    M2.close();

    n_ele = (n1 * m2) / P;

    // Criação das threads
    pthread_t threads[P];

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    for (int i = 0; i < P - 1; i++) {
        int* ponteiro = new int(i);
        pthread_create(&threads[i], NULL, multiplicacao, (void*)(ponteiro));
    }

    int* ponteiro_main = new int(P - 1);
    pthread_create(&threads[P - 1], NULL, multiplicacao, (void*)(ponteiro_main));

    // A thread principal espera as outras acabarem
    for (int i = 0; i < P; i++)
        pthread_join(threads[i], NULL);

    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    // Arq para salvar o tempo que levou do começo das criações das threads ao fim de todas elas.
    fstream arq_tempo;
    arq_tempo.open("Tempo.txt", ios_base::app);
    arq_tempo << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
    arq_tempo.close();

    // Liberar a memória alocada para os ponteiros
    delete ponteiro_main;
    return 0;
}
