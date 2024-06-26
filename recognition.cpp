#include "recognition.h"
#include "ui_recognition.h"

Recognition::Recognition(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Recognition)
{
    ui->setupUi(this);

    this->window()->move(30, 100);

    // 得到摄像头的所有信息
    camera_info_list = QCameraInfo::availableCameras();
    for (const QCameraInfo& temp_cam : camera_info_list)
    {
        qDebug() << "deviceName " << temp_cam.deviceName() << "|||" << "description " << temp_cam.description();
        ui->comboBox->addItem(temp_cam.description());
    }
    camera_info_list.append(QCameraInfo("测试"));
    ui->comboBox->addItem("测试");

    ui->comboBox->addItem("null");
    ui->comboBox->addItem("可以添加外接摄像头或者虚拟摄像头");

    QVariant v(0);          //禁用
    ui->comboBox->setItemData(camera_info_list.length(), v, Qt::UserRole - 1);
    ui->comboBox->setItemData(camera_info_list.length() - 1, v, Qt::UserRole - 1);
    ui->comboBox->setItemData(camera_info_list.length() + 1, v, Qt::UserRole - 1);
    qDebug() << "list size " << camera_info_list.length();

    // 绑定下拉菜单选项与实际摄像头的切换
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Recognition::PickCamera);

    // 检测是否可以使用https进行网络请求
    // qDebug() << "Qt version:" << qVersion();
    // qDebug() << "Qt ssl v" << QSslSocket::sslLibraryBuildVersionString();
    // qDebug() << "OpenSSL支持情况:" << QSslSocket::supportsSsl();
    // 初始化指针
    camera = new QCamera();                                                     // 唤起摄像头
    c_v_finder = new QCameraViewfinder();                                       // 摄像头取景器
    c_i_capture = new QCameraImageCapture(camera);                              // 图像捕捉器
    image_label = new QLabel();                                                 // 初始化图像label

    camera->setViewfinder(c_v_finder);                                          // 为摄像头设置取景器
    camera->setCaptureMode(QCamera::CaptureStillImage);                         // 设置摄像头的取景方式，为捕捉静态图片
    // 弃用该方法，会在图片文件夹下，生成一系列照片
    // c_i_capture->setCaptureDestination(QCameraImageCapture::CaptureToFile);     // 设置捕获的图像的存储方式——————文件
    // 改用 将图片输出到缓冲
    c_i_capture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
    camera->start();                                                            // 摄像头开始工作

    connect(ui->pushButton, &QPushButton::clicked, this, &Recognition::preparePostData);    // 拍照

    connect(c_i_capture, &QCameraImageCapture::imageCaptured, this, &Recognition::ShowCamera);// 拍照成功之后将图片显示到label组件上，对捕获到的静态图片进行反转


    // 进行页面布局
    this->resize(900, 600);
    QVBoxLayout* v_box_l = new QVBoxLayout;
    v_box_l->addWidget(ui->label);
    v_box_l->addWidget(ui->pushButton);
    QVBoxLayout* v_box_r = new QVBoxLayout;
    v_box_r->addWidget(ui->comboBox);
    v_box_r->addWidget(c_v_finder);
    v_box_r->addWidget(ui->textBrowser);

    h_box = new QHBoxLayout(this);
    h_box->addWidget(image_label);
    h_box->addLayout(v_box_l);
    h_box->addLayout(v_box_r);

    this->setLayout(h_box);


    refresh_time = new QTimer(this);                                            // 利用定时器不断刷新拍照页面
    connect(refresh_time, &QTimer::timeout, this, &Recognition::TakePhoto);     // 当定时器时间到了之后，进行拍照
    refresh_time->start(25);                                                    // 启动定时器

    net_timer = new QTimer(this);                                               // 利用定时器去发送给人脸识别请求
    connect(net_timer, &QTimer::timeout, this, &Recognition::preparePostData);


    // 服务器
    token_manager = new QNetworkAccessManager(this);
    connect(token_manager, &QNetworkAccessManager::finished, this, &Recognition::TokenReply);   // 接受服务器传过来的信号，进行处理
    image_manager = new QNetworkAccessManager(this);
    connect(image_manager, &QNetworkAccessManager::finished, this, &Recognition::ImageReply);   // 图像识别

    // 输出日志查看qt支持哪些协议
//    qDebug() << token_manager->supportedSchemes();
//    qDebug() << "--------------------------------------";
//    qDebug() << "SSL support:" << QSslSocket::supportsSsl();
//    qDebug() << "--------------------------------------";
//    qDebug()<<"QSslSocket="<<QSslSocket::sslLibraryBuildVersionString();


    ////////////////////拼接url及参数/////////////////////////
    QUrl url = QUrl("https://aip.baidubce.com/oauth/2.0/token");                        // 发送请求
    QUrlQuery query;                                                                    // 拼接键值对
    query.addQueryItem("grant_type", "client_credentials");
    query.addQueryItem("client_id", "AuS0kkIHTNNTzggpY20SYzgf");
    query.addQueryItem("client_secret", "EQnDZ9o8eGWxWrS7wlMztzton3ZmVFM1");

    url.setQuery(query);                                                                // 将拼接好的网址对url进行重新赋值
    qDebug() << url;

