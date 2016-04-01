#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QLabel"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    //MainWindow(QWidget *parent, QString filename);
    void setFilename(QString filename);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QImage *img;
    QLabel *imageLabel;
    int oldSize;
    int oldScale;
    char reverse(char b);
    QString oldFile;

protected:
  void paintEvent(QPaintEvent *);
private slots:
  void on_actionOpen_triggered();
  void on_actionClose_triggered();
  void on_spinBoxScale_valueChanged(int arg1);
};

#endif // MAINWINDOW_H
