#ifndef RECOGNITION_H
#define RECOGNITION_H

#include "worker.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include <QComboBox>

#include <QCamera>                                  // qt摄像头
#include <QCameraViewfinder>                        // qt取景器
#include <QCameraImageCapture>                      // qt拍照
#include <QCameraInfo>                              // 摄像头信息类
#include <QDir>                                     // qt文件
#include <QDateTime>                                // qt时间
#include <QTimer>                                   // qt定时器

#include <QNetworkAccessManager>                    // qt网络请求和应答

#include <QSslSocket>                               // 检测是否包含QSslSocket

#include <QUrl>
#include <QUrlQuery>

#include <QSslConfiguration>                        // 对https里面的ssl协议进行配置

#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonParseError>                          // 解析JSON字符串
#include <QJsonDocument>                            // |||
#include <QJsonObject>                              // |||
#include <QJsonArray>                               // END

#include <QImage>

// 作用：The QBuffer class provides a QIODevice interface for a QByteArray
#include <QBuffer>                                  // Qt的缓冲区

#include <QThread>                                  // Qt线程

#include <QPainter>                                 // Qt 画笔


QT_BEGIN_NAMESPACE
namespace Ui {
class Recognition;
}
QT_END_NAMESPACE

class Recognition : public QWidget
{
    Q_OBJECT

signals:
    void beginWork(QImage);                      // 将信号传给“工人”，让工人开始拼接 post 请求数据
//    void beginWork(QImage, QThread*);                      // 将信号传给“工人”，让工人开始拼接 post 请求数据，并传入当前子线程

public slots:
    void ShowCamera(int id, QImage preview);               // 展示照片
    void TakePhoto();                                      // 拍照
    void TokenReply(QNetworkReply *reply);                 // 用于接受服务器返回的finish信号————请求access_token
    void BeginFaceDetect(QByteArray post_data);            // 开启人脸识别
//    void BeginFaceDetect(QByteArray post_data, QThread*);  // 开启人脸识别
    void ImageReply(QNetworkReply *reply);                 // 用于接受服务器返回的finish信号————图像处理
    void preparePostData();                                // 等待 post 请求结果
    void PickCamera(int);                                  // 切换当前摄像头
public:
    Recognition(QWidget *parent = nullptr);
    ~Recognition();

private:
    Ui::Recognition *ui;


    QCamera* camera;                                        // 唤起摄像头
    QCameraViewfinder* c_v_finder;                          // 摄像头取景器
    QCameraImageCapture* c_i_capture;                       // 图像捕捉器
    QHBoxLayout* h_box;                                     // 定义水平布局
    QLabel* image_label;                                    // 图像标签，显示在左边的大图
    QTimer* refresh_time;                                   // 创建定时器
    QTimer* net_timer;                                      // 定时发送人脸识别请求
    QNetworkAccessManager* token_manager;                   // 创建 QNetworkAccessManager 对象，目的是获取服务器返回的字符串
    QNetworkAccessManager* image_manager;                   // 创建用于给服务器发送请求的图像管理者对象
    QSslConfiguration ssl_configuration;                    // 创建ssl配置
    QString access_token;                                   // 创建用于保存 access_token 的对象
    QImage img;                                             // 全局 img 用于存放保存的拍照图片
    QThread* childThread;                                   // 创建 线程 指针
    double face_left;                                       // 创建用于接受服务器返回来的人脸的坐标
    double face_top;                                        // |||
    double face_width;                                      // |||
    double face_height;                                     // END
    /*
        ---------------------------------->x
        |           ↓ 距离顶端的距离——记作top
        |←         ———————
        |  距离    |      |  height
        |   y轴    ———————
        | 的距离     width
        | 记作left
        |
        |
        |
        ↓
        y
    */
    QList<QCameraInfo> camera_info_list;                     // 摄像头信息列表

    // 人脸信息
    double age;
    QString gender;
    QString emotion;
    double beauty;
    int face_num;
    QString type;
    QString glasses;

    int cur_time;                                            // 用于记录人脸识别返回的信息的最新的时间
};
#endif // RECOGNITION_H
