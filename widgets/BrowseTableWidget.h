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
    TotalColumns = 4,
  };

  QStringList FieldNames = {tr("Bank"), tr("Category"), tr("Balance"),
                            tr("Average")};

  explicit BrowseTableWidget(QWidget *parent = nullptr);
  ~BrowseTableWidget();

  void setHeaders(QList<BrowseTableWidget::Table> headers,
                  QString viewName = QString(""));
  void addBank(int row, QString bank, double amount);
  void addCategory(int row, QString category, double amount, int average);
  void addTotal(double total);
};

#endif // BROWSETABLEWIDGET_H
