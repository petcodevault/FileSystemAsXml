#include <QList>
#include <QThreadPool>
#include "hashrunnable.h" // Assuming HashRunnable class definition
#include "file.h" // Assuming File class definition

#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

class FileProcessor {
public:
    FileProcessor(QList<File*>& files, int maxThreads)
        : files(files), maxThreads(maxThreads) {}

    void processFiles() {
        QThreadPool threadPool;
        threadPool.setMaxThreadCount(maxThreads);

        for (auto& file : files) {
            HashRunnable* runnable = new HashRunnable(file);
            threadPool.start(runnable);
        }

        threadPool.waitForDone();
    }

private:
    QList<File*>& files;
    int maxThreads;
};


#endif // FILEPROCESSOR_H
