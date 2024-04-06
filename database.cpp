#include "database.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <qtypes.h>

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
    return false;
  }

  return true;
}

void Database::close() { sqlDatabase.close(); }

QString Database::getLastErrorText() {
  QSqlError error = sqlDatabase.lastError();
  return error.databaseText();
}

ulong Database::storeRows(QList<QStringList> rows) {
  ulong rowCount = 0;
  
  for (; rowCount < rows.length(); rowCount++) {
    QSqlQuery query;
    query.prepare("INSERT INTO transactions (bank, date, concept, amount) "
                  "VALUES (:bank, :date, :concept, :amount)");
    query.bindValue(":bank", rows.at(rowCount).at(0));
    query.bindValue(":date", rows.at(rowCount).at(1));
    query.bindValue(":concept", rows.at(rowCount).at(2));
    query.bindValue(":amount", rows.at(rowCount).at(3));

    if (!query.exec()) {
      break;
    }
  }

  return rowCount;
}
