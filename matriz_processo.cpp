
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <fstream>

#include <chrono>
#include <iostream>
#include <math.h>

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
MyArray *matrixC;

// fim variaveis globias

void printArray(MyArray *matrix)
{
    for (int ii = 0; ii < matrix->nrow; ii++)
    {
        for (int jj = 0; jj < matrix->ncol; jj++)
        {
            cout << matrix->mat[ii * matrix->ncol + jj] << " ";
        }
        cout << endl;
    }
}

void calculeElementInMatrix(MatrixPartition *matrixPartition, MyArray *matrixC)
{
    int xx, aux = 0;
    // printf("PROCESSO [%d]: %d %d\n", getpid(), matrixPartition->posStart, matrixPartition->posEnd);

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
}

// void generateMatrix(MyArray *array)
// {
//     for (int ii = 0; ii < array->nrow; ii++)
//     {
//         for (int jj = 0; jj < array->ncol; jj++)
//         {
//             array->mat[ii * array->ncol + jj] = rand() % 10 + 1;
//         }
//     }
// }

void writeMatrixFile(MyArray *matrix, string filePath, int tempo)
{
    ofstream file;
    file.open(filePath);
    file << matrix->nrow << " " << matrix->ncol << endl;
    for (int ii = 0; ii < matrix->nrow; ii++)
    {
        for (int jj = 0; jj < matrix->ncol; jj++)
        {
            file << matrix->mat[ii * matrix->ncol + jj] << " ";
        }
        file << endl;
    }
    file << tempo << " [ms]" << endl;
    file.close();
}

MyArray *readMatrix(string filePath)
{
    ifstream file;
    int nrow, ncol, ii = 0, jj = 0;

    file.open(filePath, ios::app);

    MyArray *matrix;

    file >> nrow >> ncol;

    matrix = createMyArray(nrow, ncol);

    while (file)
    {
        file >> matrix->mat[ii * matrix->ncol + jj];
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

int main()
{

    // Definição de variaveis
    // int ii, jj, xx, rowA, colA, rowB, colB;
    int length_processes, elements_per_processes;

    // Entrada de dados
    // cout << "Informe a quantidade de elementos a serem calculados na matriz por thread: ";
    scanf("%d", &length_processes);
    // cout << "Informe a quntidade de linhas da matriz A : ";
    // scanf("%d", &rowA);
    // cout << "Informe a quantidade de colunas da matriz A : ";
    // scanf("%d", &colA);

    // cout << "Informe a quntidade de linhas da matriz B : ";
    // scanf("%d", &rowB);
    // cout << "Informe a quantidade de colunas da matriz B : ";
    // scanf("%d", &colB);

    // matrixA = createMyArray(rowA, colA);
    // matrixB = createMyArray(rowB, colB);

    matrixA = readMatrix("matrizM1.txt");
    matrixB = readMatrix("matrizM2.txt");
    matrixC = createMyArray(matrixA->nrow, matrixB->ncol);

    int shmid = shmget(IPC_PRIVATE, sizeof(int) * matrixA->nrow * matrixB->ncol, IPC_CREAT | 0666);
    int *sharedMem = (int *)shmat(shmid, NULL, 0);

    free(matrixC->mat);
    matrixC->mat = sharedMem;

    length_processes = length_processes <= 0 ? 1 : length_processes; // criar no minimo 1 processo.
    elements_per_processes = ceil((float)(matrixA->nrow * matrixB->ncol) / (float)length_processes);
    if (matrixA->ncol == matrixB->nrow)
    {

        // generateMatrix(matrixA);

        // generateMatrix(matrixB);

        // Imprime as matrizes definidas
        // cout << "================ MATRIZ A ================" << endl;
        // printArray(matrixA);

        // cout << "================ MATRIZ B ================" << endl;
        // printArray(matrixB);

        // Processamento e saida em tela  =  PRODUTO DAS MATRIZES
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        // int index_thread = 0;
        const int lastPosMatrixC = matrixA->nrow * matrixB->ncol - 1;

        for (int pp = 0; pp < matrixA->nrow * matrixB->ncol; pp = pp + elements_per_processes)
        {
            MatrixPartition *matrixPartition = (MatrixPartition *)malloc(sizeof(MatrixPartition));
            matrixPartition->posStart = pp;
            int posEnd = (pp + elements_per_processes - 1);

            matrixPartition->posEnd = posEnd > lastPosMatrixC ? lastPosMatrixC : posEnd;

            pid_t pid = fork();
            if (pid == 0)
            {
                calculeElementInMatrix(matrixPartition, matrixC);
                exit(EXIT_SUCCESS);
            }
        }

        for (int ii = 0; ii < length_processes; ii++)
        {
            // Esperar os processos acabarem para depois poder printar a matriz C (caso queira)
            wait(NULL);
            if (elements_per_processes * (ii + 1) >= matrixA->nrow * matrixB->ncol)
            {
                break;
            }
        }
        chrono::steady_clock::time_point end = chrono::steady_clock::now();

        // cout << "================ MATRIZ C - MATRIZ GERADA ================" << endl;
        // printArray(matrixC);

        // cout << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " [ms]" << endl;
        writeMatrixFile(matrixC, "multiplicacaoProcesso.txt", chrono::duration_cast<chrono::milliseconds>(end - begin).count());
    }
    else
    {
        cout << "Nao ha com multiplicar as matrizes dadas ";
    }

    return 0;
}