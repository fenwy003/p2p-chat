#ifndef IMAGEMESSAGE_H
#define IMAGEMESSAGE_H
#include "message.h"
#include <QPixmap>

/**
 * @brief The ImageMessage class represents a message containing an image.
 */
class ImageMessage : public Message
{
public:
    /**
     * @brief ImageMessage constructor
     * @param sender the sender of this message
     * @param imagePath the path of the message image
     * @param image the message image
     * @param timestamp the creation or received time of this message
     */
    ImageMessage(const QString &sender, const QString &imagePath, const QPixmap &image,
                 const QDateTime &timestamp = QDateTime::currentDateTime());

    /**
     * @brief data convert data to the format required by the network.
     * @return a QByteArray containing a network compatible representation of this ImageMessage.
     */
    QByteArray data() const override;

    /**
     * @brief imagePath retrieve the message image's path
     * @return the message image's path
     */
    QString imagePath() const;

    /**
     * @brief imageName retrieve the message image's name
     * @return the message image's name
     */
    QString imageName() const;

    /**
     * @brief image retrieve the message image
     * @return the message image
     */
    QPixmap image() const;

    /**
     * @brief imageData the message image raw data
     * @return the message image raw data
     */
    QByteArray imageData() const;

private:
    QString _imagePath; /**< the message image's file path. */
    QString _imageName; /**< the message image's file name */
    QPixmap _image; /**< the message image. */
    QByteArray _imageData; /**< the message image's raw data. */
};

#endif // IMAGEMESSAGE_H
