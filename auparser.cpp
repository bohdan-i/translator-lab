#include "auparser.h"

// Парсер
AuParser::AuParser()
{
    automate.setAutomate("au.txt");
    nextState = 1;
    initErrorMessagesList("errors.txt");
}

QVector<QString> AuParser::parse()
{
    // ошибки и другие сообщения - в QVector<QString> outputText
    try
    {
        getToken();
        while(doMove()) ;
    }
    catch (EndOfLexListReached e)
    {
        outputText.append("Unexpected end of file.");
    }
    return outputText;
}

bool AuParser::doMove() // уже приходим с лексемой
{
    currState = automate.getState(nextState);
//    getToken();
    currRule = currState.getRule(currLexeme);
    checkIdentifier();
    switch (currRule.beta)
    {
    case  0:    // Снять со стека. Если стек пуст - выйти.
    {
        if(stack.isEmpty()) return false;
        nextState = stack.pop();
        break;
    }
    case -1:    // Ошибка. В поле stack лежит код ошибки
    {
        lastParseErrors = true;
        outputText.append("Error in row " + QString().number(currLexeme.row)
                          + ": " + errorMessage[currRule.stack]);
        while(currLexeme.lexName != ";") getToken();
        nextState = 2;
        stack.clear();
        break;
    }
    default:    // Перейти к состоянию beta
        if (currRule.stack != 0) stack.push(currRule.stack); // 0 = не ложить в стек ничего
        nextState = currRule.beta;
        break;
    }
    if (currRule.lexName != "$") getToken();
    return true;
}

void AuParser::initErrorMessagesList(QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QDebug(&file.errorString());
    }
    QTextStream au(&file);
    QString line = au.readLine();
    while (line != "~") //файл с ошибками заканчивается ~ с новой строки.
    {
        if (line.isEmpty()) {line = au.readLine(); continue;} //если строка пустая - перейти к следующей
        errorMessage.append(line);
        line = au.readLine();
    }
    file.close();
}

void AuParser::checkIdentifier()    // Проверка идентификаторов
{
    if (currState.alpha == 23) declaring = true;
    else declaring = false;
    if(currLexeme.code == 29 && !(currState.alpha == 1 || currState.alpha == 3))
    {
        if(idDeclared[currLexeme.ind]&&declaring)           // Объявлена и объявляется
        {
            outputText.append(QString("Row ") + QString().number(currLexeme.row)
                              +" Identifier " + currLexeme.lexName
                              + " is already declared.");
            lastParseErrors = true;
        }
        else if((!idDeclared[currLexeme.ind])&&declaring)   // Необъявлена и объявляется
        {
            idDeclared[currLexeme.ind] = true;
        }
        if ((!idDeclared[currLexeme.ind])&& (!declaring))   // Необъявлена и используется
        {
            outputText.append(QString("Row ") + QString().number(currLexeme.row)
                              +" Identifier " + currLexeme.lexName
                              + " is not declared.");
            lastParseErrors = true;
        }
    }
}



// Автомат


AuPAutomate::AuPAutomate(QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QDebug(&file.errorString());
    }
    QTextStream au(&file);
    QString line = au.readLine();
    while (line != "~") //файл с автоматом заканчивается ~ с новой строки.
    {
        if (line.isEmpty()) {line = au.readLine(); continue;} //если строка пустая - перейти к следующей
        AuPState a(line.toInt());
        do
        {
            line = au.readLine();
            a.addRule(AuPRule(line));
        } while (line[0] != '$');//$ - последнее правило в состоянии, означает "любой другой символ"
        line = au.readLine();
        addState(a);
    }
    file.close();
}

AuPAutomate::AuPAutomate()
{
}

AuPState AuPAutomate::getState(int st)
{
    foreach(AuPState a, states)
        if(a.alpha == st) return a;
    return NULL;
}

// Добавить состояние в автомат
void AuPAutomate::addState(AuPState st)
{
    states.append(st);
}

void AuPAutomate::setAutomate(QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QDebug(&file.errorString());
    }
    QTextStream au(&file);
    QString line = au.readLine();
    while (line != "~") //файл с автоматом заканчивается ~ с новой строки.
    {
        if (line.isEmpty()) {line = au.readLine(); continue;} //если строка пустая - перейти к следующей
        AuPState a(line.toInt());
        do
        {
            line = au.readLine();
            a.addRule(AuPRule(line));
        } while (line[0] != '$');//$ - последнее правило в состоянии, означает "любой другой символ"
        line = au.readLine();
        addState(a);
    }
    file.close();
}



// Состояние
AuPState::AuPState() // чисто для вектора
{
}

// Добавить правило в состояние
void AuPState::addRule(AuPRule r)
{
    rules.append(r);
}

AuPRule AuPState::getRule(Lexeme l)
{
    QString lex;
    if (l.code == 29) lex = "id";
    else if (l.code == 30) lex = "con";
    else lex = l.lexName;
    for (int i = 0; i<rules.size()-1;i++)
    {
        if (lex == rules[i].lexName)
            return rules[i];
    }
    return rules.last(); // Вернуть последнее правило с $ - любой символ
}

//int AuPState::doMove(Lexeme l)
//{
//    AuPRule rule = getRule(l);
//}



// Правило
AuPRule::AuPRule(QString s)
{
    QStringList sl(s.split('|'));
    lexName = sl[0];
    beta = sl[1].toInt();
    stack = sl[2].toInt();
}

AuPRule::AuPRule() // чисто для вектора
{
}

