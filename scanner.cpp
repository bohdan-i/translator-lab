#include "scanner.h"

Scanner::Scanner()
{
    lastScanErrors = false;
}

bool isor(char c); // �������� �� ������ �������������� ������������
int lexemeCode(QString c);

QVector<Lexeme> Scanner::scan(QString text)
{
    QVector<Lexeme> result;
    currSymbPointer = 0;
    currSymb = text[currSymbPointer].toAscii(); // ������� ������
    int rowNumber = 1;

    STATE_1:
    // ��, � ��� ������ ����� ������ goto, ��������. ������ ����� - ��������� �� ���������.
    // ��������� - � ����� � ��������. ����� ����� ���
    // STATE_#, # - ����� ��������� �� ���������.

    while(currSymb == ' ' || currSymb == '\t')
        // �������� �������� ������� � ���������
    {
        currSymbPointer++;
        currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������
    }

    if (currSymb == '\n')
        // ���������� ������� �� ����� ������
    {
        rowNumber++; // ������� �� ��������� ������
        currSymbPointer++;
        currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������
        goto STATE_1;
    }

    currentLexeme.clean();
    if(currSymb == 0) goto  STATE_EXIT; // ��� ������� - ����� ����������

    if (isalpha(currSymb))
    {
        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        goto STATE_2;
    }
    else if (isdigit(currSymb))
    {
        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        goto STATE_3;
    }
    else if (currSymb == '.')
    {
        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        goto STATE_4;
    }
    else if (isor(currSymb))
    {
        currentLexeme.lexName += currSymb;
        currentLexeme.code = lexemeCode(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme); // �������� ������� � ������ ������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        goto STATE_1; // ������������ � ��������� ���� ��� ������� ��� ����� �������
    }
    else if (currSymb == '=')
    {
        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        goto STATE_9;
    }
    else if (currSymb == '>')
    {
        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        goto STATE_10;
    }
    else if (currSymb == '<')
    {
        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        goto STATE_11;
    }
    else if (currSymb == '!')
    {
        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        goto STATE_12;
    }
    else // ����������������� ������ - ������
    {
        errorStringsVector.append( (QString) "Invalid symbol \'"
                                   + currSymb + "\' in row "
                                   + QString().number(rowNumber)); // ������ ����� ������ ������!
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������
        lastScanErrors = true;
        goto STATE_1; // ������������ � ��������� ���� ��� ������� ����� �������
    }


    STATE_2:
    if (isalnum(currSymb))
    {
        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        goto STATE_2;
    }
    else
    {
        currentLexeme.code = lexemeCode(currentLexeme.lexName);
        if (currentLexeme.code == -1) // ��� ��� ����������, ����� - ������� �� �������
        {
            currentLexeme.code = lexemeCode("_id");
            currentLexeme.ind = idIndex(currentLexeme.lexName);
        }

        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // ������ ��� ��������, ��� ��� �������� �� �����
        goto STATE_1; // ������������ � ��������� ���� ��� ������� ����� �������
    }


    STATE_3:
    if (isdigit(currSymb))
    {
        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        goto STATE_3;
    }
    else if (currSymb == 'e' || currSymb == 'E')
    {
        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        goto STATE_6;
    }
    else if (currSymb == '.')
    {
        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        goto STATE_5;
    }
    else // ��� ���������
    {
        currentLexeme.code = lexemeCode("_con");
        currentLexeme.ind = conIndex(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // ��������� ������ ��� �������
        goto STATE_1; // ������������ � ��������� ���� ��� ������� ����� �������
    }

    STATE_4:
    if(isdigit(currSymb))
    {
        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        goto STATE_5;
    }
    else // ����������������� ������ - ������
    {
        errorStringsVector.append( (QString) "Invalid symbol \'"
                                   + currSymb + "\' in row "
                                   + QString().number(rowNumber)); // ������ ����� ������ ������!
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������
        lastScanErrors = true;
        goto STATE_1; // ������������ � ��������� ���� ��� ������� ����� �������
    }

    STATE_5:
    if(isdigit(currSymb))
    {
        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        goto STATE_5;
    }
    else if (currSymb == 'e' || currSymb == 'E')
    {
        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        goto STATE_6;
    }
    else // ��� ���������
    {
        currentLexeme.code = lexemeCode("_con");
        currentLexeme.ind = conIndex(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // ��������� ������ ��� �������
        goto STATE_1; // ������������ � ��������� ���� ��� ������� ����� �������
    }

    STATE_6:
    if(currSymb == '+' || currSymb == '-')
    {
        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        goto STATE_7;
    }
    else if(isdigit(currSymb))
    {
        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        goto STATE_8;
    }
    else // ����������������� ������ - ������
    {
        errorStringsVector.append( (QString) "Invalid symbol \'"
                                   + currSymb + "\' in row "
                                   + QString().number(rowNumber)); // ������ ����� ������ ������!
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������
        lastScanErrors = true;
        goto STATE_1; // ������������ � ��������� ���� ��� ������� ����� �������
    }

    STATE_7:
    if(isdigit(currSymb))
    {

        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        goto STATE_8;
    }
    else // ����������������� ������ - ������
    {
        errorStringsVector.append( (QString) "Invalid symbol \'"
                                   + currSymb + "\' in row "
                                   + QString().number(rowNumber)); // ������ ����� ������ ������!
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������
        lastScanErrors = true;
        goto STATE_1; // ������������ � ��������� ���� ��� ������� ����� �������
    }

    STATE_8:
    if(isdigit(currSymb))
    {

        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        goto STATE_8;
    }
    else // ��� ���������
    {
        currentLexeme.code = lexemeCode("_con");
        currentLexeme.ind = conIndex(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // ��������� ������ ��� �������
        goto STATE_1; // ������������ � ��������� ���� ��� ������� ����� �������
    }

    STATE_9:
    if (currSymb == '=')
        // ��� ������� ==
    {
        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        currentLexeme.code = lexemeCode(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // ��������� ������ ��� �������
        goto STATE_1; // ������������ � ��������� ���� ��� ������� ����� �������
    }
    else
        // ��� ������� =
    {
        currentLexeme.code = lexemeCode(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // ��������� ������ ��� �������
        goto STATE_1; // ������������ � ��������� ���� ��� ������� ����� �������
    }

    STATE_10:
    if (currSymb == '=')
        // ��� ������� >=
    {
        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        currentLexeme.code = lexemeCode(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // ��������� ������ ��� �������
        goto STATE_1; // ������������ � ��������� ���� ��� ������� ����� �������
    }
    else
        // ��� ������� >
    {
        currentLexeme.code = lexemeCode(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // ��������� ������ ��� �������
        goto STATE_1; // ������������ � ��������� ���� ��� ������� ����� �������
    }

    STATE_11:
    if (currSymb == '=')
        // ��� ������� <=
    {
        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        currentLexeme.code = lexemeCode(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // ��������� ������ ��� �������
        goto STATE_1; // ������������ � ��������� ���� ��� ������� ����� �������
    }
    else
        // ��� ������� <
    {
        currentLexeme.code = lexemeCode(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // ��������� ������ ��� �������
        goto STATE_1; // ������������ � ��������� ���� ��� ������� ����� �������
    }

    STATE_12:
    if (currSymb == '=')
        // ��� ������� !=
    {
        currentLexeme.lexName += currSymb; // �������� ������ � ������� �������
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������

        currentLexeme.code = lexemeCode(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // ��������� ������ ��� �������
        goto STATE_1; // ������������ � ��������� ���� ��� ������� ����� �������
    }
    else // ����������������� ������ - ������
    {
        errorStringsVector.append( (QString) "Invalid symbol \'"
                                   + currSymb + "\' in row "
                                   + QString().number(rowNumber)); // ������ ����� ������ ������!
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // �������� ��������� ������ �� ������
        lastScanErrors = true;
        goto STATE_1; // ������������ � ��������� ���� ��� ������� ����� �������
    }

STATE_EXIT:
    return result;
}




// ������ �������



int Scanner::idIndex(QString v)
//��������� ���� �� ������������� � ������, ���� ��� - ���������
//���������� ������ ����� �������������� � ������
{
    for (int i = 0; i<idList.size(); i++)
        if (idList[i] == v) return i;
    idList.append(v);
    return idList.size()-1;
}

int Scanner::conIndex(QString v)
//��������� ���� �� ��������� � ������ ��������, ���� ��� - ���������
//���������� ������ ���� ��������� � ������
{
    for (int i = 0; i<conList.size(); i++)
        if (conList[i] == v) return i;
    conList.append(v);
    return conList.size()-1;
}

bool isor(char c)
// ���������, �������� �� ������ ������ �������������� ������������ or
{
    switch(c){
    case '+' :
    case '-' :
    case '*' :
    case '/' :
    case ';' :
    case ',' :
    case '(' :
    case ')' :
    case '[' :
    case ']' :
    case '#' :
        return true;
    default:
        return false;
    }
}

int lexemeCode(QString c)
// ���������� ��� ������� � ������� ������
{
    QString lexems[] = {"#", "float", "read", "write", "dowhile", "end", "if", "then",
                        "else", "or", "and", "not", ";", ",", "=", "+", "-", "*", "/",
                        "(", ")", "[", "]", ">", "<","==", ">=","<=","!=", "_id", "_con"};
    // _id & _con - ��� ����, ��� �� ������������ �������������� id � con
    int l = 30;
    for (int i = 0; i<=l; i++)
        if(c.compare(lexems[i]) == 0)
            return i;
    return -1;
}
