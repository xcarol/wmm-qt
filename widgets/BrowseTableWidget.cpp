#include "BrowseTableWidget.h"
#include "CustomTableWidget.h"
#include <QHeaderView>
#include <QLabel>
#include <QLocale>
#include <qforeach.h>

BrowseTableWidget::BrowseTableWidget(QWidget *parent)
    : CustomTableWidget(parent) {}

BrowseTableWidget::~BrowseTableWidget() {}

void BrowseTableWidget::setHeaders(QList<BrowseTableWidget::Table> headers,
                                   QString viewName) {
  int columnCount = 0;

  setColumnCount(headers.length());
  verticalHeader()->setVisible(false);

  disableNesting();

  foreach (int header, headers) {
    setHorizontalHeaderItem(columnCount++,
                            new QTableWidgetItem(FieldNames.at(header)));
  }

  enableNesting();

  if (!viewName.isEmpty()) {
    tableName = viewName;
    restoreColumnsWidths(tableName);
  }
}

void BrowseTableWidget::addBank(int row, QString bank, double amount,
                                QString date) {
  QTableWidgetItem *bankItem = new QTableWidgetItem(bank);
  bankItem->setTextAlignment(Qt::AlignLeft);
  setItem(row, 0, bankItem);

  QTableWidgetItem *dateItem =
      new QTableWidgetItem(date.remove(trimTimeFromDate));
  dateItem->setTextAlignment(Qt::AlignRight);
  setItem(row, 1, dateItem);

  QTableWidgetItem *amountItem = new QTableWidgetItem(QString::number(amount));
  amountItem->setTextAlignment(Qt::AlignRight);
  setItem(row, 2, amountItem);
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
