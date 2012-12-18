#ifndef AUPARSER_H
#define AUPARSER_H

#include "abstractparser.h"
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QDebug>

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

    AuPState();
    AuPState(int a) {alpha = a;}
    void addRule(AuPRule r);
    int doMove(Lexeme l);        // берёт текущую лексему, возвращает номер следующего состояния
};

class AuPAutomate
{
    QVector<AuPState>   states;
public:
    AuPAutomate(QString filename);
    AuPState getState(int );    // возвращает состояние по номеру
    void addState(AuPState st);
};

class AuParser : public AbstractParser
{
public:
    AuParser();
    QVector<QString> parse();
};

#endif // AUPARSER_H
