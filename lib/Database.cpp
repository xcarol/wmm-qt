#include "Database.h"

#include <QList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlResult>

Database::Database(QObject *parent) : QObject{parent} {
  hostname = settings.value(HOSTNAME).toString();
  database = settings.value(DATABASE).toString();
  username = settings.value(USERNAME).toString();
  userpass = settings.value(USERPASS).toString();
}

Database::~Database() { sqlDatabase.close(); }

void Database::setHostname(QString name) {
  settings.setValue(HOSTNAME, name);
  hostname = name;
}

void Database::setDatabase(QString name) {
  settings.setValue(DATABASE, name);
  database = name;
}

void Database::setUsername(QString name) {
  settings.setValue(USERNAME, name);
  username = name;
}

void Database::setUserpass(QString pass) {
  settings.setValue(USERPASS, pass);
  userpass = pass;
}

bool Database::openDatabase() {
  lastError.clear();
  sqlDatabase = QSqlDatabase::addDatabase("QMYSQL");

  sqlDatabase.setHostName(hostname);
  sqlDatabase.setDatabaseName(database);
  sqlDatabase.setUserName(username);
  sqlDatabase.setPassword(userpass);

  if (!sqlDatabase.open()) {
    lastError = sqlDatabase.lastError().databaseText();
    return false;
  }

  return true;
}

void Database::closeDatabase() {
  sqlDatabase.close();
  sqlDatabase = QSqlDatabase();
}

bool Database::checkConnection() {
  if (openDatabase()) {
    closeDatabase();
    return true;
  }

  return false;
}

bool Database::storeRow(QString bank, QString date, QString description,
                        double amount) {
  if (openDatabase()) {
    bool success = true;

    QSqlQuery query = QSqlQuery(sqlDatabase);
    query.prepare("INSERT INTO transactions (bank, date, description, amount) "
                  "VALUES (:bank, :date, :description, :amount)");
    query.bindValue(":bank", bank);
    query.bindValue(":date", date);
    query.bindValue(":description", description);
    query.bindValue(":amount", amount);

    if (!query.exec()) {
      lastError = query.lastError().databaseText();
      success = false;
    }

    closeDatabase();
    return success;
  }

  return false;
}

QStringList Database::getBankNames() {
  QStringList bankNames;

  if (openDatabase()) {
    QSqlQuery query = QSqlQuery(sqlDatabase);

    if (query.exec("SELECT DISTINCT bank FROM transactions")) {
      while (query.next()) {
        bankNames.append(query.value("bank").toString());
      }
    } else {
      lastError = query.lastError().databaseText();
    }

    closeDatabase();
  }

  return bankNames;
}

QStringList Database::getCategoryNames() {
  QStringList categoryNames;

  if (openDatabase()) {
    QSqlQuery query = QSqlQuery(sqlDatabase);

    if (query.exec("SELECT DISTINCT category FROM transactions WHERE category "
                   "IS NOT NULL AND TRIM(category) <> ''")) {
      while (query.next()) {
        categoryNames.append(query.value("category").toString());
      }
    } else {
      lastError = query.lastError().databaseText();
    }

    closeDatabase();
  }

  return categoryNames;
}

QList<QStringList> Database::getUncategorizedRows(QString filter) {
  QList<QStringList> rows;

  if (openDatabase()) {
    QSqlQuery query = QSqlQuery(sqlDatabase);
    QString queryString =
        "SELECT * FROM transactions WHERE (TRIM(category) = '' "
        "OR category IS NULL)";

    if (!filter.isEmpty()) {
      queryString.append(QString(" AND description REGEXP '%1'").arg(filter));
    }

    if (query.exec(queryString)) {
      while (query.next()) {

        QSqlRecord rec = query.record();
        QStringList fields;

        for (int n = 0; n < rec.count(); n++) {
          fields.append(rec.field(n).value().toString());
        }

        rows.append(fields);
      }
    } else {
      lastError = query.lastError().databaseText();
    }

    closeDatabase();
  }

  return rows;
}

QStringList Database::getColumnNames() {
  QStringList names;

  if (openDatabase()) {
    QSqlQuery query = QSqlQuery(sqlDatabase);

    if (query.exec("SELECT * FROM transactions LIMIT 1")) {
      while (query.next()) {

        QSqlRecord rec = query.record();
        for (int n = 0; n < rec.count(); n++) {
          names.append(rec.fieldName(n));
        }
      }
    } else {
      lastError = query.lastError().databaseText();
    }

    closeDatabase();
  }

  return names;
}

ulong Database::updateRowsCategory(QString regexp, QString category) {
  ulong updatedRows;

  if (openDatabase()) {
    QSqlQuery query = QSqlQuery(sqlDatabase);
    QString queryString =
        QString("UPDATE transactions SET category = '%2' WHERE description "
                "REGEXP '%1' AND TRIM(category) = ''")
            .arg(regexp.length() ? regexp : ".*")
            .arg(category);

    if (query.exec(queryString)) {
      updatedRows = query.numRowsAffected();
    } else {
      lastError = query.lastError().databaseText();
    }

    closeDatabase();
  }

  return updatedRows;
}
