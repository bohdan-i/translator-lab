#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QFileDialog>
#include <QTextCodec>
#include "scanner.h"
#include "rdparser.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    
private slots:
    void fileOpen();

    void on_pushButton_clicked();

    void on_pushButton_clean_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::MainWindow *ui;
    QTime T;

    bool load(const QString &f);
};

#endif // MAINWINDOW_H
