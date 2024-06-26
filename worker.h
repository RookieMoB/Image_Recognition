#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QImage>
#include <QBuffer>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);                 // explicit 可以防止隐式类型转换

public slots:
    void DoWork(QImage);
//    void DoWork(QImage, QThread*);

signals:
    void resultReady(QByteArray);
//    void resultReady(QByteArray, QThread*);

};

#endif // WORKER_H
