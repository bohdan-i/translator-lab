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
    int                 alpha;  // ����� ����� ���������
    QVector<AuPRule>    rules;  // ������� ����� ���������

    AuPState();
    AuPState(int a) {alpha = a;}
    void addRule(AuPRule r);
    int doMove(Lexeme l);        // ���� ������� �������, ���������� ����� ���������� ���������
};

class AuPAutomate
{
    QVector<AuPState>   states;
public:
    AuPAutomate(QString filename);
    AuPState getState(int );    // ���������� ��������� �� ������
    void addState(AuPState st);
};

class AuParser : public AbstractParser
{
public:
    AuParser();
    QVector<QString> parse();
};

#endif // AUPARSER_H
