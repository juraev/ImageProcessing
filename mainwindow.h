#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define WINDOW_TITLE "Pictures viewer"
#define WINDOW_CRITICAL "Error - Image viewer"
#define WINDOW_WARNING "Notice - Image viewer"
#define WINDOW_ABOUT "About - Image viewer"
#define GRAYSCALE 1
#define BINARY 2

#include <QMainWindow>
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QLabel>
#include "image.h"
#include "graphicsview.hpp"
#undef Bool
#define uchar unsigned char

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsScene *scene2;
    Image<uchar> *image;
    QLabel *size, *zoom;
    int STATE;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    public slots:
//  File
    void on_actionOpen_triggered(QString rutaImage = "");
    void on_actionSave_as_triggered();
    void on_actionClose_triggered();
    void on_actionQuit_triggered();
//  Edit
    void on_actionRename_triggered();
    void on_actionRemove_triggered();
//  Tools

    void on_actionToGrayscale_triggered();
    void on_actionTo_W_B_triggered();
    void on_actionComputeMoments_triggered();

//  Preferences
    void on_actionFullscreen_triggered(bool cond);

//  Help
    void on_actionHelp_triggered();
    void on_actionAbout_triggered();

    void on_actionEnlarge_triggered();
    void on_actionReduce_triggered();
    void on_actionNormal_triggered();
    void on_actionAdjust_triggered();
    void adjustZoom();
public:
    void clearImages();
    void activateFunctionalities(bool cond);
};

#endif // MAINWINDOW_H
