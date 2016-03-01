//#include <iostream>
#include "Libs.h"

//using namespace std;

template<typename T>
T** memAlloc(size_t n, size_t m, size_t alignment=16)
//T** memAlloc(size_t n, size_t m, size_t alligment, size_t offset)
{
    /* alignment could not be less then zero */
    if (alignment < 0) {
        return NULL;
    }

    T** ptrMatrix = NULL;

    //ptrMatrix = (T**)malloc(sizeof(T*)*n);
    ptrMatrix = (T**)aligned_alloc(alignment, n * sizeof(T*));
    if (ptrMatrix == NULL)
    {
        printf( "Error allocation aligned memory.");

        return NULL;
    }

    for(size_t i=0; i<n; i++)
    {
        //ptrMatrix[i] = (T*)malloc(sizeof(T)*m);
        ptrMatrix[i] = (T*)aligned_alloc(alignment, m * sizeof(T*));
        if (ptrMatrix[i] == NULL)
        {
            printf( "Error allocation aligned memory.");
            return NULL;
        }
    }

    return ptrMatrix;
}

template<typename T>
void memFree(T** ptr, size_t n)
{
    //if(ptr == nullptr) return ;
    if(ptr == NULL) return ;
    for(size_t i=0; i<n; i++)
    {
        free(ptr[i]);
    }
    free(ptr);
}

template<typename T>
void matrixConsoleOutput(T** ptrMatrix, size_t n, size_t m)
{
    for(size_t i=0; i<n; i++)
    {
        for(size_t j=0; j<m; j++)
        {
            cout << ptrMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

template<typename T>
void matrixFileOutput(string filepath, T** ptrMatrix, size_t n, size_t m)
{
    fstream file;
    file.open(filepath.c_str(), ios:: out | ios:: trunc);
    if(!file.is_open())
    {
        cout << endl << "Error! Can't open the file: " << endl << filepath << endl;
        return ;
    }
    for(size_t i=0; i<n; i++)
    {
        for(size_t j=0; j<m; j++)
        {
            //file << setw(5) << ptrMatrix[i][j] << " ";
            file << setw(5) << ptrMatrix[i][j];
        }
        file << endl;
    }
    file.close();
}

unsigned long long int time_RDTSC()
{
    union ticks
    {
        unsigned long long int tx;
        struct dblword { long int tl, th; } dw;
    } t;
    asm("rdtsc\n" : "=a"(t.dw.tl), "=d"(t.dw.th));
    return t.tx;
}

long long int time_start()
{
    long long int curTimeValue = 0;
    curTimeValue = time_RDTSC();
    return curTimeValue;
}

long long int time_stop(long long int startTime)
{
    return (time_RDTSC() - startTime)*1000/CLOCKS_PER_SEC;
}

template<typename T>
T** matrixMultAutoVect(T** ma, size_t n1, size_t m1, T** mb, size_t n2, size_t m2)
{
    T** resultMatrix = NULL;
    if((ma != NULL) && (mb != NULL))
    {
        if( (n1 == m2) && (m1 == n2) )
        {
            resultMatrix = memAlloc<T>(n1, m2);
            long long int startTime = time_start();
            for(size_t i=0; i<n1; i++)
            {
                for(size_t j=0; j<m2; j++)
                {
                    for(size_t k=0; k<m1; k++)
                    {
                        resultMatrix[i][j] += ma[i][k] * mb[k][j];
                    }
                }
            }
            long long int processorTicks = time_stop(startTime);
            cout << "=============================================================" << endl;
            cout << "Clear Processor Ticks = " << processorTicks << endl;
            cout << "=============================================================" << endl;
        }
        else
        {
            cout << endl << "Error! Check up the sizes of matrixes" << endl;
            return NULL;
        }
    }
    else
    {
        cout << endl << "Error! Memory can't allocate" << endl;
        return NULL;
    }

    return resultMatrix;
}

template<typename T>
T** matrixMult(T** ma, size_t n1, size_t m1, T** mb, size_t n2, size_t m2)
{
    T** resultMatrix = NULL;
    if((ma != NULL) && (mb != NULL))
    {
        if( (n1 == m2) && (m1 == n2) )
        {
            resultMatrix = memAlloc<T>(n1, m2);
            long long int startTime = time_start();

            //#pragma simd
            for(size_t i=0; i<n1; i++)
            {
                for(size_t j=0; j<m2; j++)
                {
                    #pragma novector
                    for(size_t k=0; k<m1; k++)
                    {
                        resultMatrix[i][j] += ma[i][k] * mb[k][j];
                    }
                }
            }
            long long int processorTicks = time_stop(startTime);
            cout << "=============================================================" << endl;
            cout << "Clear Processor Ticks (without autovectorization) = " << processorTicks << endl;
            cout << "=============================================================" << endl;
        }
        else
        {
            cout << endl << "Error! Check up the sizes of matrixes" << endl;
            return NULL;
        }
    }
    else
    {
        cout << endl << "Error! Memory can't allocate" << endl;
        return NULL;
    }

    return resultMatrix;
}

template<typename T>
T** matrixInit(int n, int m)
{
	T** matrix = NULL;
	matrix = memAlloc<T>(n, m);

	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			matrix[i][j] = (T)(rand() % 10);
        }
    }
	return matrix;
}

template<typename T>
T** matrixInitWithFloatingPoint(int n, int m)
{
    srand(time(0));

	T** matrix = NULL;
	matrix = memAlloc<T>(n, m);

	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			matrix[i][j] = (T)(rand() % 1000.0);
        }
    }
	return matrix;
}

