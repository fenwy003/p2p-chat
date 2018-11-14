#ifndef PRIVATEMESSAGE_H
#define PRIVATEMESSAGE_H
#include "message.h"

/**
 * @brief The PrivateMessage class is a message sent to a private chatroom or a specific user.
 */
class PrivateMessage : public Message
{
public:
    /**
     * @brief PrivateMessage constructor
     * @param sender the sender of this message
     * @param target the receiver of this message
     * @param message the message text
     * @param timestamp the creation or received time of this message
     */
    PrivateMessage(const QString &sender, const QString &target, const QString &message,
                   const QDateTime &timestamp = QDateTime::currentDateTime());

    /**
    * @brief data convert data to the format required by the network.
    * @return a QByteArray containing a network compatible representation of this PrivateMessage.
    */
    QByteArray data() const override; // using override here is best practice.

    /**
    * @brief message retrieve the message receipient
    * @return the message receipient
    */
    QString target() const;

    /**
    * @brief message retrieve the message text
    * @return the message text
    */
    QString message() const;

private:
    QString _target; /**< the message receipient. */
    QString _message; /**< the message text. */
};

#endif // PRIVATEMESSAGE_H
