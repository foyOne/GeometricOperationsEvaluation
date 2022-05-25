#include "RawTransformation.h"

#include "Math.h"
#include "AffineTransformation.h"
#include "TransformationView.h"

#include <omp.h>
#include <mkl.h>
#include <type_traits>
#include <emmintrin.h>

namespace RawTransformation
{
    TransformationView view;

	// Набор векторов в виде матрицы Nx3
    template<class T>
	double A_Nx3_StandardReflection(std::vector<T>& vectors, ReflectionPtr reflection)
	{
        size_t row = vectors.size() / 3;
        size_t col = 3;
        size_t coordinatesCount = vectors.size();

        auto m = view.RelfectionAsMatrix<T>(reflection);
        Math::Transpose3x3<T>(m);

        auto time = omp_get_wtime();
        for (size_t i = 0; i < coordinatesCount; i += 3)
        {
            Math::VectorMatrix3x3(&vectors[i], m);
        }
        time = omp_get_wtime() - time;

        return time;
	}
    template double A_Nx3_StandardReflection<float>(std::vector<float>& vectors, ReflectionPtr reflection);
    template double A_Nx3_StandardReflection<double>(std::vector<double>& vectors, ReflectionPtr reflection);

    // vectors - 3xN matrix
    double S_3xN_MKLReflection(std::vector<float>& vectors, ReflectionPtr reflection)
    {
        // Обозначения m, n из документации
        size_t m = 3;
        size_t n = vectors.size() / 3;

        auto time = omp_get_wtime();
        auto L_slarfx = LAPACKE_slarfx(
            LAPACK_ROW_MAJOR,
            'L',
            m,
            n,
            reflection->normal.data(),
            2,
            vectors.data(),
            n,
            nullptr);
        time = omp_get_wtime() - time;

        return time;
    }

    // vectors - 3xN matrix
    double S_3xN_HessenbergReflection(std::vector<float>& vectors, ReflectionPtr reflection)
    {
        auto Nrow = 3;
        auto Ncol = vectors.size() / 3;

        auto& mat = vectors;
        auto& vect = reflection->normal;
        std::vector<float> scalar_ar(Ncol, 0.0f);
        float gamma;

        gamma = 2;

        auto time = omp_get_wtime();

        for (int column = 0; column < Ncol; column++)
        {
            for (int i = 0; i < Nrow; i++)
                scalar_ar[column] += mat[i * Ncol + column] * vect[i];
            scalar_ar[column] *= gamma;
        }

        for (int row = 0; row < Nrow; row++)
            for (int col = 0; col < Ncol; col++)
                mat[row * Ncol + col] -= scalar_ar[col] * vect[row];

        time = omp_get_wtime() - time;

        return time;
    }

    double D_Nx3_VectorizedStandardReflection(std::vector<double>& vectors, ReflectionPtr reflection)
    {
        auto data = vectors.data();

        size_t row = vectors.size() / 3;

        auto reflectionMatrix = view.RelfectionAsMatrix<double>(reflection);
        Math::Transpose3x3<double>(reflectionMatrix);

        double m[9];
        std::memcpy(m, reflectionMatrix.data(), 9 * sizeof(double));

        __m128d x, y, z;
        __m128d xm, ym, zm;
        __m128d value;

        double stepData[6];
        size_t s = 0;

        double time = omp_get_wtime();
        for (size_t i = 0; i < row; i += 2)
        {
            s = i * 3;
            std::memcpy(stepData, &data[s], 6 * sizeof(double));

            // x1, y1
            x = _mm_set1_pd(stepData[0]);
            y = _mm_set1_pd(stepData[1]);
            z = _mm_set1_pd(stepData[2]);
            xm = _mm_set_pd(m[1], m[0]);
            ym = _mm_set_pd(m[4], m[3]);
            zm = _mm_set_pd(m[7], m[6]);

            x = _mm_mul_pd(x, xm);
            y = _mm_mul_pd(y, ym);
            z = _mm_mul_pd(z, zm);
            value = _mm_add_pd(x, y);
            value = _mm_add_pd(value, z);

            _mm_store_pd(&data[s + 0], value);

            // z1, x2
            x = _mm_set_pd(stepData[3], stepData[0]);
            y = _mm_set_pd(stepData[4], stepData[1]);
            z = _mm_set_pd(stepData[5], stepData[2]);
            xm = _mm_set_pd(m[0], m[2]);
            ym = _mm_set_pd(m[3], m[5]);
            zm = _mm_set_pd(m[6], m[8]);

            x = _mm_mul_pd(x, xm);
            y = _mm_mul_pd(y, ym);
            z = _mm_mul_pd(z, zm);
            value = _mm_add_pd(x, y);
            value = _mm_add_pd(value, z);

            _mm_store_pd(&data[s + 2], value);

            // y2, z2
            x = _mm_set1_pd(stepData[3]);
            y = _mm_set1_pd(stepData[4]);
            z = _mm_set1_pd(stepData[5]);
            xm = _mm_set_pd(m[2], m[1]);
            ym = _mm_set_pd(m[5], m[4]);
            zm = _mm_set_pd(m[8], m[7]);

            x = _mm_mul_pd(x, xm);
            y = _mm_mul_pd(y, ym);
            z = _mm_mul_pd(z, zm);
            value = _mm_add_pd(x, y);
            value = _mm_add_pd(value, z);

            _mm_store_pd(&data[s + 4], value);
        }
        time = omp_get_wtime() - time;

        /*
            [x1, y1, z1]
            [x2, y2, z2]

            [a, d, e]
            [d, b, f]
            [e, f, c]

            [x1 * a + y1 * d + z1 * e] [x1 * d + y1 * b + z1 * f] [x1 * e + y1 * f + z1 * c]
            [x2 * a + y2 * d + z2 * e] [x2 * d + y2 * b + z2 * f] [x2 * e + y2 * f + z2 * c]

            -1-
            (x1, x1) * (a, d) -> (x1*a, x1*d)
            (y1, y1) * (d, b) -> (y1*d, y1*b)
            (z1, z1) * (e, f) -> (z1*e, z1*f)

            -2-
            (x1, x2) * (e, a) -> (x1*e, x2*a)
            (y1, y2) * (f, d) -> (y1*f, y2*d)
            (z1, z2) * (c, e) -> (z1*c, z2*e)

            -2-
            (x2, x2) * (d, e) -> (x2*d, x2*e)
            (y2, y2) * (b, f) -> (y2*b, y2*f)
            (z2, z2) * (f, c) -> (z2*f, z2*c)
        */
        return time;
    }

