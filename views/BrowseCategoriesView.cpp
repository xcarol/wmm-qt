#include "BrowseCategoriesView.h"
#include "../lib/Database.h"
#include "../widgets//MessageBox.h"
#include "ui_BrowseCategoriesView.h"

BrowseCategoriesView::BrowseCategoriesView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::BrowseCategoriesView) {
  ui->setupUi(this);
  fillYearsCombo();
}

BrowseCategoriesView::~BrowseCategoriesView() { delete ui; }

void BrowseCategoriesView::on_byyearButton_clicked() {
  int year = ui->yearBox->currentText().toInt();
  startDate = QString("%1-01-01").arg(year);
  endDate = QString("%1-12-31").arg(year);
  updateCategoryTable();
}

void BrowseCategoriesView::on_yearBox_currentIndexChanged(int index) {
  ui->byyearButton->setEnabled(index > 0);
}

void BrowseCategoriesView::fillYearsCombo() {
  Database database = Database();
  QStringList years = database.getYears(false);
  ui->yearBox->addItem("");
  ui->yearBox->addItems(years);
}

void BrowseCategoriesView::updateCategoryTable() {
  int noAmountCategoriesCount = 0;
  Database database = Database();

  QList<QStringList> balances = database.getCategoriesBalance(
      QStringList(), QDate::fromString(startDate, Qt::DateFormat::ISODate),
      QDate::fromString(endDate, Qt::DateFormat::ISODate));

  if (balances.isEmpty()) {
    MessageBox::DatabaseError(database.getLastErrorText());
    return;
  }

  ui->categoryTable->setRowCount(balances.length());
  ui->categoryTable->setHeaders(
      {
          BrowseTableWidget::Table::CategoryColumn,
          BrowseTableWidget::Table::BalanceColumn,
          BrowseTableWidget::Table::AverageColumn,
      },
      "BrowseCategoriesView");

  int averageMonths = 12;

  int rowcount = 0;
  for (; rowcount < balances.length(); rowcount++) {
    double amount = balances.at(rowcount).at(Position::amount).toDouble();

    if (amount == 0) {
      noAmountCategoriesCount++;
      continue;
    }

    ui->categoryTable->addCategory(rowcount - noAmountCategoriesCount,
                                   balances.at(rowcount).at(Position::category),
                                   amount, averageMonths);
  }

  ui->categoryTable->setRowCount(ui->categoryTable->rowCount() -
                                 noAmountCategoriesCount);
}
