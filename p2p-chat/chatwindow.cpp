#include "htmldelegate.h"
#include "messagefactory.h"
#include "textmessage.h"
#include "imagemessage.h"
#include "filemessage.h"
#include "identitymessage.h"
#include "privatemessage.h"
#include "actionmessage.h"

#include "chatwindow.h"
#include "imagedisplaywindow.h"
#include "privatechatdialog.h"
#include "privateroomdialog.h"
#include "privatesenddialog.h"
#include "roomactionsdialog.h"
#include "userprofiledialog.h"

#include "ui_chatwindow.h"
#include "ui_imagedisplaywindow.h"
#include "ui_privatechatdialog.h"
#include "ui_privateroomdialog.h"
#include "ui_privatesenddialog.h"
#include "ui_roomactionsdialog.h"
#include "ui_userprofiledialog.h"

#include <QFileDialog>
#include <QHostInfo>
#include <QMessageBox>

ChatWindow::ChatWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::ChatWindow) {
    ui->setupUi(this);
    connect(&client, &p2pnetworking::Client::newMessage, this, &ChatWindow::handleMessage);
    connect(&client, &p2pnetworking::Client::newParticipant, this, &ChatWindow::newParticipant);
    connect(&client, &p2pnetworking::Client::participantLeft, this, &ChatWindow::participantLeft);
    ui->listView->setModel(&historyModel);
    // The HTMLDelegate allows display of HTML formatted text, a subset of HTML is supported.
    // See http://doc.qt.io/qt-5/richtext-html-subset.html - also can be found in built-in help.
    HTMLDelegate *delegate = new HTMLDelegate(this);
    ui->listView->setItemDelegate(delegate);
}

ChatWindow::~ChatWindow() {
    delete ui;
}

