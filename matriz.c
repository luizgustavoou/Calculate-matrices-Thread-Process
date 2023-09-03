#include <stdio.h>
#include <stdlib.h>
#include <time.h>

clock_t begin, end;
double time_spent;

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
            printf("%d", matrix->mat[ii][jj]);
            printf(" ");
        }
        printf("\n");
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
    printf("Informe a quntidade de linhas da matriz A : ");
    scanf("%d", &rowA);
    printf("Informe a quantidade de colunas da matriz A : ");
    scanf("%d", &colA);

    printf("Informe a quntidade de linhas da matriz B : ");
    scanf("%d", &rowB);
    printf("Informe a quantidade de colunas da matriz B : ");
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
        printf("================ MATRIZ A ================\n");
        printMatrix(matrixA);

        printf("================ MATRIZ B ================\n");
        printMatrix(matrixB);

        // Processamento e saida em tela  =  PRODUTO DAS MATRIZES
        begin = clock(); // Registra o tempo de início
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
        end = clock(); // Registra o tempo de término

        printf("================ MATRIZ C - MATRIZ GERADA ================\n");
        printMatrix(matrixC);

        time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Tempo de execução: %f ms\n", time_spent*1000);
    }
    else
    {
        printf("Não ha com multiplicar as matrizes dadas ");
    }

    return 0;
}