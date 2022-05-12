#include "Readers/STLReader/STLReader.h"
#include "Writers/STLWriter/STLWriter.h"

#include "TransformationExecutors/StandardSTLTransformationExecutor/StandardSTLTransformationExecutor.h"
#include "TransformationExecutors/MKLSTLTransformationExecutor/MKLSTLTransformationExecutor.h"
#include "TransformationExecutors/TransformationExecutor.h"
#include "Result.h"

#include "STLModelTest.h"

#include <time.h>
#include <sstream>

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
    builder.AddRotation(Rotation({ 0, 0, 1 }, 180));

    TransformationExecutorPtr stlExecutor(new StandardSTLTransformationExecutor(builder));

    test.Start(stlExecutor, true);
}

int main()
{
    std::string pathToTest = "TestData/InputModels/";

    STLModelTest test(pathToTest);

    TransformationBuilder builder;
    builder.AddTranslation(Translation(1, 1, 1));
    builder.AddRotation(Rotation({ 1, 1, 1 }, 90));
    builder.AddReflection(Reflection(0, 0, 1));

    TransformationExecutorPtr stlExecutor(std::make_shared<MKLSTLTransformationExecutor>(builder));

    //test.Start(stlExecutor, true);

    STLReader reader;
    auto model = reader.Read(pathToTest + "Sphere.stl");

    stlExecutor->Transform(model);

    return 0;
}