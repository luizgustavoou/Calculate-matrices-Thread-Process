
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>

#include <chrono>
#include <iostream>

using namespace std;

typedef struct
{
    int posStart;
    int posEnd;
} MatrixPartition;

typedef struct
{
    int nrow, ncol;
    int *mat;
} MyArray;

MyArray *createMyArray(int nrow, int ncol)
{
    MyArray *myArray = (MyArray *)malloc(sizeof(MyArray));
    myArray->nrow = nrow;
    myArray->ncol = ncol;

    myArray->mat = (int *)calloc(nrow * ncol, sizeof(int));

    return myArray;
}

// inicio variaveis globais

MyArray *matrixA;
MyArray *matrixB;

// fim variaveis globias

void printArray(int *array, int nrow, int ncol)
{
    for (int ii = 0; ii < nrow; ii++)
    {
        for (int jj = 0; jj < ncol; jj++)
        {
            cout << array[ii * nrow + jj] << " ";
        }
        cout << endl;
    }
}

void calculeElementInMatrix(MatrixPartition *matrixPartition, int *sharedMemory, int nrow, int ncol)
{
    int xx, aux = 0;
    // printf("PROCESSO [%d]: %d %d\n", getpid(), matrixPartition->posStart, matrixPartition->posEnd);

    for (int ii = matrixPartition->posStart; ii <= matrixPartition->posEnd; ii++)
    {
        sharedMemory[ii] = 0;
        int colB = ii % ncol;
        int rowA = (ii - colB) / nrow;
        for (xx = 0; xx < matrixB->nrow; xx++)
        {
            aux += matrixA->mat[rowA * matrixA->nrow + xx] * matrixB->mat[xx * matrixB->nrow + colB];
        }
        sharedMemory[ii] = aux;
        aux = 0;
    }
}

int main()
{

    // Definição de variaveis
    int ii, jj, xx, rowA, colA, rowB, colB;
    int PARTITION_MATRIX;

    // Entrada de dados
    cout << "Informe a quantidade de elementos a serem calculados na matriz por thread: ";
    scanf("%d", &PARTITION_MATRIX);
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

    int shmid = shmget(IPC_PRIVATE, sizeof(int) * matrixA->nrow * matrixB->ncol, IPC_CREAT | 0666);
    int *sharedMem = (int *)shmat(shmid, NULL, 0);

    int lenProcesses = (matrixA->nrow * matrixB->ncol) / PARTITION_MATRIX;
    lenProcesses = lenProcesses <= 0 ? 1 : lenProcesses; // criar no minimo 1 processo.

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
        printArray(matrixA->mat, matrixA->nrow, matrixA->ncol);

        cout << "================ MATRIZ B ================" << endl;
        printArray(matrixB->mat, matrixB->nrow, matrixB->ncol);

        // Processamento e saida em tela  =  PRODUTO DAS MATRIZES
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        // int index_thread = 0;
        const int lastPosMatrixC = matrixA->nrow * matrixB->ncol - 1;

        for (int pp = 0; pp < matrixA->nrow * matrixB->ncol; pp = pp + PARTITION_MATRIX)
        {
            MatrixPartition *matrixPartition = (MatrixPartition *)malloc(sizeof(MatrixPartition));
            matrixPartition->posStart = pp;
            int posEnd = (pp + PARTITION_MATRIX - 1);

            matrixPartition->posEnd = posEnd > lastPosMatrixC ? lastPosMatrixC : posEnd;

            pid_t pid = fork();
            if (pid == 0)
            {
                calculeElementInMatrix(matrixPartition, sharedMem, matrixA->nrow, matrixB->ncol);
                exit(EXIT_SUCCESS);
            }
        }

        chrono::steady_clock::time_point end = chrono::steady_clock::now();

        for (int ii = 0; ii < lenProcesses; ii++)
        {
            // Esperar os processos acabarem para depois poder printar a matriz C (caso queira)
            wait(NULL);
        }

        cout << "================ MATRIZ C - MATRIZ GERADA ================" << endl;
        printArray(sharedMem, matrixA->nrow, matrixB->ncol);

        cout << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " [ms]" << endl;
    }
    else
    {
        cout << "Nao ha com multiplicar as matrizes dadas ";
    }

    return 0;
}