#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <QObject>
#include <QFile>
#include <QCryptographicHash>

class HashWorker : public QObject {
    Q_OBJECT
public:
    explicit HashWorker(const QString &filePath, QObject *parent = nullptr)
        : QObject(parent), m_filePath(filePath) {}

public slots:
    void process() {
        QFile file(m_filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            emit error("Can't open the file!");
            emit finished();
            return;
        }

        QCryptographicHash md5(QCryptographicHash::Md5);
        QCryptographicHash sha1(QCryptographicHash::Sha1);
        QCryptographicHash sha256(QCryptographicHash::Sha256);
        QCryptographicHash sha512(QCryptographicHash::Sha512);

        qint64 totalBytes = file.size();
        qint64 bytesProcessed = 0;
        const qint64 chunkSize = 4 * 1024 * 1024;

        while (!file.atEnd()) {
            QByteArray buffer = file.read(chunkSize);

            md5.addData(buffer);
            sha1.addData(buffer);
            sha256.addData(buffer);
            sha512.addData(buffer);

            bytesProcessed += buffer.size();

            if (totalBytes > 0) {
                emit progress(static_cast<int>((bytesProcessed * 100) / totalBytes));
            }
        }
        emit resultReady(QString(md5.result().toHex()),
                         QString(sha1.result().toHex()),
                         QString(sha256.result().toHex()),
                         QString(sha512.result().toHex()));
        emit finished();
    }

signals:
    void progress(int percent);
    void resultReady(const QString &md5, const QString &sha1, const QString &sha256, const QString &sha512);
    void error(const QString &msg);
    void finished();

private:
    QString m_filePath;
};

#endif
