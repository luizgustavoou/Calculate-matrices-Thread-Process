#include <iostream>
#include <vector>
#include <chrono>
#include <math.h>
#include <fstream>
#include <pthread.h>

using namespace std;

typedef struct
{
    int posStart;
    int posEnd;
    int numThread;
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

void printArray(int *array, int nrow, int ncol)
{
    for (int ii = 0; ii < nrow; ii++)
    {
        for (int jj = 0; jj < ncol; jj++)
        {
            cout << array[ii * ncol + jj] << " ";
        }
        cout << endl;
    }
}

int convertColArrayToMatrix(int ii, MyArray *matrix)
{
    int colB = ii % matrix->ncol;

    return colB;
}

int convertrowArrayToMatrix(int ii, int col, MyArray *matrix)
{
    int rowA = (ii - col) / matrix->nrow;

    return rowA;
}

void *calculeElementInMatrix(void *tid)
{
    int xx, aux = 0;
    MatrixPartition *matrixPartition = (MatrixPartition *)tid;
    ofstream file;
    string nameFile = "MatrzThread" + to_string(matrixPartition->numThread) + ".txt";
    string filePath = "../results/multiplicacaoThreadPfiles/" + nameFile;
    file.open(filePath);
    // printf("THREAD: %d %d\n", matrixPartition->posStart, matrixPartition->posEnd);

    for (int ii = matrixPartition->posStart; ii <= matrixPartition->posEnd; ii++)
    {
        matrixC->mat[ii] = 0;
        int colB = convertColArrayToMatrix(ii, matrixC);
        int rowA = convertrowArrayToMatrix(ii, colB, matrixC);

        // printf("Thread colB = %d e rowA = %d\n", colB, rowA);
        for (xx = 0; xx < matrixB->nrow; xx++)
        {
            aux += matrixA->mat[rowA * matrixA->nrow + xx] * matrixB->mat[xx * matrixB->nrow + colB];
        }
        matrixC->mat[ii] = aux;
        file << "c" << rowA+1 << ":" << colB+1 << " = " << aux << endl;
        aux = 0;
    }
    file.close();
    pthread_exit(NULL);
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

int main()
{

    // Definição de variaveis
    void *threads_arg;
    // int ii, jj, xx, rowA, colA, rowB, colB;
    int length_threads, elements_per_thread;

    pthread_t *threads;

    // Entrada de dados
    // cout << "Informe a quantidade de elementos a serem calculados na matriz por thread: ";
    scanf("%d", &length_threads);
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
    // matrixC = createMyArray(matrixA->nrow, matrixB->ncol);

    matrixA = readMatrix("../matrices/matrizM1.txt");
    matrixB = readMatrix("../matrices/matrizM2.txt");

    matrixC = createMyArray(matrixA->nrow, matrixB->ncol);

    length_threads = length_threads <= 0 ? 1 : length_threads; // criar no minimo 1 thread.
    elements_per_thread = ceil((float)(matrixC->nrow * matrixC->ncol) / (float)length_threads);

    threads = (pthread_t *)malloc(sizeof(pthread_t) * length_threads);

    if (matrixA->ncol == matrixB->nrow)
    {

        // generateMatrix(matrixA);

        // generateMatrix(matrixB);

        // Imprime as matrizes definidas
        // cout << "================ MATRIZ A ================" << endl;
        // printArray(matrixA->mat, matrixA->nrow, matrixA->ncol);

        // cout << "================ MATRIZ B ================" << endl;
        // printArray(matrixB->mat, matrixB->nrow, matrixB->ncol);

        // CÁLCULO PRODUTO DAS MATRIZES C = A * B
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        int index_thread = 0;
        const int lastPosMatrixC = matrixC->nrow * matrixC->ncol - 1;

        for (int pp = 0; pp < matrixC->nrow * matrixC->ncol; pp = pp + elements_per_thread)
        {
            MatrixPartition *matrixPartition = (MatrixPartition *)malloc(sizeof(MatrixPartition));
            matrixPartition->posStart = pp;
            int posEnd = (pp + elements_per_thread - 1);

            matrixPartition->posEnd = posEnd > lastPosMatrixC ? lastPosMatrixC : posEnd;
            matrixPartition->numThread = index_thread + 1;

            pthread_create(&threads[index_thread], NULL, calculeElementInMatrix, matrixPartition);
            index_thread++;
        }

        for (int ii = 0; ii < length_threads; ii++)
        {
            // Esperar os processos acabarem para depois poder printar a matriz C (caso queira)
            pthread_join(threads[ii], &threads_arg);
            if (elements_per_thread * (ii + 1) >= matrixC->nrow * matrixC->ncol)
            {
                break;
            }
        }
        chrono::steady_clock::time_point end = chrono::steady_clock::now();

        // cout << "================ MATRIZ C - MATRIZ GERADA ================" << endl;
        // printArray(matrixC->mat, matrixC->nrow, matrixC->ncol);

        cout << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " [ms]" << endl;
        writeMatrixFile(matrixC, "../results/multiplicacaoThread.txt", chrono::duration_cast<chrono::milliseconds>(end - begin).count());
    }
    else
    {
        cout << "Nao ha com multiplicar as matrizes dadas ";
    }

    return 0;
}