#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QProgressDialog>
#include <QSettings>
#include <QSqlDatabase>
#include <QDateTime>

#define HOSTNAME "hostname"
#define PORT     "port"
#define DATABASE "database"
#define USERNAME "username"
#define USERPASS "userpass"

#define DEFAULT_HOSTNAME "localhost"
#define DEFAULT_PORT     "3306"
#define DEFAULT_DATABASE "wmm"

// These sizes are defined in the file: ../database/database.sql
#define BANK_LENGTH         200
#define DESCRIPTION_LENGTH  200
#define CATEGORY_LENGHT     200

class Database : public QObject {
  Q_OBJECT

private:
  QString lastError;
  QString hostname;
  int     port;
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
  int     getPort() { return port;}
  QString getDatabase() { return database; }
  QString getUsername() { return username; }
  QString getUserpass() { return userpass; }

  void setHostname(QString name);
  void setPort(int port);
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
  QList<QSqlRecord> execCommand(QString queryString);
};

#endif // DATABASE_H
