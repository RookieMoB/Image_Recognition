/****************************************************************************
** Meta object code from reading C++ file 'recognition.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../recognition.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'recognition.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Recognition_t {
    QByteArrayData data[14];
    char stringdata0[140];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Recognition_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Recognition_t qt_meta_stringdata_Recognition = {
    {
QT_MOC_LITERAL(0, 0, 11), // "Recognition"
QT_MOC_LITERAL(1, 12, 9), // "beginWork"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 10), // "ShowCamera"
QT_MOC_LITERAL(4, 34, 2), // "id"
QT_MOC_LITERAL(5, 37, 7), // "preview"
QT_MOC_LITERAL(6, 45, 9), // "TakePhoto"
QT_MOC_LITERAL(7, 55, 10), // "TokenReply"
QT_MOC_LITERAL(8, 66, 14), // "QNetworkReply*"
QT_MOC_LITERAL(9, 81, 5), // "reply"
QT_MOC_LITERAL(10, 87, 15), // "BeginFaceDetect"
QT_MOC_LITERAL(11, 103, 9), // "post_data"
QT_MOC_LITERAL(12, 113, 10), // "ImageReply"
QT_MOC_LITERAL(13, 124, 15) // "preparePostData"

    },
    "Recognition\0beginWork\0\0ShowCamera\0id\0"
    "preview\0TakePhoto\0TokenReply\0"
    "QNetworkReply*\0reply\0BeginFaceDetect\0"
    "post_data\0ImageReply\0preparePostData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Recognition[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    2,   52,    2, 0x0a /* Public */,
       6,    0,   57,    2, 0x0a /* Public */,
       7,    1,   58,    2, 0x0a /* Public */,
      10,    1,   61,    2, 0x0a /* Public */,
      12,    1,   64,    2, 0x0a /* Public */,
      13,    0,   67,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QImage,    4,    5,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QByteArray,   11,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,

       0        // eod
};

void Recognition::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Recognition *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->beginWork((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 1: _t->ShowCamera((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QImage(*)>(_a[2]))); break;
        case 2: _t->TakePhoto(); break;
        case 3: _t->TokenReply((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 4: _t->BeginFaceDetect((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 5: _t->ImageReply((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 6: _t->preparePostData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Recognition::*)(QImage );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Recognition::beginWork)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Recognition::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Recognition.data,
    qt_meta_data_Recognition,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Recognition::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Recognition::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Recognition.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Recognition::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Recognition::beginWork(QImage _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
