#ifndef AUPARSER_H
#define AUPARSER_H

#include "abstractparser.h"
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QStack>

class AuPRule
{
public:
    AuPRule(QString s);
    AuPRule();

    QString             lexName;
    int                 beta;
    int                 stack;
};

class AuPState
{
public:
    int                 alpha;  // номер этого состояния
    QVector<AuPRule>    rules;  // правила этого состояния
    int                 toStack;// после doMove что отправлять в стек

    AuPState();
    AuPState(int a) {alpha = a;}
    void addRule(AuPRule r);
    AuPRule getRule(Lexeme l);
//    int doMove(Lexeme l);        // берёт текущую лексему, возвращает номер следующего состояния
};

class AuPAutomate
{
    QVector<AuPState>   states;
public:
    AuPAutomate(QString filename);
    AuPAutomate();
    AuPState getState(int st);    // возвращает состояние по номеру
    void addState(AuPState st);
    void setAutomate(QString filename);
};

class AuParser : public AbstractParser
{
private:
    AuPRule             currRule;       // текущее правило
    AuPState            currState;      // текущее состояние
    AuPAutomate         automate;       // автомат с правилами
    int                 nextState;      // номер следующего состояния
    QStack<int>         stack;          // стек
    QVector<QString>    errorMessage;   // возможные сообщения об ошибках
public:
    AuParser();
    QVector<QString>    parse();
    bool                doMove();
    void                initErrorMessagesList(QString filename);
    void                checkIdentifier();
};

#endif // AUPARSER_H