//    if (QSslSocket::supportsSsl())
//    {// 检测是否支持Ssl
//        qDebug() << "SSL libraries are available.";
//    }

    ///////////////////////配置ssl//////////////////////////
    ssl_configuration = QSslConfiguration::defaultConfiguration();      // 对ssl配置进行默认初始化
    ssl_configuration.setPeerVerifyMode(QSslSocket::QueryPeer);         // 设置ssl的对等验证模式为 QueryPeer
    ssl_configuration.setProtocol(QSsl::TlsV1_2);                       // 设置协议为  TlsV1_2

    QNetworkRequest req;                                                // 组装请求
    req.setUrl(url);
    req.setSslConfiguration(ssl_configuration);

    token_manager->get(req);                                            // 使用get方法发送请求
}

void Recognition::ShowCamera(int id, QImage preview)                    // 对拍好的图片进行展示
{
    Q_UNUSED(id);

    QImage flippedImage = preview.mirrored(true, false);                // 展示的时候是否对图像进行反转显示

    this->img = flippedImage;                                                // 将图片进行保存，用于之后的和百度的人脸识别

    // 绘制人脸框
    QPainter m_painter(&flippedImage);                                  // 将该图片保存一个副本给到「this->img」让「this->img」去向百度进行访问，之后再对该图片进行绘制放置到「ui->label」上，其实是上一张图片的框
    m_painter.setPen(Qt::green);                                        // 设置🖌画笔颜色为绿色
    m_painter.drawRect(face_left, face_top, face_width, face_height);   // 绘制矩形

    // 在展示图片的时候，展示对应该帧的图像信息
    // 设置字体
    QFont font = m_painter.font();      // 使用  m_painter.font()进行初始化 是为了获取当前的字体设置，以便可以根据需要对其进行修改或保存，并在后续的绘图操作中使用它。
    font.setPixelSize(30);
    m_painter.setFont(font);

    int longitudinal_spacing = 40;

    m_painter.drawText(face_left + face_width + 10, face_top, QString("当前人脸个数为 : ").append(QString::number(face_num)));
    m_painter.drawText(face_left + face_width + 10, face_top + longitudinal_spacing * 2, QString("年龄 : ").append(QString::number(age)));
    m_painter.drawText(face_left + face_width + 10, face_top + longitudinal_spacing * 3, QString("类型 : ").append(type));
    m_painter.drawText(face_left + face_width + 10, face_top + longitudinal_spacing * 4, QString("性别 : ").append(gender));
    m_painter.drawText(face_left + face_width + 10, face_top + longitudinal_spacing * 5, QString("是否戴眼镜 : ").append(glasses));
    m_painter.drawText(face_left + face_width + 10, face_top + longitudinal_spacing * 6, QString("表情 : ").append(emotion));
    m_painter.drawText(face_left + face_width + 10, face_top + longitudinal_spacing * 7, QString("颜值 : ")
                       .append(QString::number(beauty > 40 ? beauty : beauty * 2.5, 'f', 2)));

    ui->label->setPixmap(QPixmap::fromImage(flippedImage));

    // 保存到文件
    // QString filePath = QDir::currentPath() + "CapturedImage_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".jpg";
    // flippedImage.save(filePath);
}

void Recognition::TakePhoto()                                                   // 异步实现拍照
{
    c_i_capture->capture();
}

void Recognition::TokenReply(QNetworkReply *reply)                              // 用于接受服务器返回的finish信号
{
    if (reply->error() != QNetworkReply::NoError)
    {// 错误处理
        qDebug() << reply->errorString();
        return;
    }

    const QByteArray reply_data = reply->readAll();                             // 正常应答
//    qDebug() << reply_data;

    QJsonParseError json_err;                                                   // JSON转换

    QJsonDocument json_doc = QJsonDocument::fromJson(reply_data, &json_err);    // 将成功解析到的JSON内容存到 json_doc 内

    if (json_err.error != QJsonParseError::NoError)
    {// 解析错误
        qDebug() << "JSON ERR: " << json_err.errorString();
    }
    // 正常解析
    QJsonObject json_obj = json_doc.object();                                   // 拿取json对象

    if (json_obj.contains("access_token"))                                  // 如果json对象包含 key access_token 的话，进行后续操作
        access_token = json_obj.take("access_token").toString();            // 将json对象的内容利用take取出来，将字符串保存到 access_token 内

    ui->textBrowser->setText(access_token);                                 // 将解析到的json文本放入到textBrowser中进行显示

    reply->deleteLater();                                                   // 用完就释放，养成好习惯

//    net_timer->start(500);
    preparePostData();                                                      // 为下一次的人脸识别做准备
}


