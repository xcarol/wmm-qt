#ifndef CATEGORIZEVIEW_H
#define CATEGORIZEVIEW_H

#include <QMainWindow>

namespace Ui {
class CategorizeView;
}

class CategorizeView : public QMainWindow
{
    Q_OBJECT

public:
    explicit CategorizeView(QWidget *parent = nullptr);
    ~CategorizeView();

private slots:
    void on_searchButton_clicked();

private:
    Ui::CategorizeView *ui;
};

#endif // CATEGORIZEVIEW_H
