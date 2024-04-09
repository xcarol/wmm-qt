#include "CategorizeView.h"
#include "../lib/Database.h"
#include "ui_CategorizeView.h"
#include <QLabel>
#include <QList>
#include <QMessageBox>
#include <QDate>

CategorizeView::CategorizeView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CategorizeView) {
  ui->setupUi(this);
  ui->updateButton->setEnabled(false);
  ui->searchResultsTable->horizontalHeader()->setStretchLastSection(true);
}

CategorizeView::~CategorizeView() { delete ui; }

void CategorizeView::on_searchButton_clicked() {
  Database database = Database();

  ui->searchResultsTable->clear();
  ui->searchResultsTable->setRowCount(0);
  ui->searchResultsTable->setColumnCount(0);
  ui->categoryComboBox->addItems(database.getCategoryNames());

  QStringList labels = database.getColumnNames();
  uncategorizedRows = database.getUncategorizedRows(appliedFilter);

  if (labels.length() == 0 || uncategorizedRows.length() == 0) {
    QString error = database.getLastErrorText();
    if (!error.isEmpty()) {
      QMessageBox(QMessageBox::Icon::Critical, QString("Database error"),
                  QString(database.getLastErrorText()))
          .exec();
    }
    return;
  }

  int numberOfRows = uncategorizedRows.count();
  int numberOfColumns = uncategorizedRows.at(0).count();

  ui->searchResultsTable->setRowCount(numberOfRows);
  ui->searchResultsTable->setColumnCount(numberOfColumns);
  ui->searchResultsTable->verticalHeader()->setVisible(false);

  for (int n = 0; n < labels.length(); n++) {
    ui->searchResultsTable->setHorizontalHeaderItem(
        n, new QTableWidgetItem(QString(labels.at(n).toUpper())));
  }

  QLocale locale;
  QString dateFormat = locale.dateFormat(QLocale::ShortFormat);

  for (int rowCount = 0; rowCount < numberOfRows; rowCount++) {
    for (int columnCount = 0; columnCount < numberOfColumns; columnCount++) {

      QStringList row = uncategorizedRows.at(rowCount);
      QString value = row.at(columnCount);

      QLabel *label;
      switch (columnCount) {
      case DATE_COLUMN:
        label = new QLabel(QDate::fromString(value, Qt::DateFormat::ISODate)
                               .toString(dateFormat));
        label->setAlignment(Qt::AlignCenter);
        break;

      case AMOUNT_COLUMN:
        label = new QLabel(QString::number(value.toDouble(), 'g', 2));
        label->setAlignment(Qt::AlignRight);
        break;

      default:
        label = new QLabel(value);
      }
      ui->searchResultsTable->setCellWidget(rowCount, columnCount, label);
    }
  }

  updateUpdateButtonState();
}

void CategorizeView::on_updateButton_clicked() {
  QMessageBox::StandardButton res =
      QMessageBox::question(this, QString("Update"),
                            QString("You're about to update %1 records "
                                    "with the category: %2\nAre you sure?")
                                .arg(uncategorizedRows.length())
                                .arg(categoryName));
  if (res == QMessageBox::StandardButton::No) {
    return;
  }

  Database database = Database();

  QProgressDialog progress = QProgressDialog("Update progress", "Cancel", 0,
                                             uncategorizedRows.length());

  ulong updatedRows = database.updateRowsCategory(appliedFilter, categoryName);

  progress.cancel();

  if (updatedRows != uncategorizedRows.length()) {
    QMessageBox(QMessageBox::Icon::Critical, QString("Database error"),
                QString(database.getLastErrorText()))
        .exec();
  } else {
    QMessageBox(QMessageBox::Icon::Information, QString("Database success"),
                QString("A total of %1 rows updated").arg(updatedRows))
        .exec();
  }
}

void CategorizeView::on_categoryComboBox_editTextChanged(const QString &arg1) {
  categoryName = arg1;
  updateUpdateButtonState();
}

void CategorizeView::updateUpdateButtonState() {
  ui->updateButton->setEnabled(uncategorizedRows.length() > 0 &&
                               categoryName.length() > 0);
}

void CategorizeView::on_filterEdit_textChanged(const QString &arg1) {
  appliedFilter = arg1;
}
