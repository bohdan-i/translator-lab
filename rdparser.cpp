#include "rdparser.h"

RDParser::RDParser()
{
}

QVector<QString> RDParser::parse()
{
    try{
        if(getToken()<0) throw new int;
        if(levelProg()<0) throw new int;
    }
    catch(...)
    {
        lastParseErrors = true;
    }
    return outputText;
}

int RDParser::levelProg()
{
    levelOpList();
    if(currLexeme.lexName != "#") // ������
    {
        lastParseErrors = true;
        outputText.append(QString("Invalid operator or end of file in row "
                                  + QString().number(currLexeme.row)));
        return -1;
    }

    if(getToken()<0)return 0;



//    currLexeme.clean();


    outputText.append(QString("Unexpected end of program in row ")
                          + QString().number(currLexeme.row-1) + '.');
    lastParseErrors = true;
    return -1;
}

int RDParser::levelOpList()
{
    int flag = levelOp();
    if (flag < 0) return -1;
//    if(getToken()<0)return -1;
    while(flag == 0) // 0 - �������� ����, ����� ";",
                        //���� 1 - ��� �� ��������, ������� �� ������� ����
    {

        // ������� ��� ��������
        if(currLexeme.lexName != ";") // ������
        {
            lastParseErrors = true;
            outputText.append(QString("Semicolon needed in row ") /////ANGER!!!!!
                              + QString().number(currLexeme.row)
                              + " Invalid operator. " + currLexeme.lexName);
            while(currLexeme.lexName != ";" || currLexeme.lexName != "#")
                if(getToken()<0)return -1;
            if(currLexeme.lexName == ";") if(getToken()<0)return -1;
            // � ���� �� #, �� �� ����� - ��� ��������� �������� ������ �� �����
            // � ���������� �� ������� ����
        }
        else if(getToken()<0)return -1; // ��� � �������, ������� ��������� �������, ���� ������ �� �����
        if (currLexeme.lexName == "#") return 0;
        flag = levelOp();
        if (flag<0) return -1;
    }
    return 0;
}

