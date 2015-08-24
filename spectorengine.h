#ifndef SPECTORENGINE_H

//NOTE(Dustin): look at replacing this with our own stuff so we don't need CRT
#include <math.h>
#include <stdint.h>

#define internal static
#define globalVar static
#define localPersist static
//--------------------------------------
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
//--------------------------------------
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float r32;
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

   u64 permanentStorageSize;
   void *permanentStorage;

   u64 transientStorageSize;
   void *transientStorage;
};

struct vector2
{
    struct
    {
        r32 x, y;
    };
    r32 E[2];
};

inline vector2
Vector2(r32 X, r32 Y)
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
        r32 x, y, z;
    };
    struct
    {
        r32 r, g, b;
    };
    r32 E[3];
};

inline vector3
Vector3(r32 X, r32 Y, r32 Z)
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
        r32 x, y, z, w;
    };
    struct
    {
        r32 r, g, b, a;
    };
    r32 E[4];
};

inline vector4
Vector4(r32 X, r32 Y, r32 Z, r32 W)
{
    vector4 result;

    result.x = X;
    result.y = Y;
    result.z = Z;
    result.w = W;

    return result;
}

inline vector2
operator*(r32 A, vector2 B)
{
    vector2 Result;

    Result.x = A*B.x;
    Result.y = A*B.y;

    return(Result);
}

inline vector2
operator*(vector2 B, r32 A)
{
    vector2 Result = A*B;

    return(Result);
}

inline vector2 &
operator*=(vector2 &B, r32 A)
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
    u16 FileType;
    u32 FileSize;
    u16 Reserved1;
    u16 Reserved2;
    u32 BitmapOffset;
    u32 Size;
    s32 Width;
    s32 Height;
    u16 Planes;
    u16 BitsPerPixel;
    u32 Compression;
    u32 SizeOfBitmap;
    s32 HorzResolution;
    s32 VertResolution;
    u32 ColorsUsed;
    u32 ColorsImportant;

    u32 RedMask;
    u32 GreenMask;
    u32 BlueMask;
};
#pragma pack(pop)

#include "spectorOpenGL.cpp"
#include "cStringLib.h"
#define SPECTORENGINE_H
#endif
