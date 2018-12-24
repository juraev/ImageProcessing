#ifndef Image_H
#define Image_H
#define cimg_display 0
#define _sqr(x) (x) * (x)
#include <QPixmap>
#include <QFile>
#include <QFileInfo>
#include <CImg.h>
#include <iostream>
#include <qpainter.h>
#include <math.h>
#define inf 1000000000000
using namespace cimg_library;

template<class T = unsigned char>
class Image {

private:

    QPixmap pixmap;
    QPixmap grayscale;
    QPixmap binary;
    QFileInfo *info;
    int _width;
    int _height;
    CImg<T> img;
    float alr, alg, alb;
    long long myu[4][4];
    double nyu[4][4];
    double* pfi;
    int cent_x, cent_y;


public:

    Image(QString path){

        for (int i = 0; i < 4; i ++)
            for (int j = 0; j < 4; j ++)
                myu[i][j] = inf;

        img = CImg<>(path.toStdString().c_str());
        _width = img._width;
        _height = img._height;
        pixmap = QPixmap(_width, _height);
        pfi = nullptr;

        QPainter* painter = new QPainter(&pixmap);
        QPen pen = QPen();
        cimg_forXY(img, x, y){
            pen.setColor(QColor((int)img(x, y, 0, 0), (int)img(x, y, 0, 1), (int)img(x, y, 0, 2)));
            painter->setPen(pen);
            painter->drawPoint(x, y);
        }
        QFile file(path);
        info = new QFileInfo(file.fileName());
    }

    ~Image(){
        if (info) {
            delete info;
            info = NULL;
        }
    }

    void load(QString path) {
        pixmap.load(path);
        QFile file(path);
        if (info) {
            delete info;
            info = new QFileInfo(file.fileName());
        }
    }

    void save(QString path) { pixmap.save(path,0,60); }

    QPixmap pixmapObject(){ return pixmap; }

    QString path() { return info->absoluteFilePath(); }

    QString name() { return info->fileName(); }

    QString baseName() { return info->baseName(); }

    QString extension() { return "." + info->suffix(); }

    QString directory() { return info->path() + "/"; }

    QPixmap getGrayScale(float alr, float alg, float alb) {
        this->alr = alr; this->alg = alg; this->alb = alb;
        computeGrayscale();
        return grayscale;
    }

    void computeGrayscale(){
        if(grayscale.isNull()){
            grayscale = QPixmap(_width, _height);
            QPainter* painter = new QPainter(&grayscale);
            QPen pen = QPen();
            int col = 0;
            long long m10 = 0, m01 = 0, m00 = 0;
            cimg_forXY(img, x, y){
                col = alr * img(x, y, 0, 0) + alg * img(x, y, 0, 1) + alb * img(x, y, 0, 2);
                m10 += x * col; m01 += y * col; m00 += col;
                pen.setColor(QColor(col, col, col));
                painter->setPen(pen);
                painter->drawPoint(x, y);
            }
            myu[0][0] = m00;
            cent_x = m10 / m00;
            cent_y = m01 / m00;
//            std::clog << std::to_string(cent_x) << " " << std::to_string(cent_y) << std::endl;
//            std::clog << std::to_string(m10) << " " << std::to_string(m01) << std::endl;
        }

    }

    QPixmap getWB(unsigned char threshold){
        if(!binary.isNull()) return binary;
        binary = QPixmap(_width, _height);
        QPainter* painter = new QPainter(&binary);
        QPen pen = QPen();
        QColor w(255, 255, 255);
        QColor b(0, 0, 0);
        cimg_forXY(img, x, y){
            pen.setColor(alr * img(x, y, 0, 0)
                         + alg * img(x, y, 0, 1)
                         + alb * img(x, y, 0, 2) > threshold ? w : b);
            //            if(x % 100 == 0 && y % 100 == 0){
            //                std::clog << alr * img(x, y, 0, 0)
            //                             + alg * img(x, y, 0, 1)
            //                             + alb * img(x, y, 0, 2) << std::endl;
            //            }
            painter->setPen(pen);
            painter->drawPoint(x, y);
        }
        return binary;
    }

    int width() { return pixmap.width(); }

    int height() { return pixmap.height(); }

    double* getHusMoments(){
        if(pfi == nullptr){
            pfi = new double[10];
            pfi[1] = getNyu(2, 0) + getNyu(0, 2);
            pfi[2] = pfi[1] * pfi[1] - 4 * getNyu(2, 0) * getNyu(0, 2)
                    + 4 * getNyu(1, 1) * getNyu(1, 1);
            pfi[3] = _sqr(getNyu(3, 0) - 3 * getNyu(1, 2))
                    + _sqr(3 * getNyu(2, 1) - getNyu(3, 0));
            pfi[4] = _sqr(getNyu(3, 0) + getNyu(1, 2)) + _sqr(getNyu(2, 1) + getNyu(3, 0));
            pfi[5] = (getNyu(3, 0) - 3 * getNyu(1, 2)) * (getNyu(3, 0 ) + getNyu(1, 2))
                    * (_sqr(getNyu(3, 0) + getNyu(1, 2)) - 3 * _sqr(getNyu(2, 1) + getNyu(0, 3)))
                    + (3 * getNyu(2, 1) - getNyu(0, 3)) * (getNyu(2, 1) + getNyu(0, 3))
                    * (3 * _sqr(getNyu(3, 0) + getNyu(1, 2)) - _sqr(getNyu(2, 1) + getNyu(0, 3)));
            pfi[6] = (getNyu(2, 0) - getNyu(0, 2))
                    * (_sqr(getNyu(3, 0) + getNyu(1, 2)) - _sqr(getNyu(2, 1) + getNyu(0, 3)))
                    + 4 * getNyu(1, 1) * (getNyu(3, 0 ) + getNyu(1, 2)) * (getNyu(2, 1) + getNyu(0, 3));
            pfi[7] = (3 * getNyu(2, 1) - getNyu(0, 3)) * (getNyu(2, 1) + getNyu(0, 3))
                    * (_sqr(getNyu(3, 0) + getNyu(1, 2)) - 3 * _sqr(getNyu(2, 1) + getNyu(0, 3)))
                    - (getNyu(3, 0) - 3 * getNyu(1, 2)) * (getNyu(3, 0 ) + getNyu(1, 2))
                    * (3 * _sqr(getNyu(3, 0) + getNyu(1, 2)) - _sqr(getNyu(2, 1) + getNyu(0, 3)));
        }
        return pfi;
    }

    double getNyu(short i, short j){
        if(myu[i][j] == inf){
            myu[i][j] = 0;
            cimg_forXY(img, x, y){
                myu[i][j] += pow(x - cent_x, i) * pow(y - cent_y, j)*getPix(x, y);
            }
            nyu[i][j] = (myu[i][j] * 1.0) / (pow(myu[0][0], (i + j) / 2 + 1) * 1.0);
        }
        return nyu[i][j];
    }

    unsigned short getPix(int x, int y){
        return alr * img(x, y) + alg * img(x, y) + alb * img(x, y);
    }
};

#endif // Image_H

