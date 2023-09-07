#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include "funcoesMatrix.hpp"

using namespace std;

Matrix *readMatrix(string filePath)
{
    ifstream file;
    int nrow, ncol, ii = 0, jj = 0;

    file.open(filePath, ios::app);

    Matrix *matrix;

    file >> nrow >> ncol;

    matrix = createMatrix(nrow, ncol);

    while (file)
    {

        file >> matrix->mat[ii][jj];
        jj++;
        if (jj >= matrix->ncol)
        {
            jj = 0;
            ii++;
        }
    }

    file.close();
    return matrix;
}

void writeResultMatrixInFile(Matrix *matrix, string filePath, int time)
{
    ofstream file;
    file.open(filePath);
    file << time;
    file << " [ms]" << endl;
    file.close();
    writeMatrixFile(matrix, filePath);
}

int main()
{
    // Definição de variaveis
    int aux = 0, ii, jj, xx;
    int time;
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
    time = chrono::duration_cast<chrono::milliseconds>(end - begin).count();
    writeResultMatrixInFile(matrixC, "multiplicacaoSequencial.txt", time);

    return 0;
}