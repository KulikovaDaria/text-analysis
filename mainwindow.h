#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQueue>
#include <QString>
#include "safe_queue.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openText();
    void startAnalysis();
    void reset();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    SafeQueue<QString> fileNames;
};
#endif // MAINWINDOW_H
