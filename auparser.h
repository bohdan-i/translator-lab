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
    int                 alpha;  // ����� ����� ���������
    QVector<AuPRule>    rules;  // ������� ����� ���������
    int                 toStack;// ����� doMove ��� ���������� � ����

    AuPState();
    AuPState(int a) {alpha = a;}
    void addRule(AuPRule r);
    AuPRule getRule(Lexeme l);
//    int doMove(Lexeme l);        // ���� ������� �������, ���������� ����� ���������� ���������
};

class AuPAutomate
{
    QVector<AuPState>   states;
public:
    AuPAutomate(QString filename);
    AuPAutomate();
    AuPState getState(int st);    // ���������� ��������� �� ������
    void addState(AuPState st);
    void setAutomate(QString filename);
};

class AuParser : public AbstractParser
{
private:
    AuPRule             currRule;       // ������� �������
    AuPState            currState;      // ������� ���������
    AuPAutomate         automate;       // ������� � ���������
    int                 nextState;      // ����� ���������� ���������
    QStack<int>         stack;          // ����
    QVector<QString>    errorMessage;   // ��������� ��������� �� �������
public:
    AuParser();
    QVector<QString>    parse();
    bool                doMove();
    void                initErrorMessagesList(QString filename);
    void                checkIdentifier();
};

#endif // AUPARSER_H
