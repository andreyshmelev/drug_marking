#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QStringList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int GenerateNumber();

    int GenerateNumber(int High, int Low);

private:

    QTimer * journalTimer;
    Ui::MainWindow *ui;
    QStringList * messages;

private slots:
    void addMessageToJournal();
};

#endif // MAINWINDOW_H
