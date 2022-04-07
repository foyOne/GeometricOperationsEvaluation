#pragma once

#include <cstring>
#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>

class AffineMap
{
public:
    AffineMap()
    {
        Reset();
    }

    void RotateX(float angle)
    {
        float M[9] = {
            1, 0, 0,
            0, cos(angle), -sin(angle),
            0, sin(angle), cos(angle)
        };
        AddMap(M);
        _operations |= 1;
    }

    void RotateY(float angle)
    {
        float M[9] = {
            cos(angle), 0, sin(angle),
            0, 1, 0,
            -sin(angle), 0, cos(angle)
        };
        AddMap(M);
        _operations |= 2;
    }

    void RotateZ(float angle)
    {
        float M[9] = {
            cos(angle), -sin(angle), 0,
            sin(angle), cos(angle), 0,
            0, 0, 1
        };
        AddMap(M);
        _operations |= 4;
    }

    void Rotate(float x, float y, float z, float angle)
    {
        float magnitude = sqrt(x * x + y * y + z * z);
        if (abs(magnitude - 1) > 1e-6)
        {
            x /= magnitude;
            y /= magnitude;
            z /= magnitude;
        }

        float c = cos(angle);
        float s = sin(angle);
        float x2 = x * x, y2 = y * y, z2 = z * z;
        float xy = x * y, yz = y * z, zx = z * x;

        float M[9] = {
            c + x2 * (1 - c), xy * (1 - c) - z * s, zx * (1 - c) + y * s,
            xy * (1 - c) + z * s, c + y2 * (1 - c), yz * (1 - c) - x * s,
            zx * (1 - c) - y * s, yz * (1 - c) + x * s, c + z2 * (1 - c)
        };
        AddMap(M);
        _operations |= 8;
    }

    void Scale(float Sx, float Sy, float Sz)
    {
        _map[0] *= Sx;
        _map[4] *= Sy;
        _map[8] *= Sz;
    }

    void Translate(float Tx, float Ty, float Tz)
    {
        _map[9] += Tx;
        _map[10] += Ty;
        _map[11] += Tz;
    }

    void Reflection(float x, float y, float z)
    {
        float magnitude = sqrt(x * x + y * y + z * z);
        if (abs(magnitude - 1) > 1e-6)
        {
            x /= magnitude;
            y /= magnitude;
            z /= magnitude;
        }

        float xx = 1 - 2 * x * x, yy = 1 - 2 * y * y, zz = 1 - 2 * z * z;
        float xy = -2 * x * y;
        float yz = -2 * y * z;
        float zx = -2 * z * x;
        float M[9] = {
            xx, xy, zx,
            xy, yy, yz,
            zx, yz, zz
        };
        AddMap(M);
        _operations ^= 16;
    }

    void Transpose()
    {
        std::swap(_map[1], _map[3]);
        std::swap(_map[2], _map[6]);
        std::swap(_map[5], _map[7]);
    }

    static void Transpose(float M[9])
    {
        std::swap(M[1], M[3]);
        std::swap(M[2], M[6]);
        std::swap(M[5], M[7]);
    }

    void Get(float* map, size_t size = 12) const
    {
        if (size == 3)
            std::memcpy(map, _map + 9, sizeof(float) * 3);
        else if (size == 9)
            std::memcpy(map, _map, sizeof(float) * 9);
        else if (size == 12)
            std::memcpy(map, _map, sizeof(float) * 12);
    }

    void Get(float M[9], float T[3]) const
    {
        std::memcpy(M, _map, sizeof(float) * 9);
        std::memcpy(T, _map + 9, sizeof(float) * 3);
    }

    void Reset()
    {
        std::memset(_map, 0., sizeof(float) * 12);
        _map[0] = _map[4] = _map[8] = 1.;
        _operations = (unsigned char)0;
    }

    bool IsRotation() const
    {
        return (_operations & 15) != 0;
    }

    bool IsReflection() const
    {
        return (_operations & 16) == 16;
    }

    bool IsScale() const
    {
        return std::abs(1 - Determinant()) > 1e-6;
    }

    bool IsTranslation() const
    {
        return std::abs(_map[9]) > 1e-6 || std::abs(_map[10]) > 1e-6 || std::abs(_map[11]) > 1e-6;
    }

    float Determinant() const
    {
        float a = _map[0] * _map[4] * _map[8] + _map[1] * _map[5] * _map[6] + _map[2] * _map[3] * _map[7];
        float b = _map[2] * _map[4] * _map[6] + _map[1] * _map[3] * _map[8] + _map[0] * _map[5] * _map[7];
        return a - b;
    }


private:
    void AddMap(float* other)
    {
        float a11 = other[0] * _map[0] + other[1] * _map[3] + other[2] * _map[6];
        float a12 = other[0] * _map[1] + other[1] * _map[4] + other[2] * _map[7];
        float a13 = other[0] * _map[2] + other[1] * _map[5] + other[2] * _map[8];

        float a21 = other[3] * _map[0] + other[4] * _map[3] + other[5] * _map[6];
        float a22 = other[3] * _map[1] + other[4] * _map[4] + other[5] * _map[7];
        float a23 = other[3] * _map[2] + other[4] * _map[5] + other[5] * _map[8];

        float a31 = other[6] * _map[0] + other[7] * _map[3] + other[8] * _map[6];
        float a32 = other[6] * _map[1] + other[7] * _map[4] + other[8] * _map[7];
        float a33 = other[6] * _map[2] + other[7] * _map[5] + other[8] * _map[8];

        float M[9] = {
            a11, a12, a13,
            a21, a22, a23,
            a31, a32, a33
        };
        std::memcpy(_map, M, sizeof(float) * 9);
    }

private:

    /// <summary>
    /// M 3x3
    /// T 1x3
    /// a0 a1 a2
    /// a3 a4 a5
    /// a6 a7 a8
    /// Tx Ty Tz
    /// </summary>
    float _map[12];
    unsigned char _operations; // Ref R z y x 
};