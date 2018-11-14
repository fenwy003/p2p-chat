#include "identitymessage.h"
#include <QBuffer>

IdentityMessage::IdentityMessage(const QString &sender, const UserProfile &profile,
                                 const QDateTime &timestamp)
    : Message(sender, timestamp), _profile(profile)
{
    // Extract the raw data of the QPixmap (user profile photo)
    QBuffer buffer(&_photoData);
    _profile.profilePhoto().save(&buffer, "PNG");
}

QByteArray IdentityMessage::data() const{
    // The data across the network must start with the type identifier
    QByteArray data = QByteArray::number(Message::IdentityMessage);
    // then a separator
    data += Message::Separator;
    // Then the user's nickname
    data += _profile.nickname();
    // then a separator
    data += Message::Separator;
    // then user's location
    data += _profile.location();
    // then a separator
    data += Message::Separator;
    // then user's timezone
    data += _profile.timezone();
    // then a separator
    data += Message::Separator;
    // then user's profile photo raw data
    data += _photoData;
    return data;
}

UserProfile IdentityMessage::profile() const {
    return _profile;
}

QByteArray IdentityMessage::photoData() const {
    return _photoData;
}
