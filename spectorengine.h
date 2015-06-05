#ifndef SPECTORENGINE_H

//NOTE(Dustin): look at replacing this with our own stuff so we don't need CRT
#include <math.h>
#include <stdint.h>

#define internal static
#define globalVar static
#define localPersist static
//--------------------------------------
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
//--------------------------------------
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
//--------------------------------------
//TODO: When Casey Updates the HMH Assertion macro update this
#if SPECTOR_ENGINE_DEBUG_MODE
    #define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}
#else
    #define Assert(Expression)
#endif

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024)
#define Gigabytes(Value) (Megabytes(Value)*1024)
#define Terabytes(Value) (Gigabytes(Value)*1024)

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

struct Game_Memory
{
   bool IsInitialized;

   uint64 permanentStorageSize;
   void *permanentStorage;

   uint64 transientStorageSize;
   void *transientStorage;
};

struct vector2
{
    struct
    {
        float x, y;
    };
    float E[2];
};

inline vector2
Vector2(float X, float Y)
{
    vector2 result;

    result.x = X;
    result.y = Y;

    return result;
}

union vector3
{
    struct
    {
        float x, y, z;
    };
    struct
    {
        float r, g, b;
    };
    float E[3];
};

inline vector3
Vector3(float X, float Y, float Z)
{
    vector3 result;

    result.x = X;
    result.y = Y;
    result.z = Z;

    return result;
}

union vector4
{
    struct
    {
        float x, y, z, w;
    };
    struct
    {
        float r, g, b, a;
    };
    float E[4];
};

inline vector4
Vector4(float X, float Y, float Z, float W)
{
    vector4 result;

    result.x = X;
    result.y = Y;
    result.z = Z;
    result.w = W;

    return result;
}

inline vector2
operator*(float A, vector2 B)
{
    vector2 Result;

    Result.x = A*B.x;
    Result.y = A*B.y;

    return(Result);
}

inline vector2
operator*(vector2 B, float A)
{
    vector2 Result = A*B;

    return(Result);
}

inline vector2 &
operator*=(vector2 &B, float A)
{
    B = A * B;

    return(B);
}

inline vector2
operator-(vector2 A)
{
    vector2 Result;

    Result.x = -A.x;
    Result.y = -A.y;

    return(Result);
}

inline vector2
operator+(vector2 A, vector2 B)
{
    vector2 Result;

    Result.x = A.x + B.x;
    Result.y = A.y + B.y;

    return(Result);
}

inline vector2 &
operator+=(vector2 &A, vector2 B)
{
    A = A + B;

    return(A);
}

inline vector2
operator-(vector2 A, vector2 B)
{
    vector2 Result;

    Result.x = A.x - B.x;
    Result.y = A.y - B.y;

    return(Result);
}

struct RectCords
{ //rename these latter maybe
    vector3 upperLeft;
    vector3 upperRight;
    vector3 lowerLeft;
    vector3 lowerRight;
};

//Todo(dustin): Do we want to change how this is setup once we start loading bitmaps
#pragma pack(push, 1)
struct bitmap_header
{
    uint16 FileType;
    uint32 FileSize;
    uint16 Reserved1;
    uint16 Reserved2;
    uint32 BitmapOffset;
    uint32 Size;
    int32 Width;
    int32 Height;
    uint16 Planes;
    uint16 BitsPerPixel;
    uint32 Compression;
    uint32 SizeOfBitmap;
    int32 HorzResolution;
    int32 VertResolution;
    uint32 ColorsUsed;
    uint32 ColorsImportant;

    uint32 RedMask;
    uint32 GreenMask;
    uint32 BlueMask;
};
#pragma pack(pop)

#include "spectorOpenGL.cpp"
#include "cStringLib.h"
#define SPECTORENGINE_H
#endif
