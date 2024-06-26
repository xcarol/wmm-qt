#ifndef DATABASE_H
#define DATABASE_H

#include <QDateTime>
#include <QObject>
#include <QProgressDialog>
#include <QSettings>
#include <QSqlDatabase>

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

  QString queryCategoryNames = QString(
      "SELECT DISTINCT category FROM transactions WHERE (TRIM(category) != '' "
      "AND category IS NOT NULL) ORDER BY category ASC");

  QString queryFilter = "SELECT category FROM filters WHERE filter = '%1'";

  QString queryFilterNames = "SELECT DISTINCT filter FROM filters WHERE "
                             "category='%1' ORDER BY filter ASC";

  QString queryDescriptions = "SELECT DISTINCT description FROM transactions "
                              "WHERE category='%1' ORDER BY description ASC";

  QString queryUncategorizedRows = QString(
      "SELECT id, bank, date, description, category, amount FROM transactions "
      "WHERE (TRIM(category) = '' OR category IS NULL)");

  QString queryUncategorizedRowsFilter =
      QString(" AND description REGEXP '%1'");

  QString queryColumnNames = QString("SELECT * FROM transactions LIMIT 1");

  QString queryUpdateRowsCategoryWithAllFilters =
      QString("UPDATE transactions AS t"
              " JOIN filters AS f ON t.description LIKE CONCAT('%', "
              " REPLACE(f.filter, '%', '\\%'), '%')"
              " SET t.category = f.category"
              " WHERE f.category = '%1' AND "
              " (TRIM(t.category) = '' OR t.category IS NULL)");

  QString queryUpdateRowsCategoryWithDescriptionRegex =
      QString("UPDATE transactions SET category = '%2' WHERE description "
              "REGEXP '%1' AND (TRIM(category) = '' OR category IS NULL)");

  QString queryUpdateRowsCategoryWithIds =
      QString("UPDATE transactions SET category = '%1' WHERE id IN(%2)");

  QString queryBankBalances =
      QString("SELECT SUM(amount) as balance, MAX(date) AS latest_date from "
              "transactions WHERE bank = '%1' AND "
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

  QString queryAddCategoryFilters =
      QString("INSERT INTO filters (category, filter) VALUES %1");

  QString queryDeleteCategoryFilters =
      QString("DELETE FROM filters WHERE category = '%1'");

  QString queryDeleteCategories =
      QString("DELETE FROM filters WHERE category IN (%1)");

  QString queryDeleteFilters =
      QString("DELETE FROM filters WHERE filter IN (%1)");

  QString queryResetRowsCategories =
      QString("UPDATE transactions SET category = '' WHERE category in (%1)");

  QString queryAddFilter =
      QString("INSERT INTO filters (category, filter) VALUES ('%1', '%2')");

  QString queryRenameRowsCategory =
      QString("UPDATE transactions SET category = '%2' WHERE category = '%1'");

  QString queryRenameCategoryFilters =
      QString("UPDATE filters SET category = '%2' WHERE category = '%1'");

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
  int updateRowsCategory(QString category);
  int updateRowsCategory(QString descriptionRegex, QString category);
  int updateRowsCategory(QList<int> rowIds, QString category);
  QStringList getBankNames();
  QStringList getCategoryNames();
  QStringList getColumnNames();
  QStringList getFilter(QString filter);
  QStringList getFilterNames(QString category);
  QStringList getDescriptionsByCategory(QString category);
  QList<QStringList> getUncategorizedRows(QString filter = QString(),
                                          QProgressDialog *dialog = NULL);
  QList<QStringList> getBanksBalance(
      QStringList bankNames = QStringList(),
      QDate initialDate = QDate::fromString("1970-01-01", Qt::ISODate),
      QDate finalDate = QDate::currentDate());
  QList<QStringList> getCategoriesBalance(
      QStringList bankNames = QStringList(),
      QDate initialDate = QDate::fromString("1970-01-01", Qt::ISODate),
      QDate finalDate = QDate::currentDate());
  QList<QStringList> getDuplicateRows();
  QStringList getYears(bool ascending = true);

  bool addFilter(QString category, QString filter);
  bool addFilters(QString category, QStringList filters);
  int deleteRows(QList<int> rows);
  int markAsNotDuplicateRows(QList<int> rows);
  QList<QSqlRecord> execCommand(QString queryString);
  int deleteCategories(QStringList categories);
  int deleteFilters(QStringList filters);
  int resetRowsCategories(QStringList categories);
  int updateCategoryFilters(QString category, QStringList filters);
  bool renameCategory(QString category, QString newName);

  bool backup(QString fileName);
  bool restore(QString fileName);
};

#endif // DATABASE_H
