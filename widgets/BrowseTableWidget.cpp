#include "BrowseTableWidget.h"
#include "CustomTableWidget.h"
#include <QHeaderView>
#include <QLabel>
#include <QLocale>
#include <qtablewidget.h>

BrowseTableWidget::BrowseTableWidget(QWidget *parent)
    : CustomTableWidget(parent) {}

BrowseTableWidget::~BrowseTableWidget() {}

void BrowseTableWidget::setHeaders(QList<BrowseTableWidget::Table> headers,
                                   QString viewName) {
  int columnCount = 0;

  setColumnCount(headers.length());
  verticalHeader()->setVisible(false);

  disableNesting();

  if (headers.contains(Table::BankColumn)) {
    setHorizontalHeaderItem(
        columnCount++, new QTableWidgetItem(FieldNames.at(Table::BankColumn)));
  }
  if (headers.contains(Table::CategoryColumn)) {
    setHorizontalHeaderItem(columnCount++, new QTableWidgetItem(FieldNames.at(
                                               Table::CategoryColumn)));
  }
  if (headers.contains(Table::BalanceColumn)) {
    setHorizontalHeaderItem(columnCount++, new QTableWidgetItem(FieldNames.at(
                                               Table::BalanceColumn)));
  }
  if (headers.contains(Table::AverageColumn)) {
    setHorizontalHeaderItem(columnCount++, new QTableWidgetItem(FieldNames.at(
                                               Table::AverageColumn)));
  }

  enableNesting();

  if (!viewName.isEmpty()) {
    tableName = viewName;
    restoreColumnsWidths(tableName);
  }
}

void BrowseTableWidget::addBank(int row, QString bank, double amount)
{
    QTableWidgetItem *bankItem = new QTableWidgetItem(bank);
    bankItem->setTextAlignment(Qt::AlignLeft);
    setItem(row, 0, bankItem);

    QTableWidgetItem *amountItem = new QTableWidgetItem(QString::number(amount));
    amountItem->setTextAlignment(Qt::AlignRight);
    setItem(row, 1, amountItem);
}

void BrowseTableWidget::addCategory(int row, QString category, double amount,
                                    int average) {
  QTableWidgetItem *item;

  item = new QTableWidgetItem(category);
  item->setTextAlignment(Qt::AlignLeft);
  setItem(row, 0, item);

  item = new QTableWidgetItem(QString::number(amount));
  item->setTextAlignment(Qt::AlignRight);
  setItem(row, 1, item);

  if (average) {
    item = new QTableWidgetItem(QString::number(amount / average));
    item->setTextAlignment(Qt::AlignRight);
    setItem(row, 2, item);
  }
}

void BrowseTableWidget::addTotal(double total) {
  QTableWidgetItem *item;
  int row = rowCount();

  setRowCount(row + 1);
  item = new QTableWidgetItem(tr("Total"));
  item->setTextAlignment(Qt::AlignLeft);
  setItem(row, 0, item);

  item = new QTableWidgetItem(QString::number(total));
  item->setTextAlignment(Qt::AlignRight);
  setItem(row, 1, item);
}