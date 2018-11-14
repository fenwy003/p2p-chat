#ifndef IDENTITYMESSAGE_H
#define IDENTITYMESSAGE_H
#include "message.h"
#include "userprofile.h"

/**
 * @brief The IdentityMessage class represents a identify message that is being sent whenever
 * a new user joins a room or a user change their profile.
 */
class IdentityMessage : public Message
{
public:
    /**
     * @brief IdentityMessage constructor
     * @param sender the sender of this message
     * @param profile the profile of this message's sender
     * @param timestamp the creation or received time of this message
     */
    IdentityMessage(const QString &sender, const UserProfile &profile,
                    const QDateTime &timestamp = QDateTime::currentDateTime());

    /**
     * @brief data convert data to the format required by the network.
     * @return a QByteArray containing a network compatible representation of this ImageMessage.
     */
    QByteArray data() const override;

    /**
     * @brief profile gets the identity message's associated user profile
     * @return message's associating user profile
     */
    UserProfile profile() const;

    /**
     * @brief photoDatagets the raw data of identity message's sender user profile
     * @return message's associating user profile photo's raw data
     */
    QByteArray photoData() const;

private:
    UserProfile _profile; /**< the message sender's profile. */
    QByteArray _photoData; /**< the message sender's profile photo raw data. */
};

#endif // IDENTITYMESSAGE_H
