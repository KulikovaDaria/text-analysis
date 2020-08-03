#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include "textanalysis.h"
#include "ngramms.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(openText()));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(startAnalysis()));
    connect(ui->pushButton_3, SIGNAL(clicked(bool)), this, SLOT(reset()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::openText() {
    QString fileName = QFileDialog::getOpenFileName(this, "Choose a file", "C:/", "*.txt");
    ui->listWidget->addItem(fileName);
    fileNames.push(fileName);
}

void MainWindow::startAnalysis() {
    TextAnalysis* anl = new TextAnalysis(this);
    anl->setFileNames(fileNames);
    anl->show();
    this->hide();
    ui->listWidget->clear();
}

void MainWindow::reset() {
    ui->listWidget->clear();
    fileNames.clear();
}

void MainWindow::on_pushButton_4_clicked() {
    NGramms* ng = new NGramms(this);
    ng->setFileNames(fileNames);
    ng->show();
    this->hide();
    ui->listWidget->clear();
}
