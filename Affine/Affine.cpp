#include "AffineMap.h"
#include "DataModels/STLModel/STLModel.h"
#include "TransformationExecutors/StandardSTLTransformationExecutor/StandardSTLTransformationExecutor.h"

#include <iostream>
#include <memory>
#include <filesystem>

#define _USE_MATH_DEFINES
#include <math.h>

void main()
{
    std::string file = "cube_ascii.stl";
    std::string transformed = "Transformed-" + file;

    std::filesystem::path testDataPath("TestData");

    std::filesystem::path input = testDataPath / "InputModels" / file;
    std::filesystem::path output = testDataPath / "OutputModels" / transformed;

    STLModel model(input.u8string());

    std::shared_ptr<StandardSTLTransformationExecutor> ste(new StandardSTLTransformationExecutor());
    model.SetTtransformationExecutor(ste);

    auto map = AffineMap();
    map.RotateX(M_PI / 6);
    map.RotateY(M_PI / 3);
    map.RotateZ(M_PI / 2);

    //map.Rotate(1, 1, 1, M_PI_2);
    //map.Reflection(1, 1, 1);
    //map.RotateX(-M_PI_2);
    //map.Reflection(1, 2, 3);
    //map.Translate(0, 0, -10);

    model.Transform(map);
    model.Write(output.u8string());
}