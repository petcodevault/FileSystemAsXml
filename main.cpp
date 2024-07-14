#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QXmlStreamWriter>
#include <QDateTime>
#include <QDebug>
#include <QCryptographicHash>
#include <QThreadPool>
#include <QRunnable>
#include <QMutex>
#include <QQueue>
#include <QStack>
#include <QMutexLocker>
#include <QTemporaryFile>

QMutex hashMutex;  // Мьютекс для синхронизации доступа к хэш-мап

struct FileHashTask : public QRunnable {
    QString filePath;
    QHash<QString, QString> &hashMap;

    FileHashTask(const QString &filePath, QHash<QString, QString> &hashMap)
        : filePath(filePath), hashMap(hashMap) {}

    void run() override {
        qDebug().noquote() << "Processing:" << filePath;

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Cannot open file for reading:" << file.errorString();
            return;
        }

        QCryptographicHash hash(QCryptographicHash::Sha1);
        if (!hash.addData(&file)) {
            qWarning() << "Failed to calculate hash for file:" << filePath;
            return;
        }

        QString fileHash = hash.result().toHex();
        file.close();

        // Запись хэша в хэш-мап
        QMutexLocker locker(&hashMutex);
        hashMap.insert(filePath, fileHash);

        qDebug().noquote() << "OK - Completed:" << filePath;
    }
};

void writeDirectoryXml(const QDir &directory, QXmlStreamWriter &xmlWriter) {
    xmlWriter.writeStartElement("directory");
    xmlWriter.writeAttribute("name", directory.dirName());

    QFileInfoList entries = directory.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    for (const QFileInfo &entry : entries) {
        if (entry.isDir()) {
            writeDirectoryXml(QDir(entry.absoluteFilePath()), xmlWriter);
        } else if (entry.isFile()) {
            xmlWriter.writeStartElement("file");
            xmlWriter.writeAttribute("name", entry.fileName());
            xmlWriter.writeAttribute("size", QString::number(entry.size()));
            xmlWriter.writeAttribute("lastModified", entry.lastModified().toString(Qt::ISODate));
            xmlWriter.writeAttribute("originalPath", entry.absoluteFilePath());
            xmlWriter.writeEndElement(); // file
        }
    }

    xmlWriter.writeEndElement(); // directory
}

