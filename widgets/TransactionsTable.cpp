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
  setColumnCount(Table::TotalColumns);

  setHorizontalHeaderItem(
      Table::BankColumn,
      new QTableWidgetItem(
          QString(headers.at(Database::BankField).toUpper())));
  setHorizontalHeaderItem(
      Table::DateColumn,
      new QTableWidgetItem(
          QString(headers.at(Database::DateField).toUpper())));
  setHorizontalHeaderItem(
      Table::DescriptionColumn,
      new QTableWidgetItem(
          QString(headers.at(Database::DescriptionField).toUpper())));
  setHorizontalHeaderItem(
      Table::CategoryColumn,
      new QTableWidgetItem(
          QString(headers.at(Database::CategoryField).toUpper())));
  setHorizontalHeaderItem(
      Table::AmountColumn,
      new QTableWidgetItem(
          QString(headers.at(Database::AmountField).toUpper())));
}

void TransactionsTable::addTransaction(int row, QStringList fields) {
  QString value;
  QLabel *label;
  QString dateFormat = QLocale().dateFormat(QLocale::ShortFormat);

  setVerticalHeaderItem(
      row, new QTableWidgetItem(fields.at(Database::IdField)));

  value = fields.at(Database::BankField);
  label = new QLabel(value);
  setCellWidget(row, Table::BankColumn, label);

  value = fields.at(Database::DateField);
  label = new QLabel(
      QDate::fromString(value, Qt::DateFormat::ISODate).toString(dateFormat));
  label->setAlignment(Qt::AlignCenter);
  setCellWidget(row, Table::DateColumn, label);

  value = fields.at(Database::DescriptionField);
  label = new QLabel(value);
  setCellWidget(row, Table::DescriptionColumn, label);

  value = fields.at(Database::CategoryField);
  label = new QLabel(value);
  setCellWidget(row, Table::CategoryColumn, label);

  value = fields.at(Database::AmountField);
  label = new QLabel(QString::number(value.toDouble()));
  label->setAlignment(Qt::AlignRight);
  setCellWidget(row, Table::AmountColumn, label);
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
