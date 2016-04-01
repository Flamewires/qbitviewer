#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFile"
#include "QFileDialog"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    oldSize = 1;
    imageLabel = new QLabel();
    ui->scrollArea->setVisible(true);
    ui->scrollArea->setEnabled(true);
    ui->scrollArea->setWidgetResizable(true);
    imageLabel->setAlignment(Qt::AlignTop);
    ui->scrollArea->setWidget(imageLabel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

char MainWindow::reverse(char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

void MainWindow::paintEvent(QPaintEvent *) {
    //If we dont have a file, just return.
    if (ui->lineEdit->text().trimmed().isEmpty()) return;

    //Only regenerate bit view if things have changed.
    if (ui->spinBox->value() != oldSize || ui->lineEdit->text() != oldFile || ui->spinBoxScale->value() != oldScale) {

        // Get file handle and file size
        QFile inFile(ui->lineEdit->text());
        QString filename = ui->lineEdit->text();
        ui->lineEdit->setText(filename);
        //If we cant get open the file, return.
        if (!inFile.open(QIODevice::ReadOnly)) return;

        size_t size = inFile.size();
        //qDebug() << "Size of file: " << size;

        //Dont process if size is 0, scrollwidget shouldn't allow it anyway, but to be safe.
        if (ui->spinBox->value() < 1) {
            return;
        }

        //Hard cap it at 3 meg for now
        if (size > 5000000)
            size = 5000000;

        // Setup the image, initialize it to black, then we dont have to set those bits.
        QSize sz;
        sz.setHeight(size/ui->spinBox->value());
        sz.setWidth(ui->spinBox->value());
        img = new QImage(sz, QImage::Format_RGB32);
        img->fill(QColor(Qt::black));

        //Grab bit array from image to manipulate
        uchar *bits = img->bits();


        char oData = '\0'; // Store each byte as we set pixels
        for (unsigned int i = 0; i < (uint)img->byteCount()/4 && (i/8)<size; i++) {
            if (i%8 == 0) {
                inFile.read( &oData, 1 );
            }
            char dat = ((reverse(oData) >> (i%8)) & 1);
            if (dat == 1) {
                const int bp = 4; // Bytes/pixel
                bits[bp*i] = 0xFF;
                bits[bp*i+1] = 0xFF;
                bits[bp*i+2] = 0xFF;
                bits[bp*i+3] = 0xFF;
            }
        }
        if (ui->spinBoxScale->value() > 1) {
            *img = img->scaledToHeight(img->height()*(1+(.25*ui->spinBoxScale->value())));
            *img = img->scaledToWidth(img->width()*(1+(.25*ui->spinBoxScale->value())));
        }
        imageLabel->setPixmap(QPixmap::fromImage(*img));

        //Cleanup.
        inFile.close();
        delete img;

        //Save off values, to avoid reprocessing.
        oldSize = ui->spinBox->value();
        oldScale = ui->spinBoxScale->value();
        oldFile = filename;
    }

    this->show();
}

void MainWindow::setFilename(QString filename) {
    if (filename.trimmed().isEmpty()) return;
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open file:" << filename;
        close();
        return;
    } else f.close();
    ui->lineEdit->setText(filename);
    repaint();
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog *fd = new QFileDialog();
    QString filename = fd->getOpenFileName();
    if(filename.trimmed().isEmpty()) return;
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) return; else f.close();
    ui->lineEdit->setText(filename);
    repaint();
}

void MainWindow::on_actionClose_triggered()
{
    this->close();
}

void MainWindow::on_spinBoxScale_valueChanged(int arg1)
{
    if (arg1 > 1)
        repaint();
}
