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

private:
    Ui::CategorizeView *ui;
};

#endif // CATEGORIZEVIEW_H
