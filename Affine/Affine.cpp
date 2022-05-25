#include "Readers/STLReader/STLReader.h"
#include "Writers/STLWriter/STLWriter.h"

#include "TransformationExecutors/TestSTLTransformationExecutor/TestSTLTransformationExecutor.h"
#include "TransformationExecutors/StandardSTLTransformationExecutor/StandardSTLTransformationExecutor.h"
#include "TransformationExecutors/MKLSTLTransformationExecutor/MKLSTLTransformationExecutor.h"
#include "TransformationExecutors/TransformationExecutor.h"

#include "SimpleTest.h"
#include "RawTransformation.h"
#include "TransformationView.h"

#include "Result.h"

#include "STLModelTest.h"

#include <time.h>
#include <fstream>
#include <sstream>
#include <functional>
#include <vector>
#include <map>
#include <omp.h>

std::string GetDate()
{
    std::time_t now = std::time(nullptr);
    std::tm timeInfo;
    char buffer[100];

    localtime_s(&timeInfo, &now);
    strftime(buffer, sizeof(buffer), "%H_%M_%S_%d_%m_%Y", &timeInfo);
    return std::string(buffer);
}

std::string GenerateOutputFile(std::string file)
{
    std::stringstream ss;
    ss
        << "Output"
        << "-"
        << GetDate()
        << "-"
        << file;
    return ss.str();
}

void StandardSTLTest()
{
    std::string pathToTest = "TestData/InputModels/";

    STLModelTest test(pathToTest);

    TransformationBuilder builder;
    builder.AddReflection(Reflection({ 0, 0, 1 }));

    TransformationExecutorPtr stlExecutor(new StandardSTLTransformationExecutor(builder));

    test.Start(stlExecutor, true);
}


template<class T>
void WriteArray(std::string file, const std::vector<T>& arr, std::ios_base::openmode ios = std::ios::out)
{
    std::ofstream out(file, ios);

    if (out.is_open())
    {
        for (auto& value : arr)
            out << value << std::endl;
    }
    out.close();
}

template<class T>
std::vector<double> RawTest(std::function<double(std::vector<T>&, ReflectionPtr)> reflectionMethod, std::vector<size_t> sizeArray)
{
    //auto reflection = std::make_shared<Reflection>(2, 3, -5);
    auto reflection = std::make_shared<Reflection>(0, 0, 1);
    auto reflect = std::bind(reflectionMethod, std::placeholders::_1, reflection);

    SimpleTest test;

    return test.VectorSetTransformationTest<T>(reflect, sizeArray);
}

void AddTime(std::vector<double>& result, std::vector<double>& values)
{
    std::transform(
        result.begin(),
        result.end(),
        values.begin(),
        result.begin(),
        std::plus<double>());
}

void GetMeanTime(std::vector<double>& result, size_t n)
{
    std::transform(
        result.begin(),
        result.end(),
        result.begin(),
        [n](const double& value) {return value / n; });
}

void RawReflectionTest()
{
#define SIZE(s) static_cast<size_t>(s)

    std::vector<size_t> sizeArray =
    {
        SIZE(12)
        //SIZE(3e3),
        //SIZE(6e3),
        //SIZE(9e3),

        //SIZE(3e4),
        //SIZE(6e4),
        //SIZE(9e4),

        //SIZE(3e5),
        //SIZE(6e5),
        //SIZE(9e5),

        //SIZE(3e6),
        //SIZE(6e6),
        //SIZE(9e6),

        //SIZE(3e7),
        //SIZE(6e7),
        //SIZE(9e7),

        //SIZE(3e8),
        //SIZE(6e8),
        //SIZE(9e8),
    };

    std::vector<double> standard_float(sizeArray.size());
    std::vector<double> standard_double(sizeArray.size());
    std::vector<double> mkl_float(sizeArray.size());
    std::vector<double> hessenberg_float(sizeArray.size());
    std::vector<double> vectorized_standard_double(sizeArray.size());
    std::vector<double> vectorized_hessenberg_double(sizeArray.size());

    size_t n = 1;
    for (size_t i = 0; i < n; ++i)
    {
        std::cout << "iteration = " << i << std::endl;

        //auto current_standard_float = RawTest<float>(RawTransformation::A_Nx3_StandardReflection<float>, sizeArray);
        //AddTime(standard_float, current_standard_float);

        auto current_standard_double = RawTest<double>(RawTransformation::A_Nx3_StandardReflection<double>, sizeArray);
        AddTime(standard_double, current_standard_double);

        //auto current_mkl_float = RawTest<float>(RawTransformation::S_3xN_MKLReflection, sizeArray);
        //AddTime(mkl_float, current_mkl_float);

        //auto current_hessenberg_float = RawTest<float>(RawTransformation::S_3xN_HessenbergReflection, sizeArray);
        //AddTime(hessenberg_float, current_hessenberg_float);

        auto current_vectorized_standard_double = RawTest<double>(RawTransformation::D_Nx3_VectorizedStandardReflection, sizeArray);
        AddTime(vectorized_standard_double, current_vectorized_standard_double);

        auto current_vectorized_hessenberg_double = RawTest<double>(RawTransformation::D_Nx3_VectorizedReflection_V1, sizeArray);
        AddTime(vectorized_hessenberg_double, current_vectorized_hessenberg_double);
    }

    GetMeanTime(standard_float, n);
    GetMeanTime(standard_double, n);
    GetMeanTime(mkl_float, n);
    GetMeanTime(hessenberg_float, n);
    GetMeanTime(vectorized_standard_double, n);
    GetMeanTime(vectorized_hessenberg_double, n);

    //WriteArray("TestResult/size.txt", sizeArray);

    //WriteArray("TestResult/standard_float.txt", standard_float);
    //WriteArray("TestResult/standard_double.txt", standard_double);
    //WriteArray("TestResult/mkl_float.txt", mkl_float);
    //WriteArray("TestResult/hessenberg_float.txt", hessenberg_float);
    //WriteArray("TestResult/vectorized_standard_double.txt", vectorized_standard_double);
    //WriteArray("TestResult/vectorized_hessenberg_double.txt", vectorized_hessenberg_double);
}