void ChatWindow::handleMessage(QSharedPointer<Message> message) {

    if (QSharedPointer<TextMessage> txt = qSharedPointerDynamicCast<TextMessage>(message)) {
        appendMessage(txt->sender(), txt->message());
    }
    else if (QSharedPointer<ImageMessage> img = qSharedPointerDynamicCast<ImageMessage>(message)) {
        appendMessage(img->sender(), "sent " + img->imageName());
        // Setting up the QVariant which holds the message's image
        QVariant image;
        QPixmap pix = img->image();
        image.setValue(pix);
        // Then create the listItem with the QVariant image
        QListWidgetItem *imageItem = new QListWidgetItem();
        imageItem->setText(img->imageName());
        imageItem->setData(Qt::UserRole, image);
        // Finally, adds the list item to image attachment list in the ui.
        ui->imageAttachmentsList->addItem(imageItem);
    }
    else if (QSharedPointer<FileMessage> file = qSharedPointerDynamicCast<FileMessage>(message)) {
        appendMessage(file->sender(), "sent " + file->fileName());
        // Setting up the QVariant which holds the message's file data
        QVariant sentFile;
        QByteArray sentFileData = file->fileData();
        sentFile.setValue(sentFileData);
        // Then create the listItem with the QVariant sentFile
        QListWidgetItem *fileItem = new QListWidgetItem();
        fileItem->setText(file->fileName());
        fileItem->setData(Qt::UserRole, sentFile);
        // Finally, adds the list item to file attachment list in the ui.
        ui->fileAttachmentsList->addItem(fileItem);
    }
    else if (QSharedPointer<IdentityMessage> identity = qSharedPointerDynamicCast<IdentityMessage>(message)) {
        if (identity->sender() != client.nickName()) {
            QList<QVariant> profileDetails;
            QVariant profilePhoto(identity->profile().profilePhoto());
            QVariant nickname(identity->profile().nickname());
            QVariant location(identity->profile().location());
            QVariant timezone(identity->profile().timezone());
            profileDetails.append(profilePhoto);
            profileDetails.append(nickname);
            profileDetails.append(location);
            profileDetails.append(timezone);

            QList<QListWidgetItem*> users = ui->participantsListWidget->findItems(identity->sender(),
                                                                                  Qt::MatchExactly);
            QVariant userProfile(profileDetails);
            users.at(0)->setData(Qt::UserRole, userProfile);
        }
    }
    else if (QSharedPointer<PrivateMessage> privateMes = qSharedPointerDynamicCast<PrivateMessage>(message)) {
        // Private message to a user
        if (privateMes->target().contains('@')) {
            // Evaluate client name with message receipient name
            QList<QString> splitedName = privateMes->target().split('@');
            QString target = splitedName.at(0) + "@" + QHostInfo::localHostName();
            if (client.nickName() == target) {
                // If the private message is for a single user
                if (!privateMes->message().endsWith("</font>")) {
                    // Set display color to red to indicate single target private message
                    QString sender = "<font color=\"red\">";
                    sender += "[Whisper]"+ privateMes->sender() + "</font>";
                    QString text = "<font color=\"red\">";
                    text += privateMes->message() + "</font>";
                    appendMessage(sender, text);
                }
                // If the private message is for users of a private room
                else {
                    // Deattach room name from message text
                    QList<QString> splitedText = privateMes->message().split('/');
                    QString text = splitedText.at(1);
                    // If / appears more than once then adds the rest of the splited text to text
                    if (splitedText.size() > 2) {
                        for (int x = 2; x < splitedText.size(); ++x) {
                            text += "/" + splitedText.at(x);
                        }
                    }
                    // Attach green color open tag to sender along with room name to indicate private room message
                    QString sender = "<font color=\"green\">";
                    sender += "[" + splitedText.at(0) + "]";
                    sender += privateMes->sender() + "</font>";
                    appendMessage(sender, text);
                }
            }
        }
        // Private message to a private chat room
        else {
            // Gets the participant of the targeting room
            QList<QString> participants;
            for (int i = 0; i < _privateChats.size(); ++i) {
                if (_privateChats.at(i).roomName() == privateMes->target()) {
                    participants = _privateChats.at(i).participantNames();
                }
            }
            if (participants.size() != 0) {
                // Attach room name to sending text as private message to single user have no way of knowing the room name.
                QString text = privateMes->target() + "/";
                // Attach green font color html tag to indicate a private group message
                text += "<font color=\"green\">";
                text += privateMes->message() + "</font>";
                // Send a private message to each of the participants
                for (int i = 0; i < participants.size(); ++i) {
                    QString receiver = participants.at(i);
                    QSharedPointer<PrivateMessage> mess(new PrivateMessage(privateMes->sender(), receiver, text));
                    client.sendMessage(mess);
                }
                // Attach green font color html tag to indicate a private group message
                QString sender = "<font color=\"green\">";
                sender += "[" + privateMes->target() + "]";
                sender += client.nickName() + "</font>";
                appendMessage(sender, ("<font color=\"green\">" + privateMes->message() + "</font>"));
            }
        }
    }
    else if (QSharedPointer<ActionMessage> actionMes = qSharedPointerDynamicCast<ActionMessage>(message)) {
        if (actionMes->action() == "INVITE") {
            // Create a private chatroom based on the invite message
            PrivateChatRoom room(actionMes->targetRoom());
            // Add the room to the corresponding list widget
            ui->roomListWidget->addItem(actionMes->targetRoom());
            // Enable private room action menu
            ui->menuRoom_Actions->setEnabled(true);
            // Adds the inviter to the room
            room.addParticipant(actionMes->sender());
            // Adds the invitee to the room
            room.addParticipant(client.nickName());
            // Adds the room to invitee's list of private chat rooms
            _privateChats.append(room);
            // Notify the invitee about the invite
            QString text = "<font color=\"green\">" + actionMes->sender() +" invited you to ";
            text += actionMes->targetRoom() + "!</font>";
            appendMessage("<font color=\"green\">System</font>", text);
            // Sends an join message back to the inviter
            QSharedPointer<ActionMessage> message(new ActionMessage(client.nickName(), "JOIN", actionMes->targetRoom(),
                                                                    actionMes->targetUser(), actionMes->sender()));
            client.sendMessage(message, actionMes->sender());
        }
        else if (actionMes->action() == "JOIN") {
            // Evaluation variables for Inviter receiving JOIN from Invitee
            QList<QString> splitInviterName = actionMes->inviter().split('@');
            QString inviter = splitInviterName.at(0) + "@" + QHostInfo::localHostName();

            // Evaluation variables for other participants receiving JOIN from Inviter
            QList<QString> splitedSenderName = actionMes->sender().split('@');
            QString senderName = splitedSenderName.at(0) + "@" + QHostInfo::localHostName();
            QList<QString> splitedInviteeName = actionMes->targetUser().split('@');
            QString inviteeName = splitedInviteeName.at(0) + "@" + QHostInfo::localHostName();

            // JOIN from invitee back to the inviter
            if (client.nickName() == inviter) {
                // Adds the invitee to the corresponding private room
                int matchingRoomIndex = findRoomIndex(actionMes->targetRoom());
                _privateChats[matchingRoomIndex].addParticipant(actionMes->targetUser());

                // Notify the inviter about the invitee's join
                QString text = "<font color=\"green\">" + actionMes->sender() +" joined ";
                text += actionMes->targetRoom() + "!</font>";
                appendMessage("<font color=\"green\">System</font>", text);

                // The the inviter sends a JOIN message based on the invitee to other room participants
                QSharedPointer<ActionMessage> newJoinMessage(new ActionMessage(actionMes->inviter(), "JOIN", actionMes->targetRoom(),
                                                                               actionMes->sender()));
                QList<QString> participants = _privateChats.at(matchingRoomIndex).participantNames();
                for (int i = 0; i < participants.size(); ++i) {
                    // Avoid sending JOIN to inviter and invitee
                    if (participants.at(i) != inviter
                            && participants.at(i) != actionMes->sender()) {
                        QString receiver = participants.at(i);
                        client.sendMessage(newJoinMessage, receiver);
                    }
                }
            }
            // Other participant receiving JOIN from Inviter
            else if (client.nickName() != senderName && client.nickName() != inviteeName
                     && senderName != inviteeName) {
                // Other participant find the target room from their list of rooms
                int roomIndex = findRoomIndex(actionMes->targetRoom());
                if (roomIndex != -1) {
                    // Inviter adds Invitee to room's participant list
                    _privateChats[roomIndex].addParticipant(actionMes->targetUser());

                    // Other participant sends a JOIN message to the Invitee
                    QSharedPointer<ActionMessage> newJoinMessage(new ActionMessage(client.nickName(), "JOIN", actionMes->targetRoom(),
                                                                                   client.nickName()));
                    client.sendMessage(newJoinMessage, actionMes->targetUser());
                }
                // Notify other participant about the invitee's join
                QString text = "<font color=\"green\">" + actionMes->targetUser() +" joined ";
                text += actionMes->targetRoom() + "!</font>";
                appendMessage("<font color=\"green\">System</font>", text);
            }
            // Invitee receives JOIN message from other participants
            else {
                // Invitee finds the corresponding room from his/her list of rooms
                int roomIndex = findRoomIndex(actionMes->targetRoom());
                if (roomIndex != -1) {
                    // Invitee adds other participant to his/her list of participants
                    _privateChats[roomIndex].addParticipant(actionMes->sender());
                }
                // Notify invitee about join from other participants
                QString text = "<font color=\"green\">" + actionMes->sender() +" joined ";
                text += actionMes->targetRoom() + "!</font>";
                appendMessage("<font color=\"green\">System</font>", text);
            }
        }
        else if (actionMes->action() == "KICK") {
            QList<QString> splitTargetName = actionMes->targetUser().split('@');
            QString toBeKicked = splitTargetName.at(0) + "@" + QHostInfo::localHostName();
            // Finds the index of the targeting room
            int targetRoomIndex = findRoomIndex(actionMes->targetRoom());
            // Kick
            if(client.nickName() == toBeKicked) {
                _privateChats.removeAt(targetRoomIndex);
                QList<QListWidgetItem *> toRemove = ui->roomListWidget->findItems(actionMes->targetRoom(), Qt::MatchExactly);
                foreach (QListWidgetItem *item, toRemove) {
                    ui->roomListWidget->removeItemWidget(item);
                    delete item;
                }
                QString text = "<font color=\"green\">You got kicked out of ";
                text += actionMes->targetRoom() + "!</font>";
                appendMessage("<font color=\"green\">System</font>", text);
            }
            else {
                // Find sender index in the room's list of participants
                int targetIndex = -1;
                for (int x = 0; x < _privateChats.at(targetRoomIndex).participantNames().size(); ++x) {
                    if (_privateChats.at(targetRoomIndex).participantNames().at(x) == actionMes->targetUser()) {
                        targetIndex = x;
                    }
                }
                if (targetIndex != -1) {
                    // Remove sender from the room's list of participants by replacing room at roomIndex
                    PrivateChatRoom newRoom(actionMes->targetRoom());
                    for (int x = 0; x < targetIndex; ++x) {
                        newRoom.addParticipant(_privateChats.at(targetRoomIndex).participantNames().at(x));
                    }
                    for (int x = targetIndex + 1; x < _privateChats.at(targetRoomIndex).participantNames().size(); ++x) {
                        newRoom.addParticipant(_privateChats.at(targetRoomIndex).participantNames().at(x));
                    }
                    _privateChats.replace(targetRoomIndex, newRoom);
                    QString text = "<font color=\"green\">" + actionMes->targetUser();
                    text += " got kicked out of " + actionMes->targetRoom() + "!</font>";
                    appendMessage("<font color=\"green\">System</font>", text);
                }
            }
        }
        // LEAVE
        else {
            // Find the index of the targeting room
            int roomIndex = findRoomIndex(actionMes->targetRoom());
            // If targeting room exist
            if (roomIndex != -1) {
                // Finds sender's index in the room's list of participants
                QList<QString> participants = _privateChats.at(roomIndex).participantNames();
                int senderIndex = -1;
                for (int x = 0; x < participants.size(); ++x) {
                    if (participants.at(x) == actionMes->sender()) {
                        senderIndex = x;
                    }
                }
                if (senderIndex != -1) {
                    // Remove sender from the room's list of participants
                    _privateChats[roomIndex].removeParticipant(senderIndex);
                    QString text = "<font color=\"green\">" + actionMes->sender();
                    text += " left " + actionMes->targetRoom() + "</font>";
                    appendMessage("<font color=\"green\">System</font>", text);
                }
            }
        }
        // Disables room action menu if user is not in any private rooms
        if (ui->roomListWidget->count() == 0) {
            ui->menuRoom_Actions->setEnabled(false);
        }
    }
}

