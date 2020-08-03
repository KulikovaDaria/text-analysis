#ifndef TEXTANALYSIS_H
#define TEXTANALYSIS_H

#include <QDialog>
#include <QString>
#include <QMultiMap>
#include <QVector>
#include <QAtomicInt>
#include "safe_queue.h"
#include "safe_map.h"

namespace Ui {
class TextAnalysis;
}

class TextAnalysis : public QDialog {
    Q_OBJECT

public:
    TextAnalysis(QWidget *parent = nullptr);
    ~TextAnalysis();
    void setFileNames(SafeQueue<QString>& fn) { fileNames = &fn; }
    void addThread();

private slots:
    void start();
    void finished();
    void finishedTop();
    void back();
    void incProgressBar(double step);

private:
    Ui::TextAnalysis *ui;
    SafeQueue<QString>* fileNames;
    SafeMap<QString> words;
    QMultiMap<int, QString> topWords;
    QAtomicInt cnt_th = 0;
    int topN;
    int numFiles;
    double progress = 0;
};

#endif // TEXTANALYSIS_H
