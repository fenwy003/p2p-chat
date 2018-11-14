#ifndef FILEMESSAGE_H
#define FILEMESSAGE_H
#include "message.h"
#include <QFile>

/**
 * @brief The FileMessage class represents a message containing a file.
 */
class FileMessage : public Message
{
public:
    /**
     * @brief FileMessage constructor
     * @param sender the sender of this message
     * @param filePath the path of the message file
     * @param fileData the raw data of the message file
     * @param timestamp the creation or received time of this message
     */
    FileMessage(const QString &sender, const QString &filePath, const QByteArray &fileData,
                const QDateTime &timestamp = QDateTime::currentDateTime());

    /**
     * @brief data convert data to the format required by the network.
     * @return a QByteArray containing a network compatible representation of this FileMessage.
     */
    QByteArray data() const override;

    /**
     * @brief filePath retrieve the message file's path
     * @return the message file's path
     */
    QString filePath() const;

    /**
     * @brief fileName retrieve the message file's name
     * @return the message file's name
     */
    QString fileName() const;

    /**
     * @brief fileData the message file raw data
     * @return the message file raw data
     */
    QByteArray fileData() const;

private:
    QString _filePath; /**< the message file's path. */
    QString _fileName; /**< the message file's name */
    QByteArray _fileData; /**< the message file's raw data. */
};

#endif // FILEMESSAGE_H
