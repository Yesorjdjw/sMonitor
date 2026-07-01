/****************************************************************************
** Meta object code from reading C++ file 'VideoWorker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "VideoWorker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoWorker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VideoWorker_t {
    QByteArrayData data[20];
    char stringdata0[219];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoWorker_t qt_meta_stringdata_VideoWorker = {
    {
QT_MOC_LITERAL(0, 0, 11), // "VideoWorker"
QT_MOC_LITERAL(1, 12, 10), // "frameReady"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 5), // "image"
QT_MOC_LITERAL(4, 30, 10), // "photoSaved"
QT_MOC_LITERAL(5, 41, 4), // "path"
QT_MOC_LITERAL(6, 46, 16), // "recordingStarted"
QT_MOC_LITERAL(7, 63, 16), // "recordingStopped"
QT_MOC_LITERAL(8, 80, 11), // "cameraError"
QT_MOC_LITERAL(9, 92, 3), // "msg"
QT_MOC_LITERAL(10, 96, 12), // "cameraOpened"
QT_MOC_LITERAL(11, 109, 16), // "occlusionStarted"
QT_MOC_LITERAL(12, 126, 9), // "videoPath"
QT_MOC_LITERAL(13, 136, 16), // "occlusionStopped"
QT_MOC_LITERAL(14, 153, 11), // "startCamera"
QT_MOC_LITERAL(15, 165, 10), // "stopCamera"
QT_MOC_LITERAL(16, 176, 12), // "setRecording"
QT_MOC_LITERAL(17, 189, 6), // "record"
QT_MOC_LITERAL(18, 196, 9), // "takePhoto"
QT_MOC_LITERAL(19, 206, 12) // "processFrame"

    },
    "VideoWorker\0frameReady\0\0image\0photoSaved\0"
    "path\0recordingStarted\0recordingStopped\0"
    "cameraError\0msg\0cameraOpened\0"
    "occlusionStarted\0videoPath\0occlusionStopped\0"
    "startCamera\0stopCamera\0setRecording\0"
    "record\0takePhoto\0processFrame"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoWorker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       4,    1,   82,    2, 0x06 /* Public */,
       6,    1,   85,    2, 0x06 /* Public */,
       7,    0,   88,    2, 0x06 /* Public */,
       8,    1,   89,    2, 0x06 /* Public */,
      10,    0,   92,    2, 0x06 /* Public */,
      11,    1,   93,    2, 0x06 /* Public */,
      13,    1,   96,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    0,   99,    2, 0x0a /* Public */,
      15,    0,  100,    2, 0x0a /* Public */,
      16,    1,  101,    2, 0x0a /* Public */,
      18,    0,  104,    2, 0x0a /* Public */,
      19,    0,  105,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QString,   12,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   17,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void VideoWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoWorker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->frameReady((*reinterpret_cast< const QImage(*)>(_a[1]))); break;
        case 1: _t->photoSaved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->recordingStarted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->recordingStopped(); break;
        case 4: _t->cameraError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->cameraOpened(); break;
        case 6: _t->occlusionStarted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->occlusionStopped((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->startCamera(); break;
        case 9: _t->stopCamera(); break;
        case 10: _t->setRecording((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->takePhoto(); break;
        case 12: _t->processFrame(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VideoWorker::*)(const QImage & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoWorker::frameReady)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VideoWorker::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoWorker::photoSaved)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VideoWorker::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoWorker::recordingStarted)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VideoWorker::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoWorker::recordingStopped)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (VideoWorker::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoWorker::cameraError)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (VideoWorker::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoWorker::cameraOpened)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (VideoWorker::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoWorker::occlusionStarted)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (VideoWorker::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoWorker::occlusionStopped)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VideoWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_VideoWorker.data,
    qt_meta_data_VideoWorker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VideoWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VideoWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void VideoWorker::frameReady(const QImage & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VideoWorker::photoSaved(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void VideoWorker::recordingStarted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void VideoWorker::recordingStopped()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void VideoWorker::cameraError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void VideoWorker::cameraOpened()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void VideoWorker::occlusionStarted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void VideoWorker::occlusionStopped(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
