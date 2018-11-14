#include "filemessage.h"

FileMessage::FileMessage(const QString &sender, const QString &filePath, const QByteArray &fileData,
                         const QDateTime &timestamp)
    : Message(sender, timestamp), _filePath(filePath) ,_fileData(fileData)
{
    // Extract the file's name from its path
    QStringList splitPath = _filePath.split('/');
    _fileName = splitPath[splitPath.size()-1];
}

QByteArray FileMessage::data() const {
    // The data across the network must start with the type identifier
    QByteArray data = QByteArray::number(Message::FileMessage);
    // then a separator
    data += Message::Separator;
    // Then the data required for the message type.
    data += _fileName;
    // then a separator
    data += Message::Separator;
    // then image raw data
    data += _fileData;
    return data;
}

QString FileMessage::filePath() const {
    return _filePath;
}

QString FileMessage::fileName() const {
    return _fileName;
}

QByteArray FileMessage::fileData() const {
    return _fileData;
}
