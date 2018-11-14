#ifndef IMAGEDISPLAYWINDOW_H
#define IMAGEDISPLAYWINDOW_H

#include <QMainWindow>

namespace Ui {
class ImageDisplayWindow;
}

/**
 * @brief The ImageDisplayWindow class represents a window that is used to display images
 * send by users in a chatroom.
 */
class ImageDisplayWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImageDisplayWindow(QWidget *parent = 0);
    void setDisplayImage(QPixmap image);
    ~ImageDisplayWindow();

private:
    Ui::ImageDisplayWindow *ui;
};

#endif // IMAGEDISPLAYWINDOW_H
