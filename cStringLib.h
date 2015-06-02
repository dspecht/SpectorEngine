#ifndef CSTRINGLIB_H
#define CSTRINGLIB_H
#include <stdlib.h>

int getStringLength(char *str)
{
    int result= 0;

    for(int index = 0; str[index]; index++)
    {
        ++result;
    }
    return result;
}

// Utliity function
void clearTempStringsToNull(char *str)
{
    for(int index = 0; str[index] ; index++)
    {
        str[index] = 0;
    }
}

//TODO: make another compreString that is case insensitive
bool compareString(char *fString, char *sString)
{
    int index = 0;
    bool result = false;

    while(fString[index])
    {
        if(fString[index] != sString[index])
        {break;}

        index++;
    }
    if (!sString[index])
    {
        result = true;
    }
    return result;
}

//NOTE(dustin): if you don't pass a placeToPutCopiedString then you will have to free the result
char* CopyString(char *strToCopy, char *placeToPutCopiedString=NULL)
{
    if(!placeToPutCopiedString)
    {
        char *result = (char*)calloc(getStringLength(strToCopy)+1, sizeof(char));

        for (int index = 0; strToCopy[index]; index++)
        {
            result[index] = strToCopy[index];
        }
        return result;
    }
    else
    {
        for (int index = 0; strToCopy[index]; index++)
        {
            placeToPutCopiedString[index] = strToCopy[index];
        }
        return placeToPutCopiedString; // if you check for a return, this make it not fail everytime
    }
}

char* CatString(char *originString, char *strToCat, char *outputString=NULL)
{
    int originLength = getStringLength(originString);
    int catStrLength = getStringLength(strToCat);
    int outstringLength = (originLength + catStrLength);
    int catStrIndex = 0;

    if(!outputString)
    {
        char *result = ((char*)calloc(outstringLength+1, sizeof(char)));

        for(int index = 0; index < outstringLength; index++)
        {
            if(index < originLength)
            {
                result[index] = originString[index];
            }
            else
            {
                result[index] = strToCat[catStrIndex];
                catStrIndex++;
            }
        }
        return result;
    }
    else
    {
        for(int index = 0; index < outstringLength; index++)
        {
            if(index < originLength)
            {
                outputString[index] = originString[index];
            }
            else
            {
                outputString[index] =strToCat[catStrIndex];
                catStrLength++;
            }
        }
        return outputString; // if you check for a return, this  makes it not just fail
    }
}

// if no outputstring then calle has to free | if you don't give me a valid savePlace you get nothing back about where you were in the string/array

char* SplitString(char *inputString, char strDelim, char *savePlace, char *outputString=NULL)
{
    bool isParsingStringToDelim = true;
    int index = 0;
    int resultIndex = 0;

    if(inputString)
    {
        while (isParsingStringToDelim)
        {
            //TODO: make this take multiple possible delim characters
            if(inputString[index] == strDelim)
            {
                clearTempStringsToNull(savePlace);
                isParsingStringToDelim = false;
                index++;

                if (savePlace)
                {
                    for(int localIndex = 0; inputString[index]; localIndex++)
                    {
                        savePlace[localIndex] = inputString[index];
                        index++;
                    }
                }
            }
            else
            {
                if (inputString[index])
                {
                    resultIndex++;
                    index++;
                }
                else
                {
                    break;
                }
            }
        }

        if(resultIndex)
        {
            if(!outputString)
            {
                char *result = ((char*)calloc(resultIndex+1, sizeof(char)));

                for(int localIndex = 0; localIndex < resultIndex; localIndex++)
                {
                    result[localIndex] = inputString[localIndex];
                }

                return result;
            }
            else
            {
                for(int localIndex = 0; localIndex < resultIndex; localIndex++)
                {
                    outputString[localIndex] = inputString[localIndex];
                }
                return outputString; // make sure if this is in a if that it doesnt fail all the time
            }
        }
    }

    return NULL;
}
#endif
