#include "ngramms.h"
#include "ui_ngramms.h"
#include "ngworker.h"
#include <QThread>
#include <QDebug>

NGramms::NGramms(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NGramms) {
    ui->setupUi(this);
}

NGramms::~NGramms() {
    delete ui;
}

void NGramms::on_start_clicked() {
    ui->tableWidget->clear();
    N = ui->spinBox->value();
    numFiles = fileNames->size();
    int numth = QThread::idealThreadCount();
    for (int i = 0; i < numth; ++i) {
        addThread();
    }
}

void NGramms::on_back_clicked() {
    parentWidget()->show();
    this->hide();
    ui->tableWidget->clear();
}

void NGramms::addThread() {
    NGWorker* worker = new NGWorker();
    QThread* thread = new QThread;
    worker->setFileNames(*fileNames);
    worker->setWords(words);
    worker->moveToThread(thread);
    worker->setNumFiles(numFiles);
    worker->setN(N);
    // При запуске потока будет вызван метод process
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finishedMyStem()), this, SLOT(finished()));
    connect(worker, SIGNAL(incProgressBar(double)), this, SLOT(incProgressBar(double)));
    // При завершении работы потока - вызове сигнала finished, сработает слот quit
    connect(worker, SIGNAL(finishedMyStem()), thread, SLOT(quit()));
    // worker пометит себя для удаления при окончании работы
    connect(worker, SIGNAL(finishedMyStem()), worker, SLOT(deleteLater()));
    // поток пометит себя для удаления при окончании работы
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void NGramms::finished() {
    cnt_th++;
    if(cnt_th == QThread::idealThreadCount()) {
        cnt_th = 0;
        NGWorker* worker = new NGWorker();
        QThread* thread = new QThread;
        worker->setWords(words);
        worker->setTable(table);
        worker->moveToThread(thread);
        // При запуске потока будет вызван метод process
        connect(thread, SIGNAL(started()), worker, SLOT(processTable()));
        connect(worker, SIGNAL(finishedTable()), this, SLOT(finishedTable()));
        connect(worker, SIGNAL(incProgressBar(double)), this, SLOT(incProgressBar(double)));
        // При завершении работы потока - вызове сигнала finished, сработает слот quit
        connect(worker, SIGNAL(finishedTable()), thread, SLOT(quit()));
        // worker пометит себя для удаления при окончании работы
        connect(worker, SIGNAL(finishedTable()), worker, SLOT(deleteLater()));
        // поток пометит себя для удаления при окончании работы
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        thread->start();
    }
}

void NGramms::finishedTable() {
    ui->tableWidget->setRowCount(table.size());
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("N-грамма"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Частота"));
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for (int i = 0; i < table.size(); ++i) {
        QString s = table[i].first;
        int num = table[i].second;
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(s));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(tr("%1").arg(num)));
    }
    fileNames->clear();
    words.clear();
    table.clear();

}

void NGramms::incProgressBar(double step) {
    progress += step;
    ui->progressBar->setValue(progress + 0.5);
}
