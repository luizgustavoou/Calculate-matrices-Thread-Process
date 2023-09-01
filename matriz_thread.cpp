#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
const int PARTITION_MATRIX = 50;

typedef struct
{
    int posStart;
    int posEnd;
} MatrixPartition;

typedef struct
{
    int nrow, ncol;
    int **mat;
} Matrix;

typedef struct
{
    int nrow, ncol;
    int *mat;
} MyArray;

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

MyArray *createMyArray(int nrow, int ncol)
{
    MyArray *myArray = (MyArray *)malloc(sizeof(MyArray));
    myArray->nrow = nrow;
    myArray->ncol = ncol;

    myArray->mat = (int *)calloc(nrow * ncol, sizeof(int));

    return myArray;
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

MyArray *matrixA;
MyArray *matrixB;
MyArray *matrixC;

// fim variaveis globias

void *calculeElementInMatrix(void *tid)
{
    int xx, aux = 0;
    MatrixPartition *matrixPartition = (MatrixPartition *)tid;
    // printf("THREAD: %d %d\n", matrixPartition->posStart, matrixPartition->posEnd);

    for (int ii = matrixPartition->posStart; ii <= matrixPartition->posEnd; ii++)
    {
        matrixC->mat[ii] = 0;
        int colB = ii % matrixC->ncol;
        int rowA = (ii - colB) / matrixC->nrow;
        for (xx = 0; xx < matrixB->nrow; xx++)
        {
            aux += matrixA->mat[rowA * matrixA->nrow + xx] * matrixB->mat[xx * matrixB->nrow + colB];
        }
        matrixC->mat[ii] = aux;
        aux = 0;
    }

    pthread_exit(NULL);
}

int main()
{

    // Definição de variaveis
    void *threads_arg;
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

    matrixA = createMyArray(rowA, colA);
    matrixB = createMyArray(rowB, colB);
    matrixC = createMyArray(rowA, colB);
    int lenThreads = (matrixC->nrow * matrixC->ncol) / PARTITION_MATRIX;
    lenThreads = lenThreads <= 0 ? 1 : lenThreads; // criar no minimo 1 thread.
    threads = (pthread_t *)malloc(sizeof(pthread_t) * lenThreads);

    if (colA == rowB)
    {

        for (ii = 0; ii < rowA; ii++)
        {
            for (jj = 0; jj < colA; jj++)
            {
                matrixA->mat[ii * rowA + jj] = rand() % 10 + 1;
            }
        }

        for (ii = 0; ii < rowB; ii++)
        {
            for (jj = 0; jj < colB; jj++)
            {
                matrixB->mat[ii * rowB + jj] = rand() % 10 + 1;
            }
        }

        // Imprime as matrizes definidas
        cout << "================ MATRIZ A ================" << endl;

        for (ii = 0; ii < rowA; ii++)
        {
            for (jj = 0; jj < colA; jj++)
            {
                cout << matrixA->mat[ii * rowA + jj] << " ";
            }
            cout << endl;
        }

        cout << "================ MATRIZ B ================" << endl;
        for (ii = 0; ii < rowB; ii++)
        {
            for (jj = 0; jj < colB; jj++)
            {
                cout << matrixB->mat[ii * rowB + jj] << " ";
            }
            cout << endl;
        }

        // Processamento e saida em tela  =  PRODUTO DAS MATRIZES
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        int index_thread = 0;
        const int lastPosMatrixC = matrixC->nrow * matrixC->ncol - 1;

        for (int pp = 0; pp < matrixC->nrow * matrixC->ncol; pp = pp + PARTITION_MATRIX)
        {
            MatrixPartition *matrixPartition = (MatrixPartition *)malloc(sizeof(MatrixPartition));
            matrixPartition->posStart = pp;
            int posEnd = (pp + PARTITION_MATRIX - 1);

            matrixPartition->posEnd = posEnd > lastPosMatrixC ? lastPosMatrixC : posEnd;

            pthread_create(&threads[index_thread], NULL, calculeElementInMatrix, matrixPartition);
            index_thread++;
        }

        chrono::steady_clock::time_point end = chrono::steady_clock::now();

        for (int ii = 0; ii < lenThreads; ii++)
        {
            // Esperar os processos acabarem para depois poder printar a matriz C (caso queira)
            pthread_join(threads[ii], &threads_arg);
        }

        cout << "================ MATRIZ C - MATRIZ GERADA ================" << endl;

        for (ii = 0; ii < matrixC->nrow; ii++)
        {
            for (jj = 0; jj < matrixC->ncol; jj++)
            {
                cout << matrixC->mat[ii * rowA + jj] << " ";
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