#ifndef PRIVATECHATROOM_H
#define PRIVATECHATROOM_H
#include <QString>
#include <QList>

/**
 * @brief The PrivateChatRoom class acts as an information holder that holds
 * the name and names of participants of a private chartroom.
 */
class PrivateChatRoom
{
public:
    /**
     * @brief PrivateChatRoom constructor
     * @param roomName the room's name
     */
    PrivateChatRoom(QString roomName);

    /**
     * @brief setRoomName replaces/updates the current room name with a new name
     * @param roomName the room name to set to
     */
    void setRoomName(QString roomName);

    /**
     * @brief roomName get the private room's name
     * @return private room's name
     */
    QString roomName() const;

    /**
     * @brief addParticipant adds the name of a recently joined participant
     * @param partName the name of the recently joined participant
     */
    void addParticipant(QString partName);

    /**
     * @brief removeParticipant removes the participant at the specified index
     * @param removeIndex the index of the removing participant
     */
    void removeParticipant(int removeIndex);

    /**
     * @brief participantNames get the participants of the private chatroom
     * @return a list of participant names
     */
    QList<QString> participantNames() const;

private:
    QString _roomName; /**< the private room's name. */
    QList<QString> _participantNames; /**< the list of private room's participant names. */
};

#endif // PRIVATECHATROOM_H
