#ifndef DATABASESETTINGS_H
#define DATABASESETTINGS_H

#include <QDialog>

namespace Ui {
class DatabaseSettings;
}

class DatabaseSettings : public QDialog {
  Q_OBJECT

public:
  explicit DatabaseSettings(QWidget *parent = nullptr);
  ~DatabaseSettings();

  void init(QString hostname, int port, QString database, QString username,
            QString userpass);

public:
  QString hostname;
  int port;
  QString database;
  QString username;
  QString userpass;

private slots:
  void on_buttonBox_accepted();

  void on_databaseStatusButton_clicked();

private:
  Ui::DatabaseSettings *ui;
  void updateAttributes();
  bool checkDatabaseConnection();
};

#endif // DATABASESETTINGS_H