int RDParser::levelOp()
{
    try
    {
    switch(currLexeme.code)
    {
    case 29: // �������������:      <��> = <���>
    {
        if (!idDeclared[currLexeme.ind])
        {
            outputText.append(QString("Row ") + QString().number(currLexeme.row)
                              +" Identifier " + currLexeme.lexName
                              + " is not declared.");
            lastParseErrors = true;
        }
        if(getToken()<0)return -1;
        if (currLexeme.lexName != "=")
        {outputText.append(QString("Row ") + QString().number(currLexeme.row) + ": "
                               +"Should be \'=\' after identifier in assignment."); return -1;}
//            throw InvalidSymbolInOperator("Should be \'=\' after identifier in assignment.");
        if(getToken()<0)return -1;
        if(levelExpr()<0)return -1;// ���� � ��� ����� ������, ����� �� �� ���������� � ������ �������
        return 0;
    }
    case 2: // read:        read(<������ ��>)
    {
        declaring = false;
        if(getToken()<0)return -1;
        if (currLexeme.lexName != "(")
            {outputText.append(QString("Row ") + QString().number(currLexeme.row) + ": "
                               +"Should be \'(\' after \'read\'."); return -1;}
//            throw InvalidSymbolInOperator("Should be \'(\' after \'read\'.");
        if(getToken()<0)return -1;
        if(levelIdList()<0)return -1;
        if (currLexeme.lexName != ")")
            {outputText.append(QString("Row ") + QString().number(currLexeme.row) + ": "
                               +"Should be \')\' here."); return -1;}
//            throw InvalidSymbolInOperator("Should be \')\' here.");
        if(getToken()<0)return -1;
        return 0;
    }
    case 3: // write:       write(<������ ��>)
    {
        declaring = false;
        if(getToken()<0)return -1;
        if (currLexeme.lexName != "(")
            {outputText.append(QString("Row ") + QString().number(currLexeme.row) + ": "
                               +"Should be \'(\' after \'write\'."); return -1;}
//            throw InvalidSymbolInOperator("Should be \'(\' after \'write\'.");
        if(getToken()<0)return -1;
        if(levelIdList()<0)return -1;
        if (currLexeme.lexName != ")")
            {outputText.append(QString("Row ") + QString().number(currLexeme.row) + ": "
                               +"Should be \')\' here."); return -1;}
//            throw InvalidSymbolInOperator("Should be \')\' here.");
        if(getToken()<0)return -1;
        return 0;
    }
    case 6: // if:       if <���.���> then <��> else <��>
    {
        if(getToken()<0)return -1;
        if(levelLogExpr()<0)return -1;
        if (currLexeme.lexName != "then")
            {outputText.append(QString("Row ") + QString().number(currLexeme.row) + ": "
                               +"Should be \'then\' here in if-expression."); return -1;}
//            throw InvalidSymbolInOperator("Should be \'then\' here in if-expression.");
        if(getToken()<0)return -1;
        int flag = levelOp();
        if(flag<0)return -1;
        if(flag >0)
        {outputText.append(QString("Row ") + QString().number(currLexeme.row) + ": "
                           +"Should be operator here after \'then\' in if-expression."); return -1;}
        if (currLexeme.lexName != "else")
            {outputText.append(QString("Row ") + QString().number(currLexeme.row) + ": "
                               +"Should be \'else\' here in if-expression."); return -1;}
//            throw InvalidSymbolInOperator("Should be \'else\' here in if-expression.");
        if(getToken()<0)return -1;
        if(levelOp()<0)return -1;
        return 0;
    }
    case 4: // dowhile:      dowhile (<���.���>); <������ ��.> end
    {
        if(getToken()<0)return -1;
        if (currLexeme.lexName != "(")
            {outputText.append(QString("Row ") + QString().number(currLexeme.row) + ": "
                               +"Should be \'(\' after \'dowhile\'."); return -1;}
//            throw InvalidSymbolInOperator("Should be \'(\' after \'dowhile\'.");
        if(getToken()<0)return -1;
        if(levelLogExpr()<0)return -1;
        if (currLexeme.lexName != ")")
            {outputText.append(QString("Row ") + QString().number(currLexeme.row) + ": "
                               +"Should be \')\' here."); return -1;}
//            throw InvalidSymbolInOperator("Should be \')\' here.");
        if(getToken()<0)return -1;
        if (currLexeme.lexName != ";")
            {outputText.append(QString("Row ") + QString().number(currLexeme.row) + ": "
                               +"Should be \';\' here."); return -1;}
//            throw InvalidSymbolInOperator("Should be \';\' here.");
        if(getToken()<0)return -1;
        if(levelOpList()<0)return -1;
        if (currLexeme.lexName != "end")
            {outputText.append(QString("Row ") + QString().number(currLexeme.row) + ": "
                               +"Maybe \'end\' should be here."); return -1;}
//            throw InvalidSymbolInOperator("Maybe \'end\' should be here here.");
        if(getToken()<0)return -1;
        return 0;
    }
    case 1: // float:        float <������ ��>
    {
        declaring = true;
        if(getToken()<0)return -1;
        if(levelIdList()<0)return -1;
//        if(getToken()<0)return -1;
        return 0;
    }
    default:
        return 1;
    }
    }
    catch (InvalidSymbolInOperator& expt)
    {
        lastParseErrors = true;
        outputText.append(expt.what()); // ����� ������
        while(currLexeme.lexName != ";")// ������������ �� ������ ���������� ���������
            if(getToken()<0)return -1;
    }
    return 0;
}

int RDParser::levelIdList()
{
    if(currLexeme.code != 29)
    {outputText.append(QString("Row ") + QString().number(currLexeme.row) + ": "
                       +"Should be identifier here."); return -1;}
    else if(idDeclared[currLexeme.ind]&&declaring)
    {
        outputText.append(QString("Row ") + QString().number(currLexeme.row)
                          +" Identifier " + currLexeme.lexName
                          + " is already declared.");
        lastParseErrors = true;
    }
    else if((!idDeclared[currLexeme.ind])&&declaring)
    {
        idDeclared[currLexeme.ind] = true;
    }
    if ((!idDeclared[currLexeme.ind])&& (!declaring))
    {
        outputText.append(QString("Row ") + QString().number(currLexeme.row)
                          +" Identifier " + currLexeme.lexName
                          + " is not declared.");
        lastParseErrors = true;
    }
//        throw InvalidSymbolInOperator("Should be identifier here.");
    if(getToken()<0)return -1;
    while(currLexeme.lexName == ",")
    {
        if(getToken()<0)return -1;
        if(currLexeme.code != 29)
            {outputText.append(QString("Row ") + QString().number(currLexeme.row) + ": "
                               +"Should be identifier here."); return -1;}
//            throw InvalidSymbolInOperator("Should be identifier here.");
        else
        if(idDeclared[currLexeme.ind]&&declaring)
        {
            outputText.append(QString("Row ") + QString().number(currLexeme.row)
                              +" Identifier " + currLexeme.lexName
                              + " is already declared.");
            lastParseErrors = true;
        }
        else if((!idDeclared[currLexeme.ind])&&declaring)
        {
            idDeclared[currLexeme.ind] = true;
        }
        if ((!idDeclared[currLexeme.ind])&& (!declaring))
        {
            outputText.append(QString("Row ") + QString().number(currLexeme.row)
                              +" Identifier " + currLexeme.lexName
                              + " is not declared.");
            lastParseErrors = true;
        }
        if(getToken()<0)return -1;
    }
    return 0;
}

