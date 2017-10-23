#ifndef SPECTORENGINE_H

//NOTE(Dustin): look at replacing this with our own stuff so we don't need as much CRT
#include <math.h>
#include <stdint.h>
#include "das_string.h"
#include "das_vector.h"
#define SPECTOR_ENGINE_DEBUG_MODE

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
//SPECTOR_ENGINE_DEBUG_MODE
#if 1
    #define Assert(Expression) if(!(Expression)) {*(u8 *)0 = 0;}
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

#define SPECTORENGINE_H
#endif
