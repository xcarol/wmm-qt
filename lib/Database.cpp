#include "Database.h"

#include <QDate>
#include <QList>
#include <QProcess>
#include <QRegularExpression>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlResult>
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
    query.prepare(queryInsertRow);
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

  return QString(tr("%1 invalid date")).arg(date);
}

QStringList Database::getBankNames() {
  QStringList bankNames;

  if (openDatabase()) {
    QSqlQuery query = QSqlQuery(sqlDatabase);

    if (query.exec(queryBankNames)) {
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

    if (query.exec(queryCategoryNames)) {
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
    QString queryString = queryUncategorizedRows;

    if (!filter.isEmpty()) {
      queryString.append(QString(queryUncategorizedRowsFilter).arg(filter));
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

    if (query.exec(queryColumnNames)) {
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
    QString queryString = QString(queryUpdateRowsCategory)
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
                                             QDate finalDate) {
  QList<QStringList> bankBalance;

  if (bankNames.isEmpty()) {
    bankNames = getBankNames();
  }

  if (openDatabase()) {
    QSqlQuery query = QSqlQuery(sqlDatabase);

    foreach (QString bankName, bankNames) {
      QString queryString =
          QString(queryBankBalances)
              .arg(bankName)
              .arg(initialDate.toString(Qt::DateFormat::ISODate))
              .arg(finalDate.toString(Qt::DateFormat::ISODate));

      if (query.exec(queryString)) {
        if (query.next()) {
          bankBalance.append(
              QStringList({bankName, query.value("balance").toString()}));
        }
      } else {
        lastError = query.lastError().databaseText();
      }
    }

    closeDatabase();
  }

  return bankBalance;
}

QList<QStringList> Database::getCategoriesBalance(QStringList categoryNames,
                                                  QDate initialDate,
                                                  QDate finalDate) {
  QList<QStringList> bankBalance;

  if (categoryNames.isEmpty()) {
    categoryNames = getCategoryNames();
  }

  if (openDatabase()) {
    QSqlQuery query = QSqlQuery(sqlDatabase);

    foreach (QString categoryName, categoryNames) {
      QString queryString =
          QString(queryCategoryBalances)
              .arg(categoryName)
              .arg(initialDate.toString(Qt::DateFormat::ISODate))
              .arg(finalDate.toString(Qt::DateFormat::ISODate));

      if (query.exec(queryString)) {
        if (query.next()) {
          bankBalance.append(
              QStringList({categoryName, query.value("balance").toString()}));
        }
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

QList<QStringList> Database::getDuplicateRows() {
  QList<QStringList> result;

  if (openDatabase()) {
    QSqlQuery query = QSqlQuery(sqlDatabase);

    QString queryString = QString(queryDuplicateRows);

    if (query.exec(queryString)) {
      while (query.next()) {
        QStringList row;
        row.append(query.value("id").toString());
        row.append(query.value("bank").toString());
        row.append(query.value("date").toString());
        row.append(query.value("description").toString());
        row.append(query.value("amount").toString());
        result.append(row);
      }

    } else {
      lastError = query.lastError().databaseText();
    }

    closeDatabase();
  }

  return result;
}

int Database::deleteRows(QList<int> rows) {
  int affectedRows = 0;

  if (openDatabase()) {
    QSqlQuery query = QSqlQuery(sqlDatabase);

    QString strIds;

    foreach (int row, rows) {
      strIds.append(QString::number(row)).append(",");
    }

    QString queryString = QString(queryDeleteRows).arg(strIds.removeLast());

    if (query.exec(queryString)) {
      affectedRows = query.numRowsAffected();
    } else {
      lastError = query.lastError().databaseText();
    }

    closeDatabase();
  }

  return affectedRows;
}

int Database::markAsNotDuplicateRows(QList<int> rows) {
  int affectedRows = 0;

  if (openDatabase()) {
    QSqlQuery query = QSqlQuery(sqlDatabase);

    QString strIds;

    foreach (int row, rows) {
      strIds.append(QString::number(row)).append(",");
    }

    QString queryString =
        QString(queryMarkNotDuplicateRows).arg(strIds.removeLast());

    if (query.exec(queryString)) {
      affectedRows = query.numRowsAffected();
    } else {
      lastError = query.lastError().databaseText();
    }

    closeDatabase();
  }

  return affectedRows;
}

bool Database::backup(QString fileName) {
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

bool Database::restore(QString fileName) {
  QProcess process;
  QStringList parameters;
  QString sqlCommand = MYSQL_PROGRAM;

  parameters.append(QString("--host=%1").arg(hostname));
  parameters.append(QString("--port=%1").arg(port));
  parameters.append(QString("--user=%1").arg(username));
  parameters.append(QString("--password=%1").arg(userpass));
  parameters.append(DEFAULT_DATABASE);

  process.setStandardInputFile(fileName);
  process.start(sqlCommand, parameters);

  while (process.waitForFinished()) {
    QThread::sleep(1);
  }

  if (process.exitCode()) {
    lastError = QString("Error executing: %1.\nexitCode: %2\nErrorString: %3")
                    .arg(sqlCommand)
                    .arg(process.exitCode())
                    .arg(process.errorString());
    return false;
  }

  return true;
}

QStringList Database::getYears(bool ascending) {
  QStringList years;

  if (openDatabase()) {
    QSqlQuery query = QSqlQuery(sqlDatabase);

    if (query.exec(queryYears)) {
      while (query.next()) {

        years.append(query.value(0).toString());
      }
    } else {
      lastError = query.lastError().databaseText();
    }

    closeDatabase();
  }

  if (ascending == false) {
    std::reverse(years.begin(), years.end());
  }

  return years;
}
