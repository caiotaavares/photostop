#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imagemanagement.h"
#include "structs.h"
#include "PPMimagetreatment.h"

#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Adiciona as imagens ao ComboBox
    ui->comboBox->addItem("Lenna", "lenna.ppm");
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


void MainWindow::on_pushButton_2_clicked()
{
    QString imagePath = ui->comboBox->currentData().toString();
    QPixmap image(imagePath);

    if (image.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load image.");
        return;
    }

    Image newImage = read_image(imagePath.toUtf8().constData());  // Converter QString para const char*
    newImage = median_filter(newImage);
    savePPMP3("result.ppm", newImage);

    QPixmap resImage ("result.ppm");
    ui->image->setPixmap(resImage.scaled(471,401,Qt::KeepAspectRatio));
}

