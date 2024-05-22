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
  void on_addButton_clicked();
  void on_updateButton_clicked();

  private:
  Ui::CategorizeView *ui;

  bool filterExists(QString filter);
  void searchUncategorizedRows();
  void setFilter(QString filter);
  bool saveFilter(QString category, QString filter);
  void updateSelectedRows();
  void updateUncategorizedRows(QList<int> rowsIds);
  void updateButtonsState();
  void updateView();
};

#endif // CATEGORIZEVIEW_H
