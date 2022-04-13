#include "AffineMap.h"
#include "DataModels/STLModel/STLModel.h"
#include "TransformationExecutors/StandardSTLTransformationExecutor/StandardSTLTransformationExecutor.h"
#include "TransformationExecutors/MKLSTLTransformationExecutor/MKLSTLTransformationExecutor.h"

#include <iostream>
#include <memory>
#include <filesystem>

#define _USE_MATH_DEFINES
#include <math.h>

#include <string>
#include <time.h>
#include <sstream>


#include <mkl.h>
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

void main()
{
    std::string file = "cube_ascii.stl";
    std::string transformed = GenerateOutputFile(file);

    std::filesystem::path testDataPath("TestData");

    std::filesystem::path input = testDataPath / "InputModels" / file;
    std::filesystem::path output = testDataPath / "OutputModels" / transformed;

    STLModel model(input.u8string());

    std::shared_ptr<ITtransformationExecutor> ste;
    ste.reset(new MKLSTLTransformationExecutor());
    model.SetTtransformationExecutor(ste);

    auto map = AffineMap();
    map.RotateX(M_PI / 6);
    map.RotateY(M_PI / 3);
    map.RotateZ(M_PI / 2);
    map.Scale(1, 2, 3);
    map.Reflection(1, 1, 1);
    map.Reflection(1, 2, 3);

    //map.Rotate(1, 1, 1, M_PI_2);
    //map.Reflection(1, 1, 1);
    //map.RotateX(-M_PI_2);
    //map.Reflection(1, 2, 3);
    //map.Translate(0, 0, -10);

    model.Transform(map);
    model.Write(output.u8string());
}