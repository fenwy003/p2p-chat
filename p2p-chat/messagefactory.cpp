#include "messagefactory.h"
#include "textmessage.h"
#include "imagemessage.h"
#include "filemessage.h"
#include "privatemessage.h"
#include "identitymessage.h"
#include "actionmessage.h"
#include <QDebug>
#include <QFile>
#include <QPixmap>


MessageFactory::MessageFactory() {
}

QSharedPointer<Message> MessageFactory::create(const QString &sender, const QByteArray &data,
                                               int type) const {
    switch (type) {
    case Message::TextMessage:
    {
        // As with std::shared_ptr, a QSharedPointer to a base class can hold a derived class pointer.
        return QSharedPointer<Message>(new TextMessage(sender, QString::fromUtf8(data)));
    }
    case Message::ImageMessage:
    {
        int separatorIndex = data.indexOf('|');
        QString imgName;
        for (int i=0; i < separatorIndex; ++i) {
            imgName += data.at(i);
        }

        QByteArray imgData;
        for (int i = separatorIndex+1; i < data.size(); ++i) {
            imgData += data.at(i);
        }

        QPixmap img;
        img.loadFromData(imgData);
        return QSharedPointer<Message>(new ImageMessage(sender, imgName, img));
    }
    case Message::FileMessage:
    {
        int separatorIndex = data.indexOf('|');
        QString fileName;
        for (int i=0; i < separatorIndex; ++i) {
            fileName += data.at(i);
        }

        QByteArray fileData;
        for (int i = separatorIndex+1; i < data.size(); ++i) {
            fileData += data.at(i);
        }

        return QSharedPointer<Message>(new FileMessage(sender, fileName, fileData));
    }
    case Message::PrivateMessage:
    {
        QList<QByteArray> splitData = data.split('|');
        QString target = splitData.at(0);
        QString message = splitData.at(1);
        return QSharedPointer<Message>(new PrivateMessage(sender, target, message));
    }
    case Message::IdentityMessage:
    {
        QList<QByteArray> splitData = data.split('|');
        QString nickname = splitData.at(0);
        QString location = splitData.at(1);
        QString timezone = splitData.at(2);  
        int timezoneSeparatorIndex;

        if (location == "" && timezone == "") {
            timezoneSeparatorIndex = nickname.size() + 2;
        }
        else if (location == "" && timezone!="") {
            timezoneSeparatorIndex = nickname.size() + timezone.size() + 2;
        }
        else if (location != "" && timezone == "") {
            timezoneSeparatorIndex = nickname.size() + location.size() + 2;
        }
        else {
            timezoneSeparatorIndex = nickname.size() + location.size() + timezone.size()+2;
        }

        QByteArray photoData;
        for (int i = timezoneSeparatorIndex + 1; i < data.size(); ++i) {
            photoData += data.at(i);
        }

        QPixmap profilePhoto;
        profilePhoto.loadFromData(photoData);
        UserProfile profile(profilePhoto, nickname, location, timezone);

        return QSharedPointer<Message>(new IdentityMessage(sender, profile));
    }
    case Message::ActionMessage:
    {
        QList<QByteArray> splitData = data.split('|');
        QString action = splitData.at(0);
        QString targetRoom = splitData.at(1);
        if (action != "LEAVE") {
            QString targetUser = splitData.at(2);
            // If splitData size is 4 then it is a JOIN message from Invitee back to Inviter
            if (splitData.size() == 4) {
                QString inviter = splitData.at(3);
                return QSharedPointer<Message>(new ActionMessage(sender, action, targetRoom, targetUser, inviter));
            }
            else {
                return QSharedPointer<Message>(new ActionMessage(sender, action, targetRoom, targetUser));
            }
        }
        else {
            return QSharedPointer<Message>(new ActionMessage(sender, action, targetRoom));
        }
    }
    }
    return nullptr;
}

void MessageFactory::runTest() {
    QByteArray imageData;
    {
        // This reads a file from the resources.
        // See Qt documentation on The Qt Resource System.
        QFile imageFile(":/test/unisa-48px.png");
        if (imageFile.open(QIODevice::ReadOnly)) {
            imageData = imageFile.readAll();
            imageFile.close();
        }
    }

    QByteArray fileData;
    {
        // This reads a file from the resources.
        // See Qt documentation on The Qt Resource System.
        QFile file(":/test/Helpful Links.docx");
        if (file.open(QIODevice::ReadOnly)) {
            fileData = file.readAll();
            file.close();
        }
    }

    // Example data for each message type note you should always use the defined constants, not hard
    // coded strings for generating these (see TextMessage).
    QByteArray textMessageData = QByteArrayLiteral("1|Hello World!");
    QByteArray identityMessageData = QByteArrayLiteral("0|David|Adelaide|+930|") + imageData;
    QByteArray imageMessageData = QByteArrayLiteral("4|unisa.png|") + imageData;
    QByteArray fileMessageData = QByteArrayLiteral("3|Helpful Links.docx|") + fileData;
    QByteArray actionMessageData = QByteArrayLiteral("2|JOIN|Room Name|David@10.1.1.10");
    QByteArray privateMessageData = QByteArrayLiteral("5|David@10.1.1.10|Hi, David!");

    MessageFactory factory;

    QByteArray data = stripTypeId(textMessageData);
    QSharedPointer<Message> textMessage = factory.create("none", data, Message::TextMessage);
    data = stripTypeId(identityMessageData);
    QSharedPointer<Message> identityMessage = factory.create("none", data, Message::IdentityMessage);
    data = stripTypeId(imageMessageData);
    QSharedPointer<Message> imageMessage = factory.create("none", data, Message::ImageMessage);
    data = stripTypeId(fileMessageData);
    QSharedPointer<Message> fileMessage = factory.create("none", data, Message::FileMessage);
    data = stripTypeId(actionMessageData);
    QSharedPointer<Message> actionMessage = factory.create("none", data, Message::ActionMessage);
    data = stripTypeId(privateMessageData);
    QSharedPointer<Message> privateMessage = factory.create("none", data, Message::PrivateMessage);

    // Without a complete implementation of message types, only test can be to see if a valid pointer
    // was created by MessageFactory::create();
    if (textMessage == nullptr) {
        qDebug() << "TextMessage is not implemented in MessageFactory::create().";
    }
    if (identityMessage == nullptr) {
        qDebug() << "IdentityMessage is not implemented in MessageFactory::create().";
    }
    if (imageMessage == nullptr) {
        qDebug() << "ImageMessage is not implemented in MessageFactory::create().";
    }
    if (fileMessage == nullptr) {
        qDebug() << "FileMessage is not implemented in MessageFactory::create().";
    }
    if (actionMessage == nullptr) {
        qDebug() << "ActionMessage is not implemented in MessageFactory::create().";
    }
    if (privateMessage == nullptr) {
        qDebug() << "PrivateMessage is not implemented in MessageFactory::create().";
    }
}

QByteArray MessageFactory::stripTypeId(const QByteArray &array) {
    int pos = array.indexOf('|');
    return array.mid(pos + 1);
}
