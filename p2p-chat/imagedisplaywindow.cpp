#include "imagedisplaywindow.h"
#include "ui_imagedisplaywindow.h"
#include <QPixmap>

ImageDisplayWindow::ImageDisplayWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::ImageDisplayWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->imageDisplayView);
}

ImageDisplayWindow::~ImageDisplayWindow()
{
    delete ui;
}

void ImageDisplayWindow::setDisplayImage(QPixmap image) {
    QGraphicsScene *scenePtr = new QGraphicsScene(this);
    scenePtr->addPixmap(image);
    // Adds some white padding around the images for better view
    if (image.height() > ui->imageDisplayView->height() &&
             image.width() < ui->imageDisplayView->width()) {
        this->resize(this->width() + 20, image.height() + 20);
    }
    else if (image.width() > ui->imageDisplayView->width() &&
             image.height() < ui->imageDisplayView->height()) {
        this->resize(this->height() + 20, image.width() + 20);
    }
    else {
        this->resize(image.width() + 20, image.height() + 20);
    }

    ui->imageDisplayView->setScene(scenePtr);
}