void ChatWindow::appendMessage(const QString &from, const QString &message) {
    QString newLine = "<strong style=\"width:150px\">";
    newLine += from;
    newLine += ":</strong>  ";
    newLine += message;
    chatHistory << newLine;
    while (chatHistory.size() > 100) {
        chatHistory.removeFirst();
    }
    historyModel.setStringList(chatHistory);
}

void ChatWindow::newParticipant(const QString &nick) {
    // Generates and send an identity message whenever a new user joins the chatroom
    QSharedPointer<IdentityMessage> message(new IdentityMessage(client.nickName(), _profile));
    client.sendMessage(message);
    handleMessage(message);
    ui->participantsListWidget->addItem(nick);
}

void ChatWindow::participantLeft(const QString &nick) {
    //Remove the leaving participant from participating rooms (if any/found)
    for (int i = 0; i < _privateChats.size(); ++i) {
        int leaverIndex = _privateChats.at(i).participantNames().indexOf(nick,0);
        if (leaverIndex != -1) {
            _privateChats[i].removeParticipant(leaverIndex);
            //Notify client about leaving user's leave
            QString text = "<font color=\"green\">" + nick;
            text += " left " + _privateChats.at(i).roomName() + "</font>";
            appendMessage("<font color=\"green\">System</font>", text);
        }
    }
    QList<QListWidgetItem *> toRemove = ui->participantsListWidget->findItems(nick, Qt::MatchExactly);
    foreach (QListWidgetItem *item, toRemove) {
        ui->participantsListWidget->removeItemWidget(item);
        delete item;
    }
}

