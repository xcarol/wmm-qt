#include "TransactionsTable.h"
#include <QDate>
#include <QHeaderView>
#include <QLabel>

TransactionsTable::TransactionsTable(QWidget *parent)
    : CustomTableWidget(parent) {}

TransactionsTable::~TransactionsTable() {}

void TransactionsTable::setHeaders(QList<TransactionsTable::Table> headers,
                                   QString viewName) {
  int columnCount = 0;

  setColumnCount(headers.length());
  disableNesting();

  if (headers.contains(Table::BankColumn)) {
    setHorizontalHeaderItem(columnCount++, new QTableWidgetItem(FieldNames.at(
                                               Database::BankField)));
  }
  if (headers.contains(Table::DateColumn)) {
    setHorizontalHeaderItem(columnCount++, new QTableWidgetItem(FieldNames.at(
                                               Database::DateField)));
  }
  if (headers.contains(Table::DescriptionColumn)) {
    setHorizontalHeaderItem(columnCount++, new QTableWidgetItem(FieldNames.at(
                                               Database::DescriptionField)));
  }
  if (headers.contains(Table::CategoryColumn)) {
    setHorizontalHeaderItem(columnCount++, new QTableWidgetItem(FieldNames.at(
                                               Database::CategoryField)));
  }
  if (headers.contains(Table::AmountColumn)) {
    setHorizontalHeaderItem(columnCount++, new QTableWidgetItem(FieldNames.at(
                                               Database::AmountField)));
  }

  enableNesting();

  if (!viewName.isEmpty()) {
    tableName = viewName;
    restoreColumnsWidths(tableName);
  }
}

void TransactionsTable::addTransaction(
    int row, QList<TransactionsTable::Database> fields, QStringList values) {

  int fieldPosition = 0;

  if (fields.at(0) == Database::IdField) {
    setVerticalHeaderItem(row,
                          new QTableWidgetItem(values.at(fieldPosition++)));
  }

  for (int column = 0; fieldPosition < fields.length();
       fieldPosition++, column++) {
    QString value = values.at(fieldPosition);
    QTableWidgetItem *item;

    if (fields.at(fieldPosition) == Database::DateField) {
      item = new QTableWidgetItem(value.remove(trimTimeFromDate));
      item->setTextAlignment(Qt::AlignCenter);
    } else if (fields.at(fieldPosition) == Database::AmountField) {
      item = new QTableWidgetItem(QString::number(value.toDouble()));
      item->setTextAlignment(Qt::AlignRight);
    } else {
      item = new QTableWidgetItem(value);
    }

    setItem(row, column, item);
  }
}

QList<int> TransactionsTable::getAllTransactionIDs() {
  QList<int> ids;

  for (int row = 0; row < rowCount(); row++) {
    QTableWidgetItem *item = verticalHeaderItem(row);
    ids.append(item->text().toInt());
  }

  return ids;
}

QList<int> TransactionsTable::getSelectedTransactions() {
  QList<int> rows;

  foreach (QTableWidgetSelectionRange range, selectedRanges()) {
    for (int row = range.topRow(); row <= range.bottomRow(); row++) {
      rows.append(row);
    }
  }

  return rows;
}

QList<int> TransactionsTable::getSelectedTransactionIDs() {
  QList<int> ids;

  foreach (QTableWidgetSelectionRange range, selectedRanges()) {
    for (int row = range.topRow(); row <= range.bottomRow(); row++) {
      QTableWidgetItem *item = verticalHeaderItem(row);
      ids.append(item->text().toInt());
    }
  }

  return ids;
}

QStringList TransactionsTable::getSelectedTransactionDescriptions() {
  QStringList descriptions;

  for (int row : getSelectedTransactions()) {
    QLabel *item = (QLabel *)cellWidget(row, Table::DescriptionColumn);
    if (item) {
      descriptions.append(item->text());
    }
  }

  return descriptions;
}

void TransactionsTable::clearTransactions() {
  clear();
  setRowCount(0);
  setColumnCount(0);
}

void TransactionsTable::removeSelectedTransactions() {
  QList<QTableWidgetSelectionRange> ranges = selectedRanges();

  if (ranges.isEmpty()) {
    clearTransactions();
    return;
  }

  while (!ranges.isEmpty()) {
    removeRow(ranges.at(0).topRow());
    ranges = selectedRanges();
  }
}
