#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QProgressDialog>
#include <QSettings>
#include <QSqlDatabase>
#include <QDateTime>

#define HOSTNAME "hostname"
#define DATABASE "database"
#define USERNAME "username"
#define USERPASS "userpass"

// These sizes are defined in the file: ../database/database.sql
#define BANK_LENGTH         200
#define DESCRIPTION_LENGTH  200
#define CATEGORY_LENGHT     200

class Database : public QObject {
  Q_OBJECT

private:
  QString lastError;
  QString hostname;
  QString database;
  QString username;
  QString userpass;

  QSqlDatabase sqlDatabase;
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

  bool checkConnection();
  bool storeRow(QString bank, QString date, QString description, double amount);
  QString unifyDateToStore(QString);
  ulong updateRowsCategory(QString, QString);
  QStringList getBankNames();
  QStringList getCategoryNames();
  QList<QStringList> getUncategorizedRows(QString filter = QString(), QProgressDialog *dialog = NULL);
  QStringList getColumnNames();
  QList<QStringList> getBanksBalance(QStringList bankNames = QStringList(), QDate initialDate = QDate::fromString("1970-01-01"), QDate finaDate = QDate::currentDate());
};

#endif // DATABASE_H
