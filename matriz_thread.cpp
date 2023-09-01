#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

typedef struct
{
    int posRow;
    int posCol;
} CurrentRowCol;

typedef struct
{
    int nrow, ncol;
    int **mat;
} Matrix;

typedef struct
{
    int age;
} Person;

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

void readMatrix(Matrix *matrix)
{
    for (int ii = 0; ii < matrix->nrow; ii++)
    {
        for (int jj = 0; jj < matrix->ncol; jj++)
        {
            scanf(" %d", &(matrix->mat[ii][jj]));
        }
    }
}

void destroyMatrix(Matrix **m)
{
    free(*m);
    *m = NULL;
}

// inicio variaveis globais

int aux = 0;
Matrix *matrixA;
Matrix *matrixB;
Matrix *matrixC;

// fim variaveis globias

void *calculeElementInMatrix(void *tid)
{
    int xx;
    CurrentRowCol *rowCol = (CurrentRowCol *)tid;
    // printf("THREAD: %d %d\n", rowCol->posRow, rowCol->posCol);

    matrixC->mat[rowCol->posRow][rowCol->posCol] = 0;
    for (xx = 0; xx < matrixB->nrow; xx++)
    {
        aux += matrixA->mat[rowCol->posRow][xx] * matrixB->mat[xx][rowCol->posCol];
    }

    matrixC->mat[rowCol->posRow][rowCol->posCol] = aux;

    aux = 0;
    pthread_exit(nullptr);
}

int main()
{

    // Definição de variaveis
    int ii, jj, xx, rowA, colA, rowB, colB;

    pthread_t *threads;

    // Entrada de dados
    cout << "Informe a quntidade de linhas da matriz A : ";
    scanf("%d", &rowA);
    cout << "Informe a quantidade de colunas da matriz A : ";
    scanf("%d", &colA);

    cout << "Informe a quntidade de linhas da matriz B : ";
    scanf("%d", &rowB);
    cout << "Informe a quantidade de colunas da matriz B : ";
    scanf("%d", &colB);

    threads = (pthread_t *)malloc(sizeof(pthread_t) * (rowA * colB));
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

        for (ii = 0; ii < rowA; ii++)
        {
            for (jj = 0; jj < colA; jj++)
            {
                cout << matrixA->mat[ii][jj] << " ";
            }
            cout << endl;
        }

        cout << "================ MATRIZ B ================" << endl;
        for (ii = 0; ii < rowB; ii++)
        {
            for (jj = 0; jj < colB; jj++)
            {
                cout << matrixB->mat[ii][jj] << " ";
            }
            cout << endl;
        }

        // Processamento e saida em tela  =  PRODUTO DAS MATRIZES
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        for (ii = 0; ii < rowA; ii++)
        {
            for (jj = 0; jj < colB; jj++)
            {
                CurrentRowCol *currentRowCol = (CurrentRowCol *)malloc(sizeof(CurrentRowCol));

                currentRowCol->posRow = ii;
                currentRowCol->posCol = jj;

                pthread_create(&threads[ii * rowA + jj], NULL, calculeElementInMatrix, currentRowCol);
                // matrixC->mat[ii][jj] = 0;
                // for (xx = 0; xx < rowB; xx++)
                // {
                //     aux += matrixA->mat[ii][xx] * matrixB->mat[xx][jj];
                // }

                // matrixC->mat[ii][jj] = aux;

                // aux = 0;
            }
        }
        chrono::steady_clock::time_point end = chrono::steady_clock::now();

        cout << "================ MATRIZ C - MATRIZ GERADA ================" << endl;

        for (ii = 0; ii < rowA; ii++)
        {
            for (jj = 0; jj < colB; jj++)
            {
                cout << matrixC->mat[ii][jj] << " ";
            }
            cout << endl;
        }
        cout << endl
             << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " [ms]" << endl;
    }
    else
    {
        cout << "Nao ha com multiplicar as matrizes dadas ";
    }

    return 0;
}