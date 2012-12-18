#include "auparser.h"

AuParser::AuParser()
{
    AuPAutomate automate("au.txt");
}

int AuPState::doMove(Lexeme l)
{

}

AuPAutomate::AuPAutomate(QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QDebug(&file.errorString());
    }
    QTextStream au(&file);
    QString line = au.readLine();
    while (line != "~")
    {
        if (line.isEmpty()) {line = au.readLine(); continue;} //���� ������ ������ - ������� � ���������
        AuPState a(line.toInt());
        do
        {
            line = au.readLine();
            a.addRule(AuPRule(line));
        } while (line[0] != '$');//$ - ��������� ������� � ���������, �������� "����� ������ ������"
        line = au.readLine();
        addState(a);
    }
    file.close();
}

AuPState AuPAutomate::getState(int)
{
}

// �������� ��������� � �������
void AuPAutomate::addState(AuPState st)
{
    states.append(st);
}

AuPState::AuPState() // ����� ��� �������
{
}

// �������� ������� � ���������
void AuPState::addRule(AuPRule r)
{
    rules.append(r);
}

AuPRule::AuPRule(QString s)
{
    QStringList sl(s.split('|'));
    lexName = sl[0];
    beta = sl[1].toInt();
    stack = sl[2].toInt();
}

AuPRule::AuPRule() // ����� ��� �������
{
}

QVector<QString> AuParser::parse()
{
    // ������ � ������ ��������� - � QVector<QString> outputText

}
