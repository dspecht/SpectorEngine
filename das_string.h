#ifndef DAS_STRING_H
#define DAS_STRING_H
// @author Dustin Specht (Spector)
// This library is going to be my personal impleneation of a string type in cpp
// Testing for now

//TODO: look at doing custom allocations but this is probally already ready
#include <malloc.h>

inline unsigned int
getStringLength(char* input)
{
    unsigned int length = 0;
    while(*input++ != 0) { ++length; }
    return length;
}

struct String
{
    char* string;
    unsigned int length; // not including termininating null

    // OPERATOR_OVERLOAD SECTION
    inline char & operator[](unsigned int index)
    { return string[index]; }

    inline void operator=(char* input)
    {
        length = getStringLength(input);
        string = input;
    }
};
//We are actually getting each character, not just setting the address
inline void
copyString(String input, String output)
{
    if(input.length != 0)
    {
        output.length = input.length;
        for(unsigned int i = 0; i < input.length; i++)
        {
            output.string[i] = input.string[i];
        }
        output.string[output.length] = '\0';
    }
}

inline String
copyString(String input)
{
    String result = {(char*)calloc((input.length +1), sizeof(char)) \
                    , input.length};
    if(input.length != 0)
    {
        for(unsigned int i = 0; i < input.length; i++)
        {
            result.string[i] = input.string[i];
        }
        result.string[input.length] = '\0';
    }
    else { result = {}; }
    return result;
}

inline String
copyString(char* input)
{
    unsigned int length = getStringLength(input);
    String result = {(char*)calloc((length +1), sizeof(char)) \
                    , length};
    if(length != 0)
    {
        result.length = length;
        for(unsigned int i = 0; i < length; i++)
        {
            result.string[i] = input[i];
        }
        result.string[length] = '\0';
    }
    else { result = {}; }
    return result;
}

inline bool
compareString(String A, String B)
{
    if(A.length != B.length) return false;
    for(unsigned int i = 0; i < A.length; i++)
    {
        if(A.string[i] != B.string[i]) return false;
    }
    return true;
}

inline bool
compareString(String A, char* B)
{
    String C = copyString(B);
    if(A.length != C.length) return false;
    for(unsigned int i = 0; i < A.length; i++)
    {
        if(A.string[i] != C.string[i]) return false;
    }
    return true;
}

inline bool
compareString(char* A, String B)
{
    String C = copyString(A);
    if(B.length != C.length) return false;
    for(unsigned int i = 0; i < B.length; i++)
    {
        if(B.string[i] != C.string[i]) return false;
    }
    return true;
}

inline String
splitString(String input, char delim)
{
    unsigned int length = 0;
    for(unsigned int i = 0; i < input.length; i++)
    {
        if(input[i] == delim)
        {
            String result = {(char*)calloc((length +1), sizeof(char)) \
                            , (length)};
            for(unsigned int i = 0; i < length; i++)
            {
                result[i] = input[i];
            }
            result[length] = '\0';
            return result;
        }

        ++length;
    }
    return input;
}

inline String
splitString(char* string, char delim)
{
    String input;
    input = string;
    return splitString(input, delim);
}

inline void
splitString(String input, char delim, String output)
{
    unsigned int length = 0;
    for(unsigned int i = 0; i < input.length; i++)
    {
        if(input[i] == delim)
        {
            output.length = length;
            for(unsigned int i = 0; i < length; i++)
            {
                output[i] = input[i];
            }
            output[length] = '\0';
        }

        ++length;
    }
}

inline void
splitString(char* string, char delim, String output)
{
    String input;
    input = string;
    return splitString(input, delim, output);
}

inline void
splitString(char* input, char delim, char* output)
{
    unsigned int length = 0;
    while(*input++ != delim) { ++length; }
    for(unsigned int i = 0; i < length; i++)
    {
        output[i] = input[i];
    }
    output[length] = '\0';

}

inline String
operator+(String A, String B)
{
    String result = {(char*)calloc((A.length + B.length +1), sizeof(char)) \
                    , (A.length + B.length)};
    unsigned int index = 0;
    if (result.length != 0)
    {
        // Loop through A then B, just a basic concatination
        for(unsigned int i = 0; i < A.length; i++)
        { result.string[index] = A.string[i]; ++index; }
        for(unsigned int i = 0; i < B.length; i++)
        { result.string[index] = B.string[i]; ++index; }
        result.string[index+1] = '\0';
    }
    else { result = {}; }
    return result;
}

inline String
operator+(String A, char* B)
{
    String C = copyString(B);
    String result = {(char*)calloc((A.length + C.length +1), sizeof(char)) \
                    , (A.length + C.length)};
    unsigned int index = 0;
    if (result.length != 0)
    {
        for(unsigned int i = 0; i < A.length; i++)
        { result.string[index] = A.string[i]; ++index; }
        for(unsigned int i = 0; i < C.length; i++)
        { result.string[index] = C.string[i]; ++index; }
        result.string[index+1] = '\0';
    }
    else { result = {}; }
    return result;
}

inline String
operator+(char* A, String B)
{
    String C = copyString(A);
    String result = {(char*)calloc((C.length + B.length +1), sizeof(char)) \
                    , (C.length + B.length)};
    unsigned int index = 0;
    if (result.length != 0)
    {
        for(unsigned int i = 0; i < C.length; i++)
        { result.string[index] = C.string[i]; ++index; }
        for(unsigned int i = 0; i < B.length; i++)
        { result.string[index] = B.string[i]; ++index; }
        result.string[index+1] = '\0';
    }
    else { result = {}; }
    return result;
}

inline String &
operator+=(String &A, char* B)
{
    A = A + copyString(B);
    return A;
}

inline String &
operator+=(char* B, String &A)
{
    A = copyString(B) + A;
    return A;
}

inline String &
operator+=(String &A, String B)
{
    A = A + copyString(B);
    return A;
}

inline bool
operator==(String A, String B)
{//TODO: do shorthand if here
    if(compareString(A, B)) return true;
    else return false;
}
inline bool
operator==(String A, char* B)
{
    if(compareString(A, B)) return true;
    else return false;
}
inline bool
operator==(char* A, String B)
{
    if(compareString(A, B)) return true;
    else return false;
}

inline bool
operator!=(String A, String B)
{
    if(compareString(A, B)) return false;
    else return true;
}
inline bool
operator!=(String A, char* B)
{
    if(compareString(A, B)) return false;
    else return true;
}
inline bool
operator!=(char* A, String B)
{
    if(compareString(A, B)) return false;
    else return true;
}
#endif

