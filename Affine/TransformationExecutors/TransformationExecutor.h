#pragma once

#include "STLModel/STLModel.h"

class Result;

class TransformationExecutor;

using TransformationExecutorPtr = std::shared_ptr<TransformationExecutor>;

class TransformationExecutor
{
public:
	virtual ~TransformationExecutor() = default;

	virtual Result Transform(const STLModelPtr model) = 0;
};