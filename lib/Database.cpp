#include "Database.h"

#include <QDate>
#include <QList>
#include <QRegularExpression>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlResult>
#include <QProcess>
#include <QThread>

Database::Database(QObject *parent) : QObject{parent} {
  hostname = settings.value(HOSTNAME, DEFAULT_HOSTNAME).toString();
  port = settings.value(PORT, DEFAULT_PORT).toInt();
  database = settings.value(DATABASE, DEFAULT_DATABASE).toString();
  username = settings.value(USERNAME).toString();
  userpass = settings.value(USERPASS).toString();
}

Database::~Database() { sqlDatabase.close(); }

void Database::setHostname(QString name) {
  settings.setValue(HOSTNAME, name);
  hostname = name;
}

void Database::setPort(int newport) {
  settings.setValue(PORT, newport);
  port = newport;
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
  sqlDatabase = QSqlDatabase::database();

  if (sqlDatabase.isValid() == false) {
    sqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
  }

  sqlDatabase.setHostName(hostname);
  sqlDatabase.setPort(port);
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
    query.bindValue(":bank", bank.left(BANK_LENGTH));
    query.bindValue(":date", unifyDateToStore(date));
    query.bindValue(":description", description.left(DESCRIPTION_LENGTH));
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

QString Database::unifyDateToStore(QString date) {
  QDate ymd = QDate::fromString(
      QString(date).replace(QRegularExpression("/"), "-"), "yyyy-MM-dd");

  QDate dmy = QDate::fromString(
      QString(date).replace(QRegularExpression("/"), "-"), "dd-MM-yyyy");

  if (ymd.isValid()) {
    return ymd.toString(Qt::DateFormat::ISODate);
  }

  if (dmy.isValid()) {
    return dmy.toString(Qt::DateFormat::ISODate);
  }

  return QString("%1 invalid date").arg(date);
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

QList<QStringList> Database::getUncategorizedRows(QString filter,
                                                  QProgressDialog *dialog) {
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

      int count = 0;
      if (dialog) {
        dialog->setMaximum(query.numRowsAffected());
      }

      while (query.next()) {

        QSqlRecord rec = query.record();
        QStringList fields;

        for (int n = 0; n < rec.count(); n++) {
          fields.append(rec.field(n).value().toString());
        }

        rows.append(fields);
        if (dialog) {
          dialog->setValue(++count);
        }

        if (dialog && dialog->wasCanceled()) {
          break;
        }
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
                "REGEXP '%1' AND (TRIM(category) = '' OR category IS NULL)")
            .arg(regexp.length() ? regexp : ".*")
            .arg(category.left(CATEGORY_LENGHT));

    if (query.exec(queryString)) {
      updatedRows = query.numRowsAffected();
    } else {
      lastError = query.lastError().databaseText();
    }

    closeDatabase();
  }

  return updatedRows;
}

QList<QStringList> Database::getBanksBalance(QStringList bankNames,
                                             QDate initialDate,
                                             QDate finaDate) {
  QList<QStringList> bankBalance;

  if (bankNames.isEmpty()) {
    bankNames = getBankNames();
  }

  if (openDatabase()) {
    QSqlQuery query = QSqlQuery(sqlDatabase);

    foreach (QString bankName, bankNames) {
      QString queryString = QString("SELECT SUM(amount) as balance from "
                                    "transactions WHERE bank = '%1'")
                                .arg(bankName);

      if (query.exec(queryString)) {
        query.next();
        bankBalance.append(
            QStringList({bankName, query.value("balance").toString()}));
      } else {
        lastError = query.lastError().databaseText();
      }
    }

    closeDatabase();
  }

  return bankBalance;
}

QList<QSqlRecord> Database::execCommand(QString queryString) {
  QList<QSqlRecord> result;

  if (openDatabase()) {
    QSqlQuery query = QSqlQuery(sqlDatabase);

    if (query.exec(queryString)) {
      while (query.next()) {
        result.append(query.record());
      }

      if (result.isEmpty()) {
        QSqlRecord record = QSqlRecord();
        record.append(QSqlField("Rows affected"));
        record.setValue("Rows affected", query.numRowsAffected());
        result.append(record);
      }

    } else {
      lastError = query.lastError().databaseText();
    }

    closeDatabase();
  }

  return result;
}

bool Database::backup(QString fileName){
  QStringList parameters;

  parameters.append(QString("--host=%1").arg(hostname));
  parameters.append(QString("--port=%1").arg(port));
  parameters.append(QString("--user=%1").arg(username));
  parameters.append(QString("--password=%1").arg(userpass));
  parameters.append(DEFAULT_DATABASE);

  QProcess process;
  process.setStandardOutputFile(fileName);
  process.start(MYSQLDUMP_PROGRAM, parameters);
  
  while (process.waitForFinished()) {
    QThread::sleep(1);
  }

  if (process.exitCode()) {
    lastError = process.errorString();
    return false;
  }

  return true;
}

bool Database::restore(QString fileName){
  return true;
}
