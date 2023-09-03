#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

clock_t begin, end;
double time_spent;

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

void printArray(int *array, int nrow, int ncol)
{
    for (int ii = 0; ii < nrow; ii++)
    {
        for (int jj = 0; jj < ncol; jj++)
        {
            printf("%d", array[ii * nrow + jj]);
            printf(" ");
        }
        printf("\n");
    }
}

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
    int PARTITION_MATRIX;

    pthread_t *threads;

    // Entrada de dados
    printf("Informe a quantidade de elementos a serem calculados na matriz por thread: ");
    scanf("%d", &PARTITION_MATRIX);
    printf("Informe a quntidade de linhas da matriz A : ");
    scanf("%d", &rowA);
    printf("Informe a quantidade de colunas da matriz A : ");
    scanf("%d", &colA);

    printf("Informe a quntidade de linhas da matriz B : ");
    scanf("%d", &rowB);
    printf("Informe a quantidade de colunas da matriz B : ");
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
        printf("================ MATRIZ A ================\n");
        printArray(matrixA->mat, matrixA->nrow, matrixA->ncol);

        printf("================ MATRIZ B ================\n");
        printArray(matrixB->mat, matrixB->nrow, matrixB->ncol);

        // CÁLCULO PRODUTO DAS MATRIZES C = A * B
        begin = clock(); // Registra o tempo de início
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

        end = clock(); // Registra o tempo de término

        for (int ii = 0; ii < lenThreads; ii++)
        {
            // Esperar os processos acabarem para depois poder printar a matriz C (caso queira)
            pthread_join(threads[ii], &threads_arg);
        }

        printf("================ MATRIZ C - MATRIZ GERADA ================\n");

        printArray(matrixC->mat, matrixC->nrow, matrixC->ncol);
        time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Tempo de execução: %f m/s\n", time_spent*1000);
    }
    else
    {
        printf("Nao ha com multiplicar as matrizes dadas ");
    }

    return 0;
}