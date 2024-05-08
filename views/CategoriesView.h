#ifndef CATEGORIESVIEW_H
#define CATEGORIESVIEW_H

#include <QMainWindow>

namespace Ui {
class CategoriesView;
}

class CategoriesView : public QMainWindow
{
    Q_OBJECT

public:
    explicit CategoriesView(QWidget *parent = nullptr);
    ~CategoriesView();

private:
    Ui::CategoriesView *ui;
};

#endif // CATEGORIESVIEW_H
