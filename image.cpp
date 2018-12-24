#include "image.h"
//template <class T>
//Image<T>::Image(QString path) {
//    pixmap.load(path);
//    QFile file(path);
//    info = new QFileInfo(file.fileName());
//}

//template <typename T>
//Image<T>::~Image<T>() {
//    if (info) {
//        delete info;
//        info = NULL;
//    }
//}
template <typename T>
void Image<T>::load(QString path) {
    pixmap.load(path);
    QFile file(path);
    if (info) {
        delete info;
        info = new QFileInfo(file.fileName());
    }
}

template <typename T>
void Image<T>::save(QString path) { pixmap.save(path,0,60); }

template <typename T>
QPixmap Image<T>::pixmapObject(){ return pixmap; }

template <typename T>
QString Image<T>::path() { return info->absoluteFilePath(); }

template <typename T>
QString Image<T>::name() { return info->fileName(); }

template <typename T>
QString Image<T>::baseName() { return info->baseName(); }

template <typename T>
QString Image<T>::extension() { return "." + info->suffix(); }

template <typename T>
QString Image<T>::directory() { return info->path() + "/"; }

template <typename T>
int Image<T>::width() { return pixmap.width(); }

template <typename T>
int Image<T>::height() { return pixmap.height(); }
