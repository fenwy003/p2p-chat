#ifndef USERPROFILE_H
#define USERPROFILE_H
#include <QPixmap>

/**
 * @brief The UserProfile class represents an user's profile. It contains details such as
 * a profile photo and user's nickname, location, and timezone.
 */
class UserProfile
{
public:
    /**
     * @brief UserProfile empty constructor
     */
    UserProfile();

    /**
     * @brief UserProfile constructor with parameters
     * @param profilePhoto the user's profile photo
     * @param nickname the user's nickname
     * @param location the user's location
     * @param timezone the user's timezone
     */
    UserProfile(QPixmap profilePhoto, QString nickname,
                QString location, QString timezone);

    /**
     * @brief profilePhoto get user's current profile photo
     * @return user's current profile photo
     */
    QPixmap profilePhoto() const;

    /**
     * @brief nickname get user's current nickname
     * @return user's current nickname
     */
    QString nickname() const;

    /**
     * @brief location get user's current location info
     * @return user's current location info
     */
    QString location() const;

    /**
     * @brief timezone get user's current timezone info
     * @return user's current timezone info
     */
    QString timezone() const;

    /**
     * @brief setProfilePhoto replace old profile photo with new profile photo
     * @param photo the new profile photo
     */
    void setProfilePhoto(QPixmap photo);

    /**
     * @brief setNickname replace old nickname with new nickname
     * @param nick the new nickname
     */
    void setNickname(QString nick);

    /**
     * @brief setLocation replace old location info with new location info
     * @param location the new location info
     */
    void setLocation(QString location);

    /**
     * @brief setTimezone replace old timezone info with new timezone info
     * @param timezone the new timezone info
     */
    void setTimezone(QString timezone);

private:
    QPixmap _profilePhoto; /**< the message text. */
    QString _nickname; /**< the message text. */
    QString _location; /**< the message text. */
    QString _timezone; /**< the message text. */
};

#endif // USERPROFILE_H
