#include <QProcess>
#include <QFile>
#include <QThread>
#include <QFuture>
#include <QString>
#include <vector>
#include <string>
#include "textanalysis.h"
#include "ui_textanalysis.h"
#include "taworker.h"

TextAnalysis::TextAnalysis(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextAnalysis) {
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(start()));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(back()));
}

TextAnalysis::~TextAnalysis() {
    delete ui;
}

void TextAnalysis::back() {
    parentWidget()->show();
    this->hide();
    ui->tableWidget->clear();
}

void TextAnalysis::finished() {
    cnt_th++;
    if(cnt_th == QThread::idealThreadCount()) {
        cnt_th = 0;
        TAWorker* worker = new TAWorker();
        QThread* thread = new QThread;
        worker->setWords(words);
        worker->setTopWords(topWords);
        worker->setTopN(topN);
        worker->moveToThread(thread);
        // При запуске потока будет вызван метод process
        connect(thread, SIGNAL(started()), worker, SLOT(processTop()));
        connect(worker, SIGNAL(finishedTop()), this, SLOT(finishedTop()));
        connect(worker, SIGNAL(incProgressBar(double)), this, SLOT(incProgressBar(double)));
        // При завершении работы потока - вызове сигнала finished, сработает слот quit
        connect(worker, SIGNAL(finishedTop()), thread, SLOT(quit()));
        // worker пометит себя для удаления при окончании работы
        connect(worker, SIGNAL(finishedTop()), worker, SLOT(deleteLater()));
        // поток пометит себя для удаления при окончании работы
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        thread->start();
    }
}

void TextAnalysis::finishedTop() {
    ui->tableWidget->setRowCount(topWords.size());
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Word"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Frequency"));
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QMapIterator<int, QString> it(topWords);
    int row = 0;
    while (it.hasNext()) {
        it.next();
        QString s = it.value();
        s.chop(1);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(s));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(tr("%1").arg(-it.key())));
        ++row;
    }
    fileNames->clear();
    words.clear();
    topWords.clear();

}

void TextAnalysis::addThread() {
    TAWorker* worker = new TAWorker();
    QThread* thread = new QThread;
    worker->setFileNames(*fileNames);
    worker->setWords(words);
    worker->moveToThread(thread);
    worker->setNumFiles(numFiles);
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

void TextAnalysis::start() {
    ui->tableWidget->clear();
    topN = ui->spinBox->value();
    numFiles = fileNames->size();
    int numth = QThread::idealThreadCount();
    for (int i = 0; i < numth; ++i) {
        addThread();
    }
}

void TextAnalysis::incProgressBar(double step) {
    progress += step;
    ui->progressBar->setValue(progress + 0.5);
}


