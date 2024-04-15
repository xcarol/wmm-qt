#include "BrowseDataView.h"
#include "../lib/Database.h"
#include "ui_BrowseDataView.h"
#include <QMessageBox>

BrowseDataView::BrowseDataView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::BrowseDataView) {
  ui->setupUi(this);
  updateBankBalanceTable();
}

BrowseDataView::~BrowseDataView() { delete ui; }

void BrowseDataView::updateBankBalanceTable() {
  Database database = Database();

  QList<QStringList> balances = database.getBanksBalance();

  if (balances.isEmpty()) {
    QMessageBox(QMessageBox::Icon::Critical, QString("Database error"),
                QString(database.getLastErrorText()))
        .exec();
    return;
  }

  ui->balanceTable->setColumnCount(2);
  ui->balanceTable->setRowCount(balances.length());
  ui->balanceTable->setHorizontalHeaderLabels({"Bank", "Balance"});

  for (int rowcount; rowcount < balances.length(); rowcount++) {
    QLabel *labelBank = new QLabel(balances.at(rowcount).at(0));
    labelBank->setAlignment(Qt::AlignLeft);
    ui->balanceTable->setCellWidget(rowcount, 0, labelBank);

    QLabel *labelAmount = new QLabel(QString::number(balances.at(rowcount).at(1).toDouble()));
    labelAmount->setAlignment(Qt::AlignRight);
    ui->balanceTable->setCellWidget(rowcount, 1, labelAmount);
  }
}
