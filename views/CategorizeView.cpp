#include <QDate>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSettings>

#include "../lib/Database.h"
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
  categoryName = arg1;
  updateUpdateButtonState();
}

void CategorizeView::on_filterEdit_currentIndexChanged(int index) {
  appliedFilter = ui->filterEdit->itemText(index);
}

void CategorizeView::on_filterEdit_editTextChanged(const QString &arg1) {
  appliedFilter = arg1;
}

void CategorizeView::on_filterEdit_returnPressed() {
  on_searchButton_clicked();
}

void CategorizeView::on_searchButton_clicked() {
  searchUncategorizedRows();
  updateUpdateButtonState();
}

void CategorizeView::on_searchResultsTable_itemSelectionChanged() {
  updateUpdateButtonState();
}

void CategorizeView::on_updateButton_clicked() {
  QList<int> selectedTransactionsId =
      ui->searchResultsTable->getSelectedTransactionIDs();
  QList<int> selectedTransactions =
      ui->searchResultsTable->getSelectedTransactions();
  QStringList selectedTransactionsDescription =
      ui->searchResultsTable->getSelectedTransactionDescriptions();
  QString category = categoryName;
  QString filter = appliedFilter;

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

  if (selectedTransactionsDescription.isEmpty()) {
    addFiltersToDatabase(category, QStringList(filter));
  } else {
    addFiltersToDatabase(category, selectedTransactionsDescription);
  }
}

void CategorizeView::searchUncategorizedRows() {
  Database database = Database();

  ui->categoryComboBox->clear();
  ui->categoryComboBox->addItem("");
  ui->categoryComboBox->addItems(database.getCategoryNames());

  ui->searchResultsTable->clearTransactions();

  setFilter(appliedFilter);

  QProgressDialog progress = QProgressDialog(
      QString(tr("Searching for rows with filter %1")).arg(appliedFilter),
      tr("Cancel"), 0, 0);
  progress.setWindowModality(Qt::WindowModal);
  progress.setWindowTitle(tr("Search..."));

  QList<QStringList> uncategorizedRows =
      database.getUncategorizedRows(appliedFilter, &progress);

  if (uncategorizedRows.length() == 0) {
    progress.cancel();
    QString error = database.getLastErrorText();
    if (!error.isEmpty()) {
      QMessageBox(QMessageBox::Icon::Critical, QString(tr("Database error")),
                  QString(database.getLastErrorText()))
          .exec();
    }
    return;
  }

  int numberOfRows = uncategorizedRows.count();

  ui->numRowsLabel->setText(
      QString(tr("Uncategorized rows: %1")).arg(QString::number(numberOfRows)));
  ui->searchResultsTable->setRowCount(numberOfRows);

  ui->searchResultsTable->setHeaders({
      TransactionsTable::Table::BankColumn,
      TransactionsTable::Table::DateColumn,
      TransactionsTable::Table::DescriptionColumn,
      TransactionsTable::Table::CategoryColumn,
      TransactionsTable::Table::AmountColumn,
  });

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

void CategorizeView::setFilter(QString filter) {
  QSettings settings = QSettings("com.xicra", "wmm");
  QStringList filters = settings.value("filters").toStringList();
  if (filters.contains(appliedFilter) == false) {
    filters.append(appliedFilter);
    settings.setValue("filters", filters);
    ui->filterEdit->addItem(appliedFilter);
  }
}

void CategorizeView::addFiltersToDatabase(QString category,
                                          QStringList filters) {
  Database database = Database();
  database.addFilters(category, filters);
}

void CategorizeView::updateUncategorizedRows(QList<int> rowIds) {
  Database database = Database();

  ulong updatedRows = database.updateRowsCategory(rowIds, categoryName);

  if (updatedRows != rowIds.length()) {
    QMessageBox(QMessageBox::Icon::Critical, QString(tr("Database error")),
                QString(database.getLastErrorText()))
        .exec();
  } else {
    QMessageBox(QMessageBox::Icon::Information, QString(tr("Database success")),
                QString(tr("A total of %1 rows updated")).arg(updatedRows))
        .exec();
  }

  updateView();
}

void CategorizeView::updateUpdateButtonState() {
  ui->updateButton->setEnabled(ui->searchResultsTable->rowCount() > 0 &&
                               categoryName.length() > 0);
}

void CategorizeView::updateView() {
  ui->categoryComboBox->clearEditText();
  ui->filterEdit->clearEditText();
  ui->searchResultsTable->removeSelectedTransactions();
}