void Recognition::preparePostData()                                                     // 等待 post 请求
{
    /*
       * 创建子线程
       * 创建工人
       * 把工人送进子线程
       * 绑定信号和槽的关系
       * 启动子线程
       * 给工人发送通知开始干活
        * 当发送了 开始干活「beginWork」的信号之后，执行「DoWork」
        * 在 完成了 「DoWork」 之后，将结果返回回来 是一个 「QByteArray post_data」
        * 子线程工作完毕之后，将子线程以及 「worker」 都进行关闭以及释放
    */
    childThread = new QThread(this);
    Worker* worker = new Worker;
    worker->moveToThread(childThread);                                                  // 讲工人送进子进程

    connect(this, &Recognition::beginWork, worker, &Worker::DoWork);                    // 开始干活
    connect(worker, &Worker::resultReady, this, &Recognition::BeginFaceDetect);         // 干完活了
    connect(childThread, &QThread::finished, worker, &QObject::deleteLater);            // 当子线程工作完毕之后，将worker释放掉
    childThread->start();                                                               // 启动子线程
    emit beginWork(this->img);                                             // 通知工人开始干活
//    emit beginWork(this->img, childThread);                                             // 通知工人开始干活
}

// 切换当前摄像头
void Recognition::PickCamera(int index)
{
    qDebug() << camera_info_list.at(index).description();
    refresh_time->stop();                                                                       // 进行切换摄像头的时候，将定时器拍照暂时先暂停
    camera->stop();                                                                             // 暂停摄像头进行切换

    camera = new QCamera(camera_info_list.at(index));                                           // 使用新创建的摄像头替换原先的
    c_i_capture = new QCameraImageCapture(camera);                                              // 创建新的拍摄按钮
    camera->setViewfinder(c_v_finder);                                                          // 为摄像头设置取景器

    camera->setCaptureMode(QCamera::CaptureStillImage);                                         // 设置摄像头的取景方式 静态图片
    c_i_capture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);                   // 将新创建的摄像头的拍摄出来的图片选择模式为 缓存

    connect(c_i_capture, &QCameraImageCapture::imageCaptured, this, &Recognition::ShowCamera);  // 拍照成功之后将图片显示到label组件上，对捕获到的静态图片进行反转

    // 开启
    camera->start();
    refresh_time->start(25);
}

void Recognition::BeginFaceDetect(QByteArray post_data)       // 开启人脸识别
//void Recognition::BeginFaceDetect(QByteArray post_data, QThread* overThread)       // 开启人脸识别
{
    /*
       * 另一个槽的内容
       * 关闭子线程
       * 组装图像识别请求
       * 用 post 发送数据给百度API
    */
    childThread->quit();                                                                // 关闭子进程
    childThread->wait();                                                                // 等待关闭子进程
    if (childThread->isFinished())
    {
        qDebug() << "子线程结束";
    } else {
        qDebug() << "子线程运行中";
    }

    //    overThread->quit();                                                                // 关闭子进程
    //    overThread->wait();                                                                // 等待关闭子进程
    //    if (overThread->isFinished())
    //    {
    //        qDebug() << "子线程结束";
    //    } else {
    //        qDebug() << "子线程运行中";
    //    }

    //////////组装图像识别请求/////////////////
    QUrl url("https://aip.baidubce.com/rest/2.0/face/v3/detect");
    QUrlQuery query;
    query.addQueryItem("access_token", this->access_token);
    url.setQuery(query);
            ///////// 组装请求 /////////
    QNetworkRequest req;
    req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    req.setUrl(url);
    req.setSslConfiguration(this->ssl_configuration);

    // 发送请求
    image_manager->post(req, post_data);
}

