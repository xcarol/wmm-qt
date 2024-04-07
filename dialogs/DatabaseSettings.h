#ifndef DATABASESETTINGS_H
#define DATABASESETTINGS_H

#include <QDialog>

namespace Ui {
class DatabaseSettings;
}

class DatabaseSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DatabaseSettings(QWidget *parent = nullptr);
    ~DatabaseSettings();

    void init(QString hostname, QString database, QString username, QString userpass);

public:
    QString hostname;
    QString database;
    QString username;
    QString userpass;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DatabaseSettings *ui;
};

#endif // DATABASESETTINGS_H
