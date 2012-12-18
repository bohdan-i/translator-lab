#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->setFixedWidth(470);
    ui->scrollArea_2->setFixedWidth(470);
    this->update();
    this->setFixedWidth(this->width()+100);
    ui->errorLog->setFixedHeight(80);
    connect(ui->action_Open,SIGNAL(triggered()), this, SLOT(fileOpen()));
    connect(ui->actionE_xit, SIGNAL(triggered()), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::load(const QString &f) // �������� ���������� �����
 {
     if (!QFile::exists(f))
         return false;
     QFile file(f);
     if (!file.open(QFile::ReadOnly))
         return false;

     QByteArray data = file.readAll();
     QTextCodec *codec = Qt::codecForHtml(data);
     QString str = codec->toUnicode(data);
     if (Qt::mightBeRichText(str)) {
         ui->textEdit->setHtml(str);
     } else {
         str = QString::fromLocal8Bit(data);
         ui->textEdit->setPlainText(str);
     }

//     setCurrentFileName(f);
     return true;
 }

void MainWindow::fileOpen() // ���� ��� �������� �����
 {
     QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                               QString(), tr("BO Files (*.bo *.txt);;All Files (*)"));
     if (!fn.isEmpty())
         load(fn);
 }

void MainWindow::on_pushButton_clicked() // ������!111
{
    Scanner textScanner;
    QVector<Lexeme> mainTableData; // ������� � ��������� � ������� ��������� �� � ����
    QString program = ui->textEdit->toPlainText(); // ��������� ����� ��������� � program
    QVector<QString> errors; // ��������� �� �������
    ui->errorLog->appendPlainText(QString("-------------------\n")
                                  + "Scan started at " + T.currentTime().toString());
    mainTableData = textScanner.scan(program); // ���������� ����. ������� � mainTableData
    errors = textScanner.getErrorStringsVector(); // �������� ��������� �� ������� ������ ����. �����������

    foreach(QString a , errors)
        ui->errorLog->appendPlainText(a); // ������� ��������� ���� � ���� ������ ���������

    // ������ ������ ������ ��� ���������� ������
    // ������� �������
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(mainTableData.size());
    // ������� ��������
    ui->tableWidget_const->clearContents();
    ui->tableWidget_const->setRowCount(textScanner.conAmount());
    // ������� ���������������
    ui->tableWidget_id->clearContents();
    ui->tableWidget_id->setRowCount(textScanner.idAmount());

    int temp=0;
    foreach(Lexeme a, mainTableData) // ��������� ������ �� ������� ������ � �������� �������
    {
        ui->tableWidget->setItem(temp,0,new QTableWidgetItem(QString().number(a.row)));
        ui->tableWidget->setItem(temp,1,new QTableWidgetItem(a.lexName));
        ui->tableWidget->setItem(temp,2,new QTableWidgetItem(QString().number(a.code)));
        if (a.code == 29 || a.code == 30)
        ui->tableWidget->setItem(temp,3,new QTableWidgetItem(QString().number(a.ind)));
        temp++;
    }
    QVector<QString> tempStrVector;
    // � ������� ��������
    tempStrVector = textScanner.getConList();
    for(int i = 0; i<textScanner.conAmount(); i++)
    {
        ui->tableWidget_const->setItem(i,0,new QTableWidgetItem(tempStrVector[i]));
        ui->tableWidget_const->setItem(i,1,new QTableWidgetItem(QString().number(i)));
    }

    // � ������� ���������������
    tempStrVector = textScanner.getIdList();
    for(int i = 0; i<textScanner.idAmount(); i++)
    {
        ui->tableWidget_id->setItem(i,0,new QTableWidgetItem(tempStrVector[i]));
        ui->tableWidget_id->setItem(i,1,new QTableWidgetItem(QString().number(i)));
    }
    if(textScanner.lastScanHasErrors())
    {
        ui->errorLog->appendHtml("<b>Unsuccessfull scan.<\\b>");
        return;
    }
    else
        ui->errorLog->appendHtml("<b>Scan finished successfully.<\\b>");

    // ������ �������������� ������
//    AbstractParser *parser = new RDParser();
    AbstractParser *parser = new AuParser();
    parser->load(mainTableData);
    parser->parse();
    errors = parser->getTextOutput();
    foreach(QString a , errors)
        ui->errorLog->appendPlainText(a); // ������� ��������� ���� � ���� ������ ���������
    if(parser->errorsHappened())
    {
        ui->errorLog->appendHtml("<b>Unsuccessfull parse.<\\b>");
        return;
    }
    else
        ui->errorLog->appendHtml("<b>Parse finished successfully.<\\b>");
    return;
}

void MainWindow::on_pushButton_clean_clicked()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget_const->clearContents();
    ui->tableWidget_const->setRowCount(0);
    ui->tableWidget_id->clearContents();
    ui->tableWidget_id->setRowCount(0);
}

void MainWindow::on_pushButton_exit_clicked()
{
    exit(0);
}
