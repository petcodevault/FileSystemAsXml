#include <QCoreApplication>
#include <QDebug>
#include "fileprocessor.h"
#include "file.h" // Assuming File class definition
#include "xmlexporter.h"
#include "xmlimporter.h"
#include "csvexporter.h"
#include "savefileutils.h"
#include "restorefileutils.h"

QList<File*> getUniqueFilesByHash(const QList<File*>& allFiles) {
    QSet<QString> seenHashes;
    QList<File*> uniqueFiles;

    for (File* file : allFiles) {
        if (file->getHash().has_value()) {
            const QString& hash = file->getHash().value();
            if (!seenHashes.contains(hash)) {
                uniqueFiles.append(file);
                seenHashes.insert(hash);
            }
        }
    }

    return uniqueFiles;
}

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    // auto folder = Folder::createFromPath("/Volumes/Storage/_GARBAGE");

    // QList<File*> allFiles;

    // folder.getAllFiles(allFiles);

    // // Process files
    // auto maxThreads = QThread::idealThreadCount();
    // FileProcessor processor(allFiles, maxThreads);
    // processor.processFiles();

    // // Print results or do other operations after processing
    // for (const auto& file : allFiles) {
    //     qDebug() << "File:" << file->getName() << "SHA1 Hash:" << file->getHash().value();
    // }

    //XmlExporter::exportToXml(folder, "/Users/dmytro/Documents/_GARBAGE.xml");

    Folder topLevelFoler("fileSystem", QDateTime::currentDateTime());
    XmlImporter::importFromXml("/Users/dmytro/Documents/test_out/out.xml", topLevelFoler);

    // XmlExporter::exportToXml(topLevelFoler.getTopLevelFolder().getTopLevelFolder(), "/Users/dmytro/Documents/test_out/out2.xml");

    QList<File*> allFiles;

    topLevelFoler.getTopLevelFolder().getTopLevelFolder().getAllFiles(allFiles);

    QList<File*> uniqueFiles = getUniqueFilesByHash(allFiles);

    CsvExporter::exportToCsv(uniqueFiles, "/Users/dmytro/Documents/test_out/out.csv");

    //FileUtils::saveUniqueFiles(allFiles, "/Users/dmytro/Documents/test_out/collection");

    // RestoreFileUtils::restore(folder, "/Users/dmytro/Documents/test_out/collection", "/Users/dmytro/Documents/test_out/restored");

    qDebug() << "Done.";

    return app.exec();
}
