#ifndef BROWSEDATAVIEW_H
#define BROWSEDATAVIEW_H

#include <QMainWindow>

namespace Ui {
class BrowseDataView;
}

class BrowseDataView : public QMainWindow {
  Q_OBJECT

public:
  explicit BrowseDataView(QWidget *parent = nullptr);
  ~BrowseDataView();

private:
  Ui::BrowseDataView *ui;

  void updateBankBalanceTable();
};

#endif // BROWSEDATAVIEW_H
