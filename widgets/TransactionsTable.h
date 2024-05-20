#ifndef TRANSACTIONSTABLE_H
#define TRANSACTIONSTABLE_H

#include <QSettings>
#include "CustomTableWidget.h"

class TransactionsTable : public CustomTableWidget {
  Q_OBJECT

public:
  enum Database {
    IdField = 0,
    BankField = 1,
    DateField = 2,
    DescriptionField = 3,
    CategoryField = 4,
    AmountField = 5,
  };

  enum Table {
    BankColumn = 0,
    DateColumn = 1,
    DescriptionColumn = 2,
    CategoryColumn = 3,
    AmountColumn = 4,
  };

  QStringList FieldNames = {"Id",           tr("Bank"),
                            tr("Date"),     tr("Description"),
                            tr("Category"), tr("Amount")};

  explicit TransactionsTable(QWidget *parent = nullptr);
  ~TransactionsTable();

  void setHeaders(QList<TransactionsTable::Table> headers,
                  QString viewName = QString(""));
  void addTransaction(int row, QList<TransactionsTable::Database> fields,
                      QStringList values);

  QList<int> getAllTransactionIDs();
  QList<int> getSelectedTransactions();
  QList<int> getSelectedTransactionIDs();
  QStringList getSelectedTransactionDescriptions();

  void clearTransactions();
  void removeSelectedTransactions();
};

#endif // TRANSACTIONSTABLE_H
