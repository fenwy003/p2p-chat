#include "actionmessage.h"

ActionMessage::ActionMessage(const QString &sender, const QString action, const QString targetRoom,
                             const QDateTime &timestamp)
    : Message(sender, timestamp), _action(action), _targetRoom(targetRoom) {
}

ActionMessage::ActionMessage(const QString &sender, const QString action, const QString targetRoom,
                             const QString targetUser, const QDateTime &timestamp)
    : Message(sender, timestamp), _action(action), _targetRoom(targetRoom), _targetUser(targetUser) {
}

ActionMessage::ActionMessage(const QString &sender, const QString action, const QString targetRoom,
                             const QString identifier1, const QString identifier2, const QDateTime &timestamp)
    : Message(sender, timestamp), _action(action), _targetRoom(targetRoom), _targetUser(identifier1), _inviter(identifier2) {
}

QByteArray ActionMessage::data() const {
    // The data across the network must start with the type identifier
    QByteArray data = QByteArray::number(Message::ActionMessage);
    // then a separator
    data += Message::Separator;
    // then the action name
    data += _action;
    // then a separator
    data += Message::Separator;
    // then the room name
    data +=  _targetRoom;
    // Only LEAVE do not needs a data field for target user
    if (_action != "LEAVE") {
        data += Message::Separator;
        data += _targetUser;
        if (_inviter != "") {
            data += Message::Separator;
            data += _inviter;
        }
    }
    return data;
}

QString ActionMessage::action() const {
    return _action;
}

QString ActionMessage::targetRoom() const {
    return _targetRoom;
}

QString ActionMessage::targetUser() const {
    return _targetUser;
}

QString ActionMessage::inviter() const {
    return _inviter;
}

void ActionMessage::setTargetUser(QString target) {
    _targetUser = target;
}
