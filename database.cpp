#include "database.h"

#include <QSqlDatabase>
#include <qsqlerror.h>

Database::Database(QObject *parent) : QObject{parent} {
  hostname = settings.value("hostname").toString();
  database = settings.value("database").toString();
  username = settings.value("username").toString();
  userpass = settings.value("userpass").toString();
}

Database::~Database() { sqlDatabase.close(); }

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