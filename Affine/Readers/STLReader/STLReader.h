#pragma once

#include <string>

#include "STLModel/STLModel.h"

class STLReader
{
public:
	STLReader() = default;
	STLModelPtr Read(std::string file);
};

