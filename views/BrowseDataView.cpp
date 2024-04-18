#include "BrowseDataView.h"
#include "../lib/Database.h"
#include "ui_BrowseDataView.h"
#include <QMessageBox>

BrowseDataView::BrowseDataView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::BrowseDataView) {
  ui->setupUi(this);

  ui->bankTable->horizontalHeader()->setStretchLastSection(true);
  ui->categoryTable->horizontalHeader()->setStretchLastSection(true);

  fillYearsCombo();
}

BrowseDataView::~BrowseDataView() { delete ui; }

void BrowseDataView::updateBankTable() {
  double totalAmount = 0.0;
  QLocale locale = QLocale();
  Database database = Database();

  QList<QStringList> balances = database.getBanksBalance(
      QStringList(), QDate::fromString(startDate, Qt::DateFormat::ISODate),
      QDate::fromString(endDate, Qt::DateFormat::ISODate));

  if (balances.isEmpty()) {
    QMessageBox(QMessageBox::Icon::Critical, QString("Database error"),
                QString(database.getLastErrorText()))
        .exec();
    return;
  }

  ui->bankTable->setColumnCount(2);
  ui->bankTable->setRowCount(balances.length());
  ui->bankTable->setHorizontalHeaderLabels({"Bank", "Balance"});
  ui->bankTable->verticalHeader()->setVisible(false);

  int rowcount = 0;
  for (; rowcount < balances.length(); rowcount++) {
    QLabel *labelBank = new QLabel(balances.at(rowcount).at(0));
    labelBank->setAlignment(Qt::AlignLeft);
    ui->bankTable->setCellWidget(rowcount, 0, labelBank);

    double amount = balances.at(rowcount).at(1).toDouble();
    totalAmount += amount;

    QLabel *labelAmount = new QLabel(locale.toCurrencyString(amount));
    labelAmount->setAlignment(Qt::AlignRight);
    ui->bankTable->setCellWidget(rowcount, 1, labelAmount);
  }

  ui->bankTable->setRowCount(ui->bankTable->rowCount() + 1);
  QLabel *labelTotal = new QLabel("Total");
  labelTotal->setAlignment(Qt::AlignLeft);
  ui->bankTable->setCellWidget(rowcount, 0, labelTotal);

  QLabel *labelTotalAmount = new QLabel(locale.toCurrencyString(totalAmount));
  labelTotalAmount->setAlignment(Qt::AlignRight);
  ui->bankTable->setCellWidget(rowcount, 1, labelTotalAmount);
}

void BrowseDataView::updateCategoryTable() {
  double totalAmount = 0.0;
  QLocale locale = QLocale();
  Database database = Database();

  QList<QStringList> balances = database.getCategoriesBalance(
      QStringList(), QDate::fromString(startDate, Qt::DateFormat::ISODate),
      QDate::fromString(endDate, Qt::DateFormat::ISODate));

  if (balances.isEmpty()) {
    QMessageBox(QMessageBox::Icon::Critical, QString("Database error"),
                QString(database.getLastErrorText()))
        .exec();
    return;
  }

  ui->categoryTable->setColumnCount(2 + addMonthAverage);
  ui->categoryTable->setRowCount(balances.length());
  if (addMonthAverage) {
    ui->categoryTable->setHorizontalHeaderLabels(
        {"Category", "Balance", "Month average"});
  } else {
    ui->categoryTable->setHorizontalHeaderLabels({"Category", "Balance"});
  }
  ui->categoryTable->verticalHeader()->setVisible(false);

  int rowcount = 0;
  for (; rowcount < balances.length(); rowcount++) {
    QLabel *labelCategory = new QLabel(balances.at(rowcount).at(0));
    labelCategory->setAlignment(Qt::AlignLeft);
    ui->categoryTable->setCellWidget(rowcount, 0, labelCategory);

    double amount = balances.at(rowcount).at(1).toDouble();
    totalAmount += amount;

    QLabel *labelAmount =
        new QLabel(locale.toCurrencyString(amount, QString(), 2));
    labelAmount->setAlignment(Qt::AlignRight);
    ui->categoryTable->setCellWidget(rowcount, 1, labelAmount);

    if (addMonthAverage) {
      QLabel *averageAmount =
          new QLabel(locale.toCurrencyString(amount / 12, QString(), 2));
      averageAmount->setAlignment(Qt::AlignRight);
      ui->categoryTable->setCellWidget(rowcount, 2, averageAmount);
    }
  }

  ui->categoryTable->setRowCount(ui->categoryTable->rowCount() + 1);
  QLabel *labelTotal = new QLabel("Total");
  labelTotal->setAlignment(Qt::AlignLeft);
  ui->categoryTable->setCellWidget(rowcount, 0, labelTotal);

  QLabel *labelTotalAmount = new QLabel(locale.toCurrencyString(totalAmount));
  labelTotalAmount->setAlignment(Qt::AlignRight);
  ui->categoryTable->setCellWidget(rowcount, 1, labelTotalAmount);
}

void BrowseDataView::fillYearsCombo() {
  Database database = Database();
  QStringList years = database.getYears();
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