int RDParser::levelExpr() // ���������, ������� �������
{
    //? ����� �� ���? ������� �����
    bool minusFlag = false;
    if (currLexeme.lexName == "-")
    {
        minusFlag = true;
        if(getToken()<0)return -1;
    }
    //?
    if(levelTerm()<0)return -1;
    while(currLexeme.lexName == "+" || currLexeme.lexName == "-")
    {
        if(getToken()<0)return -1;
        if(levelTerm()<0)return -1;
    }
    return 0;
}

int RDParser::levelTerm()
{
    if(levelMult()<0)return -1;
    while(currLexeme.lexName == "*" || currLexeme.lexName == "/")
    {
        if(getToken()<0)return -1;
        if(levelMult()<0)return -1;
    }
    return 0;
}

int RDParser::levelMult()
{
    switch(currLexeme.code)
    {
    case 29: // id
    {
        if (!idDeclared[currLexeme.ind])
        {
            outputText.append(QString("Row ") + QString().number(currLexeme.row)
                              +" Identifier " + currLexeme.lexName
                              + " is not declared.");
            lastParseErrors = true;
        }
        if(getToken()<0)return -1;
        return 0;
    }
    case 30: // const
    {
        if(getToken()<0)return -1;
        return 0;
    }
    case 19: // (       (<���>)
    {
        if(getToken()<0)return -1;
        if(levelExpr()<0)return -1;
        if(currLexeme.lexName != ")")
            {outputText.append(QString("Row ") + QString().number(currLexeme.row) + ": "
                               +"Error in expression, right braket needed."); return -1;}
//            throw InvalidSymbolInOperator("Error in expression, right braket needed.");
        if(getToken()<0)return -1;
        return 0;
    }
    default:
    {
        {outputText.append(QString("Row ") + QString().number(currLexeme.row) + ": "
                           +"Error in expression."); return -1;}
//        throw InvalidSymbolInOperator("Error in expression.");
    }
    }
}

int RDParser::levelLogExpr() // ���������� ���������, ������� �������
{
    if(levelLogTerm()<0)return -1;
    while(currLexeme.lexName == "or")
    {
        if(getToken()<0)return -1;
        if(levelLogTerm()<0)return -1;
    }
    return 0;
}

int RDParser::levelLogTerm()
{
    if(levelLogMult()<0)return -1;
    while(currLexeme.lexName == "and")
    {
        if(getToken()<0)return -1;
        if(levelLogMult()<0)return -1;
    }
    return 0;
}

int RDParser::levelLogMult()
{
    switch(currLexeme.code)
    {
    case 11: // not     not <���.����>
    {
        if(getToken()<0)return -1;
        if(levelLogMult()<0)return -1;
        return 0;
    }
    case 21: // [       [<���.���>]
    {
        if(getToken()<0)return -1;
        if(levelLogExpr()<0)return -1;
        if(currLexeme.lexName != "]")
            {outputText.append(QString("Row ") + QString().number(currLexeme.row) + ": "
                               +"Error in logical expression, right square braket needed."); return -1;}
//            throw InvalidSymbolInOperator("Error in logical expression, right square braket needed.");
        if(getToken()<0)return -1;
        return 0;
    }

    default: //         <���> ( > | < | == | >= | <= | != ) <���>
    {
        if(levelExpr()<0)return -1;
        if (currLexeme.code < 23 || currLexeme.code > 28) // �� ����� ���������, ������
            {outputText.append(QString("Row ") + QString().number(currLexeme.row) + ": "
                               +"Need < > == sign here."); return -1;}
//            throw InvalidSymbolInOperator("Need < > == sign here.");
        if(getToken()<0)return -1;
        if(levelExpr()<0)return -1;
        return 0;
    }
    }
}
