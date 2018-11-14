#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include "./Networking/client.h"
#include "message.h"
#include <QMainWindow>
#include <QStringList>
#include <QStringListModel>
#include "userprofile.h"
#include "privatechatroom.h"


namespace Ui {
class ChatWindow;
}

/**
 * @brief The ChatWindow class represents a chatroom window which can either be for
 * public chatting or private chatting.
 */
class ChatWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit ChatWindow(QWidget *parent = 0);
  ~ChatWindow();

public slots:
  void handleMessage(QSharedPointer<Message> message);

  void appendMessage(const QString &from, const QString &message);

private slots:
  void newParticipant(const QString &nick);
  void participantLeft(const QString &nick);

  void on_pushButton_clicked();

  void on_startButton_clicked();

  /**
   * @brief changeProfileSetting opens a window that allows user to change their profile
   */
  void on_profileSettings_triggered();

  /**
   * @brief on_participantsListWidget_itemClicked opens the clicked user's profile
   */
  void on_participantsListWidget_itemClicked();

  /**
   * @brief on_actionExport_As_Text_Document_triggered opens a file dialog that allow user to selects
   * where to export the chat history of the chatroom to and the name of the text file.
   */
  void on_actionExport_As_Text_Document_triggered();

  /**
   * @brief sendImage opens a file dialog that allow user to selects file to send
   * and then sends the selected file.
   */
  void on_sendFilesAction_triggered();

  /**
   * @brief sendImage opens a file dialog that allow user to selects jpg, jpeg, png and
   * gif images to send and then sends the selected image.
   */
  void on_sendImagesAction_triggered();

  /**
   * @brief on_actionSingleUser_Send_Image_triggered generates a popup dialog asking for receiver name then opens
   * a file dialog that allow user to selects a file to send and then sends the selected file to the specified user.
   */
  void on_actionSingleUser_Send_File_triggered();

  /**
   * @brief on_actionSingleUser_Send_Image_triggered generates a popup dialog asking for receiver name then opens
   * a file dialog that allow user to selects jpg, jpeg, png and gif images to send and then sends the selected image
   * to the specified user.
   */
  void on_actionSingleUser_Send_Image_triggered();

  /**
   * @brief on_actionSend_File_To_A_Room_triggered generates a popup dialog asking for a room name then opens
   * a file dialog that allow user to selects a file to send and then sends the selected file to all users in the specified room.
   */
  void on_actionSend_File_To_A_Room_triggered();

  /**
   * @brief on_actionSend_Image_To_A_Room_triggered generates a popup dialog asking for a room name then opens
   * a file dialog that allow user to selects jpg, jpeg, png and gif images to send and then sends the selected image
   * to all users in the specified room.
   */
  void on_actionSend_Image_To_A_Room_triggered();

  /**
   * @brief on_fileAttachmentsList_itemClicked calls a saveFileDialog to let user choose
   * where to save the selected file and the save the file to the selected location.
   */
  void on_fileAttachmentsList_itemClicked();

  /**
   * @brief on_attachmentsList_itemClicked displays the selected image attachment in another
   * window
   */
  void on_imageAttachmentsList_itemClicked();

  /**
   * @brief on_privateChatRoomAction_triggered generates a popup dialog asking for an private chat room name and
   * then creates a new private chat room with the given name if the name has not been taken by an existing room.
   */
  void on_privateChatRoomAction_triggered();

  /**
   * @brief on_actionTo_Another_User_triggered generates a popup dialog asking for private message receiver(a user) name and
   * message text then sends the message to the specified user.
   */
  void on_actionTo_Another_User_triggered();

  /**
   * @brief on_actionTo_A_Prviate_Chat_Room_triggered generates a popup dialog asking for private message receiver(a private room) name and
   * message text then sends the message to the participants of the specified private room.
   */
  void on_actionTo_A_Prviate_Chat_Room_triggered();

  /**
   * @brief on_actionPerform_An_Action_triggered generates a popup dialog asking for the to-be-performed room action and the targets of the action.
   * Then executes the chosen action on the specified targets.
   */
  void on_actionPerform_An_Action_triggered();

  /**
   * @brief findRoomIndex finds the index of the given room in the list of private rooms
   * @param roomName the searching room
   * @return the index of the searched room, -1 if room is not found.
   */
  int findRoomIndex(QString roomName);

  /**
   * @brief receiverExist determine whether the given user is connected or not
   * @param receiver the name of the user whose connection requires validation
   * @return true if connected or false if not connected
   */
  bool receiverExist(QString receiver);

  /**
   * @brief reservedInputWarning notifies the user about invalid input if any of their inputs
   * contain | as the symbol is a reserved keyword for system operation.
   */
  void reservedInputWarning();

private:
  Ui::ChatWindow *ui;

  p2pnetworking::Client client;
  QStringList chatHistory;
  QStringListModel historyModel;

  UserProfile _profile; /**< the client's user profile. */
  QList<PrivateChatRoom> _privateChats; /**< the client's list of private chatrooms. */
};

#endif // CHATWINDOW_H