void ChatWindow::on_pushButton_clicked() {
    // Ensure input do not contains the reserved keyword |
    if (!ui->lineEdit->text().contains('|')) {
        // Create the shared pointer containing a TextMessage.
        QSharedPointer<TextMessage> message(new TextMessage(client.nickName(), ui->lineEdit->text()));
        // Send to all connected users
        client.sendMessage(message);
        // Display for this computer (messages from this user are not passed to this user from the
        // network) - no point sending data to yourself!
        handleMessage(message);
        ui->lineEdit->clear();
    }
    else {
        reservedInputWarning();
        ui->lineEdit->clear();
    }
}

void ChatWindow::on_startButton_clicked() {
    // Ensure input do not contains the reserved keyword |
    if (!ui->usernameEdit->text().contains('|')) {
        client.setUserName(ui->usernameEdit->text());
        _profile.setNickname(ui->usernameEdit->text());
        client.start();
        ui->startButton->setEnabled(false);
        ui->usernameEdit->setEnabled(false);
        // Enable room functionality when user is online
        ui->menuChat_History->setEnabled(true);
        ui->menuSend->setEnabled(true);
        ui->menuPrivate_Chat->setEnabled(true);
    }
    else {
        reservedInputWarning();
        ui->usernameEdit->clear();
    }
}

