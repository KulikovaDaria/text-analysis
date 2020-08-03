#ifndef TAWORKER_H
#define TAWORKER_H

#include <QObject>
#include <QString>
#include <QMultiMap>
#include "safe_queue.h"
#include "safe_map.h"

class TAWorker : public QObject {
    Q_OBJECT
public:
    explicit TAWorker(QObject *parent = nullptr);
    void setFileNames(SafeQueue<QString>& fn) { fileNames = &fn; }
    void setWords(SafeMap<QString>& w) { words = &w; }
    void setTopWords(QMultiMap<int, QString>& tw) { topWords = &tw; }
    void setNumFiles(int nf) { numFiles = nf; }
    void setTopN(int n) { topN = n; }
    void useMyStem();
    void moveWords();

public slots:
    void process();
    void processTop();

signals:
    void finishedMyStem();
    void finishedTop();
    void incProgressBar(double step);

private:
    SafeQueue<QString>* fileNames;
    SafeQueue<QString> newFileNames;
    SafeMap<QString>* words;
    QMultiMap<int, QString>* topWords;
    int topN;
    int numFiles = 0;
    int numWords = 0;
    int su=0;
};

#endif // TAWORKER_H
