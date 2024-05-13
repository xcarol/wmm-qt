#ifndef TRANSACTIONSTABLE_H
#define TRANSACTIONSTABLE_H

#include <QWidget>
#include <QTableWidget>

class TransactionsTable : public QTableWidget
{
    Q_OBJECT

private:

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

public:

    explicit TransactionsTable(QWidget *parent = nullptr);
    ~TransactionsTable();

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
