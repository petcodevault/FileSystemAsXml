#include <QFile>
#include <QXmlStreamWriter>
#include <QDebug>
#include "file.h"   // Assuming File class definition

#ifndef XMLEXPORTER_H
#define XMLEXPORTER_H

class XmlExporter {
public:
    static bool exportToXml(const Folder& rootFolder, const QString& filePath) {
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Failed to open XML file for writing:" << file.errorString();
            return false;
        }

        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true);

        xmlWriter.writeStartDocument();
        xmlWriter.writeStartElement("filesystem");

        // Write root folder data recursively
        writeFolderElement(rootFolder, xmlWriter);

        xmlWriter.writeEndElement(); // </filesystem>
        xmlWriter.writeEndDocument();

        file.close();
        return true;
    }

private:
    static void writeFolderElement(const Folder& folder, QXmlStreamWriter& xmlWriter) {
        xmlWriter.writeStartElement("folder");
        xmlWriter.writeAttribute("name", folder.getName());
        xmlWriter.writeAttribute("updateDate", folder.getUpdateDate().toString(Qt::ISODate));

        // Write subfolders recursively
        for (const Folder& subfolder : folder.getSubFolders()) {
            writeFolderElement(subfolder, xmlWriter);
        }

        // Write files
        for (const File& file : folder.getFiles()) {
            writeFileElement(file, xmlWriter);
        }

        xmlWriter.writeEndElement(); // </folder>
    }

    static void writeFileElement(const File& file, QXmlStreamWriter& xmlWriter) {
        xmlWriter.writeStartElement("file");
        xmlWriter.writeAttribute("name", file.getName());
        xmlWriter.writeAttribute("size", QString::number(file.getSize()));
        xmlWriter.writeAttribute("updateDate", file.getUpdateDate().toString(Qt::ISODate));

        if (file.getOriginalPath().has_value()) {
            xmlWriter.writeAttribute("originalPath", file.getOriginalPath().value());
        }

        if (file.getHash().has_value()) {
            xmlWriter.writeAttribute("hash", file.getHash().value());
        }

        xmlWriter.writeEndElement(); // </file>
    }
};


#endif // XMLEXPORTER_H
