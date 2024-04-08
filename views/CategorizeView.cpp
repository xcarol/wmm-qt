#include "CategorizeView.h"
#include "../helpers/Database.h"
#include "ui_CategorizeView.h"
#include <QLabel>
#include <QList>
#include <QMessageBox>

CategorizeView::CategorizeView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CategorizeView) {
  ui->setupUi(this);
  ui->searchResultsTable->horizontalHeader()->setStretchLastSection(true);
}

CategorizeView::~CategorizeView() { delete ui; }

void CategorizeView::on_searchButton_clicked() {
  Database database = Database();

  QStringList labels = database.getColumnNames();
  QList<QStringList> uncategorizedRow = database.getUncategorizedRows(ui->filterEdit->text());

  if (labels.length() == 0 || uncategorizedRow.length() == 0) {
    QString error = database.getLastErrorText();
    if (!error.isEmpty()) {
      QMessageBox(QMessageBox::Icon::Critical, QString("Database error"),
                  QString(database.getLastErrorText()))
          .exec();
    }
    return;
  }

  int rowsCount = uncategorizedRow.count();
  int columnsCount = uncategorizedRow.at(0).count();

  ui->searchResultsTable->clear();
  ui->searchResultsTable->setRowCount(rowsCount);
  ui->searchResultsTable->setColumnCount(columnsCount);
  ui->searchResultsTable->verticalHeader()->setVisible(false);

  for (int n = 0; n < labels.length(); n++) {
    ui->searchResultsTable->setHorizontalHeaderItem(
        n, new QTableWidgetItem(QString(labels.at(n).toUpper())));
  }

  for (int column = 0; column < columnsCount; column++) {
    for (int row = 0; row < rowsCount; row++) {
      ui->searchResultsTable->setCellWidget(
          row, column, new QLabel(uncategorizedRow.at(row).at(column)));
    }
  }
}
