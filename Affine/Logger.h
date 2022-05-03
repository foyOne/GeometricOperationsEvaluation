#pragma once

#include <iostream>
#include <sstream>

class LogLine
{

public:
    LogLine(std::ostream& out = std::cout) : _out(out)
    {

    }
    ~LogLine()
    {
        _stream << '\n';
        _out << _stream.rdbuf();
        _out.flush();
    }

    template <class T>
    LogLine& operator<<(const T& data)
    {
        _stream << data;
        return *this;
    }

private:
    std::stringstream _stream;
    std::ostream& _out;
};