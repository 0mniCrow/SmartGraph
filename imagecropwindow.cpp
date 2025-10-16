#include "imagecropwindow.h"
#include "ui_imagecropwindow.h"

ImageCropWindow::ImageCropWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageCropWindow)
{
    ui->setupUi(this);
    connect(ui->button_imgFileDialog,&QPushButton::clicked,this,&ImageCropWindow::chooseFile);
    connect(ui->button_loadImage,&QPushButton::clicked,this,&ImageCropWindow::loadImage);
}

ImageCropWindow::~ImageCropWindow()
{
    delete ui;
}

void ImageCropWindow::chooseFile()
{
    QString filename(QFileDialog::getOpenFileName(
                         this,"Open Image to crop",QDir::currentPath(),
                         "Images (*.jpg *.png *.bmp)",nullptr,QFileDialog::DontUseNativeDialog));
    ui->line_imgAddr->setText(filename);
}

void ImageCropWindow::loadImage()
{
    QString file_addr(ui->line_imgAddr->text());
    if(!QFileInfo::exists(file_addr))
        return;
    QImage bg(file_addr);
    if(bg.isNull())
        return;
    ui->label->setFixedSize(bg.size());
    ui->label->setPixmap(QPixmap::fromImage(bg));
    return;
}