void ChatWindow::on_profileSettings_triggered() {
    UserProfileDialog *upd = new UserProfileDialog();
    // When usernameEdit is disabled, it means the user is online
    if (!ui->usernameEdit->isEnabled()) {
        // Hence prevent nickname to be updated
        upd->toggleNicknameEdit(false);
    }
    upd->setProfile(_profile);
    upd->exec();
    _profile = upd->profile();

    // If the user's nickname in the main gui is different to the one in profile then
    // replace the nickname in the main gui with the one in his/her profile
    if (ui->usernameEdit->text() != _profile.nickname()) {
        ui->usernameEdit->setText(_profile.nickname());
    }
    // Generates and send an identity message whenever user closes their user profile dialog
    QSharedPointer<IdentityMessage> message(new IdentityMessage(client.nickName(), _profile));
    client.sendMessage(message);
    handleMessage(message);
}

void ChatWindow::on_participantsListWidget_itemClicked() {
    // Generate the clicked user's user profile window populated with his/her profile details
    UserProfileDialog *upd = new UserProfileDialog();
    upd->setWindowTitle(ui->participantsListWidget->currentItem()->text() + "'s Profile");
    upd->toggleWindowMode(false);
    QList<QVariant> profileDetails = ui->participantsListWidget->currentItem()->data(Qt::UserRole).value<QList<QVariant>>();
    QPixmap photo(profileDetails.at(0).value<QPixmap>());
    QString nickname(profileDetails.at(1).value<QString>());
    QString location(profileDetails.at(2).value<QString>());
    QString timezone(profileDetails.at(3).value<QString>());
    UserProfile profile(photo,nickname,location,timezone);
    upd->setProfile(profile);
    upd->exec();
}

void ChatWindow::on_actionExport_As_Text_Document_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Chat History As Text Document"), "",
                                                    tr("TXT (*.txt)"));
    if (!fileName.isNull()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            for (int i = 0; i < chatHistory.size(); ++i) {
                QString message = chatHistory.at(i);
                // Removes the strong html tag from the messages in chat history
                message.remove(QRegExp("<[^>]*>"));
                // Then write the plain text message to the output txt document
                stream << message + "\n";
            }
        }
        file.close();
    }
}

void ChatWindow::on_sendFilesAction_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Send File"), "",
                                                    tr("All (*.*)"));
    // Prevent empty file messages to be sent
    if (!filePath.isNull()) {
        // Extract the raw data of the to-be-sent file
        QByteArray fileData;
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            fileData = file.readAll();
            file.close();
        }

        QSharedPointer<FileMessage> message(new FileMessage(client.nickName(), filePath, fileData));
        client.sendMessage(message);
        handleMessage(message);
    }
}

void ChatWindow::on_sendImagesAction_triggered() {
    QString imagePath = QFileDialog::getOpenFileName(this,
                                                     tr("Send Image"), "",
                                                     tr("JPEG (*.jpg *.jpeg);;PNG (*png);;GIF (*.gif)"));
    // Prevent empty image messages to be sent
    if (!imagePath.isNull()) {
        QPixmap image;
        bool validImage = image.load(imagePath);
        // Send image messages only when the selected image can be loaded
        if (validImage == true) {
            QSharedPointer<ImageMessage> message(new ImageMessage(client.nickName(), imagePath, image));
            client.sendMessage(message);
            handleMessage(message);
        }
    }
}

void ChatWindow::on_actionSingleUser_Send_File_triggered()
{
    PrivateSendDialog *psd = new PrivateSendDialog();
    psd->setWindowTitle("Sending File to a Single Connected User");
    psd->exec();
    // Ignore send request if client did not provide an username
    if (psd->receiver() != "") {
        if (receiverExist(psd->receiver())) {
            QString filePath = QFileDialog::getOpenFileName(this,
                                                            tr("Send File"), "",
                                                            tr("All (*.*)"));
            // Prevent empty file messages to be sent
            if (!filePath.isNull()) {
                // Extract the raw data of the to-be-sent file
                QByteArray fileData;
                QFile file(filePath);
                if (file.open(QIODevice::ReadOnly)) {
                    fileData = file.readAll();
                    file.close();
                }
                // Prepare and send the file message
                QSharedPointer<FileMessage> message(new FileMessage(client.nickName(), filePath, fileData));
                client.sendMessage(message, psd->receiver());
                handleMessage(message);
            }
        }
        // Notify client about send failure
        else {
            QMessageBox::warning(this, "Send File Failed", "Invalid Recipient!",
                                 QMessageBox::Ok);
        }
    }
}

