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
  void on_searchButton_clicked();

  void on_updateButton_clicked();

  void on_categoryComboBox_editTextChanged(const QString &arg1);

  void on_filterEdit_textChanged(const QString &arg1);

  private:
  Ui::CategorizeView *ui;

  QList<QStringList> uncategorizedRows;
  QString categoryName;
  QString appliedFilter;

  void updateUpdateButtonState();
};

#endif // CATEGORIZEVIEW_H
