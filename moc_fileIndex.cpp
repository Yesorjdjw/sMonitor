/****************************************************************************
** Meta object code from reading C++ file 'fileIndex.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "fileIndex.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fileIndex.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ThumbnailWorker_t {
    QByteArrayData data[6];
    char stringdata0[52];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ThumbnailWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ThumbnailWorker_t qt_meta_stringdata_ThumbnailWorker = {
    {
QT_MOC_LITERAL(0, 0, 15), // "ThumbnailWorker"
QT_MOC_LITERAL(1, 16, 10), // "thumbReady"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 5), // "index"
QT_MOC_LITERAL(4, 34, 5), // "thumb"
QT_MOC_LITERAL(5, 40, 11) // "processJobs"

    },
    "ThumbnailWorker\0thumbReady\0\0index\0"
    "thumb\0processJobs"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ThumbnailWorker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   29,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QPixmap,    3,    4,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void ThumbnailWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ThumbnailWorker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->thumbReady((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QPixmap(*)>(_a[2]))); break;
        case 1: _t->processJobs(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ThumbnailWorker::*)(int , const QPixmap & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ThumbnailWorker::thumbReady)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ThumbnailWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ThumbnailWorker.data,
    qt_meta_data_ThumbnailWorker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ThumbnailWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ThumbnailWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ThumbnailWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ThumbnailWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ThumbnailWorker::thumbReady(int _t1, const QPixmap & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_fileIndex_t {
    QByteArrayData data[22];
    char stringdata0[304];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_fileIndex_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_fileIndex_t qt_meta_stringdata_fileIndex = {
    {
QT_MOC_LITERAL(0, 0, 9), // "fileIndex"
QT_MOC_LITERAL(1, 10, 13), // "exitRequested"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 18), // "on_exit_bt_clicked"
QT_MOC_LITERAL(4, 44, 20), // "on_refreshBt_clicked"
QT_MOC_LITERAL(5, 65, 19), // "on_renameBt_clicked"
QT_MOC_LITERAL(6, 85, 19), // "on_deleteBt_clicked"
QT_MOC_LITERAL(7, 105, 25), // "on_searchEdit_textChanged"
QT_MOC_LITERAL(8, 131, 4), // "text"
QT_MOC_LITERAL(9, 136, 13), // "onItemClicked"
QT_MOC_LITERAL(10, 150, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(11, 167, 4), // "item"
QT_MOC_LITERAL(12, 172, 30), // "on_picWidget_itemDoubleClicked"
QT_MOC_LITERAL(13, 203, 30), // "on_vidWidget_itemDoubleClicked"
QT_MOC_LITERAL(14, 234, 12), // "onThumbReady"
QT_MOC_LITERAL(15, 247, 5), // "index"
QT_MOC_LITERAL(16, 253, 5), // "thumb"
QT_MOC_LITERAL(17, 259, 14), // "onKeyboardDone"
QT_MOC_LITERAL(18, 274, 11), // "eventFilter"
QT_MOC_LITERAL(19, 286, 3), // "obj"
QT_MOC_LITERAL(20, 290, 7), // "QEvent*"
QT_MOC_LITERAL(21, 298, 5) // "event"

    },
    "fileIndex\0exitRequested\0\0on_exit_bt_clicked\0"
    "on_refreshBt_clicked\0on_renameBt_clicked\0"
    "on_deleteBt_clicked\0on_searchEdit_textChanged\0"
    "text\0onItemClicked\0QListWidgetItem*\0"
    "item\0on_picWidget_itemDoubleClicked\0"
    "on_vidWidget_itemDoubleClicked\0"
    "onThumbReady\0index\0thumb\0onKeyboardDone\0"
    "eventFilter\0obj\0QEvent*\0event"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_fileIndex[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   75,    2, 0x08 /* Private */,
       4,    0,   76,    2, 0x08 /* Private */,
       5,    0,   77,    2, 0x08 /* Private */,
       6,    0,   78,    2, 0x08 /* Private */,
       7,    1,   79,    2, 0x08 /* Private */,
       9,    1,   82,    2, 0x08 /* Private */,
      12,    1,   85,    2, 0x08 /* Private */,
      13,    1,   88,    2, 0x08 /* Private */,
      14,    2,   91,    2, 0x08 /* Private */,
      17,    0,   96,    2, 0x08 /* Private */,
      18,    2,   97,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::Int, QMetaType::QPixmap,   15,   16,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QObjectStar, 0x80000000 | 20,   19,   21,

       0        // eod
};

void fileIndex::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<fileIndex *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->exitRequested(); break;
        case 1: _t->on_exit_bt_clicked(); break;
        case 2: _t->on_refreshBt_clicked(); break;
        case 3: _t->on_renameBt_clicked(); break;
        case 4: _t->on_deleteBt_clicked(); break;
        case 5: _t->on_searchEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->onItemClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 7: _t->on_picWidget_itemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 8: _t->on_vidWidget_itemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 9: _t->onThumbReady((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QPixmap(*)>(_a[2]))); break;
        case 10: _t->onKeyboardDone(); break;
        case 11: { bool _r = _t->eventFilter((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QEvent*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (fileIndex::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&fileIndex::exitRequested)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject fileIndex::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_fileIndex.data,
    qt_meta_data_fileIndex,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *fileIndex::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *fileIndex::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_fileIndex.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int fileIndex::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void fileIndex::exitRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
