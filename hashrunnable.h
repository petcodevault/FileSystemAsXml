#include <QRunnable>
#include <QFile>
#include <QCryptographicHash>
#include "file.h" // Assuming File class definition

#ifndef HASHRUNNABLE_H
#define HASHRUNNABLE_H

class HashRunnable : public QRunnable {
public:
    HashRunnable(File* file)
        : file(file)
    {
        setAutoDelete(true);
    }

    void run() override {
        QFile f(file->getOriginalPath().value());
        if (!f.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open file:" << file->getName();
            return;
        }

        QCryptographicHash hash(QCryptographicHash::Sha1);
        while (!f.atEnd()) {
            QByteArray buffer = f.read(8192); // Read in chunks of 8KB
            hash.addData(buffer);
        }
        f.close();

        QByteArray resultHash = hash.result().toHex();
        file->setHash(resultHash);
    }

private:
    File* file;
};

#endif // HASHRUNNABLE_H
