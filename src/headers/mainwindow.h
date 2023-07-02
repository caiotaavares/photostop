#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <functional>
#include "../../structs/structs.h"

using ImageFilterFunction = std::function<Image(const Image&)>;
using ImageFilterFunctionPgm = std::function<ImagePgm(const ImagePgm&)>;

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
    void ppm_buttons(bool setButton);

    QString getImagePath();

    void applyFilter(const QString& imagePath, const ImageFilterFunction& filterFunction, const ImageFilterFunctionPgm& filterFunctionPgm, const QString& outputFilename);

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

    void on_pushButtonLoadPpm_clicked();

    void on_pushButtonlaplaciano8_clicked();

    void on_pushButtonNegativo_clicked();

    void on_pushButtonTurnPlus90_clicked();

    void on_pushButtonTurnMinus90_clicked();

    void on_pushButtonTurnPlus180_clicked();

    void on_pushButtonBinarizing_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
