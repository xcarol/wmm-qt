#include "TransactionsTable.h"
#include <QDate>
#include <QHeaderView>
#include <QLabel>

TransactionsTable::TransactionsTable(QWidget *parent) : QTableWidget(parent) {
  horizontalHeader()->setStretchLastSection(true);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setEditTriggers(QAbstractItemView::NoEditTriggers);

  connect(horizontalHeader(), &QHeaderView::sectionResized, this,
          &TransactionsTable::on_headerResized);
}

TransactionsTable::~TransactionsTable() {}

void TransactionsTable::on_headerResized() { saveColumnsWidths(); }

void TransactionsTable::setHeaders(QList<TransactionsTable::Table> headers,
                                   QString viewName) {
  int columnCount = 0;

  setColumnCount(headers.length());

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

  QString dateFormat = QLocale().dateFormat(QLocale::ShortFormat);
  for (int column = 0; fieldPosition < fields.length();
       fieldPosition++, column++) {
    QLabel *label;
    QString value = values.at(fieldPosition);

    if (fields.at(fieldPosition) == Database::DateField) {
      label = new QLabel(QDate::fromString(value, Qt::DateFormat::ISODate)
                             .toString(dateFormat));
      label->setAlignment(Qt::AlignCenter);
    } else if (fields.at(fieldPosition) == Database::AmountField) {
      label = new QLabel(QString::number(value.toDouble()));
      label->setAlignment(Qt::AlignRight);
    } else {
      label = new QLabel(value);
    }

    setCellWidget(row, column, label);
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

void TransactionsTable::saveColumnsWidths() {
  if (tableName.isEmpty()) {
    return;
  }

  for (int column = 0; column < columnCount(); column++) {
    qDebug() << QString(settingTemplate).arg(tableName).arg(column);
    qDebug() << columnWidth(column);
    settings.setValue(QString(settingTemplate).arg(tableName).arg(column), columnWidth(column));
  }
}

void TransactionsTable::restoreColumnsWidths(QString tableName) {
  for (int column = 0; column < columnCount(); column++) {
    int width =
        settings.value(QString(settingTemplate).arg(tableName).arg(column), "0")
            .toInt();
    qDebug() << QString(settingTemplate).arg(tableName).arg(column);
    qDebug() << width;
    if (width) {
      setColumnWidth(column, width);
    }
  }
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