int Nrow = 3;
int Ncol = 1000;
int num_exps = 1000000;

void haus_gor(double* matr, double* vect, double* scalar_ar, double gamma)
{
    // !!!
    // Столбцы
    for (int column = 0; column < Ncol; column++)
    {
        scalar_ar[column] = matr[0 * Ncol + column] * vect[0] + matr[1 * Ncol + column] * vect[1] + matr[2 * Ncol + column] * vect[2];
        scalar_ar[column] *= gamma;
    }
    for (int column = 0; column < Ncol; column++)
    {
        matr[0 * Ncol + column] -= scalar_ar[column] * vect[0];
        matr[1 * Ncol + column] -= scalar_ar[column] * vect[1];
        matr[2 * Ncol + column] -= scalar_ar[column] * vect[2];
    }
}
// Невысокая широкая матрица
void marix_vector_mul_gor(double* matr, double* matrix)
{
    double x, y, z;
    for (int j = 0; j < Ncol; j++)
    {
        x = matr[0 * Ncol + j] * matrix[0] + matr[1 * Ncol + j] * matrix[3] + matr[2 * Ncol + j] * matrix[6];
        y = matr[0 * Ncol + j] * matrix[1] + matr[1 * Ncol + j] * matrix[4] + matr[2 * Ncol + j] * matrix[7];
        z = matr[0 * Ncol + j] * matrix[2] + matr[1 * Ncol + j] * matrix[5] + matr[2 * Ncol + j] * matrix[8];

        matr[0 * Ncol + j] = x;
        matr[1 * Ncol + j] = y;
        matr[2 * Ncol + j] = z;
    }
}

void haus_vert(double* matr, double* vect, double gamma)
{
    // !!!
    // Столбцы
    double scalar_ar;
    for (int row = 0; row < Ncol; row++)
    {
        scalar_ar = matr[row * 3 + 0] * vect[0] + matr[row * 3 + 1] * vect[1] + matr[row * 3 + 2] * vect[2];
        scalar_ar *= gamma;
        matr[row * 3 + 0] -= scalar_ar * vect[0];
        matr[row * 3 + 1] -= scalar_ar * vect[1];
        matr[row * 3 + 2] -= scalar_ar * vect[2];
    }
}

// Высокая узкая матрица
void marix_vector_mul_vert(double* matr, double* matrix)
{
    double x, y, z;
    for (int j = 0; j < Ncol; j++)
    {
        x = matr[j * 3 + 0] * matrix[0] + matr[j * 3 + 1] * matrix[3] + matr[j * 3 + 2] * matrix[6];
        y = matr[j * 3 + 0] * matrix[1] + matr[j * 3 + 1] * matrix[4] + matr[j * 3 + 2] * matrix[7];
        z = matr[j * 3 + 0] * matrix[2] + matr[j * 3 + 1] * matrix[5] + matr[j * 3 + 2] * matrix[8];

        matr[j * 3 + 0] = x;
        matr[j * 3 + 1] = y;
        matr[j * 3 + 2] = z;
    }
}

void TestTimer()
{
    size_t N = 5000;
    std::vector<double> time_chrono(N);
    std::vector<double> time_omp(N);

    auto reflection = std::make_shared<Reflection>(2, 3, -5);
    auto reflect = std::bind(RawTransformation::A_Nx3_StandardReflection<double>, std::placeholders::_1, reflection);

    SimpleTest test;

#define SIZE(s) static_cast<size_t>(s)

    std::vector<size_t> sizeArray(20, SIZE(15e4));

    for (size_t i = 0; i < N; ++i)
    {
        if (i % 500 == 0)
            std::cout << i << " iterations done" << std::endl;

        auto start = std::chrono::high_resolution_clock::now();

        test.VectorSetTransformationTest<double>(reflect, sizeArray);

        auto end = std::chrono::high_resolution_clock::now();

        auto diff = std::chrono::duration<double>(end - start).count();
        time_chrono[i] = diff;
    }

    for (size_t i = 0; i < N; ++i)
    {
        if (i % 500 == 0)
            std::cout << i << " iterations done" << std::endl;

        auto start = omp_get_wtime();

        test.VectorSetTransformationTest<double>(reflect, sizeArray);

        auto end = omp_get_wtime();

        auto diff = end - start;
        time_omp[i] = diff;
    }

    WriteArray("chrono.txt", time_chrono, std::ios::out | std::ios::app);
    WriteArray("omp.txt", time_omp, std::ios::out | std::ios::app);
}

int main()
{
    RawReflectionTest();

    return 0;
}