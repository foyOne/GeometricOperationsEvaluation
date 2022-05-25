#pragma once

#include "AffineTransformation.h"

#include <vector>

class TransformationView
{
public:
	TransformationView() = default;

	template<class T>
	std::vector<T> RelfectionAsMatrix(ReflectionPtr reflection)
	{
		T x, y, z;
		x = static_cast<T>(reflection->normal[0]);
		y = static_cast<T>(reflection->normal[1]);
		z = static_cast<T>(reflection->normal[2]);

		T xx = 1 - 2 * x * x, yy = 1 - 2 * y * y, zz = 1 - 2 * z * z;
		T xy = -2 * x * y;
		T yz = -2 * y * z;
		T zx = -2 * z * x;

		std::vector<T> matrix =
		{
			xx, xy, zx,
			xy, yy, yz,
			zx, yz, zz
		};

		return matrix;
	}

	std::vector<float> AsMatrix(TransformationPtr t)
	{
		switch (t->GetType())
		{
			case TransformationType::Translation:
			{
				auto translation = std::dynamic_pointer_cast<Translation>(t);
				return std::vector<float>(translation->vector.begin(), translation->vector.end());
			}
			break;

			case TransformationType::Scale:
			{
				auto scale = std::dynamic_pointer_cast<Scale>(t);

				float x, y, z;
				x = scale->scale[0];
				y = scale->scale[1];
				z = scale->scale[2];

				std::vector<float> matrix =
				{
					x, 0, 0,
					0, y, 0,
					0, 0, z,
				};
				return matrix;
			}
			break;

			case TransformationType::Rotation:
			{
				auto rotation = std::dynamic_pointer_cast<Rotation>(t);

				float x, y, z;
				x = rotation->axis[0];
				y = rotation->axis[1];
				z = rotation->axis[2];

				float c = std::cos(rotation->angle);
				float s = std::sin(rotation->angle);
				float x2 = x * x, y2 = y * y, z2 = z * z;
				float xy = x * y, yz = y * z, zx = z * x;

				std::vector<float> matrix =
				{
					c + x2 * (1 - c), xy* (1 - c) - z * s, zx* (1 - c) + y * s,
					xy* (1 - c) + z * s, c + y2 * (1 - c), yz* (1 - c) - x * s,
					zx* (1 - c) - y * s, yz* (1 - c) + x * s, c + z2 * (1 - c),
				};
				return matrix;
			}
			break;

			case TransformationType::Reflection:
			{
				auto ref = std::dynamic_pointer_cast<Reflection>(t);

				float x, y, z;
				x = ref->normal[0];
				y = ref->normal[1];
				z = ref->normal[2];

				float xx = 1 - 2 * x * x, yy = 1 - 2 * y * y, zz = 1 - 2 * z * z;
				float xy = -2 * x * y;
				float yz = -2 * y * z;
				float zx = -2 * z * x;

				std::vector<float> matrix =
				{
					xx, xy, zx,
					xy, yy, yz,
					zx, yz, zz
				};

				return matrix;	
			}
			break;

			case TransformationType::General:
			{
				auto g = std::dynamic_pointer_cast<GeneralTransformation>(t);
				return std::vector<float>(g->data.begin(), g->data.end());
			}
			break;
		}
	}
};