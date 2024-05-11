#ifndef CATEGORIZEVIEW_H
#define CATEGORIZEVIEW_H

#include <QMainWindow>
#include <QTableWidget>
#include <qcontainerfwd.h>

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

namespace Ui {
class CategorizeView;
}

class CategorizeView : public QMainWindow {
  Q_OBJECT

public:
  explicit CategorizeView(QWidget *parent = nullptr);
  ~CategorizeView();

private slots:
  void on_categoryComboBox_editTextChanged(const QString &arg1);
  void on_filterEdit_currentIndexChanged(int index);
  void on_filterEdit_editTextChanged(const QString &arg1);
  void on_filterEdit_returnPressed();
  void on_searchButton_clicked();
  void on_searchResultsTable_itemSelectionChanged();
  void on_updateButton_clicked();

private:
  Ui::CategorizeView *ui;

  QString categoryName;
  QString appliedFilter;

  void addHeadersToSearchResultsTable(QStringList headers);
  void addRowToSearchResultsTable(int row, QStringList rowFields);
  QList<int> getAllRowsHeaders(QTableWidget *tableWidget);
  QList<int> getSelectedRows(QTableWidget *tableWidget);
  QList<int> getSelectedRowsHeaders(QTableWidget *tableWidget);
  void searchUncategorizedRows();
  void setFilter(QString filter);
  void addFiltersToDatabase(QString category, QStringList filters);
  void updateUncategorizedRows(QList<int> rowsIds);
  void updateUpdateButtonState();
  void updateView();
};

#endif // CATEGORIZEVIEW_H