void ChatWindow::on_actionSingleUser_Send_Image_triggered()
{
    PrivateSendDialog *psd = new PrivateSendDialog();
    psd->setWindowTitle("Sending File to a Single Connected User");
    psd->exec();
    // Ignore send request if client did not provide an username
    if (psd->receiver() != "") {
        if (receiverExist(psd->receiver())) {
            QString imagePath = QFileDialog::getOpenFileName(this,
                                                             tr("Send Image"), "",
                                                             tr("JPEG (*.jpg *.jpeg);;PNG (*png);;GIF (*.gif)"));
            // Prevent empty image messages to be sent
            if (!imagePath.isNull()) {
                QPixmap image;
                bool validImage = image.load(imagePath);
                // Send image messages only when the selected image can be loaded
                if (validImage == true) {
                    QSharedPointer<ImageMessage> message(new ImageMessage(client.nickName(), imagePath, image));
                    client.sendMessage(message, psd->receiver());
                    handleMessage(message);
                }
            }
        }
        // Notify client about send failure
        else {
            QMessageBox::warning(this, "Send Image Failed", "Invalid Recipient!",
                                 QMessageBox::Ok);
        }
    }
}

void ChatWindow::on_actionSend_File_To_A_Room_triggered()
{
    //Modify the instruction label to match current context
    PrivateSendDialog *psd = new PrivateSendDialog();
    psd->setWindowTitle("Sending File to a Private Chat Room");
    psd->ui->instructionLabel->setText("Please Enter the Private Room Name:");
    psd->exec();
    // Ignore send request if client did not provide a room name
    if (psd->receiver() != "") {
        if (findRoomIndex(psd->receiver()) != -1) {
            QString filePath = QFileDialog::getOpenFileName(this,
                                                            tr("Send File"), "",
                                                            tr("All (*.*)"));
            // Prevent empty file messages to be sent
            if (!filePath.isNull()) {
                // Extract the raw data of the to-be-sent file
                QByteArray fileData;
                QFile file(filePath);
                if (file.open(QIODevice::ReadOnly)) {
                    fileData = file.readAll();
                    file.close();
                }

                QSharedPointer<FileMessage> message(new FileMessage(client.nickName(), filePath, fileData));
                // Finds the room participants
                QList<QString> participants;
                for (int i = 0; i < _privateChats.size(); ++i) {
                    if (_privateChats.at(i).roomName() == psd->receiver()) {
                        participants = _privateChats.at(i).participantNames();
                    }
                }
                // The sends message to each room participant
                for (int i = 0; i < participants.size(); ++i) {
                    QString receiver = participants.at(i);
                    client.sendMessage(message, receiver);
                }
                handleMessage(message);
            }
        }
        // Notify client about send failure
        else {
            QMessageBox::warning(this, "Send File Failed", "Invalid Private Chatroom Name!",
                                 QMessageBox::Ok);
        }
    }
}

void ChatWindow::on_actionSend_Image_To_A_Room_triggered()
{
    //Modify the instruction label to match current context
    PrivateSendDialog *psd = new PrivateSendDialog();
    psd->setWindowTitle("Sending File to a Private Chat Room");
    psd->ui->instructionLabel->setText("Please Enter the Private Room Name:");
    psd->exec();
    // Ignore send request if client did not provide a room name
    if (psd->receiver() != "") {
        if (findRoomIndex(psd->receiver()) != -1) {
            QString imagePath = QFileDialog::getOpenFileName(this,
                                                             tr("Send Image"), "",
                                                             tr("JPEG (*.jpg *.jpeg);;PNG (*png);;GIF (*.gif)"));
            // Prevent empty image messages to be sent
            if (!imagePath.isNull()) {
                QPixmap image;
                bool validImage = image.load(imagePath);
                // Send image messages only when the selected image can be loaded
                if (validImage == true) {
                    QSharedPointer<ImageMessage> message(new ImageMessage(client.nickName(), imagePath, image));
                    // Finds the room participants
                    QList<QString> participants;
                    for (int i = 0; i < _privateChats.size(); ++i) {
                        if (_privateChats.at(i).roomName() == psd->receiver()) {
                            participants = _privateChats.at(i).participantNames();
                        }
                    }

                    // The sends message to each room participant
                    for (int i = 0; i < participants.size(); ++i) {
                        QString receiver = participants.at(i);
                        client.sendMessage(message, receiver);
                    }
                    handleMessage(message);
                }
            }
        }
        // Notify client about send failure
        else {
            QMessageBox::warning(this, "Send Image Failed", "Invalid Private Chatroom Name!",
                                 QMessageBox::Ok);
        }
    }
}

