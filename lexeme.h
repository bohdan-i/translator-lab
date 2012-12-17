#ifndef LEXEME_H
#define LEXEME_H

#include <QString>

struct Lexeme {
    QString lexName;
    int code;
    int ind;
    int row;
    void clean()
    {
        lexName = "";
        code = 0;
        ind = 0;
        row = 0;
    }
    void set(QString Name, int Code, int Ind, int Row)
    {
        lexName = Name;
        code = Code;
        ind = Ind;
        row = Row;
    }
};

#endif // LEXEME_H
