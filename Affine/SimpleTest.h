#pragma once

#include <functional>
#include <vector>

class SimpleTest
{
public:
	SimpleTest() = default;
	~SimpleTest() = default;

	template<class T>
	std::vector<double> VectorSetTransformationTest(const std::function<double(std::vector<T>&)>& functionToTest, const std::vector<size_t>& sizeArray);
};