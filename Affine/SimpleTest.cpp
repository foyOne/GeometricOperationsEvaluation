#include "SimpleTest.h"

#include <random>
#include <type_traits>

template<class T>
void CreateRandomArray(std::vector<T>& arr, size_t size)
{
	static_assert(std::is_floating_point<T>::value, "T must be a floating point");

	std::random_device rd;
	std::mt19937 gen;
	//gen.seed(rd());
	std::uniform_real_distribution<T> urd(-1000, 1000);
	auto roll = std::bind(urd, gen);

	arr.resize(size);
	std::generate(arr.begin(), arr.end(), roll);
}

template<class T>
std::vector<T> CreateRandomArray(size_t size)
{
	static_assert(std::is_floating_point<T>::value, "T must be a floating point");

	std::vector<T> arr;
	CreateRandomArray<T>(arr, size);

	return arr;
}

template<class T>
std::vector<double> SimpleTest::VectorSetTransformationTest(const std::function<double(std::vector<T>&)>& functionToTest, const std::vector<size_t>& sizeArray)
{
	static_assert(std::is_floating_point<T>::value, "T must be a floating point");

	std::vector<double> time;
	std::vector<T> vectorSet;

	for (const auto& size : sizeArray)
	{
		CreateRandomArray<T>(vectorSet, size);
		time.push_back(functionToTest(vectorSet));
	}

	return time;
}

#pragma region TemplateDefinition

// Для определения шаблонов вне .h файла

template std::vector<double> SimpleTest::VectorSetTransformationTest<float>(
	const std::function<double(std::vector<float>&)>& functionToTest,
	const std::vector<size_t>& sizeArray);

template std::vector<double> SimpleTest::VectorSetTransformationTest<double>(
	const std::function<double(std::vector<double>&)>& functionToTest,
	const std::vector<size_t>& sizeArray);

#pragma endregion
