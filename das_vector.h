#ifndef DAS_VECTOR_H
// @author Dustin Specht
// float and int vectors
// Operators overloaded
//      vector# * float && vector#i * int
//      float * vector# && int * vector#i
//      vector# * vector# && vector#i * vector#i
//      vector# *= float && vector#i *= int
//      vector# *= vector# && vector#i *= vector#i
//      Negitive float vector && Negitive int vector
//      vector# + vector# && vector#i + vector#i
//      vector# += vector# && vector#i += vector#i
//      vector# - vector# && vector#i - vector#i
//      vector# == vector# && vector#i == vector#i
//      vector# != vector# && vector#i != vector#i
//      ! vector# && ! vector#i
union vector2
{
    struct{ float x,y; };
    struct{ float u,v; };
    struct{ float width, height; };
    float E[2];
};

union vector3
{
    struct { float x,y,z; };
    struct { float r,g,b; };
    struct { float u,v,__; };
    float E[3];
};

union vector4
{
    struct { float x,y,z,w; };
    struct { float r,g,b,a; };
    float E[4];
};

//INT
union vector2i
{
    struct { int x,y; };
    struct { int u,v; };
    int E[2];
};

union vector3i
{
    struct { int x,y,z; };
    struct { int r,g,b; };
    struct { int u,v,__; };
    int E[3];
};

union vector4i
{
    struct { int x,y,z,w; };
    struct { int r,g,b,a; };
    int E[4];
};
// Create a float Vector
vector2 Vector2(float x, float y)
{ vector2 result = {x,y}; return result; }
vector3 Vector3(float x, float y, float z)
{ vector3 result = {x,y,z}; return result; }
vector4 Vector4(float x, float y, float z, float w)
{ vector4 result = {x,y,z,w}; return result; }
// Create a Int Vector
vector2i Vector2i(int x, int y)
{ vector2i result = {x,y}; return result; }
vector3i Vector3i(int x, int y, int z)
{ vector3i result = {x,y,z}; return result; }
vector4i Vector4i(int x, int y, int z, int w)
{ vector4i result = {x,y,z,w}; return result; }

//Multiply a float by a float vector
inline vector2 operator*(float a, vector2 b)
{ vector2 result = {a*b.x, a*b.y}; return result; }
inline vector3 operator*(float a, vector3 b)
{ vector3 result = {a*b.x, a*b.y, a*b.z}; return result; }
inline vector4 operator*(float a, vector4 b)
{ vector4 result = {a*b.x, a*b.y, a*b.z, a*b.w}; return result; }
//Multiply a float vector by a float
inline vector2 operator*(vector2 b, float a) { return a*b; }
inline vector3 operator*(vector3 b, float a) { return a*b; }
inline vector4 operator*(vector4 b, float a) { return a*b; }
//Multiply a float vector by a float vector
inline vector2 operator*(vector2 a, vector2 b)
{ vector2 result = {(a.x * b.x),(a.y * b.y)}; return result; }
inline vector3 operator*(vector3 a, vector3 b)
{ vector3 result = {(a.x * b.x),(a.y * b.y),(a.z * b.z)}; return result; }
inline vector4 operator*(vector4 a, vector4 b)
{ vector4 result = {(a.x * b.x),(a.y * b.y),(a.z * b.z),(a.w * b.z)}; return result; }

//Multiply a int by a int vector
inline vector2i operator*(int a, vector2i b)
{ vector2i result = {a*b.x, a*b.y}; return result; }
inline vector3i operator*(int a, vector3i b)
{ vector3i result = {a*b.x, a*b.y, a*b.z}; return result; }
inline vector4i operator*(int a, vector4i b)
{ vector4i result = {a*b.x, a*b.y, a*b.z, a*b.w}; return result; }
//Multiply a int vector by a int
inline vector2i operator*(vector2i b, int a) { return a*b; }
inline vector3i operator*(vector3i b, int a) { return a*b; }
inline vector4i operator*(vector4i b, int a) { return a*b; }
//Multiply a int vector by a int vector
inline vector2i operator*(vector2i a, vector2i b)
{ vector2i result = {(a.x * b.x),(a.y * b.y)}; return result; }
inline vector3i operator*(vector3i a, vector3i b)
{ vector3i result = {(a.x * b.x),(a.y * b.y),(a.z * b.z)}; return result; }
inline vector4i operator*(vector4i a, vector4i b)
{ vector4i result = {(a.x * b.x),(a.y * b.y),(a.z * b.z),(a.w * b.w)}; return result; }

//*= float vector ,float
inline vector2 & operator*=(vector2 &b, float a)
{ return b = a * b; }
inline vector3 & operator*=(vector3 &b, float a)
{ return b = a * b; }
inline vector4 & operator*=(vector4 &b, float a)
{ return b = a * b; }
//*= float vector ,float vector
inline vector2 & operator*=(vector2 &b, vector2 a)
{ return b = a * b; }
inline vector3 & operator*=(vector3 &b, vector3 a)
{ return b = a * b; }
inline vector4 & operator*=(vector4 &b, vector4 a)
{ return b = a * b; }

//*= int vector ,int
inline vector2i & operator*=(vector2i &b, int a)
{ return b = a * b; }
inline vector3i & operator*=(vector3i &b, int a)
{ return b = a * b; }
inline vector4i & operator*=(vector4i &b, int a)
{ return b = a * b; }
//*= int vector, int vector
inline vector2i & operator*=(vector2i &b, vector2i a)
{ return b = a * b; }
inline vector3i & operator*=(vector3i &b, vector3i a)
{ return b = a * b; }
inline vector4i & operator*=(vector4i &b, vector4i a)
{ return b = a * b; }

