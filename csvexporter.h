#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDateTime>
#include "file.h" // Assuming File class definition

#ifndef CSVEXPORTER_H
#define CSVEXPORTER_H

class CsvExporter {
public:
    static bool exportToCsv(const QList<File*>& files, const QString& filePath) {
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Failed to open file:" << file.errorString();
            return false;
        }

        QTextStream stream(&file);
        stream.setEncoding(QStringConverter::Encoding::Utf8);

        // Write header
        stream << "Name;Size;UpdateDate;OriginalPath;Hash\n";

        // Write files
        for (const File* file : files) {
            stream << escapeCsvField(file->getName()) << ";"
                   << QString::number(file->getSize()) << ";"
                   << file->getUpdateDate().toString(Qt::ISODate) << ";"
                   << escapeCsvField(file->getOriginalPath().value_or("")) << ";"
                   << file->getHash().value_or("") << "\n";
        }

        file.close();
        return true;
    }

private:
    static QString escapeCsvField(const QString& field) {
        // If the field contains a semicolon, surround it with double quotes and double any existing double quotes
        if (field.contains(";")) {

            QString temp = field;
            auto newValue = temp.replace("\"", "\"\"");
            return "\"" + newValue + "\"";
        }
        return field;
    }
};


#endif // CSVEXPORTER_H
