#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imagemanagement.h"
#include "structs.h"
#include "PPMimagetreatment.h"

#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPixmap>
#include <QSpinBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Adiciona as imagens ao ComboBox
    ui->comboBox->addItem("Lenna (P3)", "lennap3.ppm");
    ui->comboBox->addItem("Lenna (P6)", "lennap6.ppm");

    QSpinBox *median = ui->spinBoxMedian;
    median->setValue(3);
    median->setSingleStep(2);
    median->setMinimum(3);

    QSpinBox *average = ui->spinBoxAverage;
    average->setValue(3);
    average->setSingleStep(2);
    average->setMinimum(3);

    QSpinBox *filterHeighBlurring = ui->spinBoxBlurring;
    filterHeighBlurring->setValue(3);
    filterHeighBlurring->setSingleStep(2);
    filterHeighBlurring->setMinimum(3);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    // Defina o valor inicial do caminho da imagem com base na seleção inicial do comboBox
    QString imagePath = ui->comboBox->currentData().toString();
    QPixmap image(imagePath);

    if (image.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load image.");
        return;
    }

    ui->image->setPixmap(image.scaled(471,401,Qt::KeepAspectRatio));
    Image newImage = read_image(imagePath.toUtf8().constData());  // Converter QString para const char*
    ui->textEditInfos->setPlainText(QString("Version: %1\nComment: %2\n%3 %4\n%5")
                                        .arg(QString::fromStdString(newImage.version),
                                        QString::fromStdString(newImage.comment),
                                         QString::number(newImage.numcols),
                                         QString::number(newImage.numrows),
                                         QString::number(newImage.maxval)
                                        ));
}
//
// @ FIltro da Mediana
//
void MainWindow::on_pushButtonMedian_clicked()
{
    QString imagePath = ui->comboBox->currentData().toString();
    QPixmap image(imagePath);

    if (image.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load image.");
        return;
    }

    Image newImage = read_image(imagePath.toUtf8().constData());  // Converter QString para const char*
    double height = ui->spinBoxMedian->value();
    newImage = median_filter(newImage, height);
    savePPMP3("result.ppm", newImage);

    QPixmap resImage ("result.ppm");
    ui->image->setPixmap(resImage.scaled(471,401,Qt::KeepAspectRatio));
}

//
// @FIltro da Média
//
void MainWindow::on_pushButtonAverage_clicked()
{
    QString imagePath = ui->comboBox->currentData().toString();
    QPixmap image(imagePath);

    if (image.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load image.");
        return;
    }

    Image newImage = read_image(imagePath.toUtf8().constData());  // Converter QString para const char*
    double height = ui->spinBoxAverage->value();
    newImage = average_filter(newImage, height);
    savePPMP3("result.ppm", newImage);

    QPixmap resImage ("result.ppm");
    ui->image->setPixmap(resImage.scaled(471,401,Qt::KeepAspectRatio));
}

void MainWindow::on_pushButton_2_clicked()
{
    QString imagePath = ui->comboBox->currentData().toString();
    QPixmap image(imagePath);

    if (image.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load image.");
        return;
    }

    Image newImage = read_image(imagePath.toUtf8().constData());  // Converter QString para const char*

    newImage = laplace(newImage);
    savePPMP3("result.ppm", newImage);

    QPixmap resImage ("result.ppm");
    ui->image->setPixmap(resImage.scaled(471,401,Qt::KeepAspectRatio));
}


void MainWindow::on_ButtonR_clicked()
{
    QString imagePath = ui->comboBox->currentData().toString();
    QPixmap image(imagePath);

    if (image.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load image.");
        return;
    }

    Image newImage = read_image(imagePath.toUtf8().constData());  // Converter QString para const char*

    newImage = r(newImage, 'R');
    savePPMP3("result.ppm", newImage);

    QPixmap resImage ("result.ppm");
    ui->image->setPixmap(resImage.scaled(471,401,Qt::KeepAspectRatio));
}


void MainWindow::on_ButtonG_clicked()
{
    QString imagePath = ui->comboBox->currentData().toString();
    QPixmap image(imagePath);

    if (image.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load image.");
        return;
    }

    Image newImage = read_image(imagePath.toUtf8().constData());  // Converter QString para const char*

    newImage = r(newImage, 'G');
    savePPMP3("result.ppm", newImage);

    QPixmap resImage ("result.ppm");
    ui->image->setPixmap(resImage.scaled(471,401,Qt::KeepAspectRatio));
}


void MainWindow::on_ButtonB_clicked()
{
    QString imagePath = ui->comboBox->currentData().toString();
    QPixmap image(imagePath);

    if (image.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load image.");
        return;
    }

    Image newImage = read_image(imagePath.toUtf8().constData());  // Converter QString para const char*

    newImage = r(newImage, 'B');
    savePPMP3("result.ppm", newImage);

    QPixmap resImage ("result.ppm");
    ui->image->setPixmap(resImage.scaled(471,401,Qt::KeepAspectRatio));
}

//High Boost
void MainWindow::on_pushButtonHighBoost_clicked()
{
    QString imagePath = ui->comboBox->currentData().toString();
    QPixmap image(imagePath);

    if (image.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load image.");
        return;
    }

    Image newImage = read_image(imagePath.toUtf8().constData());  // Converter QString para const char*
    double boost = ui->doubleSpinBoxHighBoost->value();
    newImage = high_boost(newImage, boost);
    savePPMP3("result.ppm", newImage);

    QPixmap resImage ("result.ppm");
    ui->image->setPixmap(resImage.scaled(471,401,Qt::KeepAspectRatio));
}


void MainWindow::on_pushButtonBluring_clicked()
{
    QString imagePath = ui->comboBox->currentData().toString();
    QPixmap image(imagePath);

    if (image.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load image.");
        return;
    }

    Image newImage = read_image(imagePath.toUtf8().constData());  // Converter QString para const char*
    double filterHeight = ui->spinBoxBlurring->value();
    newImage = blurring(newImage, filterHeight);
    savePPMP3("result.ppm", newImage);

    QPixmap resImage ("result.ppm");
    ui->image->setPixmap(resImage.scaled(471,401,Qt::KeepAspectRatio));
}


void MainWindow::on_pushButtonGlobalEq_clicked()
{
    QString imagePath = ui->comboBox->currentData().toString();
    QPixmap image(imagePath);

    if (image.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load image.");
        return;
    }

    Image newImage = read_image(imagePath.toUtf8().constData());  // Converter QString para const char*

    newImage = histogram_equalization(newImage);
    savePPMP3("result.ppm", newImage);

    QPixmap resImage ("result.ppm");
    ui->image->setPixmap(resImage.scaled(471,401,Qt::KeepAspectRatio));
}

