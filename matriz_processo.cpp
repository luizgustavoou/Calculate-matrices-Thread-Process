
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>

#include <chrono>
#include <iostream>

using namespace std;
int main()
{

    // Definição de variaveis
    int ii, jj, linhaA, colunaA, linhaB, colunaB, x;

    // Entrada de dados
    cout << "Informe a quntidade de linhas da matriz A : ";
    cin >> linhaA;
    cout << "Informe a quantidade de colunas da matriz A : ";
    cin >> colunaA;
    cout << "Informe a quntidade de linhas da matriz B : ";
    cin >> linhaB;
    cout << "Informe a quantidade de colunas da matriz B : ";
    cin >> colunaB;

    int matrizA[linhaA][colunaA];
    int matrizB[linhaB][colunaB];
    int matrizC[linhaA][colunaB];

    // vector<vector<int>> *mem;
    // int valor = shmget(IPC_PRIVATE, 20 * sizeof(vector<vector<int>>), IPC_CREAT | 0666);
    // mem = static_cast<vector<vector<int>> *>(shmat(valor, NULL, 0));
    int shmid = shmget(IPC_PRIVATE, sizeof(int) * linhaA * colunaB, IPC_CREAT | 0666);
    int *sharedMem = (int *)shmat(shmid, NULL, 0);
    int aux = 0;

    if (colunaA == linhaB)
    {

        for (ii = 0; ii < linhaA; ii++)
        {
            for (jj = 0; jj < colunaA; jj++)
            {
                matrizA[ii][jj] = rand() % 10 + 1;
            }
        }

        for (ii = 0; ii < linhaB; ii++)
        {
            for (jj = 0; jj < colunaB; jj++)
            {
                matrizB[ii][jj] = rand() % 10 + 1;
            }
        }

        // Imprime as matrizes definidas
        // cout << "---------------------------- MATRIZ A ---------------------------------" << endl;

        // for (ii = 0; ii < linhaA; ii++)
        // {
        //     for (jj = 0; jj < colunaA; jj++)
        //     {
        //         cout << matrizA[ii][jj] << " ";
        //     }
        //     cout << endl;
        // }

        // cout << "---------------------------- MATRIZ B ---------------------------------" << endl;
        // for (ii = 0; ii < linhaB; ii++)
        // {
        //     for (jj = 0; jj < colunaB; jj++)
        //     {
        //         cout << matrizB[ii][jj] << " ";
        //     }
        //     cout << endl;
        // }

        // cout << "---------------------------- MATRIZ C - MATRIZ GERADA ---------------------------------" << endl;

        // Processamento e saida em tela  =  PRODUTO DAS MATRIZES
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        for (ii = 0; ii < linhaA; ii++)
        {
            for (jj = 0; jj < colunaB; jj++)
            {
                matrizC[ii][jj] = 0;

                pid_t pid = fork();

                if (pid == 0)
                {
                    // mem = static_cast<vector<vector<int>> *>(shmat(valor, NULL, 0));

                    for (x = 0; x < linhaB; x++)
                    {
                        aux += matrizA[ii][x] * matrizB[x][jj];
                    }

                    // cout << aux << " - " << (*mem)[ii][jj] << endl;
                    // cout << aux << " - " <<  endl;
                    // (*mem)[ii][jj] = aux;
                    sharedMem[ii * colunaB + jj] = aux;

                    // cout << aux << " - " << sharedMem[ii * colunaB + jj] << endl;
                    aux = 0;
                    exit(EXIT_SUCCESS);
                }
                else if (pid > 0)
                {
                    // pai
                }
            }
        }
        chrono::steady_clock::time_point end = chrono::steady_clock::now();

        // for (ii = 0; ii < linhaA; ii++)
        // {
        //     for (jj = 0; jj < colunaB; jj++)
        //     {
        //         // cout << matrizC[ii][jj] << " ";
        //         cout << sharedMem[ii * colunaB + jj] << " ";
        //     }
        //     cout << endl;
        // }
        cout << endl
             << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " [ms]" << endl;
    }
    else
    {
        cout << "Nao ha com multiplicar as matrizes dadas ";
    }

    return 0;

} // cabooooo
