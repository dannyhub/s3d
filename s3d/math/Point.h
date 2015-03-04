#ifndef S3D_MATH_POINT_H
#define S3D_MATH_POINT_H
#include "Vector.h"

namespace s3d
{

template<typename T>
using Point2 = Vector2<T>;

template<typename T>
using Point3 = Vector3<T>;

template<typename T>
using Point4 = Vector4<T>;

typedef Vector2I Point2I;
typedef Vector3I Point3I;

typedef Vector2F Point2F;
typedef Vector3F Point3F;
typedef Vector4F Point4F;

typedef Vector2FD Point2FD;
typedef Vector3FD Point3FD;
typedef Vector4FD Point4FD;

}// s3d


#endif