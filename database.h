#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSettings>
#include <QSqlDatabase>

#define HOSTNAME "hostname"
#define DATABASE "database"
#define USERNAME "username"
#define USERPASS "userpass"

class Database : public QObject {
  Q_OBJECT

private:
  QString hostname;
  QString database;
  QString username;
  QString userpass;

  QSqlDatabase sqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
  QSettings settings = QSettings("com.xicra", "wmm");

public:
  explicit Database(QObject *parent = nullptr);
    ~Database();

  QString getHostname() { return hostname; }
  QString getDatabase() { return database; }
  QString getUsername() { return username; }
  QString getUserpass() { return userpass; }

  void setHostname(QString name);
  void setDatabase(QString name);
  void setUsername(QString name);
  void setUserpass(QString pass);

  bool open();
  void close();

  QString getLastErrorText();
};

#endif // DATABASE_H
