#include "file.h"

Folder Folder::createFromPath(const QString& path) {
    QDir dir(path);
    if (!dir.exists()) {
        throw std::runtime_error("Path does not exist or is not a directory");
    }

    auto lastWriteTime = QFileInfo(dir.absolutePath()).lastModified();
    Folder folder(dir.dirName(), lastWriteTime);

    for (const auto& entry : dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden)) {
        if (entry.isDir()) {
            folder.addSubFolder(Folder::createFromPath(entry.absoluteFilePath()));
        } else if (entry.isFile()) {
            auto fileSize = entry.size();
            auto fileLastWriteTime = entry.lastModified();
            folder.addFile(File(entry.fileName(), fileSize, fileLastWriteTime, entry.absoluteFilePath()));
        }
    }

    folder.sortContents();

    return folder;
}
