#pragma once

#include "STLModel/STLModel.h"

#include <string>
#include <vector>


class STLWriter
{
public:
	STLWriter(bool binary = false) : binary(binary) {}
	void Write(std::string file, const STLModelPtr model);

public:
	bool binary;
};

