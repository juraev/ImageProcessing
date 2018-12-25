#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#undef Bool
#define uchar unsigned char

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene;
    scene2 = new QGraphicsScene;
    image = nullptr;
    size = new QLabel;
    zoom = new QLabel;
    STATE = 0;
    scene->setBackgroundBrush(QColor::fromRgb(224,224,224));
    scene2->setBackgroundBrush(QColor::fromRgb(224,224,224));
    ui->graphicsView->setScene(scene);
    ui->graphicsView_2->setScene(scene2);
    ui->statusBar->addPermanentWidget(size);
    ui->statusBar->addWidget(zoom);
    connect(ui->openButton, SIGNAL(clicked()), this, SLOT(on_actionOpen_triggered()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(on_actionSave_as_triggered()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(on_actionClose_triggered()));
    connect(ui->computeButton, SIGNAL(clicked()), this, SLOT(on_actionComputeMoments_triggered()));
}

MainWindow::~MainWindow()
{
    delete ui;

    if (scene) {
        delete scene;
        scene = nullptr;
    }

    if (scene2) {
        delete scene2;
        scene2 = nullptr;
    }

    if (image) {
        delete image;
        image = nullptr;
    }

    if (size) {
        delete size;
        size = nullptr;
    }

    if (zoom) {
        delete zoom;
        zoom = nullptr;
    }
}

void MainWindow::clearImages(){
    if (scene) {
        delete scene;
        scene = new QGraphicsScene;
        scene->setBackgroundBrush(QColor::fromRgb(224,224,224));
        ui->graphicsView->setScene(scene);
    }

    if (scene2) {
        delete scene2;
        scene2 = new QGraphicsScene;
        scene2->setBackgroundBrush(QColor::fromRgb(224,224,224));
        ui->graphicsView_2->setScene(scene2);
    }

    if (image) {
        delete image;
        image = nullptr;
    }

    if (size) {
        delete size;
        size = new QLabel;
        ui->statusBar->addPermanentWidget(size);
    }


    this->setWindowTitle(WINDOW_TITLE);
    ui->graphicsView->resetTransform();
    ui->graphicsView->setFactor(0);
    ui->graphicsView_2->resetTransform();
    ui->graphicsView_2->setFactor(0);
    activateFunctionalities(false);
}

void MainWindow::activateFunctionalities(bool cond){
    ui->actionSave_as->setEnabled(cond);
    ui->actionClose->setEnabled(cond);
    ui->actionOpen->setEnabled(cond);
    ui->actionRename->setEnabled(cond);
    ui->actionRemove->setEnabled(cond);
    ui->closeButton->setEnabled(cond);
    ui->saveButton->setEnabled(cond);
    ui->actionTo_W_B->setEnabled(cond);
    ui->actionToGrayscale->setEnabled(cond);
    ui->actionComputeMoments->setEnabled(cond);
}

void MainWindow::on_actionAdjust_triggered() {

    int height = image->height();
    int width = image->width();
    int max_height = ui->graphicsView->height();
    int max_width = ui->graphicsView->width();
    int size,max_size;
    double val1 = 0;
    double v1 = max_height * 1.0 / (height * 1.0);
    double v2 = max_width * 1.0 / (width * 1.0);

    //Search maximum and minimum measurement of image and window
    size = qMin(width,height);
    max_size = qMin(max_width,max_height);

    //Depending on whether it is smaller or larger than the window
    if(v1 > 1 && v2 > 1){
        val1 = v1 > v2 ? v1: v2;
    } else if(true){
        val1 = v1 < v2 ? v1: v2;
    }
    ui->graphicsView->scale(val1,val1);
    ui->graphicsView_2->scale(val1, val1);
    adjustZoom();
}

void MainWindow::adjustZoom() {
    int num = 100*pow(1.2, ((GraphicsView*)ui->graphicsView)->getFactor());
    QString percentage = QString::number(num);
    zoom->setText(percentage + "%");
}

void MainWindow::on_actionOpen_triggered(QString nameOfFile){
    if(nameOfFile.isEmpty()){
        nameOfFile = QFileDialog::getOpenFileName(this, tr("Open images"));
    }
    if(!nameOfFile.isEmpty()){
        QFile file(nameOfFile);
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::critical(this, tr(WINDOW_CRITICAL),tr("Image cannot be opened."));
            return;
        }

        clearImages();

        image = new Image<uchar>(nameOfFile);
        scene->addPixmap(image->pixmapObject());

        this->setWindowTitle(image->name() + " - Pictures Viewer");
        activateFunctionalities(true);
        size->setText(QString::number(image->width()) + " x " + QString::number(image->height()));
        if (qMax(image->width(),image->height()) > qMin(ui->graphicsView->width(),ui->graphicsView->height()))
            on_actionAdjust_triggered();
        else
            adjustZoom();
    }
}

