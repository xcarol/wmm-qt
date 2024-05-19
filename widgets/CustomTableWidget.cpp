#include "CustomTableWidget.h"
#include <QHeaderView>
#include <QTableWidget>

CustomTableWidget::CustomTableWidget(QWidget *parent) : QTableWidget{parent} {
  horizontalHeader()->setStretchLastSection(true);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setEditTriggers(QAbstractItemView::NoEditTriggers);

  connect(horizontalHeader(), &QHeaderView::sectionClicked, this,
          &CustomTableWidget::on_headerClicked);

  connect(horizontalHeader(), &QHeaderView::sectionResized, this,
          &CustomTableWidget::on_headerResized);
}

void CustomTableWidget::on_headerClicked(int index) { sortByColumn(index); }

void CustomTableWidget::on_headerResized() { saveColumnsWidths(); }

void CustomTableWidget::sortByColumn(int column) {
  Qt::SortOrder order = Qt::SortOrder::DescendingOrder;

  if (column == sortedColumn) {
    order = sortOrder == Qt::SortOrder::AscendingOrder
                ? Qt::SortOrder::DescendingOrder
                : Qt::SortOrder::AscendingOrder;
  }

  sortedColumn = column;
  sortOrder = order;

  sortItems(sortedColumn, order);
  sortHeaders(column, sortOrder);
}

void CustomTableWidget::sortHeaders(int sortedColumn, Qt::SortOrder order) {
  for (int column = 0; column < columnCount(); column++) {
    QString currentLabel = horizontalHeaderItem(column)->text();
    QString newLabel =
        currentLabel.remove(QRegularExpression(sortSymbolsRegexp));

    if (column == sortedColumn) {
      newLabel.append(sortOrder == Qt::SortOrder::AscendingOrder
                          ? sortSymbolUp
                          : sortSymbolDown);
    }

    horizontalHeaderItem(column)->setText(newLabel);
  }
}

void CustomTableWidget::saveColumnsWidths() {

  if (tableName.isEmpty() || isNesting()) {
    return;
  }

  disableNesting();

  for (int column = 0; column < columnCount(); column++) {
    settings.setValue(QString(settingTemplate).arg(tableName).arg(column),
                      columnWidth(column));
  }

  enableNesting();
}

void CustomTableWidget::restoreColumnsWidths(QString tableName) {
  if (isNesting()) {
    return;
  }

  disableNesting();
  for (int column = 0; column < columnCount(); column++) {
    int width =
        settings.value(QString(settingTemplate).arg(tableName).arg(column), "0")
            .toInt();
    if (width) {
      setColumnWidth(column, width);
    }
  }
  enableNesting();
}

void CustomTableWidget::enableNesting() { resizeNastyReentering = false; }
void CustomTableWidget::disableNesting() { resizeNastyReentering = true; }
bool CustomTableWidget::isNesting() { return resizeNastyReentering; }

