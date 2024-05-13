#ifndef CATEGORIZEVIEW_H
#define CATEGORIZEVIEW_H

#include <QMainWindow>

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
  void searchUncategorizedRows();
  void setFilter(QString filter);
  void addFiltersToDatabase(QString category, QStringList filters);
  void updateUncategorizedRows(QList<int> rowsIds);
  void updateUpdateButtonState();
  void updateView();
};

#endif // CATEGORIZEVIEW_H
