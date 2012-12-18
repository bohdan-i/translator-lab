#ifndef ABSTRACTPARSER_H
#define ABSTRACTPARSER_H

#include <QVector>
#include <QString>
#include "lexeme.h"

class AbstractParser
{
public:
    AbstractParser();
    void load(QVector<Lexeme> l)
    {
        lexList = l;
        idDeclared.clear();
        foreach(Lexeme a, l)
        {
            if(a.code == 29) idDeclared.append(false);
        }

        return;
    }
    virtual QVector<QString> parse() = 0;
    QVector<QString> getTextOutput() {return outputText;}
    bool errorsHappened() { return lastParseErrors;}
protected:
    QVector<bool> idDeclared;
    QVector<Lexeme> lexList;
    bool lastParseErrors;
    QVector<QString> outputText;
    int nextLexPointer;
    Lexeme currLexeme;
    int getToken();
};

class EndOfLexListReached
{
public:
    EndOfLexListReached() throw() { }
    const QString what() {return QString("End of program.");}
};

class InvalidSymbolInOperator
{
public:
    InvalidSymbolInOperator(QString m = "Error.") throw() {msg = m;}
    QString what() {return msg;}
private:
    QString msg;
};
#endif // ABSTRACTPARSER_H
