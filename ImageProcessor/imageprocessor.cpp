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
        Gray(m_source, path+"/huidu.jpg");
        emit p->myfinish(m_type,path+"/huidu.jpg");
        break;
    case ImageProcessor::ROUHUA:
        Soften(m_source, path+"/rouhua.jpg");
        emit p->myfinish(m_type, path+"/rouhua.jpg");
        break;
    case ImageProcessor::RUIHUA:
        Sharpen(m_source, path+"/ruihua.jpg");
        emit p->myfinish(m_type, path+"/ruihua.jpg");
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

void Runnable::Soften(QString sourceFile, QString destFile)
{
    QUrl url(sourceFile);
    QImage image(url.toLocalFile());
    if(image.isNull())
    {
        qDebug() << "load " << sourceFile << " failed! ";
        return;
    }
    int width = image.width();
    int height = image.height();
    int r, g, b;
    QRgb color;
    int xLimit = width - 1;
    int yLimit = height - 1;
    for(int i = 1; i < xLimit; i++)
    {
        for(int j = 1; j < yLimit; j++)
        {
            r = 0;
            g = 0;
            b = 0;
            for(int m = 0; m < 9; m++)
            {
                int s = 0;
                int p = 0;
                switch(m)
                {
                case 0:
                    s = i - 1;
                    p = j - 1;
                    break;
                case 1:
                    s = i;
                    p = j - 1;
                    break;
                case 2:
                    s = i + 1;
                    p = j - 1;
                    break;
                case 3:
                    s = i + 1;
                    p = j;
                    break;
                case 4:
                    s = i + 1;
                    p = j + 1;
                    break;
                case 5:
                    s = i;
                    p = j + 1;
                    break;
                case 6:
                    s = i - 1;
                    p = j + 1;
                    break;
                case 7:
                    s = i - 1;
                    p = j;
                    break;
                case 8:
                    s = i;
                    p = j;
                }
                color = image.pixel(s, p);
                r += qRed(color);
                g += qGreen(color);
                b += qBlue(color);
            }

            r = (int) (r / 9.0);
            g = (int) (g / 9.0);
            b = (int) (b / 9.0);

            r = qMin(255, qMax(0, r));
            g = qMin(255, qMax(0, g));
            b = qMin(255, qMax(0, b));

            image.setPixel(i, j, qRgb(r, g, b));
        }
    }

    image.save(destFile);
}

void Runnable::Sharpen(QString sourceFile, QString destFile)
{
    QUrl url(sourceFile);
    QImage image(url.toLocalFile());
    if(image.isNull())
    {
        qDebug() << "load " << sourceFile << " failed! ";
        return;
    }
    int width = image.width();
    int height = image.height();
    int threshold = 80;
    QImage sharpen(width, height, QImage::Format_ARGB32);
    int r, g, b, gradientR, gradientG, gradientB;
    QRgb rgb00, rgb01, rgb10;
    for(int i = 0; i < width; i++)
    {
        for(int j= 0; j < height; j++)
        {
            if(image.valid(i, j) &&
                    image.valid(i+1, j) &&
                    image.valid(i, j+1))
            {
                rgb00 = image.pixel(i, j);
                rgb01 = image.pixel(i, j+1);
                rgb10 = image.pixel(i+1, j);
                r = qRed(rgb00);
                g = qGreen(rgb00);
                b = qBlue(rgb00);
                gradientR = abs(r - qRed(rgb01)) + abs(r - qRed(rgb10));
                gradientG = abs(g - qGreen(rgb01)) + abs(g - qGreen(rgb10));
                gradientB = abs(b - qBlue(rgb01)) + abs(b - qBlue(rgb10));

                if(gradientR > threshold)
                {
                    r = qMin(gradientR + 100, 255);
                }

                if(gradientG > threshold)
                {
                    g = qMin( gradientG + 100, 255);
                }

                if(gradientB > threshold)
                {
                    b = qMin( gradientB + 100, 255);
                }

                sharpen.setPixel(i, j, qRgb(r, g, b));
            }
        }
    }

    sharpen.save(destFile);
}

void test(){cout<<"what?"<<endl;}
