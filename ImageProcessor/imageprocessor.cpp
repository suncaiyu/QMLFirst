#include "imageprocessor.h"
#include <QImage>
#include <QDebug>
#include <QUrl>
#include <QThreadPool>

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

void ImageProcessor::process(QString source, QString dest, Handletype type)
{
    Runnable *r = new Runnable(source, dest, type, this);
    r->start();
}

Runnable::Runnable(QString source, QString dest, ImageProcessor::Handletype type, ImageProcessor *processor)
{
    m_source = source;
    m_dest = dest;
    m_type = type;
    p = processor;
}

void Runnable::run()
{
    switch (m_type) {
        case ImageProcessor::Handletype::HEIBAI:
        Binarize(m_source, m_dest);
        emit p->myfinish(m_type);
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
