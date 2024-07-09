#include <QCoreApplication>
#include <QDebug>
#include "fileprocessor.h"
#include "file.h" // Assuming File class definition
#include "xmlexporter.h"
#include "xmlimporter.h"
#include "csvexporter.h"
#include "savefileutils.h"
#include "restorefileutils.h"

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    auto folder = Folder::createFromPath("/Users/dmytro/Desktop/test_folder");

    QList<File*> allFiles;

    folder.getAllFiles(allFiles);

    // Process files
    auto maxThreads = QThread::idealThreadCount();
    FileProcessor processor(allFiles, maxThreads);
    processor.processFiles();

    // Print results or do other operations after processing
    for (const auto& file : allFiles) {
        qDebug() << "File:" << file->getName() << "SHA1 Hash:" << file->getHash().value();
    }

    XmlExporter::exportToXml(folder, "/Users/dmytro/Documents/test_out/out.xml");

    Folder topLevelFoler("fileSystem", QDateTime::currentDateTime());
    XmlImporter::importFromXml("/Users/dmytro/Documents/test_out/out.xml", topLevelFoler);

    XmlExporter::exportToXml(topLevelFoler.getTopLevelFolder().getTopLevelFolder(), "/Users/dmytro/Documents/test_out/out2.xml");

    CsvExporter::exportToCsv(allFiles, "/Users/dmytro/Documents/test_out/out.csv");

    FileUtils::saveUniqueFiles(allFiles, "/Users/dmytro/Documents/test_out/collection");

    RestoreFileUtils::restore(folder, "/Users/dmytro/Documents/test_out/collection", "/Users/dmytro/Documents/test_out/restored");

    qDebug() << "Done.";

    return app.exec();
}
