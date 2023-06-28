#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imagemanagement.h"
#include "structs.h"
#include "PPMimagetreatment.h"

#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPixmap>
#include <QSpinBox>

using ImageFilterFunction = std::function<Image(const Image&)>;

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

/*
 * FUNÇÃO LAMBDA REPONSÁVEL POR APLICAR AS OPERAÇÕES NAs IMAGEM
 *
 * Carrega a imagem pelo caminho especificado
 *
 * checa se a imagem foi carregada corretamente
 *
 * usa read_image() para ler a imagem
 *
 * Aplica a função passada como parâmetro para filterFunction()
 *
 * salva a imagem como P3
 *
 * Salva o resultado da operação com base no que foi passado em outputFilename
 */
void MainWindow::applyFilter(const QString& imagePath, const ImageFilterFunction& filterFunction, const QString& outputFilename) {
    QPixmap image(imagePath);

    if (image.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load image.");
        return;
    }

    Image newImage = read_image(imagePath.toUtf8().constData());  // Converter QString para const char*
    newImage = filterFunction(newImage);
    savePPMP3(outputFilename.toUtf8().constData(), newImage);

    QPixmap resImage(outputFilename);
    ui->image->setPixmap(resImage.scaled(471, 401, Qt::KeepAspectRatio));
}


void MainWindow::on_pushButton_clicked()
{
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

/*
 * Botão do Filtro da mediana
 */
void MainWindow::on_pushButtonMedian_clicked()
{
    QString imagePath = ui->comboBox->currentData().toString();   
    double height = ui->spinBoxMedian->value();
    applyFilter(imagePath, [height](const Image& image) {
            return median_filter(image, height);
    }, "result.ppm");
}

/*
 * Botão do fitltro da média
 */
void MainWindow::on_pushButtonAverage_clicked()
{
    QString imagePath = ui->comboBox->currentData().toString();
    double height = ui->spinBoxAverage->value();
    applyFilter(imagePath, [height](const Image &image) {
        return average_filter(image, height);
        }, "result.ppm");
}

/*
 * Botão do filtro da mediana / Laplaciano
 */
void MainWindow::on_pushButton_2_clicked()
{
    QString imagePath = ui->comboBox->currentData().toString();
    applyFilter(imagePath, laplace ,"result.ppm");
}

/*
 * Botão separador do canal R
 */
void MainWindow::on_ButtonR_clicked()
{
    QString imagePath = ui->comboBox->currentData().toString();
    applyFilter(imagePath, [](const Image& image) {
            return r(image, 'R');
        }, "result.ppm");
}

/*
 * Botão separador do canal G
 */
void MainWindow::on_ButtonG_clicked()
{
    QString imagePath = ui->comboBox->currentData().toString();
    applyFilter(imagePath, [](const Image& image) {
            return r(image, 'G');
        }, "result.ppm");
}

/*
 * Botão separador do canal B
 */
void MainWindow::on_ButtonB_clicked()
{
    QString imagePath = ui->comboBox->currentData().toString();
    applyFilter(imagePath, [](const Image& image) {
            return r(image, 'B');
        }, "result.ppm");
}

/*
 * botão Filtro High boost
 */
void MainWindow::on_pushButtonHighBoost_clicked()
{
    QString imagePath = ui->comboBox->currentData().toString();
    double boost = ui->doubleSpinBoxHighBoost->value();
    applyFilter(imagePath, [boost](const Image& image) {
            return high_boost(image, boost);
        }, "result.ppm");
}

/*
 * botão Borramento
 */
void MainWindow::on_pushButtonBluring_clicked()
{
    QString imagePath = ui->comboBox->currentData().toString();
    double filterHeight = ui->spinBoxBlurring->value();
    applyFilter(imagePath, [filterHeight](const Image& image) {
            return blurring(image, filterHeight);
        }, "result.ppm");
}

/*
 * Botão Equalização global
 */
void MainWindow::on_pushButtonGlobalEq_clicked()
{
    QString imagePath = ui->comboBox->currentData().toString();
    applyFilter(imagePath, histogram_equalization, "result.ppm");
}

