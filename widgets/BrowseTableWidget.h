#ifndef BROWSETABLEWIDGET_H
#define BROWSETABLEWIDGET_H

#include "CustomTableWidget.h"

class BrowseTableWidget : public CustomTableWidget {
  Q_OBJECT

public:
  enum Table {
    BankColumn = 0,
    CategoryColumn = 1,
    BalanceColumn = 2,
    AverageColumn = 3,
    DateColumn = 4,
  };

  QStringList FieldNames = {tr("Bank"), tr("Category"), tr("Balance"),
                            tr("Average"), tr("At date")};

  explicit BrowseTableWidget(QWidget *parent = nullptr);
  ~BrowseTableWidget();

  void sortItems(int column, Qt::SortOrder order = Qt::AscendingOrder);
  void setHeaders(QList<BrowseTableWidget::Table> headers,
                  QString viewName = QString(""));
  void addBank(int row, QString bank, double amount, QString date);
  void addCategory(int row, QString category, double amount, int average);
};

#endif // BROWSETABLEWIDGET_H
