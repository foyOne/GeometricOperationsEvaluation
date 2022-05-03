#pragma once

#include "Logger.h"
#include "TransformationExecutors/TransformationExecutor.h"
#include "Readers/STLReader/STLReader.h"
#include "Writers/STLWriter/STLWriter.h"

#include "Result.h"

#include <filesystem>

class STLModelTest
{
    using Path = std::filesystem::path;

public:

    STLModelTest(Path path) : _pathToData(path), _pathToSave("") {}

    STLModelTest(Path pathToData, Path pathToSave) : _pathToData(pathToData), _pathToSave(pathToSave) {}

    std::vector<Result> Start(TransformationExecutorPtr executor, bool destroyModelAfterTest = false)
    {
        std::vector<Result> resultArray;
        STLReader reader;
        STLWriter writer(true);

        std::string prefix = "transformed_";
        

        for (const auto& file : std::filesystem::directory_iterator(_pathToData))
        {
            auto extension = file.path().extension().u8string();

            if (extension == ".stl")
            {
                LogLine() << "--------------------";
                LogLine() << "file to test - " << file.path().filename();

                auto model = reader.Read(file.path().u8string());

                if (model)
                {
                    LogLine() << "file read correctly";
                    LogLine() << "test started";

                    Result r = executor->Transform(model);

                    LogLine() << "elapsed time" << " = " << r.time;

                    if (!_pathToSave.empty())
                    {
                        auto filename = *(--file.path().end());
                        auto pathToSave = _pathToSave / Path(prefix + filename.u8string());

                        LogLine() << "file is saving";

                        writer.Write(pathToSave.u8string(), r.model);

                        LogLine() << "file saved";

                    }

                    if (destroyModelAfterTest)
                        r.model.reset();

                    resultArray.push_back(r);
                }
            }
        }

        return resultArray;
    }

private:

    Path _pathToData;
    Path _pathToSave;
};