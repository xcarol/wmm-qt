#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QProgressDialog>
#include <QSettings>
#include <QSqlDatabase>

#define HOSTNAME "hostname"
#define DATABASE "database"
#define USERNAME "username"
#define USERPASS "userpass"

class Database : public QObject {
  Q_OBJECT

private:
  QString lastError;
  QString hostname;
  QString database;
  QString username;
  QString userpass;

  QSqlDatabase sqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
  QSettings settings = QSettings("com.xicra", "wmm");

  bool openDatabase();
  void closeDatabase();

public:
  explicit Database(QObject *parent = nullptr);
  ~Database();

  QString getLastErrorText() { return lastError; };

  QString getHostname() { return hostname; }
  QString getDatabase() { return database; }
  QString getUsername() { return username; }
  QString getUserpass() { return userpass; }

  void setHostname(QString name);
  void setDatabase(QString name);
  void setUsername(QString name);
  void setUserpass(QString pass);

  bool storeRow(QString bank, QString date, QString description, double amount);
  ulong updateRowsCategory(QString, QString);
  QStringList getBankNames();
  QStringList getCategoryNames();
  QList<QStringList> getUncategorizedRows(QString filter = QString());
  QStringList getColumnNames();
};

#endif // DATABASE_H
