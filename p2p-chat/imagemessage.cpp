#include "imagemessage.h"
#include <QBuffer>

ImageMessage::ImageMessage(const QString &sender, const QString &imagePath, const QPixmap &image,
                           const QDateTime &timestamp) : Message(sender, timestamp),
    _imagePath(imagePath), _image(image) {
    // Extract the image name from its path
    QStringList splitPath = imagePath.split('/');
    _imageName = splitPath[splitPath.size()-1];

    // Extract the raw data of the QPixmap
    QBuffer buffer(&_imageData);
    if (_imagePath.endsWith(".jpg"), Qt::CaseInsensitive ||
            _imagePath.endsWith(".jpeg"), Qt::CaseInsensitive) {
        image.save(&buffer, "JPEG");
    }
    else {
        image.save(&buffer, "PNG");
    }
}

QByteArray ImageMessage::data() const {
    // The data across the network must start with the type identifier
    QByteArray data = QByteArray::number(Message::ImageMessage);
    // then a separator
    data += Message::Separator;
    // Then the data required for the message type.
    data += _imageName;
    // then a separator
    data += Message::Separator;
    // then image raw data
    data += _imageData;
    return data;
}

QString ImageMessage::imagePath() const {
    return _imagePath;
}

QString ImageMessage::imageName() const {
    return _imageName;
}

QPixmap ImageMessage::image() const {
    return _image;
}

QByteArray ImageMessage::imageData() const {
    return _imageData;
}
