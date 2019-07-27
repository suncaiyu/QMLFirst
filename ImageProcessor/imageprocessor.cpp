#include "imageprocessor.h"
#include <QImage>
#include <QDebug>
#include <QUrl>
#include <QDir>

ImageProcessor::ImageProcessor(QObject *parent)
{
}

ImageProcessor::~ImageProcessor()
{

}

void ImageProcessor::Binarize(QString sourcefile, QString destfile)
{
    QUrl url(sourcefile);
    QImage image(url.toLocalFile());
    if(image.isNull()){
        qDebug()<<"faild";
        return;
    }
    int width = image.width();
    int height = image.height();
    QRgb color;
    QRgb average;
    QRgb black = qRgb(0, 0, 0);
    QRgb white = qRgb(255, 255, 255);
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            color = image.pixel(i,j);
            average = (qRed(color) + qGreen(color) + qBlue(color)) / 3;
            image.setPixel(i, j, average >= 128 ? white : black);
        }
    }
    image.save(destfile);
}

void ImageProcessor::process(QString source, Handletype type)
{
    Runnable *r = new Runnable(source, type, this);
    r->start();
}

Runnable::Runnable(QString source, ImageProcessor::Handletype type, ImageProcessor *processor)
{
    m_source = source;
    m_type = type;
    p = processor;
}

void Runnable::run()
{
   QString path = QDir::currentPath();
    switch (m_type) {
    case ImageProcessor::Handletype::HEIBAI:
        Binarize(m_source, path+"/heibai.jpg");
        emit p->myfinish(m_type, path+"/heibai.jpg");
        break;
    case ImageProcessor::HUIDU:
        Gray(m_source, path+"//huidu.jpg");
        emit p->myfinish(m_type,path+"//huidu.jpg");
        break;
    default:
        break;
    }
}

void Runnable::Binarize(QString sourcefile, QString destfile)
{
    QUrl url(sourcefile);
    QImage image(url.toLocalFile());
    if(image.isNull()){
        qDebug()<<"faild";
        return;
    }
    int width = image.width();
    int height = image.height();
    QRgb color;
    QRgb average;
    QRgb black = qRgb(0, 0, 0);
    QRgb white = qRgb(255, 255, 255);
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            color = image.pixel(i,j);
            average = (qRed(color) + qGreen(color) + qBlue(color)) / 3;
            image.setPixel(i, j, average >= 128 ? white : black);
        }
    }
    image.save(destfile);
}

void Runnable::Gray(QString sourceFile, QString destFile)
{
    QUrl url(sourceFile);
    QImage image(url.toLocalFile());
    if(image.isNull())
    {
        qDebug() << "load " << sourceFile << " failed! ";
        return;
    }
    qDebug() << "depth - " << image.depth();

    int width = image.width();
    int height = image.height();
    QRgb color;
    int gray;
    for(int i = 0; i < width; i++)
    {
        for(int j= 0; j < height; j++)
        {
            color = image.pixel(i, j);
            gray = qGray(color);
            image.setPixel(i, j, qRgba(gray, gray, gray, qAlpha(color)));
        }
    }

    image.save(destFile);
}
