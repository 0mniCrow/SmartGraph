#ifndef IMAGECROPWINDOW_H
#define IMAGECROPWINDOW_H

#include <QWidget>
#include <QFileDialog>
#include <QDir>
#include <QImage>
#include <QPainter>
#include <QFileInfo>

namespace Ui {
class ImageCropWindow;
}

class ImageCropWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ImageCropWindow(QWidget *parent = nullptr);
    ~ImageCropWindow();

private:
    Ui::ImageCropWindow *ui;
private slots:
    void chooseFile();
    void loadImage();
};

#endif // IMAGECROPWINDOW_H
