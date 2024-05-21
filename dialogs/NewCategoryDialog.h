#ifndef NEWCATEGORYDIALOG_H
#define NEWCATEGORYDIALOG_H

#include <QDialog>

namespace Ui {
class NewCategoryDialog;
}

class NewCategoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewCategoryDialog(QString category = "", QString filter = "", QWidget *parent = nullptr);
    ~NewCategoryDialog();

    QString category;
    QString filter;
    
private slots:
    void on_buttonBox_accepted();

    void on_categoryName_textChanged(const QString &arg1);

    void on_filterName_textChanged(const QString &arg1);

private:
    Ui::NewCategoryDialog *ui;
};

#endif // NEWCATEGORYDIALOG_H
