#pragma once

#include "Models/STLModel/STLModel.h"

class Result
{
public:

    Result() : model(nullptr), time(0.0) {}

    Result(STLModelPtr model, double time) : model(model), time(time) {}

    STLModelPtr model;
    double time; // sec
};