template<typename T>
T** matrixTranspose(T** matrix, int n, int m)
{
    if(n != m)
    {
        cout << endl << "Error! Check up the sizes of matrices" << endl;
        return NULL;
    }

	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < i; j++)
		{
            /**
            a = a + b;
            b = a - b;
            a = a - b;
            */
			matrix[i][j] = matrix[i][j] + matrix[j][i];
			matrix[j][i] = matrix[i][j] - matrix[j][i];
			matrix[i][j] = matrix[i][j] - matrix[j][i];
        }
    }
	return matrix;
}

//template<typename T>
float** handmadeVectorizationByASM(float** ma, size_t n1, size_t m1, float** mb, size_t n2, size_t m2)
{
    float** resultMatrix = NULL;

    if((ma != NULL) && (mb != NULL))
    {
        if( (n1 == m2) && (m1 == n2) )
        {
            // transpose the second matrix
            mb = matrixTranspose(mb, n2, m2);

            resultMatrix = memAlloc<float>(n1, m2);

            float result = 0.0;
            // pointer on the matrix string
            __declspec(align(16)) float *maVector = NULL, *mbVector = NULL;

            long long int startTime = time_start();
            long long int processorTicks = 0;
            //#pragma simd
            for(size_t i=0; i<n1; i++)
            {
                for(size_t j=0; j<m2; j++)
                {
                    resultMatrix[i][j] = 0;
                    float partlyResult = 0.0;
                    //set pointers on the i and j strings of the matrices
                    maVector = ma[i];
                    mbVector = mb[j];

                    int cycles = m1;

                    // use FASM style asm

                    /**
                    HADDPS — осуществляет горизонтальное сложение элементов с одинарной точностью. Первый элемент, записываемый
                    в операнд назначения, является суммой первого и второго элементов первого (исходного) операнда; второй элемент
                     — суммой третьего и четвертого элементов первого операнда; третий элемент — суммой первого и второго элементов
                    второго операнда (операнда назначения) и, наконец, четвертый элемент — суммой третьего и четвертого элементов
                    второго операнда. Для наглядности изобразим это, как и прежде, в виде схемы:

                    Операнд A (128 бит, 4 элемента): a3 | a2 | a1 | a0
                    Операнд B (128 бит, 4 элемента): b3 | b2 | b1 | b0

                    HADDPS A, B
                    Результат (операнд A): b2+b3 | b0+b1 | a2+a3 | a0+a1
                    */

                    __asm {
                        ;// write the count of loop cycles in the ECX Register
                        mov ecx, cycles
                        ;// load maVector to EAX Register
                        mov eax, maVector
                        ;// load mbVector to EBX Register
                        mov ebx, mbVector
                        ;// we have to set to zero XMM2 Register - do XOR for XMM2 Register (a XOR a = 0)
                        xorps xmm2, xmm2 ;// xorps - Bitwise Logical XOR for Single-Precision Floating-Point Values

                        ;// write current values of cycles counters
                        ;//mov esi, i
                        ;//mov edi, j
                        xor esi, esi
                        xor edi, edi
                        mov esi, maVector
                        mov edi, mbVector

                        ;// implementations of this cycle
                        ;//for(size_t k=0; k<m1; k++)
                        ;//{
                        ;//     resultMatrix[i][j] += ma[i][k] * mb[k][j];
                        ;//}
                        internalLoop:
                                        // decrease ECX
                                        dec ecx

                                        ;// write current value of internal counter
                                        //mov esi, ecx

                        ;// use [baseReg+offset] --- "Indirect base register with displacement addressing"

                                        ;// write ma[i][k] to XMM0 Reg
                                        ;//movaps xmm0, [eax+16*ecx];// movaps - Move Aligned Packed Single-Precision Floating-Point Values
                                        movaps xmm0, [esi]
                                        ;// write mb[k][j] to XMM1 Reg
                                        movaps xmm1, [edi]
                                        ;// multiply register's values
                                        mulps xmm0, xmm1 ;// mulps - Multiply Packed Single-Precision Floating-Point Values
                                        ;// mult result write to XMM2
                                        addps xmm2, xmm0 ;// addps - Add Packed Single-Precision Floating-Point Values

                                        //go to the next value
                                        add esi, 16
                                        add edi, 16

                                        // compare with zero
                                        cmp ecx, 0
                                        jne internalLoop ;// jne - jump if not equal

                        ;// haddps - Packed Single-FP Horizontal Add
                        ;//    Adds the single-precision floating-point values in the first and second dwords of the destination
                        ;// operand and stores the result in the first dword of the destination operand.
                        ;//    Adds single-precision floating-point values in the third and fourth dword of the destination operand and
                        ;//stores the result in the second dword of the destination operand.
                        ;//    Adds single-precision floating-point values in the first and second dword of the source operand and stores
                        ;//the result in the third dword of the destination operand.
                        ;//    Adds single-precision floating-point values in the third and fourth dword of the source operand and
                        ;//stores the result in the fourth dword of the destination operand.

                        haddps xmm2, xmm2
                        haddps xmm2, xmm2
                        ;//haddps xmm2, xmm2
                        ;//haddps xmm2, xmm2

                        movss partlyResult, xmm2 ;// movss - Move Scalar Single-Precision Floating-Point Values
                    }
                    resultMatrix[i][j] = partlyResult;
                    //processorTicks = time_stop(startTime);
                }
            }
            //long long int processorTicks = time_stop(startTime);
            processorTicks = time_stop(startTime);
            cout << "=============================================================" << endl;
            cout << "Clear Processor Ticks (without autovectorization) = " << processorTicks << endl;
            cout << "=============================================================" << endl;
        }
        else
        {
            cout << endl << "Error! Check up the sizes of matrixes" << endl;
            return NULL;
        }
    }
    else
    {
        cout << endl << "Error! Memory can't allocate" << endl;
        return NULL;
    }


    return resultMatrix;
}

