#include "importfile.h"
#include "csvfile.h"
#include "ui_importfile.h"

#include <QFileDialog>
#include <QMessageBox>

ImportFile::ImportFile(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ImportFile) {
  ui->setupUi(this);
  ui->ImportButton->setEnabled(false);
}

ImportFile::~ImportFile() { delete ui; }

void ImportFile::updatePreview() {
  int rowsCount = 5;
  int headerRows = ui->firstRowCheckBox->isChecked();
  QList<QStringList> rows = csvFile.getRows(rowsCount);
  int columnsCount = rows.at(0).length();

  ui->conceptColumnComboBox->clear();
  ui->amountColumnComboBox->clear();
  ui->dateColumnComboBox->clear();

  ui->conceptColumnComboBox->addItem(QString(" "));
  ui->amountColumnComboBox->addItem(QString(" "));
  ui->dateColumnComboBox->addItem(QString(" "));

  if (headerRows) {
    foreach (QString field, rows.at(0)) {
      ui->conceptColumnComboBox->addItem(field);
      ui->amountColumnComboBox->addItem(field);
      ui->dateColumnComboBox->addItem(field);
    }
  } else {
    for (int n = 0; n < columnsCount; n++) {
      ui->conceptColumnComboBox->addItem(QString::number(n));
      ui->amountColumnComboBox->addItem(QString::number(n));
      ui->dateColumnComboBox->addItem(QString::number(n));
    }
  }

  ui->filePreviewTable->clear();
  ui->filePreviewTable->setRowCount(rowsCount - headerRows);
  ui->filePreviewTable->setColumnCount(columnsCount);

  if (headerRows) {
    QStringList headers = rows.at(0);
    for (int n = 0; n < headers.length(); n++) {
      ui->filePreviewTable->setHorizontalHeaderItem(
          n, new QTableWidgetItem(QString(headers.at(n))));
    }
  }

  for (int column = 0; column < columnsCount; column++) {
    ui->ImportButton->setEnabled(false);
    for (int row = 0; row < rowsCount - headerRows; row++) {
      ui->filePreviewTable->setCellWidget(
          row, column, new QLabel(rows.at(row + headerRows).at(column)));
    }
  }
}

void ImportFile::on_openFileButton_clicked() {
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".",
                                                  tr("Csv Files (*.csv)"));

  ui->fileNameEdit->setText(fileName);

  if (CsvFile::isValidCsvFile(fileName) && csvFile.read(fileName)) {
    updatePreview();
  }

  //     QList<QList<QString>> fileBody;

  //     QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
  //     db.setHostName("127.0.0.1");
  //     db.setDatabaseName("wmm");
  //     db.setUserName("root");
  //     db.setPassword("secret");

  //     if (!db.open()) {
  //         qDebug() << "Error al conectar a la base de dades: " <<
  //         db.lastError(); return;
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
  ui->ImportButton->setEnabled(false);

  //     } while (true);
  ui->ImportButton->setEnabled(false);
  //     // Tancar la connexió
  //     db.close();
  // }
}

void ImportFile::on_firstRowCheckBox_stateChanged(int arg1) {
  updatePreview();
  ui->ImportButton->setEnabled(false);
}

void ImportFile::on_dateColumnComboBox_currentIndexChanged(int index) {
  ui->ImportButton->setEnabled(ui->dateColumnComboBox->currentIndex() > 0 &&
                               ui->conceptColumnComboBox->currentIndex() > 0 &&
                               ui->amountColumnComboBox->currentIndex() > 0);
  ui->ImportButton->setEnabled(false);
}

void ImportFile::on_conceptColumnComboBox_currentIndexChanged(int index) {
  ui->ImportButton->setEnabled(ui->dateColumnComboBox->currentIndex() > 0 &&
                               ui->conceptColumnComboBox->currentIndex() > 0 &&
                               ui->amountColumnComboBox->currentIndex() > 0);
  ui->ImportButton->setEnabled(false);
}

void ImportFile::on_amountColumnComboBox_currentIndexChanged(int index) {
  ui->ImportButton->setEnabled(ui->dateColumnComboBox->currentIndex() > 0 &&
                               ui->conceptColumnComboBox->currentIndex() > 0 &&
                               ui->amountColumnComboBox->currentIndex() > 0);
}

void ImportFile::on_ImportButton_clicked() {
  QMessageBox box = QMessageBox(QMessageBox::Icon::Information,
                                QString("tachiiin"), QString("tachaaan"));
  box.exec();
}