    double D_Nx3_VectorizedReflection_V1(std::vector<double>& vectors, ReflectionPtr reflection)
    {
        __m128d M1, M2, M3;// элементы матрицы
        __m128d V1, V2, V3;// элементы вектора отражения
        __m128d M2scOne, V2scOne;
        __m128d M1sc, M2sc, M3sc;
        __m128d M2sc1, M2sc2;
        __m128d M1scR;
        __m128d V1sc, V2sc;
        __m128d M1R, M2R;
        __m128d GammaV;

        auto data = vectors.data();
        size_t row = vectors.size() / 3;

        double vect[6] = 
        { 
            static_cast<double>(reflection->normal[0]),
            static_cast<double>(reflection->normal[1]),
            static_cast<double>(reflection->normal[2]),
            static_cast<double>(reflection->normal[0]),
            static_cast<double>(reflection->normal[1]),
            static_cast<double>(reflection->normal[2])
        };

        double gamma = 2.0;

        __m128d Zero = _mm_setzero_pd();
        GammaV = _mm_load_pd(&gamma);
        GammaV = _mm_shuffle_pd(GammaV, GammaV, 0);

        double time = omp_get_wtime();
        for (int i = 0; i < row; i += 2) {
            // Первые три элемента
            M1 = _mm_load_pd(&data[i * 3 + 0]);
            M2 = _mm_load_pd(&data[i * 3 + 2]);
            M2scOne = _mm_shuffle_pd(M2, Zero, 0);
            V1 = _mm_load_pd(&vect[0]);
            V2 = _mm_load_pd(&vect[2]);
            V2scOne = _mm_shuffle_pd(V2, Zero, 0);
            // Скалярное произведение
            M1sc = _mm_mul_pd(M1, V1);
            M2sc = _mm_mul_pd(M2scOne, V2scOne);
            M1sc = _mm_add_pd(M1sc, M2sc);
            M1sc = _mm_add_pd(M1sc, Zero);
            M1scR = _mm_shuffle_pd(M1sc, M1sc, 0);
            // Умножаем на gamma
            M1scR = _mm_mul_pd(M1scR, GammaV);
            // Умножаем на скалярное произведение
            V1sc = _mm_mul_pd(V1, M1scR);
            V2sc = _mm_mul_pd(V2scOne, M1scR);
            // Вычитаем
            M1R = _mm_sub_pd(M1, V1sc);
            M2R = _mm_sub_pd(M2scOne, V2sc);
            // Сохраняем
            _mm_store_pd(&data[i * 3 + 0], M1R);
            _mm_store_sd(&data[i * 3 + 2], M2R);
            // Вторые три элемента
            M3 = _mm_load_pd(&data[i * 3 + 4]);
            M2scOne = _mm_shuffle_pd(M2, Zero, 1);
            V3 = _mm_load_pd(&vect[4]);
            V2scOne = _mm_shuffle_pd(V2, Zero, 1);
            // Скалярное произведение
            M1sc = _mm_mul_pd(M3, V3);
            M2sc = _mm_mul_pd(M2scOne, V2scOne);
            M1sc = _mm_add_pd(M1sc, M2sc);
            M1sc = _mm_add_pd(M1sc, Zero);
            M1scR = _mm_shuffle_pd(M1sc, M1sc, 0);
            // Умножаем на gamma
            M1scR = _mm_mul_pd(M1scR, GammaV);
            // Умножаем на скалярное произведение
            V1sc = _mm_mul_pd(V3, M1scR);
            V2sc = _mm_mul_pd(V2scOne, M1scR);
            // Вычитаем
            M1R = _mm_sub_pd(M3, V1sc);
            M2R = _mm_sub_pd(M2scOne, V2sc);
            // Сохраняем
            _mm_store_pd(&data[i * 3 + 4], M1R);
            _mm_store_sd(&data[i * 3 + 3], M2R);
        }

        time = omp_get_wtime() - time;
        return time;
    }
}