int main()
{
    cout << endl << "Vectorization is running!" << endl << endl;

    float** ptrMatrixA = memAlloc<float>(N, M);

    float** ptrMatrixB = memAlloc<float>(N, M);

    //float** ptrMatrixC = memAlloc<float>(800, 800);
    float** ptrMatrixC = NULL;
    float** ptrMatrixD = NULL;
    float** ptrMatrixF = NULL;
    //matrixOutput(ptrMatrix, 800, 800);

    //struct tms tmsBegin, tmsEnd;

    // initialization of matrices
    ptrMatrixA = matrixInit<float>(N, M);
    ptrMatrixB = matrixTranspose<float>(ptrMatrixA, N, M);

    //debug matrix values by the files
    matrixFileOutput("matrix_A.txt", ptrMatrixA, N, M);
    matrixFileOutput("matrix_B.txt", ptrMatrixB, N, M);

    //long long int startTime = 0;

    //startTime = time_start();
    cout << "================== Auto-Vectorization ==================" << endl;
    ptrMatrixC = matrixMultAutoVect<float>(ptrMatrixA, N, M, ptrMatrixB, N, M);
    cout << endl << "================== Without-Vectorization ==================" << endl;
    ptrMatrixD = matrixMult<float>(ptrMatrixA, N, M, ptrMatrixB, N, M);

    matrixFileOutput("matrix_C.txt", ptrMatrixC, N, M);

    //long long int processorTicks = time_stop(startTime);

    //cout << "Processor Ticks = " << processorTicks;

    cout << endl << "================== HandMade-Vectorization (FASM) ==================" << endl;
    //long long int startTime = 0;
    //startTime = time_start();

    //ptrMatrixF = handmadeVectorizationByASM<float>(ptrMatrixA, N, M, ptrMatrixB, N, M);
    ptrMatrixF = handmadeVectorizationByASM(ptrMatrixA, N, M, ptrMatrixB, N, M);

    matrixFileOutput("matrix_F.txt", ptrMatrixF, N, M);

    //long long int processorTicks = time_stop(startTime);
    //cout << endl << endl << "Processor Ticks (HandMade Vectorization By ASM) = " << endl << processorTicks;

/*
    memFree<float>(ptrMatrixA, 80);
    memFree<float>(ptrMatrixB, 80);
    memFree<float>(ptrMatrixC, 80);
    memFree<float>(ptrMatrixD, 80);
*/
    cout << endl << "==================================================================" << endl << endl;
    return 0;
}
