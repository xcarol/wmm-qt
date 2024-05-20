#include "BrowseBankView.h"
#include "../lib/Database.h"
#include "../widgets/MessageBox.h"
#include "ui_BrowseBankView.h"

BrowseBankView::BrowseBankView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::BrowseBankView) {
  ui->setupUi(this);
  update();
}

BrowseBankView::~BrowseBankView() { delete ui; }

void BrowseBankView::update() {
  double totalAmount = 0.0;
  Database database = Database();

  QList<QStringList> balances = database.getBanksBalance();

  if (balances.isEmpty()) {
    MessageBox::DatabaseError(database.getLastErrorText());
    return;
  }

  ui->bankTable->setRowCount(balances.length());
  ui->bankTable->setHeaders({BrowseTableWidget::Table::BankColumn,
                             BrowseTableWidget::Table::DateColumn,
                             BrowseTableWidget::Table::BalanceColumn},
                            "BrowseBankView");

  int rowcount = 0;
  for (; rowcount < balances.length(); rowcount++) {
    QString bank = balances.at(rowcount).at(0);
    double amount = balances.at(rowcount).at(1).toDouble();
    QString date = balances.at(rowcount).at(2);

    totalAmount += amount;
    ui->bankTable->addBank(rowcount, bank, amount, date);
  }

  ui->amountLabel->setText(QString(tr("Total amount to the date: %1")).arg(totalAmount));
}

