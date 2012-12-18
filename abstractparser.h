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
    QVector<bool> idDeclared;       // объ€влена ли переменна€ [i]
    QVector<Lexeme> lexList;        // список лексем
    bool lastParseErrors;           // были ли ошибки
    bool declaring;                 // происходит ли сейчас объ€вление переменных
    QVector<QString> outputText;    // вектор сообщений об ошибках
    int nextLexPointer;             // номер следующей лексемы
    Lexeme currLexeme;              // текуща€ лексема
    int getToken();                 // получить новую лескему в currLexeme
};


// »сключени€
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
