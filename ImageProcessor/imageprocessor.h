#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H
#include <QObject>
#include <QThread>

class ImageProcessor : public QObject
{
public:
    enum Handletype{
        HEIBAI,
        HUIDU,
        RUIHUA,
        DIPIAN,
        FUDIAO,ROUHUA
    };
    Q_OBJECT
//    Q_ENUMS(Handletype)
public:
    ImageProcessor(QObject *parent = 0);
    ~ImageProcessor();
    void Binarize(QString sourcefile, QString destfile);
    Q_INVOKABLE void process(QString, QString, Handletype);
signals:
    void myfinish(Handletype);

};

class Runnable : public QThread
{
    Q_OBJECT
public:
    Runnable(QString, QString, ImageProcessor::Handletype, ImageProcessor *);
    void run();

    void Binarize(QString sourcefile, QString destfile);
private:
    QString m_source;
    QString m_dest;
    ImageProcessor::Handletype m_type;
    ImageProcessor *p;
};
#endif // IMAGEPROCESSOR_H
