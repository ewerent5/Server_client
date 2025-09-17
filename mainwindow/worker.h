#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QString>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);

    void setParams(const QString &inputFile, const QString &outputFile, quint64 key);

public slots:
    void process();

signals:
    void progress(int percent);
    void finished(bool success, const QString &message);

private:
    QString m_inputFile;
    QString m_outputFile;
    quint64 m_key {0};
};

#endif // WORKER_H
