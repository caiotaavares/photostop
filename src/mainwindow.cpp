#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "headers/image_management.h"
#include "../structs/structs.h"
#include "headers/ppm.h"

#include "headers/pgm.h"

#include <QMessageBox>
#include <QPixmap>
#include <string>

using namespace std;

/*
 * 1 = PPM
 * 0 = PGM
 */
int GLOBAL_VERSION = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->spinBoxBinarizing->setMaximum(255);
    ui->spinBoxBinarizing->setMinimum(1);

    ui->spinBoxDarken->setMaximum(255);
    ui->spinBoxDarken->setMinimum(1);

    ui->spinBoxWhiten->setMaximum(255);
    ui->spinBoxWhiten->setMinimum(1);

    ui->spinBoxBinTerB->setMaximum(255);
    ui->spinBoxBinTerB->setMinimum(1);
    ui->spinBoxBinTerA->setMaximum(255);
    ui->spinBoxBinTerA->setMinimum(1);
    ui->spinBoxBinTerLimSup->setMaximum(255);
    ui->spinBoxBinTerLimSup->setMinimum(1);

    ui->spinBoxBinQuatB->setMaximum(255);
    ui->spinBoxBinQuatB->setMinimum(1);
    ui->spinBoxBinQuatA->setMaximum(255);
    ui->spinBoxBinQuatA->setMinimum(1);
    ui->spinBoxBinQuatLimSup->setMaximum(255);
    ui->spinBoxBinQuatLimSup->setMinimum(1);
    ui->spinBoxBinQuatLimInf->setMaximum(255);
    ui->spinBoxBinQuatLimInf->setMinimum(1);

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

void MainWindow::ppm_buttons(bool setButton) {
    ui->ButtonR->setEnabled(setButton);
    ui->ButtonG->setEnabled(setButton);
    ui->ButtonB->setEnabled(setButton);
}