void MainWindow::on_actionSave_as_triggered(){
    QString newRoute = QFileDialog::getSaveFileName(this, tr("Save image"), QString(),
                                                    tr("All the files (*)::Image BPM (*.bpm);;Image GIF(*.gif);;Image JPG(*.jpg);;"
                                                       "Image JPEG (*.jpeg);;Image PNG (*.png);;Image PPM (*.ppm);;Image XBM(*.xbm);;Image XPM(*.xpm);;"));
    if(!newRoute.isEmpty()){
        QFile file(newRoute);
        if(!file.open(QIODevice::WriteOnly)){
            QMessageBox::critical(this, tr(WINDOW_CRITICAL), tr("The image can not be saved."));
        }
        return;
    }
    image->save(newRoute);
}

void MainWindow::on_actionRename_triggered(){
    QInputDialog inputRename;
    QString newName, newRoute;
    bool okButton;

    newName = inputRename.getText(nullptr ,"Rename image "," Enter a new name:",
                                         QLineEdit::Normal,image->baseName(), &okButton);

    if (okButton && !newName.isEmpty()){

        newRoute = image->directory() + newName + image->extension();

        //If there is no file with the same name overwrite
        if (!QFile::exists(newRoute)) {

            QFile::rename(image->path(),newRoute);
            image->load(newRoute);
            this->setWindowTitle(image->name() + " - Pictures viewer");
        }

        else {

            QMessageBox itExists;
            itExists.setWindowTitle(WINDOW_WARNING);
            itExists.setText("An image with the same name already exists.");
            itExists.setInformativeText("Do you want to overwrite the image?");
            itExists.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
            itExists.setButtonText(QMessageBox::Save, "Rename");
            itExists.setButtonText(QMessageBox::Cancel, "Cancel");
            itExists.setDefaultButton(QMessageBox::Cancel);

            //If you want to overwrite a file with the same name
            if (itExists.exec() == QMessageBox::Save) {

                QFile newFile(newRoute);
                if (!newFile.open(QIODevice::WriteOnly)) {
                    QMessageBox::critical(this, tr(WINDOW_CRITICAL), tr("The image can not be renamed."));
                    return;
                }
                QFile::remove(newRoute);
                QFile::rename(image->path(),newRoute);
                image->load(newRoute);
                this->setWindowTitle(image->name() + " - Pictures viewer");
            }
        }
    }
}

void MainWindow::on_actionEnlarge_triggered() {
    ui->graphicsView->scale(1.2,1.2);
    ui->graphicsView->setFactor(1);
    adjustZoom();
}

void MainWindow::on_actionReduce_triggered() {

    ui->graphicsView->scale(1/1.2,1/1.2);
    ui->graphicsView->setFactor(-1);
    adjustZoom();
}

void MainWindow::on_actionNormal_triggered() {
    qreal val = pow(1.2, -ui->graphicsView->getFactor());
    ui->graphicsView->scale(val,val);
    ui->graphicsView->setFactor(0);
    adjustZoom();
}

void MainWindow::on_actionRemove_triggered() {
    QMessageBox noticeRemove;
    noticeRemove.setWindowTitle(WINDOW_WARNING);
    noticeRemove.setText("You are about to delete the disk image.");
    noticeRemove.setInformativeText("Do you want to delete the image?");
    noticeRemove.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    noticeRemove.setButtonText(QMessageBox::Save, "Remove");
    noticeRemove.setButtonText(QMessageBox::Cancel, "Cancel");
    noticeRemove.setDefaultButton(QMessageBox::Cancel);

    //Remove from the disk if the approval is given
    if (noticeRemove.exec() == QMessageBox::Save) {
        QFile::remove(image->path());
        clearImages();
    }
}

void MainWindow::on_actionClose_triggered() {
    clearImages();
}

void MainWindow::on_actionQuit_triggered(){
    qApp->quit();
}

void MainWindow::on_actionFullscreen_triggered(bool cond) {
    if (cond)
        this->showFullScreen();
    else
        this->showMaximized();
}

void MainWindow::on_actionHelp_triggered() {
    QMessageBox::warning(this, tr(WINDOW_WARNING), tr("The documentation is not currently available."));
}

void MainWindow::on_actionAbout_triggered() {
    QMessageBox::about(this, tr(WINDOW_ABOUT), tr("     Image viewer with Qt. \n \n Advanced Operating Systems."));
}

void MainWindow::on_actionToGrayscale_triggered(){
    if(STATE == GRAYSCALE) return;
    scene2->addPixmap(image->getGrayScale(0.33, 0.33, 0.34));
    ui->graphicsView_2->setScene(scene2);
    activateFunctionalities(true);
}

void MainWindow::on_actionTo_W_B_triggered(){
    if(STATE == BINARY) return;
    if (scene2) {
        delete scene2;
        scene2 = new QGraphicsScene;
        scene2->setBackgroundBrush(QColor::fromRgb(224,224,224));
        ui->graphicsView_2->setScene(scene2);
    }
    scene2->addPixmap(image->getWB(100));
    ui->graphicsView_2->setScene(scene2);
    activateFunctionalities(true);
}

void MainWindow::on_actionComputeMoments_triggered(){
    double* a = image->getHusMoments();
    QFile file("moments.txt");
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        for (int i = 1; i < 8; i ++)
            stream << a[i] << endl;
    }
}
