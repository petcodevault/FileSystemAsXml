#include <QFile>
#include <QDir>
#include <QCryptographicHash>
#include <QFileInfo>
#include <QDebug>
#include "file.h" // Assuming File class definition

#ifndef SAVEFILEUTILS_H
#define SAVEFILEUTILS_H

class FileUtils {
public:
    static bool saveUniqueFiles(const QList<File*>& files, const QString& outputDir) {
        // Create output directory if it doesn't exist
        QDir dir(outputDir);
        if (!dir.exists()) {
            if (!dir.mkpath(".")) {
                qDebug() << "Failed to create output directory:" << outputDir;
                return false;
            }
        }

        // Map to store unique files based on hash and extension
        QMap<QString, const File*> uniqueFiles;

        // Collect unique files based on hash
        for (const File* file : files) {
            QString hash = file->getHash().value_or("");

            if (hash.isEmpty() || uniqueFiles.contains(hash))
                continue;

            uniqueFiles[hash] = file;
        }

        // Save unique files to corresponding subdirectories
        for (const auto& key : uniqueFiles.keys()) {
            const File* file = uniqueFiles.value(key);

            // Determine subdirectory based on file extension
            QString extension = QFileInfo(file->getName()).suffix();

            if (extension.isEmpty()) {
                extension = "_";
            }

            QString subDirPath = QDir(outputDir).filePath(extension);

            QDir subDir(subDirPath);
            if (!subDir.exists()) {
                if (!subDir.mkpath(".")) {
                    qDebug() << "Failed to create subdirectory:" << subDirPath;
                    continue;
                }
            }

            QString originalFileName = QFileInfo(file->getName()).baseName();
            QString uniqueFileName = originalFileName + "_" + key + "." + extension;

            QString destinationFilePath = QDir(subDirPath).filePath(uniqueFileName);

            const QString originalPath = file->getOriginalPath().value_or("");

            if (!copyFile(originalPath, destinationFilePath)) {
                qDebug() << "Failed to copy file:" << originalPath << "to:" << destinationFilePath;
            }
        }

        return true;
    }

private:
    static bool copyFile(const QString& sourceFilePath, const QString& destinationFilePath) {
        QFile sourceFile(sourceFilePath);
        QFile destinationFile(destinationFilePath);

        if (!sourceFile.exists()) {
            qDebug() << "Source file does not exist:" << sourceFilePath;
            return false;
        }

        if (destinationFile.exists()) {
            qDebug() << "Destination file already exists:" << destinationFilePath;
            return false;
        }

        if (!sourceFile.copy(destinationFilePath)) {
            qDebug() << "Failed to copy file:" << sourceFilePath << "to:" << destinationFilePath;
            return false;
        }

        return true;
    }
};


#endif // SAVEFILEUTILS_H
