#include "ngworker.h"
#include <QProcess>
#include <QDebug>

NGWorker::NGWorker(QObject *parent) : QObject(parent) {
}

void NGWorker::useMyStem() {
    QString path = "D:/Vojna i mir/";
    int i = 0;
    while (!fileNames->empty()) {
        QString curPath = fileNames->pop();
        QString newPath = curPath + (i + '0') + ".txt";
        QProcess::execute("D:/mystem.exe", QStringList() <<curPath << newPath << "-nld");
        newFileNames.push(newPath);
        ++i;
        double step = 100.0 / (numFiles * 3.0);
        emit incProgressBar(step);
    }
}

void NGWorker::moveWords() {
    while(!newFileNames.empty()) {
        QString curPath = newFileNames.pop();
        QFile file(curPath);
        file.open(QIODevice::ReadOnly);
        while(!file.atEnd()) {
            QString w = file.readLine();
            for (int i = 0; i < w.size() - N; ++i) {
                QString w1;
                for (int j = 0; j < N; ++j) {
                    w1.push_back(w[i + j]);
                }
                words->inc(w1);
            }
        }
        file.close();
        file.remove();
        double step = 100.0 / (numFiles * 3.0);
        emit incProgressBar(step);
    }
}

void NGWorker::process() {
    useMyStem();
    moveWords();
    emit finishedMyStem();
}

bool NGWorker::cmp(QPair<QString, int> &a, QPair<QString, int> &b) {
    return a.second > b.second;
}

void NGWorker::processTable() {
    numWords = words->size();
    QMapIterator<QString, int> it_old = words->getIter();
    int mi = -1;
    table->resize(words->size());
    int idx = 0;
    while (it_old.hasNext()) {
        it_old.next();
        //qDebug()<<"---  "<<it_old.key()<<' '<<it_old.value();
        (*table)[idx] = {it_old.key(), it_old.value()};
        ++idx;
        double step = 100.0 * 2.0 / (numWords * 3.0 * 3.0);
        emit incProgressBar(step);
    }
    //for (int i = 0; i < table->size(); ++i) {
    //    qDebug()<<(*table)[i].first<<' '<<(*table)[i].second;
    //}
    qSort(table->begin(), table->end(), cmp);
    emit incProgressBar(100.0 / 9.0);
    emit finishedTable();
}
