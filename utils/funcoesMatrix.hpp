#ifndef TIPOMATRIX_HPP
#define TIPOMATRIX_HPP
#include <iostream>

typedef struct
{
    int nrow, ncol;
    int **mat;
} Matrix;

void printMatrix(Matrix *matrix);

Matrix *createMatrix(int nrow, int ncol);

void writeMatrixFile(Matrix *matrix, std::string caminhoArquivo);

#endif
