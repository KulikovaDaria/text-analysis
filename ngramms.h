#ifndef NGRAMMS_H
#define NGRAMMS_H

#include <QDialog>
#include "safe_queue.h"
#include "safe_map.h"

namespace Ui {
class NGramms;
}

class NGramms : public QDialog {
    Q_OBJECT

public:
    explicit NGramms(QWidget *parent = nullptr);
    ~NGramms();
    void setFileNames(SafeQueue<QString>& fn) { fileNames = &fn; }
    void addThread();


private slots:
    void on_start_clicked();
    void on_back_clicked();
    void finished();
    void finishedTable();
    void incProgressBar(double step);

private:
    Ui::NGramms *ui;
    int N;
    SafeQueue<QString>* fileNames;
    SafeMap<QString> words;
    QVector<QPair<QString, int>> table;
    QAtomicInt cnt_th = 0;
    int numFiles;
    double progress = 0;
};

#endif // NGRAMMS_H
