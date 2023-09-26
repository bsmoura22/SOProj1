#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// criar matriz
int main(int argc, char *argv[])
{
    int n1 = atoi(argv[1]);
    int m1 = atoi(argv[2]);
    int n2 = atoi(argv[3]);
    int m2 = atoi(argv[4]);

    fstream matriz1;
    matriz1.open("Matriz_1.txt", ios::out);
    if (matriz1.is_open())
    {
        matriz1 << n1 << " " << m1 << endl;
        for (int i = 0; i < n1; i++)
        {
            for (int j = 0; j < m1; j++)
            {
                matriz1 << "M1[" << i << "][" << j << "] " << (rand() % 10 + 1) << endl;
            }
        }
        matriz1.close();
    }

    fstream matriz2;
    matriz2.open("Matriz_2.txt", ios::out);
    if (matriz2.is_open())
    {
        matriz2 << n2 << " " << m2 << endl;
        for (int i = 0; i < n2; i++)
        {
            for (int j = 0; j < m2; j++)
            {
                matriz2 << "M2[" << i << "][" << j << "] " << (rand() % 10 + 1) << endl;
            }
        }
        matriz2.close();
    }
    return 0;
}
