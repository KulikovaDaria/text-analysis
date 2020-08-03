#ifndef NGWORKER_H
#define NGWORKER_H

#include <QObject>
#include "safe_map.h"
#include "safe_queue.h"

class NGWorker : public QObject
{
    Q_OBJECT
public:
    explicit NGWorker(QObject *parent = nullptr);
    void setFileNames(SafeQueue<QString>& fn) { fileNames = &fn; }
    void setWords(SafeMap<QString>& w) { words = &w; }
    void setNumFiles(int nf) { numFiles = nf; }
    void setN(int n) { N = n; }
    void setTable(QVector<QPair<QString, int>>& t) { table = &t; }
    void useMyStem();
    void moveWords();

public slots:
    void process();
    void processTable();


signals:
    void finishedMyStem();
    void finishedTable();
    void incProgressBar(double step);

private:
    static bool cmp(QPair<QString, int>& a, QPair<QString, int>& b);

    SafeQueue<QString>* fileNames;
    SafeQueue<QString> newFileNames;
    SafeMap<QString>* words;
    QVector<QPair<QString, int>>* table;
    int N;
    int numFiles = 0;
    int numWords = 0;
};

#endif // NGWORKER_H
