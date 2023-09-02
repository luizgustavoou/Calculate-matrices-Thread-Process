#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

typedef struct
{
    int nrow, ncol;
    int **mat;
} Matrix;

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

int main()
{

    // Definição de variaveis
    int aux, ii, jj, xx, rowA, colA, rowB, colB;
    Matrix *matrixA;
    Matrix *matrixB;
    Matrix *matrixC;

    // Entrada de dados
    cout << "Informe a quntidade de linhas da matriz A : ";
    scanf("%d", &rowA);
    cout << "Informe a quantidade de colunas da matriz A : ";
    scanf("%d", &colA);

    cout << "Informe a quntidade de linhas da matriz B : ";
    scanf("%d", &rowB);
    cout << "Informe a quantidade de colunas da matriz B : ";
    scanf("%d", &colB);

    matrixA = createMatrix(rowA, colA);
    matrixB = createMatrix(rowB, colB);
    matrixC = createMatrix(rowA, colB);

    if (colA == rowB)
    {

        for (ii = 0; ii < rowA; ii++)
        {
            for (jj = 0; jj < colA; jj++)
            {
                matrixA->mat[ii][jj] = rand() % 10 + 1;
            }
        }

        for (ii = 0; ii < rowB; ii++)
        {
            for (jj = 0; jj < colB; jj++)
            {
                matrixB->mat[ii][jj] = rand() % 10 + 1;
            }
        }

        // Imprime as matrizes definidas
        cout << "================ MATRIZ A ================" << endl;
        printMatrix(matrixA);

        cout << "================ MATRIZ B ================" << endl;
        printMatrix(matrixB);

        // Processamento e saida em tela  =  PRODUTO DAS MATRIZES
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        for (ii = 0; ii < matrixC->nrow; ii++)
        {
            for (jj = 0; jj < matrixC->ncol; jj++)
            {

                matrixC->mat[ii][jj] = 0;
                for (xx = 0; xx < rowB; xx++)
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

        cout << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " [ms]" << endl;
    }
    else
    {
        cout << "Nao ha com multiplicar as matrizes dadas ";
    }

    return 0;
}