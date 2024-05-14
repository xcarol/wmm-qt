#ifndef TRANSACTIONSTABLE_H
#define TRANSACTIONSTABLE_H

#include <QSettings>
#include <QTableWidget>
#include <QWidget>

class TransactionsTable : public QTableWidget {
  Q_OBJECT

private:
  QString sortSymbolsRegexp = " (▲|▼)$";
  QString sortSymbolDown = " ▼";
  QString sortSymbolUp = " ▲";
  
  bool resizeNastyReentering = false;
  int sortedColumn = -1;
  Qt::SortOrder sortOrder = Qt::SortOrder::AscendingOrder;
  QString settingTemplate = QString("%1_column_%2");
  QString tableName = QString("");
  QSettings settings = QSettings("com.xicra", "wmm");
  void sortByColumn(int column);
  void sortHeaders(int column, Qt::SortOrder order);
  void restoreColumnsWidths(QString tableName);
  void saveColumnsWidths();

public:
  enum Database {
    IdField = 0,
    BankField = 1,
    DateField = 2,
    DescriptionField = 3,
    CategoryField = 4,
    AmountField = 5,
    TotalFields = 6,
  };

  enum Table {
    BankColumn = 0,
    DateColumn = 1,
    DescriptionColumn = 2,
    CategoryColumn = 3,
    AmountColumn = 4,
    TotalColumns = 5,
  };

  QStringList FieldNames = {"Id",           tr("Bank"),
                            tr("Date"),     tr("Description"),
                            tr("Category"), tr("Amount")};

  explicit TransactionsTable(QWidget *parent = nullptr);
  ~TransactionsTable();

  void on_headerClicked(int index);
  void on_headerResized();

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
