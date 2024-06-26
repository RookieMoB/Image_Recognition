#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent)
{

}

// 拼接 post 请求
void Worker::DoWork(QImage img)
//void Worker::DoWork(QImage img, QThread* childThread)
{
    // 转成 base64 编码
    QByteArray b_a;                                                 // 创建字节数组
    QBuffer buffer(&b_a);                                           // 创建用于存放字节数组的缓冲区
    img.save(&buffer, "png");                                       // 将图片保存到字节数组内，格式为 png
    QString b64_str = b_a.toBase64();                               // 将字节数组 转换成 字符串
    if (b64_str == NULL)
        qDebug() << "b64_str is NULL";
    qDebug() << "b64_str = " << b64_str;

    ///////////////请求体body参数设置//////////////////////
    // 创建 QJsonDocument 对象，存放到 QJsonObject 中
    QJsonObject json_obj_post;
    QJsonDocument json_doc;

    // 参数释义：https://cloud.baidu.com/doc/FACE/s/yk37c1u4t
    json_obj_post.insert("image", b64_str);
    json_obj_post.insert("image_type", "BASE64");
    json_obj_post.insert("face_field", "age,face_shape,gender,glasses,emotion,face_type,mask,beauty,eye_status,spoofing");

    json_doc.setObject(json_obj_post);
    QByteArray post_data = json_doc.toJson(QJsonDocument::Compact);

    emit resultReady(post_data);                                    // 将子线程处理好的结果返回给主线程
//    emit resultReady(post_data, childThread);                        // 将子线程处理好的结果返回给主线程
}
