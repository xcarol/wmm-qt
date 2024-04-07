#include "Database.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlResult>
#include <qdebug.h>

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

bool Database::open() {
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

void Database::close() { sqlDatabase.close(); }

ulong Database::storeRows(QList<QStringList> rows, QProgressDialog *progress) {
  ulong rowCount = 0;

  if (open()) {
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

    close();
  }

  return rowCount;
}

QStringList Database::getBankNames() {
  QStringList bankNames;
  QSqlQuery query = QSqlQuery(sqlDatabase);

  if (open()) {
    if (query.exec("SELECT DISTINCT bank FROM transactions")) {
      while (query.next()) {
        bankNames.append(query.value("bank").toString());
      }
    } else {
      lastError = query.lastError().databaseText();
      qDebug() << lastError;
    }

    close();
  }

  return bankNames;
}
