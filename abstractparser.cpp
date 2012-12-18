#include "abstractparser.h"

AbstractParser::AbstractParser()
{
    lastParseErrors = false;
    nextLexPointer = 0;
}

int AbstractParser::getToken()
{
    // для рекурсивного спуска, надо пофиксить его
//    if (nextLexPointer>=lexList.size())
//        return -1;
//        //throw EndOfLexListReached(); //ЧЁ ЗА ЕРЕСЬ?!?!?!?!? не ловит исключения
//    else currLexeme = lexList[nextLexPointer];
    // для автомата
    try
    {
        currLexeme = lexList[nextLexPointer];
    }
    catch(...)
    {
        throw EndOfLexListReached();
    }

    nextLexPointer++;
}
