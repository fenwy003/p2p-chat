#ifndef ACTIONMESSAGE_H
#define ACTIONMESSAGE_H
#include "message.h"

/**
 * @brief The ActionMessage class represents a message thats is being sent to present an action
 * performed in a private chatroom. The action includes JOIN, INVITE, KICK and LEAVE.
 */
class ActionMessage : public Message
{
public:
    /**
     * @brief ActionMessage constructor for LEAVE action
     * @param sender the sender of this message
     * @param action the action of this message
     * @param targetRoom the targeting room name
     * @param timestamp the creation or received time of this message
     */
    ActionMessage(const QString &sender, const QString action, const QString targetRoom,
                  const QDateTime &timestamp = QDateTime::currentDateTime());

    /**
     * @brief ActionMessage constructor for JOIN, INVITE and KICK actions
     * @param sender the sender of this message
     * @param action the action of this message
     * @param targetRoom the targeting room name
     * @param targetUser the targeting user's name
     * @param timestamp the creation or received time of this message
     */
    ActionMessage(const QString &sender, const QString action, const QString targetRoom,
                  const QString targetUser, const QDateTime &timestamp = QDateTime::currentDateTime());

    /**
     * @brief ActionMessage constructor for JOIN action coming back from invitee to inviter
     * @param sender the sender of this message
     * @param action the action of this message
     * @param targetRoom the targeting room name
     * @param identifier1 the invitee's username
     * @param identifier2 the inviter's username
     * @param timestamp the creation or received time of this message
     */
    ActionMessage(const QString &sender, const QString action, const QString targetRoom,
                  const QString identifier1, const QString identifier2, const QDateTime &timestamp = QDateTime::currentDateTime());

    /**
     * @brief data convert data to the format required by the network.
     * @return a QByteArray containing a network compatible representation of this FileMessage.
     */
    QByteArray data() const override;

    /**
     * @brief action get the action of the action message
     * @return the action
     */
    QString action() const;

    /**
     * @brief targetRoom get the target room of the action message
     * @return the targeting room's name
     */
    QString targetRoom() const;

    /**
     * @brief targetUser get the target user of the action message
     * @return the targeting user's username
     */
    QString targetUser() const;

    /**
     * @brief inviter get the name of the client who have sent an INVITE action message
     * @return inviter name
     */
    QString inviter() const;

    /**
     * @brief setTargetUser set the targetUser of the action message
     * @param target the username of the targeting user
     */
    void setTargetUser(QString target);

private:
    QString _action; /**< the message action. */
    QString _targetRoom; /**< the message targeting room name. */
    QString _targetUser; /**< the message targeting user name. */
    QString _inviter; /**< JOIN action messages' origin client name. */
};

#endif // ACTIONMESSAGE_H
