#include <QDate>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSettings>

#include "../lib/Database.h"
#include "DuplicatesView.h"
#include "ui_DuplicatesView.h"

DuplicatesView::DuplicatesView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::DuplicatesView) {
  ui->setupUi(this);
}

DuplicatesView::~DuplicatesView() { delete ui; }

void DuplicatesView::on_deleteDuplicatesButton_clicked() {
  QList<int> selectedIds = ui->duplicateRowsTable->getSelectedTransactionIDs();

  QMessageBox::StandardButton res = QMessageBox::question(
      QApplication::topLevelWidgets().first(), QString(tr("DELETE")),
      QString(tr("You're about to DELETE %1 RECORDS "
                 "\nAre you sure?"))
          .arg(selectedIds.length()));
  if (res == QMessageBox::StandardButton::No) {
    return;
  }

  deleteDuplicateRows(selectedIds);
  updateDuplicatesTable();
}

void DuplicatesView::on_duplicateRowsTable_itemSelectionChanged() {
  int enable = ui->duplicateRowsTable->getSelectedTransactionIDs().length() > 0;
  ui->deleteDuplicatesButton->setEnabled(enable);
  ui->markNotDuplicatesButton->setEnabled(enable);
}

void DuplicatesView::on_markNotDuplicatesButton_clicked() {
  QList<int> selectedIds = ui->duplicateRowsTable->getSelectedTransactionIDs();

  QMessageBox::StandardButton res = QMessageBox::question(
      QApplication::topLevelWidgets().first(),
      QString(tr("MARK NOT DUPLICATE")),
      QString(tr("You're about to mark %1 records as not duplicate."
                 "\nAre you sure?"))
          .arg(selectedIds.length()));
  if (res == QMessageBox::StandardButton::No) {
    return;
  }

  markDuplicateRows(selectedIds);
  updateDuplicatesTable();
}

void DuplicatesView::on_searchDuplicateButton_clicked() {
  updateDuplicatesTable();
}

void DuplicatesView::deleteDuplicateRows(QList<int> ids) {
  Database database = Database();

  ulong updatedRows = database.deleteRows(ids);

  if (updatedRows != ids.length()) {
    QMessageBox(QMessageBox::Icon::Critical, QString(tr("Database error")),
                QString(database.getLastErrorText()))
        .exec();
  } else {
    QMessageBox(QMessageBox::Icon::Information, QString(tr("Database success")),
                QString(tr("A total of %1 rows deleted.")).arg(updatedRows))
        .exec();
  }
}

void DuplicatesView::markDuplicateRows(QList<int> ids) {
  Database database = Database();

  ulong updatedRows = database.markAsNotDuplicateRows(ids);

  if (updatedRows != ids.length()) {
    QMessageBox(QMessageBox::Icon::Critical, QString(tr("Database error")),
                QString(database.getLastErrorText()))
        .exec();
  } else {
    QMessageBox(QMessageBox::Icon::Information, QString(tr("Database success")),
                QString(tr("A total of %1 rows marked as not duplicate."))
                    .arg(updatedRows))
        .exec();
  }
}

void DuplicatesView::updateDuplicatesTable() {
  Database database = Database();
  QList<QStringList> duplicates = database.getDuplicateRows();
  if (database.getLastErrorText().isEmpty() == false) {
    QMessageBox(QMessageBox::Icon::Critical, QString(tr("Database error")),
                QString(database.getLastErrorText()))
        .exec();
    return;
  }
  if (duplicates.isEmpty()) {
    QMessageBox(QMessageBox::Icon::Information, QString(tr("Database")),
                QString(tr("No duplicate rows found.")))
        .exec();
    return;
  }

  int numberOfColumns = 4;
  int numberOfRows = duplicates.length();

  ui->duplicateRowsLabel->setText(
      QString(tr("Duplicate rows: %1")).arg(numberOfRows));
  ui->duplicateRowsTable->clearTransactions();
  ui->duplicateRowsTable->setRowCount(numberOfRows);

  ui->duplicateRowsTable->setHeaders({
      TransactionsTable::Table::BankColumn,
      TransactionsTable::Table::DateColumn,
      TransactionsTable::Table::DescriptionColumn,
      TransactionsTable::Table::AmountColumn,
  });

  for (int rowCount = 0; rowCount < numberOfRows; rowCount++) {
    ui->duplicateRowsTable->addTransaction(
        rowCount,
        {
            TransactionsTable::Database::IdField,
            TransactionsTable::Database::BankField,
            TransactionsTable::Database::DateField,
            TransactionsTable::Database::DescriptionField,
            TransactionsTable::Database::AmountField,
        },
        duplicates.at(rowCount));
  }
}