void ChatWindow::on_fileAttachmentsList_itemClicked() {
    // Retreive the raw file data stored in the selected list widget item
    QByteArray selectedFileData = ui->fileAttachmentsList->currentItem()->data(Qt::UserRole).value<QByteArray>();
    // Let the user choose where to save the selected file and which saving file extension should be used
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
                                                    tr("Word Document(*.docx);;JPEG (*.jpg *.jpeg);;PNG (*.png);;GIF (*.gif)"
                                                       ";;Zipped Folder (*.zip);; Other - specify file extension(*.*)"));
    if (!fileName.isNull()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(selectedFileData);
        }
        file.close();
    }
}

void ChatWindow::on_imageAttachmentsList_itemClicked(){
    // Retreive the pixmap stored in the selected list widget item
    QPixmap selectedImg = ui->imageAttachmentsList->currentItem()->data(Qt::UserRole).value<QPixmap>();

    // Display the pixmap in a pop-up window
    ImageDisplayWindow *imgPopupWindow = new ImageDisplayWindow();
    imgPopupWindow->setWindowTitle(ui->imageAttachmentsList->currentItem()->text());
    imgPopupWindow->setDisplayImage(selectedImg);
    imgPopupWindow->show();
}

void ChatWindow::on_privateChatRoomAction_triggered()
{
    // Prompt for private chatroom name
    PrivateRoomDialog *prd = new PrivateRoomDialog();
    prd->exec();
    if (prd->roomName() != "") {
        // Check if a room with the given name already exist
        bool duplicateName = false;
        for (int i = 0; i < _privateChats.size(); ++i) {
            if (_privateChats.at(i).roomName() == prd->roomName()) {
                duplicateName = true;
            }
        }
        // Only create a new private room when input name is not duplicated
        if (duplicateName == false) {
            PrivateChatRoom room(prd->roomName());
            // Add the room to the corresponding listview
            ui->roomListWidget->addItem(prd->roomName());
            // Add the creator to the room's list of participant
            room.addParticipant(client.nickName());
            _privateChats.append(room);
            QMessageBox::information(this, "Create Private Room Successed", "Private Room has been created!",
                                     QMessageBox::Ok);
            //Enable private chatroom action menu
            ui->menuRoom_Actions->setEnabled(true);
        }
        // Notify user about failed room creation
        else {
            QMessageBox::warning(this, "Create Private Room Failed", "Duplicate Room Name",
                                 QMessageBox::Ok);
        }
    }
}

void ChatWindow::on_actionTo_Another_User_triggered()
{
    //Prompt for private message receiver name and message
    PrivateChatDialog *pcd = new PrivateChatDialog();
    pcd->setWindowTitle("Private Chat with a Single Connected User");
    pcd->exec();
    if (pcd->receiver() != "") {
        if (receiverExist(pcd->receiver())) {
            // Create the shared pointer containing a TextMessage.
            QSharedPointer<PrivateMessage> message(new PrivateMessage(client.nickName(), pcd->receiver(), pcd->text()));
            // Send to the specified user(receiver)
            client.sendMessage(message, pcd->receiver());
            // Display for this computer (messages from this user are not passed to this user from the
            // network) - no point sending data to yourself!
            handleMessage(message);
        }
        // Notify client about send failure
        else {
            QMessageBox::warning(this, "Send File Failed", "Invalid Recipient!",
                                 QMessageBox::Ok);
        }
    }
}

void ChatWindow::on_actionTo_A_Prviate_Chat_Room_triggered()
{
    //Modify the instruction label to match current context
    PrivateChatDialog *pcd = new PrivateChatDialog();
    pcd->setWindowTitle("Private Message to a Private Chatroom");
    pcd->ui->instructionLabel->setText("Please Enter Targerting Private Room Name:");
    pcd->exec();
    // Ignore send request if client did not provide a room name
    if (pcd->receiver() != "") {
        if (findRoomIndex(pcd->receiver()) != -1) {
            // Create the shared pointer containing a PrivateMessage.
            QSharedPointer<PrivateMessage> message(new PrivateMessage(client.nickName(), pcd->receiver(), pcd->text()));
            // The sending client will handles the private chat room private message
            handleMessage(message);
        }
        // Notify client about send failure
        else {
            QMessageBox::warning(this, "Send Private Message Failed", "Invalid Private Chatroom Name!",
                                 QMessageBox::Ok);
        }
    }
}

