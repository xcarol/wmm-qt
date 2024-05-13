#ifndef DUPLICATESVIEW_H
#define DUPLICATESVIEW_H

#include <QMainWindow>

namespace Ui {
class DuplicatesView;
}

class DuplicatesView : public QMainWindow {
  Q_OBJECT

public:
  explicit DuplicatesView(QWidget *parent = nullptr);
  ~DuplicatesView();

private slots:
  void on_deleteDuplicatesButton_clicked();
  void on_duplicateRowsTable_itemSelectionChanged();
  void on_markNotDuplicatesButton_clicked();
  void on_searchDuplicateButton_clicked();

private:
  Ui::DuplicatesView *ui;

  QString categoryName;
  QString appliedFilter;

  void deleteDuplicateRows(QList<int> ids);
  void markDuplicateRows(QList<int> ids);
  void updateDuplicatesTable();
};

#endif // DUPLICATESVIEW_H
