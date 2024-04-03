#include "mainwindow.h"
#include "csvfile.h"
#include "./ui_mainwindow.h"
#include <qfiledialog.h>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updatePreview()
{
    QStringList headers = csvFile.getRows(1);
}

void MainWindow::on_openFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"), ".", tr("Csv Files (*.csv)"));

    ui->fileNameEdit->setText(fileName);

    if (CsvFile::isValidCsvFile(fileName) && csvFile.read(fileName))
    {
        updatePreview();
    }

    //     QList<QList<QString>> fileBody;

    //     QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    //     db.setHostName("127.0.0.1");
    //     db.setDatabaseName("wmm");
    //     db.setUserName("root");
    //     db.setPassword("secret");

    //     if (!db.open()) {
    //         qDebug() << "Error al conectar a la base de dades: " << db.lastError();
    //         return;
    //     }

    //     bool firstLineHeaders = true;
    //     bool firstLineSeen = false;
    //         QSqlQuery query;
    //         query.prepare("INSERT INTO appointments (date, concept, amount) "
    //                       "VALUES (:date, :concept, :amount)");
    //         qDebug() << sline.at(0);
    //         qDebug() << sline.at(1);
    //         qDebug() << sline.at(5);
    //         query.bindValue(":date", sline.at(0));
    //         query.bindValue(":concept", sline.at(1));
    //         query.bindValue(":amount", sline.at(5).toDouble());
    //         qDebug() << query.boundValues();
    //         if (!query.exec()) {
    //             qDebug() << "Error al afegir la fila: " << query.lastError();
    //             db.close();
    //             return;
    //         }

    //     } while (true);
    //     // Tancar la connexió
    //     db.close();
    // }
}
