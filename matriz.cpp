#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include "funcoesMatrix.hpp"

using namespace std;

Matrix * readMatrix(string caminhoArquivo)
{
    ifstream matriz;
    matriz.open(caminhoArquivo, ios::app);
    string linha1, token;
    Matrix *matrix;
    int ii = 0, jj;

    getline(matriz, linha1);
    istringstream iss(linha1);
    vector<int> tokens;  // Para armazenar os valores separados
    while (getline(iss, token, ' '))
    {
        tokens.push_back(stoi(token));
    }
    matrix = createMatrix(tokens[0], tokens[1]);

    while(getline(matriz, linha1))
    {
        istringstream iss(linha1);
        jj = 0;
        vector<int> tokens;  // Para armazenar os valores separados
        while (getline(iss, token, ' '))
        {
            tokens.push_back(stoi(token));
        }

        for (const int& value : tokens)
        {
            matrix->mat[ii][jj] = value;
            jj ++;
        }
        ii ++;
    }
    matriz.close();
    return matrix;
}

void escreveMatrixResultadoArquivo(Matrix *matrix, string caminhoArquivo, int tempo)
{
    ofstream matriz;
    matriz.open(caminhoArquivo);
    matriz << tempo;
    matriz << " [ms]" << endl;
    matriz.close();
    escreveMatrixArquivo(matrix, caminhoArquivo);
}

int main()
{
    // Definição de variaveis
    int aux = 0, ii, jj, xx;
    int tempo;
    Matrix *matrixA;
    Matrix *matrixB;
    Matrix *matrixC;

    matrixA = readMatrix("matrizM1.txt");

    matrixB = readMatrix("matrizM2.txt");

    matrixC = createMatrix(matrixA->nrow, matrixB->ncol);


    // Processamento e saida em tela  =  PRODUTO DAS MATRIZES
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    for (ii = 0; ii < matrixC->nrow; ii++)
    {
        for (jj = 0; jj < matrixC->ncol; jj++)
        {
            matrixC->mat[ii][jj] = 0;
            for (xx = 0; xx < matrixB->nrow; xx++)
            {
                aux += matrixA->mat[ii][xx] * matrixB->mat[xx][jj];
            }

            matrixC->mat[ii][jj] = aux;

            aux = 0;
        }
    }
    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    cout << "================ MATRIZ C - MATRIZ GERADA ================" << endl;
    printMatrix(matrixC);
    tempo = chrono::duration_cast<chrono::milliseconds>(end - begin).count();
    escreveMatrixResultadoArquivo(matrixC, "multiplicacaoSequencial.txt", tempo);

    return 0;
}