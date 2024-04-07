#ifndef DATABASESETTINGSDIALOG_H
#define DATABASESETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class DatabaseSettingsDialog;
}

class DatabaseSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseSettingsDialog(QWidget *parent = nullptr);
    ~DatabaseSettingsDialog();

    void init(QString hostname, QString database, QString username, QString userpass);

public:
    QString hostname;
    QString database;
    QString username;
    QString userpass;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DatabaseSettingsDialog *ui;
};

#endif // DATABASESETTINGSDIALOG_H
