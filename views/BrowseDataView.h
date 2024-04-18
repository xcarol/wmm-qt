#ifndef BROWSEDATAVIEW_H
#define BROWSEDATAVIEW_H

#include <QMainWindow>

namespace Ui {
class BrowseDataView;
}

class BrowseDataView : public QMainWindow {
  Q_OBJECT

  QString startDate;
  QString endDate;
  
public:
  explicit BrowseDataView(QWidget *parent = nullptr);
  ~BrowseDataView();

private slots:
  void on_byyearButton_clicked();

  void on_bydateButton_clicked();

private:
  Ui::BrowseDataView *ui;

  void updateBankTable();
  void updateCategoryTable();
  void fillYearsCombo();
};

#endif // BROWSEDATAVIEW_H
