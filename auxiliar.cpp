#include <iostream>
#include <vector>
#include <fstream>
#include "funcoesMatrix.hpp"

using namespace std;

void writeMatrixFileBase(Matrix *matrix, string caminhoArquivo, int nrow, int ncow)
{
    ofstream matriz;
    matriz.open(caminhoArquivo);
    matriz << nrow << " " << ncow << endl;
    matriz.close();
    writeMatrixFile(matrix, caminhoArquivo);
}



void generateMatrix(Matrix *matrix)
{
    for (int ii = 0; ii < matrix->nrow; ii++)
    {
        for (int jj = 0; jj < matrix->ncol; jj++)
        {
            matrix->mat[ii][jj] = rand() % 10 + 1;
        }
    }
}

int main()
{

    // Definição de variaveis
    int aux, ii, jj, xx, rowA, colA, rowB, colB;
    Matrix *matrixA;
    Matrix *matrixB;
    Matrix *matrixC;

    // Entrada de dados
    // cout << "Informe a quntidade de linhas da matriz A : ";
    scanf("%d", &rowA);
    // cout << "Informe a quantidade de colunas da matriz A : ";
    scanf("%d", &colA);

    // cout << "Informe a quntidade de linhas da matriz B : ";
    scanf("%d", &rowB);
    // cout << "Informe a quantidade de colunas da matriz B : ";
    scanf("%d", &colB);

    matrixA = createMatrix(rowA, colA);
    matrixB = createMatrix(rowB, colB);
    matrixC = createMatrix(rowA, colB);

    if (colA == rowB)
    {

        generateMatrix(matrixA);

        generateMatrix(matrixB);

        writeMatrixFileBase(matrixA, "matrizM1.txt", rowA, colA);
        writeMatrixFileBase(matrixB, "matrizM2.txt", rowB, colB);
    }
    else{
        cout << "As medidas das matrizes informadas não são compativeis." << endl;
    }

    return 0;
}