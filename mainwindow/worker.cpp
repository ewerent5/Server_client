#include "worker.h"
#include <QFile>
#include <QDataStream>

Worker::Worker(QObject *parent) : QObject(parent) {}

void Worker::setParams(const QString &inputFile, const QString &outputFile, quint64 key)
{
    m_inputFile = inputFile;
    m_outputFile = outputFile;
    m_key = key;
}

void Worker::process()
{
    QFile inFile(m_inputFile);
    QFile outFile(m_outputFile);

    if (!inFile.open(QIODevice::ReadOnly)) {
        emit finished(false, "Не удалось открыть входной файл");
        return;
    }
    if (!outFile.open(QIODevice::WriteOnly)) {
        emit finished(false, "Не удалось открыть выходной файл");
        return;
    }

    QByteArray buffer;
    qint64 total = inFile.size();
    qint64 processed = 0;

    const int blockSize = 4096;
    while (!inFile.atEnd()) {
        buffer = inFile.read(blockSize);

        for (int i = 0; i < buffer.size(); i += 8) {
            quint64 *chunk = reinterpret_cast<quint64*>(buffer.data() + i);
            if (i + 8 <= buffer.size()) {
                *chunk ^= m_key;
            } else {
                // обрабатываем остаток по байтам
                for (int j = i; j < buffer.size(); ++j) {
                    buffer[j] ^= reinterpret_cast<const char*>(&m_key)[j - i];
                }
            }
        }

        outFile.write(buffer);
        processed += buffer.size();
        emit progress(static_cast<int>((processed * 100) / total));
    }

    emit finished(true, "Обработка завершена");
}
