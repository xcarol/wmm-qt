#include "CategorizeView.h"
#include "../lib/Database.h"
#include "ui_CategorizeView.h"
#include <QDate>
#include <QLabel>
#include <QList>
#include <QMessageBox>

CategorizeView::CategorizeView(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CategorizeView) {
  ui->setupUi(this);
  ui->updateButton->setEnabled(false);
  ui->searchResultsTable->horizontalHeader()->setStretchLastSection(true);

  QSettings settings = QSettings("com.xicra", "wmm");
  QStringList filters = settings.value("filters").toStringList();
  ui->filterEdit->addItems(filters);
}

CategorizeView::~CategorizeView() { delete ui; }

void CategorizeView::on_searchButton_clicked() {
  Database database = Database();

  ui->categoryComboBox->clear();
  ui->categoryComboBox->addItem("");
  ui->categoryComboBox->addItems(database.getCategoryNames());

  ui->searchResultsTable->clear();
  ui->searchResultsTable->setRowCount(0);
  ui->searchResultsTable->setColumnCount(0);

  setFilter(appliedFilter);

  QStringList labels = database.getColumnNames();

  QProgressDialog progress = QProgressDialog(
      QString(tr("Searching for rows with filter %1")).arg(appliedFilter), tr("Cancel"),
      0, 0);
  progress.setWindowModality(Qt::WindowModal);
  progress.setWindowTitle(tr("Search..."));

  uncategorizedRows = database.getUncategorizedRows(appliedFilter, &progress);

  if (labels.length() == 0 || uncategorizedRows.length() == 0) {
    progress.cancel();
    QString error = database.getLastErrorText();
    if (!error.isEmpty()) {
      QMessageBox(QMessageBox::Icon::Critical, QString(tr("Database error")),
                  QString(database.getLastErrorText()))
          .exec();
    }
    return;
  }

  int numberOfRows = uncategorizedRows.count();
  int numberOfColumns = uncategorizedRows.at(0).count();

  ui->searchResultsTable->setRowCount(numberOfRows);
  ui->searchResultsTable->setColumnCount(numberOfColumns);
  ui->searchResultsTable->verticalHeader()->setVisible(false);

  for (int n = 0; n < labels.length(); n++) {
    ui->searchResultsTable->setHorizontalHeaderItem(
        n, new QTableWidgetItem(QString(labels.at(n).toUpper())));
  }

  QString dateFormat = QLocale().dateFormat(QLocale::ShortFormat);

  progress.reset();
  progress.setMaximum(numberOfRows);

  for (int rowCount = 0; rowCount < numberOfRows; rowCount++) {

    progress.setLabelText(QString(tr("Processing %1 of %2 rows found."))
                              .arg(rowCount)
                              .arg(numberOfRows));

    for (int columnCount = 0; columnCount < numberOfColumns; columnCount++) {
      QStringList row = uncategorizedRows.at(rowCount);
      QString value = row.at(columnCount);

      QLabel *label;
      switch (columnCount) {
      case DATE_COLUMN:
        label = new QLabel(QDate::fromString(value, Qt::DateFormat::ISODate)
                               .toString(dateFormat));
        label->setAlignment(Qt::AlignCenter);
        break;

      case AMOUNT_COLUMN:
        label = new QLabel(QString::number(value.toDouble()));
        label->setAlignment(Qt::AlignRight);
        break;

      case ACCOUNTABLE_COLUMN:
        label = new QLabel(value);
        label->setAlignment(Qt::AlignCenter);
        break;

      default:
        label = new QLabel(value);
      }
      ui->searchResultsTable->setCellWidget(rowCount, columnCount, label);
    }

    progress.setValue(rowCount);

    if (progress.wasCanceled()) {
      break;
    }
  }

  updateUpdateButtonState();
}

void CategorizeView::on_updateButton_clicked() {
  QMessageBox::StandardButton res = QMessageBox::question(
      QApplication::topLevelWidgets().first(), QString(tr("Update")),
      QString(tr("You're about to update %1 records "
              "with the category: %2\nAre you sure?"))
          .arg(uncategorizedRows.length())
          .arg(categoryName));
  if (res == QMessageBox::StandardButton::No) {
    return;
  }

  Database database = Database();

  ulong updatedRows = database.updateRowsCategory(appliedFilter, categoryName);

  if (updatedRows != uncategorizedRows.length()) {
    QMessageBox(QMessageBox::Icon::Critical, QString(tr("Database error")),
                QString(database.getLastErrorText()))
        .exec();
  } else {
    QMessageBox(QMessageBox::Icon::Information, QString(tr("Database success")),
                QString(tr("A total of %1 rows updated")).arg(updatedRows))
        .exec();
  }

  ui->filterEdit->clearEditText();
  ui->categoryComboBox->clearEditText();
  ui->searchResultsTable->clear();
  ui->searchResultsTable->setColumnCount(0);
  uncategorizedRows.clear();
}

void CategorizeView::on_categoryComboBox_editTextChanged(const QString &arg1) {
  categoryName = arg1;
  updateUpdateButtonState();
}

void CategorizeView::on_filterEdit_currentIndexChanged(int index) {
  appliedFilter = ui->filterEdit->itemText(index);
}

void CategorizeView::on_filterEdit_editTextChanged(const QString &arg1) {
  appliedFilter = arg1;
}

void CategorizeView::updateUpdateButtonState() {
  ui->updateButton->setEnabled(uncategorizedRows.length() > 0 &&
                               categoryName.length() > 0);
}

void CategorizeView::setFilter(QString filter) {
  QSettings settings = QSettings("com.xicra", "wmm");
  QStringList filters = settings.value("filters").toStringList();
  if (filters.contains(appliedFilter) == false) {
    filters.append(appliedFilter);
    settings.setValue("filters", filters);
    ui->filterEdit->addItem(appliedFilter);
  }
}
