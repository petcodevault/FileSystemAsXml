#include <QString>
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QVector>
#include <QList>
#include <optional>

#ifndef FILE_H
#define FILE_H

class File {
public:
    File(const QString& name, qint64 size, const QDateTime& updateDate,
         const std::optional<QString>& originalPath = std::nullopt,
         const std::optional<QString>& hash = std::nullopt)
        : name(name), size(size), updateDate(updateDate), originalPath(originalPath), hash(hash) {}

    const QString& getName() const { return name; }
    qint64 getSize() const { return size; }
    const QDateTime& getUpdateDate() const { return updateDate; }
    const std::optional<QString>& getOriginalPath() const { return originalPath; }
    const std::optional<QString>& getHash() const { return hash; }
    void setHash(const QString& hash) { this->hash = hash; }

private:
    QString name;
    qint64 size;
    QDateTime updateDate;
    std::optional<QString> originalPath;
    std::optional<QString> hash;
};

class Folder {
public:
    Folder(const QString& name, const QDateTime& updateDate,
           const std::optional<QString>& hash = std::nullopt)
        : name(name), updateDate(updateDate), hash(hash) {}

    void addSubFolder(Folder&& folder) {
        subFolders.push_back(std::move(folder));
    }

    void addFile(File&& file) {
        files.push_back(std::move(file));
    }

    const QString& getName() const { return name; }
    const QList<Folder>& getSubFolders() const { return subFolders; }
    const Folder& getTopLevelFolder() const {return subFolders.first(); }
    const QList<File>& getFiles() const { return files; }

    qint64 getSize() const {
        qint64 totalSize = 0;
        for (const auto& file : files) {
            totalSize += file.getSize();
        }
        for (const auto& folder : subFolders) {
            totalSize += folder.getSize();
        }
        return totalSize;
    }

    const QDateTime& getUpdateDate() const { return updateDate; }
    const std::optional<QString>& getHash() const { return hash; }

    static Folder createFromPath(const QString& path);

    void sortContents() {
        std::sort(subFolders.begin(), subFolders.end(), [](const Folder& a, const Folder& b) {
            return a.getName() < b.getName();
        });
        std::sort(files.begin(), files.end(), [](const File& a, const File& b) {
            return a.getName() < b.getName();
        });
    }

    void getAllFiles(QList<File*>& allFiles) {
        // Пример: добавление указателей на файлы в список
        for (File& file : files) {
            allFiles.append(&file); // Добавляем указатель на файл
        }

        // Рекурсивно добавляем файлы из подпапок
        for (Folder& subfolder : subFolders) {
            subfolder.getAllFiles(allFiles);
        }
    }

private:
    QString name;
    QList<Folder> subFolders;
    QList<File> files;
    QDateTime updateDate;
    std::optional<QString> hash;
};

#endif // FILE_H
