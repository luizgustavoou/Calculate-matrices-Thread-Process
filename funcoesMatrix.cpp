#include <iostream>
#include <fstream>
#include "funcoesMatrix.hpp"

using namespace std;

void printMatrix(Matrix *matrix)
{
    for (int ii = 0; ii < matrix->nrow; ii++)
    {
        for (int jj = 0; jj < matrix->ncol; jj++)
        {
            cout << matrix->mat[ii][jj] << " ";
        }
        cout << endl;
    }
}

Matrix *createMatrix(int nrow, int ncol)
{
    Matrix *matrix = (Matrix *)malloc(sizeof(Matrix));
    matrix->nrow = nrow;
    matrix->ncol = ncol;

    matrix->mat = (int **)calloc(nrow, sizeof(int *));

    for (int jj = 0; jj < nrow; jj++)
    {
        matrix->mat[jj] = (int *)calloc(ncol, sizeof(int));
    }

    return matrix;
}

void escreveMatrixArquivo(Matrix *matrix, string caminhoArquivo)
{
    ofstream matriz;
    matriz.open(caminhoArquivo, ios::app);
    for (int ii = 0; ii < matrix->nrow; ii++)
    {
        for (int jj = 0; jj < matrix->ncol; jj++)
        {
            matriz << matrix->mat[ii][jj] << " ";
        }
        matriz << endl;
    }
    matriz.close();
}
