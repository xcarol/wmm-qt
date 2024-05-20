#ifndef BROWSECATEGORIESVIEW_H
#define BROWSECATEGORIESVIEW_H

#include <QMainWindow>
#include <qcontainerfwd.h>
#include <qlist.h>

namespace Ui {
class BrowseCategoriesView;
}

class BrowseCategoriesView : public QMainWindow {
  Q_OBJECT

  QString startDate;
  QString endDate;
  int averageMonths = 12;

public:
  explicit BrowseCategoriesView(QWidget *parent = nullptr);
  ~BrowseCategoriesView();

private slots:
  void on_byyearButton_clicked();

  void on_yearBox_currentIndexChanged(int index);

private:
  Ui::BrowseCategoriesView *ui;

  void updateCategoryTable();
  void fillYearsCombo();
};

#endif // BROWSECATEGORIESVIEW_H