void writeInitialXml(const QString &rootDirPath, const QString &xmlFilePath) {
    QFile file(xmlFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot open file for writing:" << file.errorString();
        return;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("filesystem");

    QDir rootDir(rootDirPath);
    if (rootDir.exists()) {
        writeDirectoryXml(rootDir, xmlWriter);
    } else {
        qWarning() << "Root directory does not exist:" << rootDirPath;
    }

    xmlWriter.writeEndElement(); // filesystem
    xmlWriter.writeEndDocument();

    file.close();
}

QQueue<QString> extractFilePathsFromXml(const QString &xmlFilePath) {
    QQueue<QString> filePaths;

    QFile file(xmlFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file for reading:" << file.errorString();
        return filePaths;
    }

    QXmlStreamReader xmlReader(&file);

    while (!xmlReader.atEnd()) {
        xmlReader.readNext();
        if (xmlReader.isStartElement()) {

            // TODO убрать xmlReader.name().toString() == "directory" и проверку !originalPath.isEmpty()
            if (xmlReader.name().toString() == "file") {
                QString originalPath = xmlReader.attributes().value("originalPath").toString();
                if (!originalPath.isEmpty()) {
                    filePaths.enqueue(originalPath);
                }
            }
        }
    }

    file.close();
    return filePaths;
}

QHash<QString, QString> computeHashes(const QQueue<QString> &filePaths, int numThreads) {
    QThreadPool threadPool;
    threadPool.setMaxThreadCount(numThreads);

    QHash<QString, QString> hashMap;

    QQueue<QString> queue = filePaths;

    while (!queue.isEmpty()) {
        QString filePath = queue.dequeue();
        FileHashTask *task = new FileHashTask(filePath, hashMap);
        threadPool.start(task);
    }

    threadPool.waitForDone();
    return hashMap;
}

void updateXmlWithHashes(const QString &xmlFilePath, const QString &outputXmlFilePath, const QHash<QString, QString> &hashMap) {
    QFile xmlFile(xmlFilePath);
    if (!xmlFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open XML file for reading:" << xmlFile.errorString();
        return;
    }

    QXmlStreamReader xmlReader(&xmlFile);

    QFile outputFile(outputXmlFilePath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot open output XML file for writing:" << outputFile.errorString();
        return;
    }

    QXmlStreamWriter xmlWriter(&outputFile);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeProcessingInstruction("xml-stylesheet", "type=\"text/xsl\" href=\"stylesheet.xsl\""); // <?xml-stylesheet type="text/xsl" href="stylesheet.xsl" ?>

    // TODO: не нужно, происходит дублирование элемента (добавляется вторая вложенность).
    //xmlWriter.writeStartElement("filesystem");

    while (!xmlReader.atEnd()) {
        xmlReader.readNext();

        if (xmlReader.isStartElement()) {
            QXmlStreamAttributes attributes = xmlReader.attributes();
            QString elementName = xmlReader.name().toString();

            // if (attributes.hasAttribute("originalPath")) {
            //     QString originalPath = attributes.value("originalPath").toString();
            //     if (hashMap.contains(originalPath)) {
            //         attributes.append("hash", hashMap[originalPath]);
            //     }
            // }

            xmlWriter.writeStartElement(elementName);

            for (const QXmlStreamAttribute &attr : attributes) {
                if (attr.name().toString() != "originalPath") {
                    xmlWriter.writeAttribute(attr.name().toString(), attr.value().toString());
                } else {
                    QString originalPath = attr.value().toString();
                    if (hashMap.contains(originalPath)) {
                        //attributes.append("hash", hashMap[originalPath]);
                        xmlWriter.writeAttribute("hash", hashMap[originalPath]);
                    }
                }
            }
        } else if (xmlReader.isEndElement()) {
            xmlWriter.writeEndElement();
        } else if (xmlReader.isCharacters() && !xmlReader.isWhitespace()) {
            xmlWriter.writeCharacters(xmlReader.text().toString());
        }
    }

    xmlReader.clear();
    xmlFile.close();

    xmlWriter.writeEndElement(); // filesystem
    xmlWriter.writeEndDocument();
    outputFile.close();
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    // if (argc != 3) {
    //     qCritical() << "Usage:" << argv[0] << "<directory_path> <output_xml_file>";
    //     return -1;
    // }

    // QString rootDirPath = argv[1]; // Принимаем путь к директории из аргументов командной строки
    // QString xmlFilePath = argv[2];

    QString rootDirPath = "/Volumes/Storage/Вокруг света";
    QString xmlFilePath = "/Users/dmytro/Documents/sha1-tst.xml";


    QTemporaryFile tempXmlFile;
    tempXmlFile.setAutoRemove(true);

    if (!tempXmlFile.open()) {
        qWarning() << "Failed to create temporary file:" << tempXmlFile.errorString();
        return 1;
    }

    QString tempXmlFilePath = tempXmlFile.fileName();

    // Этап 1: Запись всех данных в XML-файл, с учетом структуры файловой системы
    writeInitialXml(rootDirPath, tempXmlFilePath);

    // Этап 2: Формирование очереди заданий на основе оригинальных путей из XML-файла
    QQueue<QString> filePaths = extractFilePathsFromXml(tempXmlFilePath);

    // Этап 3: Выполнение заданий в нескольких потоках для вычисления хэшей
    int numThreads = QThread::idealThreadCount(); // Получаем количество потоков
    QHash<QString, QString> hashMap = computeHashes(filePaths, numThreads);

    // Этап 4: Обновление XML-файла: замена originalPath на вычисленный хэш
    updateXmlWithHashes(tempXmlFilePath, xmlFilePath, hashMap);

    // Очистка

    if (!tempXmlFile.remove()) {
        qWarning() << "Failed to remove temporary file:" << tempXmlFile.errorString();
        return 2;
    }

    return 0;
}
