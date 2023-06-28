#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <functional>
#include "structs.h"

using ImageFilterFunction = std::function<Image(const Image&)>;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void applyFilter(const QString& imagePath, const ImageFilterFunction& filterFunction, const QString& outputFilename);

    void on_pushButton_clicked();

    void on_pushButtonMedian_clicked();

    void on_pushButtonAverage_clicked();

    void on_pushButton_2_clicked();

    void on_ButtonR_clicked();

    void on_ButtonG_clicked();

    void on_ButtonB_clicked();

    void on_pushButtonHighBoost_clicked();

    void on_pushButtonBluring_clicked();

    void on_pushButtonGlobalEq_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