void Recognition::ImageReply(QNetworkReply *reply)                                  // 用于接受处理服务器传过来的finish信号————图像处理
{
    if(reply->error() != QNetworkReply::NoError)
    {// 错误处理
        qDebug() << reply->errorString();
        return;
    }

    // 正常应答
    const QByteArray reply_data = reply->readAll();
    qDebug() << "ImageReply reply data is \n" << reply_data;                        // 百度返回的数据

    // 解析JSON对象
    QJsonParseError json_err;
    QJsonDocument doc = QJsonDocument::fromJson(reply_data, &json_err);


    if (json_err.error != QJsonParseError::NoError)                                 // 错误处理
        qDebug() << "ImageReply Error is " <<json_err.errorString() ;
    // 正常处理
    QString face_info;
    QJsonObject json_obj = doc.object();

    if (json_obj.contains("timestamp"))
    {// 拿取时间戳
        int tep_time = json_obj.take("timestamp").toInt();
        if (tep_time < cur_time)
        {// 如果该次拿取的时间戳比最新的时间距离1970年近，不做处理
            return;
        }
        else
        {// 否则，将该次时间戳赋值给 cur_time
            cur_time = tep_time;
        }
    }

    if (json_obj.contains("result"))                                                // 从服务器给的JSON对象内拿取 result 结果
    {
        QJsonObject result_obj = json_obj.take("result").toObject();
        if (result_obj.contains("face_num"))
        {// 拿取几张人脸
            face_num = result_obj.take("face_num").toInt();
            face_info.append("当前人脸个数为 : ").append(QString::number(face_num)).append("\r\n");
        }
        if (result_obj.contains("face_list"))
        {
            QJsonArray face_list = result_obj.take("face_list").toArray();          // 将结果中的 face_list 拿取出来
            QJsonObject face_obj = face_list.at(0).toObject();                      // 再拿取 第一个结果   因为只有一张人脸
            if (face_obj.contains("location"))
            {// 拿取人脸的坐标信息
                QJsonObject face_location = face_obj.take("location").toObject();   // 将结果中的 location 拿取出来因为人脸坐标也是JSON数据，所以使用QJsonObject进行存储
                if (face_location.contains("left"))
                {
                    face_left = face_location.take("left").toDouble();
                }
                if (face_location.contains("top"))
                {
                    face_top = face_location.take("top").toDouble();
                }
                if (face_location.contains("width"))
                {
                    face_width = face_location.take("width").toDouble();
                }
                if (face_location.contains("height"))
                {
                    face_height = face_location.take("height").toDouble();
                }
            }
            if (face_obj.contains("age"))
            {// 取出年龄
                age = face_obj.take("age").toDouble();
                face_info.append("年龄: ").append(QString::number(age, 'f', 2)).append("\r\n");                  // 拼接到字符串上
            }
            if (face_obj.contains("face_type"))
            {// 拿取人脸类型
                QJsonObject face_type_obj = face_obj.take("face_type").toObject();
                if (face_type_obj.contains("type"))
                {
                    type = face_type_obj.take("type").toString();
                    face_info.append("类型 : ").append(type).append("\r\n");
                }
            }
            if (face_obj.contains("gender"))
            {// 取出性别
                QJsonObject gender_obj = face_obj.take("gender").toObject();
                if (gender_obj.contains("type"))
                {
                    gender = gender_obj.take("type").toString();
                    face_info.append("性别: ").append(gender).append("\r\n");                                     // 拼接到字符串上
                }
            }
            if (face_obj.contains("glasses"))
            {// 取出是否戴眼镜
                QJsonObject glasses_obj = face_obj.take("glasses").toObject();
                if (glasses_obj.contains("type"))
                {
                    glasses = glasses_obj.take("type").toString();
                    face_info.append("是否戴眼镜 : ").append(glasses == "none" ? "不戴" : "戴").append("\r\n");
                }
            }
            if (face_obj.contains("emotion"))
            {// 取出表情
                QJsonObject emotion_obj = face_obj.take("emotion").toObject();
                if (emotion_obj.contains("type"))
                {
                    emotion = emotion_obj.take("type").toString();
                    face_info.append("表情: ").append(emotion).append("\r\n");                                    // 拼接到字符串上
                }
            }
//            if (face_obj.contains("mask"))
//            {// 取出是否戴口罩
//                QJsonObject mask_obj = face_obj.take("mask").toObject();
//                if (mask_obj.contains("type"))
//                {
//                    int mask = mask_obj.take("type").toInt();
//                    face_info.append("是否戴口罩: ").append(mask == 0 ? "否" : "是").append("\r\n");               // 拼接到字符串上
//                }
//            }
            if (face_obj.contains("beauty"))
            {// 颜值
                beauty = face_obj.take("beauty").toDouble();
                face_info.append("颜值: ").append(QString::number(beauty, 'f', 2)).append("\r\n");               // 拼接到字符串上
            }

            // 可以继续添加比如说：脸型，是否戴眼镜，脸的类型，等等等
        }
    }
    ui->textBrowser->setText(face_info);

    reply->deleteLater();                                                                                       // 用完就释放，养成好习惯

    preparePostData();                                                                                          // 为下一次的人脸识别做准备
}

Recognition::~Recognition()
{
    delete ui;
    // 释放指针
    if (camera)
    {
        delete camera;
        camera = nullptr;
    }
    if (c_v_finder)
    {
        delete c_v_finder;
        c_v_finder = nullptr;
    }
    if (c_i_capture)
    {
        delete c_i_capture;
        c_i_capture = nullptr;
    }
}






















