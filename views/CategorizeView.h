#ifndef CATEGORIZEVIEW_H
#define CATEGORIZEVIEW_H

#include <QMainWindow>
#include <QTableWidget>

#define IDX_COLUMN 0
#define BANK_COLUMN 1
#define DATE_COLUMN 2
#define DESCRIPTION_COLUMN 3
#define CATEGORY_COLUMN 4
#define AMOUNT_COLUMN 5

namespace Ui {
class CategorizeView;
}

class CategorizeView : public QMainWindow {
  Q_OBJECT

public:
  explicit CategorizeView(QWidget *parent = nullptr);
  ~CategorizeView();

private slots:
  void addCellToSearchResultsTable(int numberOfColumns, int rowCount, QStringList row);

  void on_searchButton_clicked();

  void on_updateButton_clicked();

  void on_categoryComboBox_editTextChanged(const QString &arg1);

  void on_filterEdit_currentIndexChanged(int index);

  void on_filterEdit_editTextChanged(const QString &arg1);

  void on_searchDuplicateButton_clicked();

  void on_deleteDuplicatesButton_clicked();

  void on_duplicateRowsTable_itemSelectionChanged();

  void on_markNotDuplicatesButton_clicked();

  private:
  Ui::CategorizeView *ui;

  QList<QStringList> uncategorizedRows;
  QString categoryName;
  QString appliedFilter;

  void updateUpdateButtonState();
  void setFilter(QString filter);
  QList<int> getSelectedRowsHeaders();
};

#endif // CATEGORIZEVIEW_H
