#ifndef SCANNER_H
#define SCANNER_H

#include <QVector>
#include "lexeme.h"

class Scanner
{
public:
    Scanner();
    QVector<Lexeme> scan(QString text);
    QVector<QString> getErrorStringsVector(){return errorStringsVector;}
        // получить строку вывода ошибок сканирования
    bool lastScanHasErrors(){return lastScanErrors;} // были ли ошибки в последнем скане
    int idAmount() {return idList.size();}
    int conAmount() {return conList.size();}
    QVector<QString> getIdList() {return idList;}
    QVector<QString> getConList() {return conList;}
private:
    QVector<QString> errorStringsVector;
    Lexeme currentLexeme;
    char currSymb;
    uint currSymbPointer;
    QVector<QString> idList, conList;
    int idIndex(QString v);
    int conIndex(QString v);
    bool lastScanErrors;

};

#endif // SCANNER_H