void MainWindow::pgm_buttons(bool setButton) {
    ui->pushButtonBinarizing->setEnabled(setButton);
    ui->pushButtonDarken->setEnabled(setButton);
    ui->pushButtonWhiten->setEnabled(setButton);
    ui->pushButtonBinTer->setEnabled(setButton);
    ui->pushButtonBinQuat->setEnabled(setButton);
    ui->pushButtonNegativo->setEnabled(setButton);
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
void MainWindow::applyFilter(const QString& imagePath, const ImageFilterFunction& filterFunction, const ImageFilterFunctionPgm& filterFunctionPgm, const QString& outputFilename)
{
    QPixmap image(imagePath);

    if (image.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load image.");
        return;
    }

    bool isChecked = ui->checkBoxSeq->isChecked();
    if (GLOBAL_VERSION == 1) {
        // Ler PPM
        Image newImage;
        if (isChecked) {
            Image newImage = read_ppm(outputFilename.toUtf8().constData());
        } else {
            Image newImage = read_ppm(imagePath.toUtf8().constData());
        }
        newImage = filterFunction(newImage);
        savePPMP3(outputFilename.toUtf8().constData(), newImage);

        QPixmap resImage(outputFilename);
        ui->image->setPixmap(resImage.scaled(471, 401, Qt::KeepAspectRatio));
    } else if (GLOBAL_VERSION == 0) {
        // Ler PGM
        ImagePgm newImagePgm;
        if (isChecked) {
            newImagePgm = read_pgm(outputFilename.toUtf8().constData());
        } else {
            newImagePgm = read_pgm(imagePath.toUtf8().constData());
        }
        newImagePgm = filterFunctionPgm(newImagePgm);
        SavePGMP2(outputFilename.toUtf8().constData(), newImagePgm);

        const QPixmap resImage(outputFilename);
        ui->image->setPixmap(resImage.scaled(471, 401, Qt::KeepAspectRatio));
    }
}

QString MainWindow::getImagePath() {
//    return ui->comboBox->currentData().toString();
    return ui->lineImagePath->text();
}

/*
 * Botão ler PPM
 */
void MainWindow::on_pushButton_clicked()
{
    GLOBAL_VERSION = 1;
    QString imagePath = getImagePath();
    QPixmap image(imagePath);

    if (image.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load image.");
        return;
    }

    ppm_buttons(true);
    pgm_buttons(false);

    ui->image->setPixmap(image.scaled(471,401,Qt::KeepAspectRatio));
    Image newImage = read_ppm(imagePath.toUtf8().constData());  // Converter QString para const char*
    ui->textEditInfos->setPlainText(QString("Version: %1\nComment: %2\n%3 %4\n%5")
                                        .arg(QString::fromStdString(newImage.version),
                                        QString::fromStdString(newImage.comment),
                                         QString::number(newImage.numcols),
                                         QString::number(newImage.numrows),
                                         QString::number(newImage.maxval)
                                        ));
}

/*
 * Botão ler PGM
 */
void MainWindow::on_pushButtonLoadPpm_clicked()
{
    GLOBAL_VERSION = 0;
    QString imagePath = getImagePath();
    QPixmap image(imagePath);

    if (image.isNull()) {
        QMessageBox::warning(this, "Error", "Failed to load image.");
        return;
    }

    ppm_buttons(false);
    pgm_buttons(true);

    ui->image->setPixmap(image.scaled(471,401,Qt::KeepAspectRatio));
    ImagePgm newImage = read_pgm(imagePath.toUtf8().constData());
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
    QString imagePath = getImagePath();
    double height = ui->spinBoxMedian->value();
    if (GLOBAL_VERSION == 0) {
        applyFilter(imagePath, nullptr, [height](const ImagePgm& imagepgm) {
                return median_filter_pgm(imagepgm, height);
            }, "resultpgm.pgm");
    }

    if (GLOBAL_VERSION == 1) {
        applyFilter(imagePath, [height](const Image& image) {
                return median_filter(image, height);
            }, nullptr, "result.ppm");
    }
}

/*
 * Botão do fitltro da média
 */
void MainWindow::on_pushButtonAverage_clicked()
{
    QString imagePath = getImagePath();
    double height = ui->spinBoxMedian->value();
    if (GLOBAL_VERSION == 0) {
        applyFilter(imagePath, nullptr, [height](const ImagePgm& imagepgm) {
                return average_filter_pgm(imagepgm, height);
            }, "result.pgm");
    }

    if (GLOBAL_VERSION == 1) {
        applyFilter(imagePath, [height](const Image& image) {
                return average_filter(image, height);
            }, nullptr, "result.ppm");
    }
}

/*
 * Laplaciano
 */
void MainWindow::on_pushButton_2_clicked()
{
    QString imagePath = getImagePath();
    if (GLOBAL_VERSION == 0) {
        applyFilter(imagePath, nullptr, laplacePgm, "result.pgm");
    }

    if (GLOBAL_VERSION == 1) {
        applyFilter(imagePath, laplace, nullptr, "result.pgm");
    }
}
 /*
  * Laplaciano de vizinhança 8
  */
void MainWindow::on_pushButtonlaplaciano8_clicked()
{
    QString imagePath = getImagePath();
    if (GLOBAL_VERSION == 0) {
        applyFilter(imagePath, nullptr, laplacePgm_8, "result.pgm");
    }

    if (GLOBAL_VERSION == 1) {
        applyFilter(imagePath, laplace_8, nullptr, "result.pgm");
    }
}

/*
 * Botão separador do canal R
 */
void MainWindow::on_ButtonR_clicked()
{
    QString imagePath = getImagePath();
    applyFilter(imagePath, [](const Image& image) {
            return r(image, 'R');
        }, nullptr, "result.ppm");
}

/*
 * Botão separador do canal G
 */
void MainWindow::on_ButtonG_clicked()
{
    QString imagePath = getImagePath();
    applyFilter(imagePath, [](const Image& image) {
            return r(image, 'G');
        }, nullptr, "result.ppm");
}

/*
 * Botão separador do canal B
 */
void MainWindow::on_ButtonB_clicked()
{
    QString imagePath = getImagePath();
    applyFilter(imagePath, [](const Image& image) {
            return r(image, 'B');
        }, nullptr, "result.ppm");
}

/*
 * botão Filtro High boost
 */
void MainWindow::on_pushButtonHighBoost_clicked()
{
    QString imagePath = getImagePath();
    double boost = ui->doubleSpinBoxHighBoost->value();
    if (GLOBAL_VERSION == 0) {
        applyFilter(imagePath, nullptr, [boost](const ImagePgm& imagepgm) {
                return highBoost_filter_pgm(imagepgm, boost);
            }, "result.pgm");
    }

    if (GLOBAL_VERSION == 1) {
        applyFilter(imagePath, [boost](const Image& image) {
                return high_boost(image, boost);
            }, nullptr, "result.ppm");
    }
}

/*
 * botão Borramento
 */
void MainWindow::on_pushButtonBluring_clicked()
{
    QString imagePath = getImagePath();
    double filterHeight = ui->spinBoxBlurring->value();
    if (GLOBAL_VERSION == 0) {
        applyFilter(imagePath, nullptr, [filterHeight](const ImagePgm& imagepgm) {
                return blurringPgm(imagepgm, filterHeight);
            }, "result.pgm");
    }

    if (GLOBAL_VERSION == 1) {
        applyFilter(imagePath, [filterHeight](const Image& image) {
                return blurring(image, filterHeight);
            }, nullptr, "result.ppm");
    }
}

/*
 * Botão Equalização global
 */
void MainWindow::on_pushButtonGlobalEq_clicked()
{
    QString imagePath = getImagePath();
    if (GLOBAL_VERSION == 0) {
        applyFilter(imagePath, nullptr, [](const ImagePgm& imagepgm) {
                return histogram_equalization_pgm(imagepgm);
            }, "result.pgm");
    }

    if (GLOBAL_VERSION == 1) {
        applyFilter(imagePath, [](const Image& image) {
                return histogram_equalization(image);
            }, nullptr, "result.ppm");
    }
}

/*
 * Botão Negativo
 */
void MainWindow::on_pushButtonNegativo_clicked()
{
    QString imagePath = getImagePath();
    if (GLOBAL_VERSION == 0) {
        applyFilter(imagePath, nullptr, [](const ImagePgm& imagepgm) {
                return negative_pgm(imagepgm);
            }, "result.pgm");
    }
}

/*
 * Botão girar 90º
 */
void MainWindow::on_pushButtonTurnPlus90_clicked()
{
    QString imagePath = getImagePath();
    if (GLOBAL_VERSION == 0) {
        applyFilter(imagePath, nullptr, [](const ImagePgm& imagepgm) {
                return turn_plus_90_pgm(imagepgm);
            }, "result.pgm");
    }
    if (GLOBAL_VERSION == 1) {
        applyFilter(imagePath, [](const Image& image) {
                return turn_plus_90(image);
            }, nullptr, "result.ppm");
    }
}

/*
 * Botão Girar -90º
 */
void MainWindow::on_pushButtonTurnMinus90_clicked()
{
    QString imagePath = getImagePath();
    if (GLOBAL_VERSION == 0) {
        applyFilter(imagePath, nullptr, [](const ImagePgm& imagepgm) {
                return turn_minus_90_pgm(imagepgm);
            }, "result.pgm");
    }
    if (GLOBAL_VERSION == 1) {
        applyFilter(imagePath, [](const Image& image) {
                return turn_minus_90(image);
            }, nullptr, "result.ppm");
    }
}

/*
 * Botão girar +180º
 */
void MainWindow::on_pushButtonTurnPlus180_clicked()
{
    QString imagePath = getImagePath();
    if (GLOBAL_VERSION == 0) {
        applyFilter(imagePath, nullptr, [](const ImagePgm& imagepgm) {
                return turn_plus_180_pgm(imagepgm);
            }, "result.pgm");
    }
    if (GLOBAL_VERSION == 1) {
        applyFilter(imagePath, [](const Image& image) {
                return turn_plus_180(image);
            }, nullptr, "result.ppm");
    }
}

/*
 * Botão binarização
 */
void MainWindow::on_pushButtonBinarizing_clicked()
{
    QString imagePath = getImagePath();
    double grey_scale = ui->spinBoxBinarizing->value();
    if (GLOBAL_VERSION == 0) {
        applyFilter(imagePath, nullptr, [grey_scale](const ImagePgm& imagepgm) {
                return binarizing_pgm(imagepgm, grey_scale);
            }, "result.pgm");
    }
}

/*
 * Espelhamento
 */
void MainWindow::on_pushButtonMirror_clicked()
{
    QString imagePath = getImagePath();
    if (GLOBAL_VERSION == 0) {
        applyFilter(imagePath, nullptr, [](const ImagePgm& imagepgm) {
                return horizontal_mirror_left_pgm(imagepgm);
            }, "result.pgm");
    }
    if (GLOBAL_VERSION == 1) {
        applyFilter(imagePath, [](const Image& image) {
                return horizontal_mirror_left(image);
            }, nullptr, "result.ppm");
    }
}


void MainWindow::on_pushButtonDarken_clicked()
{
    QString imagePath = getImagePath();
    double grey_scale = ui->spinBoxDarken->value();
    if (GLOBAL_VERSION == 0) {
        applyFilter(imagePath, nullptr, [grey_scale](const ImagePgm& imagepgm) {
                return darken_pgm(imagepgm, grey_scale);
            }, "result.pgm");
    }
}


void MainWindow::on_pushButtonWhiten_clicked()
{
    QString imagePath = getImagePath();
    double grey_scale = ui->spinBoxWhiten->value();
    if (GLOBAL_VERSION == 0) {
        applyFilter(imagePath, nullptr, [grey_scale](const ImagePgm& imagepgm) {
                return whiten_pgm(imagepgm, grey_scale);
            }, "result.pgm");
    }
}


void MainWindow::on_pushButtonBinTer_clicked()
{
    QString imagePath = getImagePath();
    double a = ui->spinBoxBinTerA->value();
    double b = ui->spinBoxBinTerB->value();
    double lim_sup = ui->spinBoxBinTerLimSup->value();
    if (GLOBAL_VERSION == 0) {
        applyFilter(imagePath, nullptr, [a, b, lim_sup](const ImagePgm& imagepgm) {
                return variables_binarize_3_factors_pgm(imagepgm, a, b, lim_sup);
            }, "result.pgm");
    }
}


void MainWindow::on_pushButtonBinQuat_clicked()
{
    QString imagePath = getImagePath();
    double a = ui->spinBoxBinQuatA->value();
    double b = ui->spinBoxBinQuatB->value();
    double lim_sup = ui->spinBoxBinQuatLimSup->value();
    double lim_inf = ui->spinBoxBinQuatLimInf->value();
    if (GLOBAL_VERSION == 0) {
        applyFilter(imagePath, nullptr, [a, b, lim_sup, lim_inf](const ImagePgm& imagepgm) {
                return variables_binarize_4_factors_pgm(imagepgm, a, b, lim_sup, lim_inf);
            }, "result.pgm");
    }
}

