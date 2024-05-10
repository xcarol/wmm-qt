#ifndef DATABASE_H
#define DATABASE_H

#include <QDateTime>
#include <QObject>
#include <QProgressDialog>
#include <QSettings>
#include <QSqlDatabase>
#include <qcontainerfwd.h>

#define HOSTNAME "hostname"
#define PORT "port"
#define DATABASE "database"
#define USERNAME "username"
#define USERPASS "userpass"

#define DEFAULT_HOSTNAME "localhost"
#define DEFAULT_PORT "3306"
#define DEFAULT_DATABASE "wmm"

#define MYSQLDUMP_PROGRAM "mysqldump"
#define MYSQL_PROGRAM "mysql"

// These sizes are defined in the file: ../database/database.sql
#define BANK_LENGTH 200
#define DESCRIPTION_LENGTH 200
#define CATEGORY_LENGHT 200

class Database : public QObject {
  Q_OBJECT

private:
  QString quote = "'";
  QString escapedQuote = "''";

  QString queryInsertRow = QString("INSERT INTO transactions (bank, date, "
                                   "description, amount) VALUES (:bank, "
                                   ":date, :description, :amount)");

  QString queryBankNames = QString("SELECT DISTINCT bank FROM transactions");

  QString queryCategoryNames =
      QString("SELECT DISTINCT category FROM transactions WHERE category IS "
              "NOT NULL AND "
              "TRIM(category) <> ''");

  QString queryFilterNames = "SELECT DISTINCT filter FROM filters WHERE category='%1' ORDER BY filter DESC";

  QString queryDescriptions = "SELECT DISTINCT description FROM transactions WHERE category='%1' ORDER BY description ASC";

  QString queryUncategorizedRows = QString(
      "SELECT id, bank, date, description, category, amount FROM transactions "
      "WHERE (TRIM(category) = '' OR category IS NULL)");

  QString queryUncategorizedRowsFilter =
      QString(" AND description REGEXP '%1'");

  QString queryColumnNames = QString("SELECT * FROM transactions LIMIT 1");

  QString queryUpdateRowsCategoryWithDescriptionRegex =
      QString("UPDATE transactions SET category = '%2' WHERE description "
              "REGEXP '%1' AND (TRIM(category) = '' OR category IS NULL)");

  QString queryUpdateRowsCategoryWithIds =
      QString("UPDATE transactions SET category = '%1' WHERE id IN(%2)");

  QString queryBankBalances = QString(
      "SELECT SUM(amount) as balance from transactions WHERE bank = '%1' AND "
      "date >= '%2' AND date <= '%3'");

  QString queryCategoryBalances =
      QString("SELECT SUM(amount) as balance from transactions WHERE category "
              "= '%1' AND "
              "date >= '%2' AND date <= '%3'");

  QString queryDuplicateRows = QString("SELECT * FROM transactions t1"
                                       " WHERE EXISTS ("
                                       "    SELECT 1"
                                       "    FROM transactions t2"
                                       "    WHERE t1.bank = t2.bank"
                                       "    AND t1.date = t2.date"
                                       "    AND t1.description = t2.description"
                                       "    AND t1.amount = t2.amount"
                                       "    AND t1.id <> t2.id"
                                       "    AND t1.not_duplicate = FALSE"
                                       "    AND t2.not_duplicate = FALSE"
                                       " )"
                                       " ORDER BY bank, date DESC");

  QString queryDeleteRows =
      QString("DELETE FROM transactions WHERE id IN (%1)");

  QString queryMarkNotDuplicateRows =
      QString("UPDATE transactions SET not_duplicate = TRUE WHERE id IN (%1)");

  QString queryYears = QString("SELECT DISTINCT YEAR(date) FROM transactions");

  QString queryAddCategoryFilters = QString("INSERT INTO filters (category, filter) VALUES %1");

  QString queryDeleteCategoryFilters = QString("DELETE FROM filters WHERE category = '%1'");

  QString queryDeleteCategories = QString("DELETE FROM filters WHERE category IN (%1)");

  QString queryDeleteFilters = QString("DELETE FROM filters WHERE filter IN (%1)");

  QString queryResetRowsCategories = QString("UPDATE transactions SET category = '' WHERE category in (%1)");

  QString queryAddFilter = QString("INSERT INTO filters (category, filter) VALUES ('%1', '%2')");

private:
  QString lastError;
  QString hostname;
  int port;
  QString database;
  QString username;
  QString userpass;

  QSqlDatabase sqlDatabase;
  QSettings settings = QSettings("com.xicra", "wmm");

  bool openDatabase();
  void closeDatabase();

  QStringList databaseConnectionParameters();
  QList<QStringList> getBalance(QString queryBalance, QStringList entites,
                                QDate initialDate, QDate finalDate);

  QString stringListToSqlList(QStringList list);
  QString filterListToSqlList(QString category, QStringList filters);
  QString rowsToSqlList(QList<int> rows);
  QString unifyDateToStore(QString);

public:
  explicit Database(QObject *parent = nullptr);
  ~Database();

  QString getLastErrorText() { return lastError; };

  QString getHostname() { return hostname; }
  int getPort() { return port; }
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
  int updateRowsCategory(QString descriptionRegex, QString category);
  int updateRowsCategory(QList<int> rowIds, QString category);
  QStringList getBankNames();
  QStringList getCategoryNames();
  QStringList getColumnNames();
  QStringList getFilterNames(QString category);
  QStringList getDescriptionsByCategory(QString category);
  QList<QStringList> getUncategorizedRows(QString filter = QString(),
                                          QProgressDialog *dialog = NULL);
  QList<QStringList>
  getBanksBalance(QStringList bankNames = QStringList(),
                  QDate initialDate = QDate::fromString("1970-01-01"),
                  QDate finalDate = QDate::currentDate());
  QList<QStringList>
  getCategoriesBalance(QStringList bankNames = QStringList(),
                       QDate initialDate = QDate::fromString("1970-01-01"),
                       QDate finalDate = QDate::currentDate());
  QList<QStringList> getDuplicateRows();
  QStringList getYears(bool ascending = true);

  bool addFilter(QString category, QString filter);
  int deleteRows(QList<int> rows);
  int markAsNotDuplicateRows(QList<int> rows);
  QList<QSqlRecord> execCommand(QString queryString);
  int deleteCategories(QStringList categories);
  int deleteFilters(QStringList filters);
  int resetRowsCategories(QStringList categories);
  int updateCategoryFilters(QString category, QStringList filters);

  bool backup(QString fileName);
  bool restore(QString fileName);
};

#endif // DATABASE_H
