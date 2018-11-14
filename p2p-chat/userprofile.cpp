#include "userprofile.h"

UserProfile::UserProfile(){
}

UserProfile::UserProfile(QPixmap profilePhoto, QString nickname, QString location, QString timezone)
    :_profilePhoto(profilePhoto), _nickname(nickname), _location(location), _timezone(timezone)
{
}

QPixmap UserProfile::profilePhoto() const {
    return _profilePhoto;
}

QString UserProfile::nickname() const {
    return _nickname;
}

QString UserProfile::location() const {
    return _location;
}

QString UserProfile::timezone() const {
    return _timezone;
}

void UserProfile::setProfilePhoto(QPixmap photo){
    _profilePhoto = photo;
}

void UserProfile::setNickname(QString nick) {
    _nickname = nick;
}

void UserProfile::setLocation(QString location) {
    _location = location;
}

void UserProfile::setTimezone(QString timezone) {
    _timezone = timezone;
}
