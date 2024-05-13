#include <QDate>
#include <QLabel>
#include <QHeaderView>
#include "TransactionsTable.h"

TransactionsTable::TransactionsTable(QWidget *parent) : QTableWidget(parent) {
  horizontalHeader()->setStretchLastSection(true);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setEditTriggers(QAbstractItemView::NoEditTriggers);
}

TransactionsTable::~TransactionsTable() {}

void TransactionsTable::setHeaders(QStringList headers) {
  setColumnCount(SEARCH_TABLE_COLUMN_COUNT);

  setHorizontalHeaderItem(
      SEARCH_TABLE_BANK_COLUMN,
      new QTableWidgetItem(
          QString(headers.at(DATABASE_TABLE_BANK_FIELD).toUpper())));
  setHorizontalHeaderItem(
      SEARCH_TABLE_DATE_COLUMN,
      new QTableWidgetItem(
          QString(headers.at(DATABASE_TABLE_DATE_FIELD).toUpper())));
  setHorizontalHeaderItem(
      SEARCH_TABLE_DESCRIPTION_COLUMN,
      new QTableWidgetItem(
          QString(headers.at(DATABASE_TABLE_DESCRIPTION_FIELD).toUpper())));
  setHorizontalHeaderItem(
      SEARCH_TABLE_CATEGORY_COLUMN,
      new QTableWidgetItem(
          QString(headers.at(DATABASE_TABLE_CATEGORY_FIELD).toUpper())));
  setHorizontalHeaderItem(
      SEARCH_TABLE_AMOUNT_COLUMN,
      new QTableWidgetItem(
          QString(headers.at(DATABASE_TABLE_AMOUNT_FIELD).toUpper())));
}

void TransactionsTable::addTransaction(int row, QStringList fields) {
  QString value;
  QLabel *label;
  QString dateFormat = QLocale().dateFormat(QLocale::ShortFormat);

  setVerticalHeaderItem(
      row, new QTableWidgetItem(fields.at(DATABASE_TABLE_ID_FIELD)));

  value = fields.at(DATABASE_TABLE_BANK_FIELD);
  label = new QLabel(value);
  setCellWidget(row, SEARCH_TABLE_BANK_COLUMN, label);

  value = fields.at(DATABASE_TABLE_DATE_FIELD);
  label = new QLabel(
      QDate::fromString(value, Qt::DateFormat::ISODate).toString(dateFormat));
  label->setAlignment(Qt::AlignCenter);
  setCellWidget(row, SEARCH_TABLE_DATE_COLUMN, label);

  value = fields.at(DATABASE_TABLE_DESCRIPTION_FIELD);
  label = new QLabel(value);
  setCellWidget(row, SEARCH_TABLE_DESCRIPTION_COLUMN, label);

  value = fields.at(DATABASE_TABLE_CATEGORY_FIELD);
  label = new QLabel(value);
  setCellWidget(row, SEARCH_TABLE_CATEGORY_COLUMN, label);

  value = fields.at(DATABASE_TABLE_AMOUNT_FIELD);
  label = new QLabel(QString::number(value.toDouble()));
  label->setAlignment(Qt::AlignRight);
  setCellWidget(row, SEARCH_TABLE_AMOUNT_COLUMN, label);
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
    QLabel *item = (QLabel *)cellWidget(row, SEARCH_TABLE_DESCRIPTION_COLUMN);
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
