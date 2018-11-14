#include "privatemessage.h"

PrivateMessage::PrivateMessage(const QString &sender, const QString &target, const QString &message,
                               const QDateTime &timestamp)
    : Message(sender, timestamp), _target(target), _message(message) {}

QByteArray PrivateMessage::data() const {
    // The data across the network must start with the type identifier
    QByteArray data = QByteArray::number(Message::PrivateMessage);
    // then a separator
    data += Message::Separator;
    // Then the name of the receiver of this message.
    data += _target;
    // then a separator
    data += Message::Separator;
    // Then the data required for the message type.
    data += _message.toUtf8(); // Use QString::toUtf8() to encode all strings for sending.
    return data;
}

QString PrivateMessage::target() const {
    return _target;
}

QString PrivateMessage::message() const {
    return _message;
}