void ChatWindow::on_actionPerform_An_Action_triggered()
{
    RoomActionsDialog *rad = new RoomActionsDialog();
    rad->exec();
    // For INVITE and KICK actions
    if (rad->roomAction() != "" && rad->targetRoom() != "" && rad->targetUser() != "") {
        int targetRoomIndex = findRoomIndex(rad->targetRoom());
        // If both targeting room and user exists
        if (targetRoomIndex != -1 && receiverExist(rad->targetUser())) {
            if (rad->roomAction() == "INVITE") {
                // Invite the invitee only if he/she is not already in the room
                if (!_privateChats.at(targetRoomIndex).participantNames().contains(rad->targetUser())) {
                    //Create a shared pointer containing an INVITE action message
                    QSharedPointer<ActionMessage> message(new ActionMessage(client.nickName(), "INVITE", rad->targetRoom(),
                                                                            rad->targetUser()));
                    // Send the message
                    client.sendMessage(message, rad->targetUser());
                }
                // If user is in room, notify user about invite failure
                else {
                    QMessageBox::warning(this, "Invite Failed", "Invitee Is Already In The Room!",
                                         QMessageBox::Ok);
                }
            }
            // KICK action
            else {
                QList<QString> participants = _privateChats.at(targetRoomIndex).participantNames();
                // Kick only if the targeting user is in the room
                if (_privateChats.at(targetRoomIndex).participantNames().contains(rad->targetUser())) {
                    //Create a shared pointer containing an KICK action message
                    QSharedPointer<ActionMessage> message(new ActionMessage(client.nickName(), "KICK", rad->targetRoom(),
                                                                            rad->targetUser()));
                    // Forward the KICK action messages to all participants of the target room.
                    for (int i = 0; i < participants.size(); ++i) {
                        client.sendMessage(message, participants.at(i));
                    }
                    QString text = "<font color=\"green\"> You Kicked " + rad->targetUser();
                    text += " from " + message->targetRoom() + "</font>";
                    appendMessage("<font color=\"green\">System</font>", text);
                    handleMessage(message);
                }
                // If user not in room, notify user about kick failure
                else {
                    QMessageBox::warning(this, "Kick Failed", "Targeting User Is Not In The Room!",
                                         QMessageBox::Ok);
                }
            }
        }
        // Notify client about failed room action
        else {
            QMessageBox::warning(this, "Room Action Failed", "Invalid Private Room Name or Recipient Name!",
                                 QMessageBox::Ok);
        }
    }
    // For LEAVE action
    else {
        if (rad->targetRoom() != "") {
            int targetRoomIndex = findRoomIndex(rad->targetRoom());
            // If the room exist
            if (targetRoomIndex != -1) {
                QList<QString> participants = _privateChats.at(targetRoomIndex).participantNames();
                QSharedPointer<ActionMessage> message(new ActionMessage(client.nickName(), "LEAVE", rad->targetRoom()));
                // Forward the LEAVE action messages to all participants of the target room.
                for (int i = 0; i < participants.size(); ++i) {
                    client.sendMessage(message, participants.at(i));
                }

                // Remove all trace of the specified room in the leaving client's window
                _privateChats.removeAt(targetRoomIndex);
                QList<QListWidgetItem *> toRemove = ui->roomListWidget->findItems(rad->targetRoom(), Qt::MatchExactly);
                foreach (QListWidgetItem *item, toRemove) {
                    ui->roomListWidget->removeItemWidget(item);
                    delete item;
                }
                QString text = "<font color=\"green\">You left ";
                text += rad->targetRoom() + "</font>";
                appendMessage("<font color=\"green\">System</font>", text);
            }
            // Notify client about failed room action
            else {
                QMessageBox::warning(this, "Room Action Failed", "Room Not Found",
                                     QMessageBox::Ok);
            }
        }
    }
}

int ChatWindow::findRoomIndex(QString roomName) {
    for (int i = 0; i < _privateChats.size(); ++i) {
        if (_privateChats.at(i).roomName() == roomName) {
            return i;
        }
    }
    return -1;
}

bool ChatWindow::receiverExist(QString receiver) {
    for (int i =0; i < ui->participantsListWidget->count(); ++i) {
        if (ui->participantsListWidget->item(i)->text() == receiver) {
            return true;
        }
    }
    return false;
}

void ChatWindow::reservedInputWarning() {
    QMessageBox::warning(this, "Input With Reserved Charater(s)",
                         "| is a reserved keyword, please ensure your input do not contains the symbol.",
                         QMessageBox::Ok);
}
