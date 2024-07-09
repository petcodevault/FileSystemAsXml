#include <QDir>
#include <QFile>
#include <QDebug>
#include <QMap>
#include <QRegularExpression>
#include <QDirIterator>
#include "file.h" // Assuming Folder class definition

#ifndef RESTOREFILEUTILS_H
#define RESTOREFILEUTILS_H


class RestoreFileUtils {
public:
    static void restore(const Folder& folderStructure, const QString& sourceDir, const QString& destDir) {
        // Step 1: Extract hashes from sourceDir
        const QMap<QString, QString> hashToFileMap = extractHashes(sourceDir);

        // Step 2: Restore folder structure and copy files
        restoreFolderStructure(folderStructure, destDir, hashToFileMap);
    }

private:
    static QString extractHash(const QString& fileName) {
        QRegularExpression re("_([a-fA-F0-9]{40})$");
        QRegularExpressionMatch match = re.match(fileName);
        if (match.hasMatch()) {
            return match.captured(1); // Возвращаем захваченную группу (хеш)
        } else {
            return QString(); // Если не найдено совпадение, возвращаем пустую строку
        }
    }


    static QMap<QString, QString> extractHashes(const QString& sourceDir) {
        QMap<QString, QString> hashToFileMap;

        // QDir dir(sourceDir);
        // dir.setFilter(QDir::Files);
        // dir.setSorting(QDir::Name | QDir::IgnoreCase);

        QDirIterator it(sourceDir, QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden, QDirIterator::Subdirectories);

        while (it.hasNext()) {
            auto fileInfo = it.nextFileInfo();

            if (fileInfo.isFile()) {
                QString fileName = fileInfo.fileName();
                QString baseName = fileInfo.baseName();

                if (baseName.isEmpty()) {
                    baseName = fileName;
                }

                QString extension = fileInfo.completeSuffix();

                QString hash = extractHash(baseName);

                if (hash.isEmpty()) {
                    qDebug() << "extractHash ERROR " << hash << " " << fileName;
                    continue;
                }

                if (hashToFileMap.contains(hash)) {
                    continue;
                }

                QString fullPath = fileInfo.filePath();
                hashToFileMap[hash] = fullPath;
            }
        }

        return hashToFileMap;
    }

    static void restoreFolderStructure(const Folder& folder, const QString& destDir, const QMap<QString, QString>& hashToFileMap) {
        QString folderPath = QDir(destDir).filePath(folder.getName());

        // Create directory if it doesn't exist
        QDir().mkpath(folderPath);

        // Copy files from hashToFileMap based on folder structure
        for (const File& file : folder.getFiles()) {
            QString fileName = file.getName();
            QString hash = file.getHash().value_or("");

            // Look up file path in hashToFileMap
            QString sourceFilePath = hashToFileMap.value(hash);

            if (!sourceFilePath.isEmpty()) {
                // Construct destination file path
                QString destFilePath = QDir(folderPath).filePath(fileName);

                // Copy file
                QFile::copy(sourceFilePath, destFilePath);
            } else {
                qDebug() << "File with hash" << hash << " " << fileName << "not found in hashToFileMap";
            }
        }

        // Recursively restore subfolders
        for (const Folder& subfolder : folder.getSubFolders()) {
            restoreFolderStructure(subfolder, folderPath, hashToFileMap);
        }
    }
};


#endif // RESTOREFILEUTILS_H
