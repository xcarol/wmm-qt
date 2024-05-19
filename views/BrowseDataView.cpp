#include "BrowseDataView.h"
#include "../lib/Database.h"
#include "../widgets/MessageBox.h"
#include "ui_BrowseDataView.h"

BrowseDataView::BrowseDataView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::BrowseDataView) {
  ui->setupUi(this);

  fillYearsCombo();
}

BrowseDataView::~BrowseDataView() { delete ui; }

void BrowseDataView::updateBankTable() {
  double totalAmount = 0.0;
  Database database = Database();

  QList<QStringList> balances = database.getBanksBalance(
      QStringList(), QDate::fromString(startDate, Qt::DateFormat::ISODate),
      QDate::fromString(endDate, Qt::DateFormat::ISODate));

  if (balances.isEmpty()) {
    MessageBox::DatabaseError(database.getLastErrorText());
    return;
  }

  ui->bankTable->setRowCount(balances.length());
  ui->bankTable->setHeaders({
    BrowseTableWidget::Table::BankColumn,
    BrowseTableWidget::Table::BalanceColumn
    },
    "");

  int rowcount = 0;
  for (; rowcount < balances.length(); rowcount++) {
    double amount = balances.at(rowcount).at(1).toDouble();
    totalAmount += amount;
    ui->bankTable->addBank(rowcount, balances.at(rowcount).at(0), amount);
  }

  ui->bankTable->addTotal(totalAmount);
}

void BrowseDataView::updateCategoryTable() {
  int noAmountCategoriesCount = 0;
  double totalAmount = 0.0;
  Database database = Database();

  QList<QStringList> balances = database.getCategoriesBalance(
      QStringList(), QDate::fromString(startDate, Qt::DateFormat::ISODate),
      QDate::fromString(endDate, Qt::DateFormat::ISODate));

  if (balances.isEmpty()) {
    MessageBox::DatabaseError(database.getLastErrorText());
    return;
  }

  ui->categoryTable->setRowCount(balances.length());
  if (addMonthAverage) {
    ui->categoryTable->setHeaders(
        {
            BrowseTableWidget::Table::CategoryColumn,
            BrowseTableWidget::Table::BalanceColumn,
            BrowseTableWidget::Table::AverageColumn,
        },
        "BrowseDataView");
  } else {
    ui->categoryTable->setHeaders(
        {
            BrowseTableWidget::Table::CategoryColumn,
            BrowseTableWidget::Table::BalanceColumn,
        },
        "BrowseDataView");
  }

  int rowcount = 0;
  for (; rowcount < balances.length(); rowcount++) {
    double amount = balances.at(rowcount).at(1).toDouble();

    if (amount == 0) {
      noAmountCategoriesCount++;
      continue;
    }

    totalAmount += amount;
    ui->categoryTable->addCategory(rowcount - noAmountCategoriesCount,
                                   balances.at(rowcount).at(0), amount,
                                   addMonthAverage ? 12 : 0);
  }

  ui->categoryTable->setRowCount(ui->categoryTable->rowCount() -
                                 noAmountCategoriesCount);

  ui->categoryTable->addTotal(totalAmount);
}

void BrowseDataView::fillYearsCombo() {
  Database database = Database();
  QStringList years = database.getYears(false);
  ui->yearBox->addItem("");
  ui->yearBox->addItems(years);
}

void BrowseDataView::on_byyearButton_clicked() {
  int year = ui->yearBox->currentText().toInt();
  startDate = QString("%1-01-01").arg(year);
  endDate = QString("%1-12-31").arg(year);
  addMonthAverage = true;
  updateBankTable();
  updateCategoryTable();
}

void BrowseDataView::on_bydateButton_clicked() {
  startDate = ui->startDateEdit->date().toString(Qt::DateFormat::ISODate);
  endDate = ui->endDateEdit->date().toString(Qt::DateFormat::ISODate);
  addMonthAverage = false;
  updateBankTable();
  updateCategoryTable();
}
