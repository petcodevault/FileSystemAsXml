#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>
#include "file.h"   // Assuming File class definition

#ifndef XMLIMPORTER_H
#define XMLIMPORTER_H

class XmlImporter {
public:
    static bool importFromXml(const QString& filePath, Folder& rootFolder) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Failed to open XML file for reading:" << file.errorString();
            return false;
        }

        QXmlStreamReader xmlReader(&file);

        while (!xmlReader.atEnd()) {
            if (xmlReader.isStartElement()) {
                if (xmlReader.name().compare("filesystem") == 0) {
                    readFolderElement(xmlReader, rootFolder);
                } else {
                    xmlReader.skipCurrentElement();
                }
            } else {
                xmlReader.readNext();
            }
        }

        if (xmlReader.hasError()) {
            qDebug() << "XML parsing error:" << xmlReader.errorString();
            return false;
        }

        file.close();
        return true;
    }

private:
    static void readFolderElement(QXmlStreamReader& xmlReader, Folder& parentFolder) {
        QString folderName = xmlReader.attributes().value("name").toString();
        QDateTime updateDate = QDateTime::fromString(xmlReader.attributes().value("updateDate").toString(), Qt::ISODate);

        Folder folder(folderName, updateDate);

        while (!xmlReader.atEnd()) {
            xmlReader.readNext();

            if (xmlReader.isEndElement() && xmlReader.name().compare("folder") == 0) {
                break;
            }

            if (xmlReader.isStartElement()) {
                if (xmlReader.name().compare("file") == 0) {
                    readFileElement(xmlReader, folder);
                } else if (xmlReader.name().compare("folder") == 0) {
                    readFolderElement(xmlReader, folder);
                } else {
                    xmlReader.skipCurrentElement();
                }
            }
        }

        parentFolder.addSubFolder(std::move(folder));
    }

    static void readFileElement(QXmlStreamReader& xmlReader, Folder& parentFolder) {
        QString fileName = xmlReader.attributes().value("name").toString();
        qint64 fileSize = xmlReader.attributes().value("size").toLongLong();
        QDateTime updateDate = QDateTime::fromString(xmlReader.attributes().value("updateDate").toString(), Qt::ISODate);

        QString originalPath = xmlReader.attributes().value("originalPath").toString();
        QString hash = xmlReader.attributes().value("hash").toString();

        File file(fileName, fileSize, updateDate, originalPath);
        if (!hash.isEmpty()) {
            file.setHash(hash);
        }

        parentFolder.addFile(std::move(file));

        // Пропускаем закрывающий элемент </File>
        xmlReader.readNext();
    }
};

#endif // XMLIMPORTER_H
