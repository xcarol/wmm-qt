#ifndef BROWSEBANKVIEW_H
#define BROWSEBANKVIEW_H

#include <QMainWindow>

namespace Ui {
class BrowseBankView;
}

class BrowseBankView : public QMainWindow {
  Q_OBJECT

  QString startDate;
  QString endDate;
  
public:
  explicit BrowseBankView(QWidget *parent = nullptr);
  ~BrowseBankView();

  void update();

private:
  Ui::BrowseBankView *ui;
};

#endif // BROWSEBANKVIEW_H
