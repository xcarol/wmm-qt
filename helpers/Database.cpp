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

void Database::closeDatabase() { sqlDatabase.close(); }

ulong Database::storeRows(QList<QStringList> rows, QProgressDialog *progress) {
  ulong rowCount = 0;

  if (openDatabase()) {
    for (; rowCount < rows.length(); rowCount++) {
      QSqlQuery query = QSqlQuery(sqlDatabase);
      query.prepare(
          "INSERT INTO transactions (bank, date, description, amount) "
          "VALUES (:bank, :date, :description, :amount)");
      query.bindValue(":bank", rows.at(rowCount).at(0));
      query.bindValue(":date", rows.at(rowCount).at(1));
      query.bindValue(":description", rows.at(rowCount).at(2));
      query.bindValue(":amount", rows.at(rowCount).at(3));

      if (!query.exec()) {
        lastError = query.lastError().databaseText();
        break;
      }

      progress->setValue(rowCount);
    }

    closeDatabase();
  }

  return rowCount;
}

QStringList Database::getBankNames() {
  QStringList bankNames;
  QSqlQuery query = QSqlQuery(sqlDatabase);

  if (openDatabase()) {
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

QList<QStringList> Database::getUncategorizedRows() {
  QList<QStringList> rows;
  QSqlQuery query = QSqlQuery(sqlDatabase);

  if (openDatabase()) {
    if (query.exec("SELECT * FROM transactions WHERE category IS NULL")) {
      while (query.next()) {

        QSqlRecord rec = query.record();
        QStringList fields;
        
        for (int n = 0; n < rec.count(); n++) {
          QSqlField f = rec.field(n);
          qDebug() << f.value();
          fields.append(f.value().toString());
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
  QSqlQuery query = QSqlQuery(sqlDatabase);

  if (openDatabase()) {
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
