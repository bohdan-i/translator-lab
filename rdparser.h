#ifndef RDPARSER_H
#define RDPARSER_H
#include "abstractparser.h"

// recursive descending parser - рекурсивный нисходящий синтаксический анализатор

class RDParser : public AbstractParser
{
public:
    RDParser();
    QVector<QString> parse();
private:
    int levelProg();
    int levelOpList();
    int levelOp();
    int levelIdList();
    int levelExpr();
    int levelTerm();
    int levelMult();
    int levelLogExpr();
    int levelLogTerm();
    int levelLogMult();
};

#endif // RDPARSER_H