//Negitive (negate) float vector
inline vector2 operator-(vector2 a)
{ vector2 result = {-a.x, -a.y}; return result;}
inline vector3 operator-(vector3 a)
{ vector3 result = {-a.x, -a.y, -a.z}; return result;}
inline vector4 operator-(vector4 a)
{ vector4 result = {-a.x, -a.y, -a.z, -a.w}; return result;}
//Negitive (negate) int vector
inline vector2i operator-(vector2i a)
{ vector2i result = {-a.x, -a.y}; return result;}
inline vector3i operator-(vector3i a)
{ vector3i result = {-a.x, -a.y, -a.z}; return result;}
inline vector4i operator-(vector4i a)
{ vector4i result = {-a.x, -a.y, -a.z, -a.w}; return result;}

//float vector + float vector
inline vector2 operator+(vector2 a, vector2 b)
{ vector2 result = {(a.x + b.x),(a.y + b.y)}; return result; }
inline vector3 operator+(vector3 a, vector3 b)
{ vector3 result = {(a.x + b.x),(a.y + b.y),(a.z + b.z)}; return result; }
inline vector4 operator+(vector4 a, vector4 b)
{ vector4 result = {(a.x + b.x),(a.y + b.y),(a.z + b.z),(a.w + b.z)}; return result; }
//int vector + int vector
inline vector2i operator+(vector2i a, vector2i b)
{ vector2i result = {(a.x + b.x),(a.y + b.y)}; return result; }
inline vector3i operator+(vector3i a, vector3i b)
{ vector3i result = {(a.x + b.x),(a.y + b.y),(a.z + b.z)}; return result; }
inline vector4i operator+(vector4i a, vector4i b)
{ vector4i result = {(a.x + b.x),(a.y + b.y),(a.z + b.z),(a.w + b.w)}; return result; }


//+= float vector ,float vector
inline vector2 & operator+=(vector2 &b, vector2 a)
{ return b = a + b; }
inline vector3 & operator+=(vector3 &b, vector3 a)
{ return b = a + b; }
inline vector4 & operator+=(vector4 &b, vector4 a)
{ return b = a + b; }
//+= int vector ,int vector
inline vector2i & operator+=(vector2i &b, vector2i a)
{ return b = a + b; }
inline vector3i & operator+=(vector3i &b, vector3i a)
{ return b = a + b; }
inline vector4i & operator+=(vector4i &b, vector4i a)
{ return b = a + b; }

//float vector - float vector
inline vector2 operator-(vector2 a, vector2 b)
{ vector2 result = {(a.x - b.x),(a.y - b.y)}; return result; }
inline vector3 operator-(vector3 a, vector3 b)
{ vector3 result = {(a.x - b.x),(a.y - b.y),(a.z - b.z)}; return result; }
inline vector4 operator-(vector4 a, vector4 b)
{ vector4 result = {(a.x - b.x),(a.y - b.y),(a.z - b.z),(a.w - b.z)}; return result; }
//int vector - int vector
inline vector2i operator-(vector2i a, vector2i b)
{ vector2i result = {(a.x - b.x),(a.y - b.y)}; return result; }
inline vector3i operator-(vector3i a, vector3i b)
{ vector3i result = {(a.x - b.x),(a.y - b.y),(a.z - b.z)}; return result; }
inline vector4i operator-(vector4i a, vector4i b)
{ vector4i result = {(a.x - b.x),(a.y - b.y),(a.z - b.z),(a.w - b.w)}; return result; }

//float vector == float vector
inline bool operator==(vector2 a, vector2 b)
{ return (a.x == b.x && a.y == b.y); }
inline bool operator==(vector3 a, vector3 b)
{ return (a.x == b.x && a.y == b.y && a.z == b.z); }
inline bool operator==(vector4 a, vector4 b)
{ return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w); }
//int vector == int vector
inline bool operator==(vector2i a, vector2i b)
{ return (a.x == b.x && a.y == b.y); }
inline bool operator==(vector3i a, vector3i b)
{ return (a.x == b.x && a.y == b.y && a.z == b.z); }
inline bool operator==(vector4i a, vector4i b)
{ return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w); }

//float vector != float vector
inline bool operator!=(vector2 a, vector2 b)
{ return (a.x != b.x && a.y != b.y); }
inline bool operator!=(vector3 a, vector3 b)
{ return (a.x != b.x && a.y != b.y && a.z != b.z); }
inline bool operator!=(vector4 a, vector4 b)
{ return (a.x != b.x && a.y != b.y && a.z != b.z && a.w != b.w); }
//int vector != int vector
inline bool operator!=(vector2i a, vector2i b)
{ return (a.x != b.x && a.y != b.y); }
inline bool operator!=(vector3i a, vector3i b)
{ return (a.x != b.x && a.y != b.y && a.z != b.z); }
inline bool operator!=(vector4i a, vector4i b)
{ return (a.x != b.x && a.y != b.y && a.z != b.z && a.w != b.w); }

//float vector !
inline bool operator!(vector2 a)
{ return (a.x == NULL && a.y == NULL); }
inline bool operator!(vector3 a)
{ return (a.x == NULL && a.y == NULL && a.z == NULL); }
inline bool operator!(vector4 a)
{ return (a.x == NULL && a.y == NULL && a.z == NULL && a.w == NULL); }
//int vector !
inline bool operator!(vector2i a)
{ return (a.x == NULL && a.y == NULL); }
inline bool operator!(vector3i a)
{ return (a.x == NULL && a.y == NULL && a.z == NULL); }
inline bool operator!(vector4i a)
{ return (a.x == NULL && a.y == NULL && a.z == NULL && a.w == NULL); }
#define DAS_VECTOR_H
#endif
