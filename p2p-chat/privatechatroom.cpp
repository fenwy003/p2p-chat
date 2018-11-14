#include "privatechatroom.h"

PrivateChatRoom::PrivateChatRoom(QString roomName) : _roomName(roomName)
{
}

void PrivateChatRoom::setRoomName(QString roomName) {
    _roomName = roomName;
}

QString PrivateChatRoom::roomName() const {
    return _roomName;
}

void PrivateChatRoom::addParticipant(QString partName) {
    _participantNames.append(partName);
}

void PrivateChatRoom::removeParticipant(int removeIndex) {
    _participantNames.removeAt(removeIndex);
}

QList<QString> PrivateChatRoom::participantNames() const {
    return _participantNames;
}
