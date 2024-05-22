#include <QDate>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSettings>

#include "../dialogs/NewCategoryDialog.h"
#include "../lib/Database.h"
#include "../widgets/MessageBox.h"
#include "CategorizeView.h"
#include "ui_CategorizeView.h"

CategorizeView::CategorizeView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CategorizeView) {
  ui->setupUi(this);

  QSettings settings = QSettings("com.xicra", "wmm");
  QStringList filters = settings.value("filters").toStringList();
  ui->filterEdit->addItems(filters);

  connect(ui->filterEdit->lineEdit(), &QLineEdit::returnPressed, this,
          &CategorizeView::on_filterEdit_returnPressed);
}

CategorizeView::~CategorizeView() { delete ui; }

void CategorizeView::on_categoryComboBox_editTextChanged(const QString &arg1) {
  updateButtonsState();
}

void CategorizeView::on_filterEdit_currentIndexChanged(int index) {
  updateButtonsState();
}

void CategorizeView::on_filterEdit_editTextChanged(const QString &arg1) {
  updateButtonsState();
}

void CategorizeView::on_filterEdit_returnPressed() {
  on_searchButton_clicked();
}

void CategorizeView::on_searchButton_clicked() {
  searchUncategorizedRows();
  updateButtonsState();
}

void CategorizeView::on_searchResultsTable_itemSelectionChanged() {
  updateButtonsState();
}

void CategorizeView::on_addButton_clicked() {
  QString category = ui->categoryComboBox->currentText();
  QString filter = ui->searchResultsTable->currentItem()->text();

  NewCategoryDialog dialog = NewCategoryDialog(category, filter);

  if (dialog.exec() == true) {
    category = dialog.category;
    filter = dialog.filter;

    if (filterExists(filter)) {
      MessageBox::Warning(
          tr("Filter exists"),
          QString(tr("A filter called %1 already exists")).arg(filter));
      return;
    }

    saveFilter(category, filter);

    QMessageBox::StandardButton res = QMessageBox::question(
        QApplication::topLevelWidgets().first(), QString(tr("Update")),
        QString(tr("Do you want to apply category %1 with filter %2 to all "
                   "uncategorized transactions?"))
            .arg(category)
            .arg(filter));
    if (res == QMessageBox::StandardButton::No) {
      updateSelectedRows();
      return;
    }

    Database database = Database();
    int updatedRows = database.updateRowsCategory(filter, category);
    QString sqlError = database.getLastErrorText();

    if (sqlError.isEmpty()) {
      MessageBox::DatabaseSuccess(
          QString(tr("A total of %1 transactions updated.")).arg(updatedRows));
    } else {
      MessageBox::DatabaseError(sqlError);
    }

    searchUncategorizedRows();
  }
}

void CategorizeView::on_updateButton_clicked() { updateSelectedRows(); }

bool CategorizeView::filterExists(QString filter) {
  Database database = Database();
  QStringList filters = database.getFilter(filter);
  return filters.length() == 1;
}

void CategorizeView::searchUncategorizedRows() {
  QString filter = ui->filterEdit->currentText();
  Database database = Database();

  ui->categoryComboBox->clear();
  ui->categoryComboBox->addItem("");
  ui->categoryComboBox->addItems(database.getCategoryNames());

  ui->searchResultsTable->clearTransactions();

  setFilter(filter);

  QProgressDialog progress = QProgressDialog(
      QString(tr("Searching for rows with filter %1")).arg(filter),
      tr("Cancel"), 0, 0);
  progress.setWindowModality(Qt::WindowModal);
  progress.setWindowTitle(tr("Search..."));

  QList<QStringList> uncategorizedRows =
      database.getUncategorizedRows(filter, &progress);

  if (uncategorizedRows.length() == 0) {
    progress.cancel();
    QString error = database.getLastErrorText();
    if (!error.isEmpty()) {
      MessageBox::DatabaseError(database.getLastErrorText());
    }
    return;
  }

  int numberOfRows = uncategorizedRows.count();

  ui->numRowsLabel->setText(
      QString(tr("Uncategorized rows: %1")).arg(QString::number(numberOfRows)));
  ui->searchResultsTable->setRowCount(numberOfRows);

  ui->searchResultsTable->setHeaders(
      {
          TransactionsTable::Table::BankColumn,
          TransactionsTable::Table::DateColumn,
          TransactionsTable::Table::DescriptionColumn,
          TransactionsTable::Table::CategoryColumn,
          TransactionsTable::Table::AmountColumn,
      },
      "CategorizeView");

  progress.reset();
  progress.setMaximum(numberOfRows);

  for (int rowCount = 0; rowCount < numberOfRows; rowCount++) {

    progress.setLabelText(QString(tr("Processing %1 of %2 rows found."))
                              .arg(rowCount)
                              .arg(numberOfRows));

    ui->searchResultsTable->addTransaction(
        rowCount,
        {
            TransactionsTable::Database::IdField,
            TransactionsTable::Database::BankField,
            TransactionsTable::Database::DateField,
            TransactionsTable::Database::DescriptionField,
            TransactionsTable::Database::CategoryField,
            TransactionsTable::Database::AmountField,
        },
        uncategorizedRows.at(rowCount));

    progress.setValue(rowCount);

    if (progress.wasCanceled()) {
      break;
    }
  }
}

bool CategorizeView::saveFilter(QString category, QString filter) {
  Database database = Database();

  database.addFilter(category, filter);
  QString error = database.getLastErrorText();
  if (!error.isEmpty()) {
    MessageBox::DatabaseError(database.getLastErrorText());
    return false;
  }

  return true;
}

void CategorizeView::setFilter(QString filter) {
  QSettings settings = QSettings("com.xicra", "wmm");
  QStringList filters = settings.value("filters").toStringList();
  if (filters.contains(filter) == false) {
    filters.append(filter);
    settings.setValue("filters", filters);
    ui->filterEdit->addItem(filter);
  }
}

void CategorizeView::updateSelectedRows() {
  QString category = ui->categoryComboBox->currentText();
  QList<int> selectedTransactionsId =
      ui->searchResultsTable->getSelectedTransactionIDs();

  if (selectedTransactionsId.length() == 0) {
    selectedTransactionsId = ui->searchResultsTable->getAllTransactionIDs();
  }

  QMessageBox::StandardButton res = QMessageBox::question(
      QApplication::topLevelWidgets().first(), QString(tr("Update")),
      QString(tr("You're about to update %1 records "
                 "with the category: %2\nAre you sure?"))
          .arg(selectedTransactionsId.length())
          .arg(category));
  if (res == QMessageBox::StandardButton::No) {
    return;
  }

  updateUncategorizedRows(selectedTransactionsId);
}

void CategorizeView::updateUncategorizedRows(QList<int> rowIds) {
  Database database = Database();
  QString category = ui->categoryComboBox->currentText();

  ulong updatedRows = database.updateRowsCategory(rowIds, category);

  if (updatedRows != rowIds.length()) {
    MessageBox::DatabaseError(database.getLastErrorText());
    return;
  }

  updateView();
}

void CategorizeView::updateButtonsState() {
  QString category = ui->categoryComboBox->currentText();
  bool enabled =
      ui->searchResultsTable->rowCount() > 0 && category.length() > 0;
  ui->updateButton->setEnabled(enabled);
  ui->addButton->setEnabled(enabled);
}

void CategorizeView::updateView() {
  ui->categoryComboBox->clearEditText();
  ui->filterEdit->clearEditText();
  ui->searchResultsTable->removeSelectedTransactions();
}
