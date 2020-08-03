#include "taworker.h"
#include <QProcess>
#include <QThread>
#include <QFile>

TAWorker::TAWorker(QObject *parent)
    : QObject(parent) {
}

void TAWorker::useMyStem() {
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

void TAWorker::moveWords() {
    while(!newFileNames.empty()) {
        QString curPath = newFileNames.pop();
        QFile file(curPath);
        file.open(QIODevice::ReadOnly);
        while(!file.atEnd()) {
            QString w = file.readLine();
            words->inc(w);
        }
        file.close();
        file.remove();
        double step = 100.0 / (numFiles * 3.0);
        emit incProgressBar(step);
    }
}

void TAWorker::process() {
    useMyStem();
    moveWords();
    emit finishedMyStem();
}

void TAWorker::processTop() {
    numWords = words->size();
    QMapIterator<QString, int> it_old = words->getIter();
    int mi = -1;
    while (it_old.hasNext()) {
        it_old.next();
        if(it_old.value() >= mi || topWords->size() < topN) {
            topWords->insert(-it_old.value(), it_old.key());
            while(mi > 0 && topWords->size() - topWords->count(-mi) >= topN) {
                topWords->remove(-mi);
                auto it_new = topWords->end();
                --it_new;
                mi = -it_new.key();
            }
            if(mi < 0) {
                mi = it_old.value();
            }
        }
        double step = 100.0 / (numWords * 3.0);
        emit incProgressBar(step);
    }
    emit finishedTop();
}
