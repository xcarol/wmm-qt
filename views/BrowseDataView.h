#ifndef BROWSEDATAVIEW_H
#define BROWSEDATAVIEW_H

#include "BrowseBankView.h"
#include "BrowseCategoriesView.h"
#include <QMainWindow>

namespace Ui {
class BrowseDataView;
}

class BrowseDataView : public QMainWindow {
  Q_OBJECT

public:
  explicit BrowseDataView(QWidget *parent = nullptr);
  ~BrowseDataView();

private slots:
  void on_tabWidget_currentChanged(int index);

private:
  Ui::BrowseDataView *ui;

  QString banksLabel = tr("Banks");
  QString categoriesLabel = tr("Categories");

  BrowseBankView bankView = BrowseBankView();
  BrowseCategoriesView categoriesView = BrowseCategoriesView();
};

#endif // BROWSEDATAVIEW_H
