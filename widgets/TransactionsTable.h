#ifndef TRANSACTIONSTABLE_H
#define TRANSACTIONSTABLE_H

#include <QWidget>
#include <QTableWidget>

#define DATABASE_TABLE_ID_FIELD 0
#define DATABASE_TABLE_BANK_FIELD 1
#define DATABASE_TABLE_DATE_FIELD 2
#define DATABASE_TABLE_DESCRIPTION_FIELD 3
#define DATABASE_TABLE_CATEGORY_FIELD 4
#define DATABASE_TABLE_AMOUNT_FIELD 5

#define SEARCH_TABLE_BANK_COLUMN 0
#define SEARCH_TABLE_DATE_COLUMN 1
#define SEARCH_TABLE_DESCRIPTION_COLUMN 2
#define SEARCH_TABLE_CATEGORY_COLUMN 3
#define SEARCH_TABLE_AMOUNT_COLUMN 4
#define SEARCH_TABLE_COLUMN_COUNT 5

class TransactionsTable : public QTableWidget
{
    Q_OBJECT

public:
    explicit TransactionsTable(QWidget *parent = nullptr);
    ~TransactionsTable();

    void setSize();
    void setHeaders(QStringList headers);
    void addTransaction(int row, QStringList fields);

    QList<int> getAllTransactionIDs();
    QList<int> getSelectedTransactions();
    QList<int> getSelectedTransactionIDs();
    QStringList getSelectedTransactionDescriptions();
    
    void clearTransactions();
    void removeSelectedTransactions();
};

#endif // TRANSACTIONSTABLE_H
