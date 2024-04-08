#include "CategorizeView.h"
#include "../helpers/Database.h"
#include "ui_CategorizeView.h"
#include <QLabel>
#include <QList>
#include <QMessageBox>

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

  int rowsCount = uncategorizedRows.count();
  int columnsCount = uncategorizedRows.at(0).count();

  ui->searchResultsTable->setRowCount(rowsCount);
  ui->searchResultsTable->setColumnCount(columnsCount);
  ui->searchResultsTable->verticalHeader()->setVisible(false);

  for (int n = 0; n < labels.length(); n++) {
    ui->searchResultsTable->setHorizontalHeaderItem(
        n, new QTableWidgetItem(QString(labels.at(n).toUpper())));
  }

  for (int column = 0; column < columnsCount; column++) {
    for (int row = 0; row < rowsCount; row++) {
      ui->searchResultsTable->setCellWidget(
          row, column, new QLabel(uncategorizedRows.at(row).at(column)));
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

  QProgressDialog progress =
      QProgressDialog("Update progress", "Cancel", 0, uncategorizedRows.length());

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
  ui->updateButton->setEnabled(uncategorizedRows.length() > 0 && categoryName.length() > 0);
}

void CategorizeView::on_filterEdit_textChanged(const QString &arg1)
{
  appliedFilter = arg1;
}

