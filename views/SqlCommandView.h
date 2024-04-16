#ifndef SQLCOMMANDVIEW_H
#define SQLCOMMANDVIEW_H

#include <QMainWindow>
#include <QProgressDialog>
#include <QSqlQuery>
#include <qcontainerfwd.h>
#include <qlist.h>
#include <qsqlrecord.h>

namespace Ui {
class SqlCommandView;
}

class SqlCommandView : public QMainWindow
{
    Q_OBJECT

public:
    explicit SqlCommandView(QWidget *parent = nullptr);
    ~SqlCommandView();

private slots:
    void on_execButton_clicked();

private:
    Ui::SqlCommandView *ui;

    void fillResultTable(QList<QSqlRecord> records, QProgressDialog* dialog);
};

#endif // SQLCOMMANDVIEW_H
