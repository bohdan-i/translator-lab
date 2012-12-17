#include "abstractparser.h"

AbstractParser::AbstractParser()
{
    lastParseErrors = false;
    nextLexPointer = 0;
}

int AbstractParser::getToken()
{
    if (nextLexPointer>=lexList.size())
        return -1;
        //throw EndOfLexListReached(); //ЧЁ ЗА ЕРЕСЬ?!?!?!?!? не ловит исключения
    else currLexeme = lexList[nextLexPointer];
//    try
//    {
//        currLexeme = lexList[nextLexPointer];
//    }
//    catch(...)
//    {
//        throw EndOfLexListReached();
//    }

    nextLexPointer++;
}
