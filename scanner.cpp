#include "scanner.h"

Scanner::Scanner()
{
    lastScanErrors = false;
}

bool isor(char c); // является ли символ односимвольным разделителем
int lexemeCode(QString c);

QVector<Lexeme> Scanner::scan(QString text)
{
    QVector<Lexeme> result;
    currSymbPointer = 0;
    currSymb = text[currSymbPointer].toAscii(); // текущий символ
    int rowNumber = 1;

    STATE_1:
    // да, и тут дальше будет дофига goto, готовься. Каждая метка - состояние на диаграмме.
    // диаграмма - в папке с проектом. Метки имеют вид
    // STATE_#, # - номер состояния на диаграмме.

    while(currSymb == ' ' || currSymb == '\t')
        // опускаем ненужные пробелы и табуляции
    {
        currSymbPointer++;
        currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста
    }

    if (currSymb == '\n')
        // пропускаем переход на новую строку
    {
        rowNumber++; // перешли на следующую строку
        currSymbPointer++;
        currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста
        goto STATE_1;
    }

    currentLexeme.clean();
    if(currSymb == 0) goto  STATE_EXIT; // нет символа - текст просмотрен

    if (isalpha(currSymb))
    {
        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        goto STATE_2;
    }
    else if (isdigit(currSymb))
    {
        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        goto STATE_3;
    }
    else if (currSymb == '.')
    {
        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        goto STATE_4;
    }
    else if (isor(currSymb))
    {
        currentLexeme.lexName += currSymb;
        currentLexeme.code = lexemeCode(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme); // добавить лексему в вектор лексем
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        goto STATE_1; // возвращаемся в состояние один для разбора уже новой лексемы
    }
    else if (currSymb == '=')
    {
        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        goto STATE_9;
    }
    else if (currSymb == '>')
    {
        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        goto STATE_10;
    }
    else if (currSymb == '<')
    {
        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        goto STATE_11;
    }
    else if (currSymb == '!')
    {
        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        goto STATE_12;
    }
    else // несоответствующий символ - ошибка
    {
        errorStringsVector.append( (QString) "Invalid symbol \'"
                                   + currSymb + "\' in row "
                                   + QString().number(rowNumber)); // добавь вывод номера строки!
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста
        lastScanErrors = true;
        goto STATE_1; // возвращаемся в состояние один для разбора новой лексемы
    }


    STATE_2:
    if (isalnum(currSymb))
    {
        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        goto STATE_2;
    }
    else
    {
        currentLexeme.code = lexemeCode(currentLexeme.lexName);
        if (currentLexeme.code == -1) // это имя переменной, иначе - лексема из таблицы
        {
            currentLexeme.code = lexemeCode("_id");
            currentLexeme.ind = idIndex(currentLexeme.lexName);
        }

        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // символ уже получили, еще раз получать не нужно
        goto STATE_1; // возвращаемся в состояние один для разбора новой лексемы
    }


    STATE_3:
    if (isdigit(currSymb))
    {
        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        goto STATE_3;
    }
    else if (currSymb == 'e' || currSymb == 'E')
    {
        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        goto STATE_6;
    }
    else if (currSymb == '.')
    {
        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        goto STATE_5;
    }
    else // это константа
    {
        currentLexeme.code = lexemeCode("_con");
        currentLexeme.ind = conIndex(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // следующий символ уже получен
        goto STATE_1; // возвращаемся в состояние один для разбора новой лексемы
    }

    STATE_4:
    if(isdigit(currSymb))
    {
        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        goto STATE_5;
    }
    else // несоответствующий символ - ошибка
    {
        errorStringsVector.append( (QString) "Invalid symbol \'"
                                   + currSymb + "\' in row "
                                   + QString().number(rowNumber)); // добавь вывод номера строки!
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста
        lastScanErrors = true;
        goto STATE_1; // возвращаемся в состояние один для разбора новой лексемы
    }

    STATE_5:
    if(isdigit(currSymb))
    {
        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        goto STATE_5;
    }
    else if (currSymb == 'e' || currSymb == 'E')
    {
        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        goto STATE_6;
    }
    else // это константа
    {
        currentLexeme.code = lexemeCode("_con");
        currentLexeme.ind = conIndex(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // следующий символ уже получен
        goto STATE_1; // возвращаемся в состояние один для разбора новой лексемы
    }

    STATE_6:
    if(currSymb == '+' || currSymb == '-')
    {
        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        goto STATE_7;
    }
    else if(isdigit(currSymb))
    {
        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        goto STATE_8;
    }
    else // несоответствующий символ - ошибка
    {
        errorStringsVector.append( (QString) "Invalid symbol \'"
                                   + currSymb + "\' in row "
                                   + QString().number(rowNumber)); // добавь вывод номера строки!
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста
        lastScanErrors = true;
        goto STATE_1; // возвращаемся в состояние один для разбора новой лексемы
    }

    STATE_7:
    if(isdigit(currSymb))
    {

        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        goto STATE_8;
    }
    else // несоответствующий символ - ошибка
    {
        errorStringsVector.append( (QString) "Invalid symbol \'"
                                   + currSymb + "\' in row "
                                   + QString().number(rowNumber)); // добавь вывод номера строки!
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста
        lastScanErrors = true;
        goto STATE_1; // возвращаемся в состояние один для разбора новой лексемы
    }

    STATE_8:
    if(isdigit(currSymb))
    {

        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        goto STATE_8;
    }
    else // это константа
    {
        currentLexeme.code = lexemeCode("_con");
        currentLexeme.ind = conIndex(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // следующий символ уже получен
        goto STATE_1; // возвращаемся в состояние один для разбора новой лексемы
    }

    STATE_9:
    if (currSymb == '=')
        // это лексема ==
    {
        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        currentLexeme.code = lexemeCode(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // следующий символ уже получен
        goto STATE_1; // возвращаемся в состояние один для разбора новой лексемы
    }
    else
        // это лексема =
    {
        currentLexeme.code = lexemeCode(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // следующий символ уже получен
        goto STATE_1; // возвращаемся в состояние один для разбора новой лексемы
    }

    STATE_10:
    if (currSymb == '=')
        // это лексема >=
    {
        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        currentLexeme.code = lexemeCode(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // следующий символ уже получен
        goto STATE_1; // возвращаемся в состояние один для разбора новой лексемы
    }
    else
        // это лексема >
    {
        currentLexeme.code = lexemeCode(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // следующий символ уже получен
        goto STATE_1; // возвращаемся в состояние один для разбора новой лексемы
    }

    STATE_11:
    if (currSymb == '=')
        // это лексема <=
    {
        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        currentLexeme.code = lexemeCode(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // следующий символ уже получен
        goto STATE_1; // возвращаемся в состояние один для разбора новой лексемы
    }
    else
        // это лексема <
    {
        currentLexeme.code = lexemeCode(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // следующий символ уже получен
        goto STATE_1; // возвращаемся в состояние один для разбора новой лексемы
    }

    STATE_12:
    if (currSymb == '=')
        // это лексема !=
    {
        currentLexeme.lexName += currSymb; // добавить символ в текущую лексему
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста

        currentLexeme.code = lexemeCode(currentLexeme.lexName);
        currentLexeme.row = rowNumber;
        result.append(currentLexeme);
        // следующий символ уже получен
        goto STATE_1; // возвращаемся в состояние один для разбора новой лексемы
    }
    else // несоответствующий символ - ошибка
    {
        errorStringsVector.append( (QString) "Invalid symbol \'"
                                   + currSymb + "\' in row "
                                   + QString().number(rowNumber)); // добавь вывод номера строки!
        currSymbPointer++; currSymb = text[currSymbPointer].toAscii(); // получить следующий символ из текста
        lastScanErrors = true;
        goto STATE_1; // возвращаемся в состояние один для разбора новой лексемы
    }

STATE_EXIT:
    return result;
}




// ДРУГИЕ ФУНКЦИИ



int Scanner::idIndex(QString v)
//проверяет есть ли идентификатор в списке, если нет - добавляет
//возвращает индекс этого идентификатора в списке
{
    for (int i = 0; i<idList.size(); i++)
        if (idList[i] == v) return i;
    idList.append(v);
    return idList.size()-1;
}

int Scanner::conIndex(QString v)
//проверяет есть ли константа в списке констант, если нет - добавляет
//возвращает индекс этой константы в списке
{
    for (int i = 0; i<conList.size(); i++)
        if (conList[i] == v) return i;
    conList.append(v);
    return conList.size()-1;
}

bool isor(char c)
// проверяет, является ли данный символ односимвольным разделителем or
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
// возвращает код лексемы в таблице лексем
{
    QString lexems[] = {"#", "float", "read", "write", "dowhile", "end", "if", "then",
                        "else", "or", "and", "not", ";", ",", "=", "+", "-", "*", "/",
                        "(", ")", "[", "]", ">", "<","==", ">=","<=","!=", "_id", "_con"};
    // _id & _con - для того, что бы существовали идентификаторы id и con
    int l = 30;
    for (int i = 0; i<=l; i++)
        if(c.compare(lexems[i]) == 0)
            return i;
    return -1;
}
