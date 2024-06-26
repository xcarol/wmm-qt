#include "CsvFile.h"
#include <QFile>
#include <QIODevice>
#include <QRegularExpression>

CsvFile::~CsvFile() {
  if (file != NULL) {
    file->close();
  }
}

bool CsvFile::isValidCsvFile(QString filename) {
  QFile file = QFile(filename);

  if (file.open(QIODevice::ReadOnly)) {
    QByteArray bytes = file.readLine();

    for (int n = 0; n < bytes.length(); n++) {
      if (QChar::isNonCharacter(bytes.at(n))) {
        return false;
      }
    }
  }

  return true;
}

void CsvFile::readRows(int index, int count) {
  csvRows.clear();
  csvFields.clear();

  file->seek(0);
  for (int n = 0; n < index + count; n++) {
    QByteArray line = file->readLine();
    if (line.length() == 0)
      break;
    if (n >= index) {
      csvRows.append(QString::fromStdString(line.toStdString()));
    }
  }
}

void CsvFile::parseRows() {
  foreach (QString row, csvRows) {
    QStringList fields = row.split("\",\"");
    QStringList parsedFields;

    foreach (QString rowField, fields) {
      parsedFields.append(
          rowField.replace(QRegularExpression("^\"|\"$|\\n$"), ""));
    }
    csvFields.append(parsedFields);
  }
}

bool CsvFile::open(QString filename) {
  file = new QFile(filename);

  rowsInFile = 0;
  columnsInFile = 0;
  csvRows.clear();
  csvFields.clear();

  if (isValidCsvFile(filename) == false) {
    return false;
  }

  if (file->open(QIODevice::ReadOnly)) {
    int count = 0;

    for (;; count++) {
      QByteArray line = file->readLine();
      if (line.length() == 0)
        break;
      if (count < MAX_ROWS) {
        csvRows.append(QString::fromStdString(line.toStdString()));
      }
    }

    parseRows();
    rowsInFile = count;
    columnsInFile = csvFields.first().length();
  }

  return false;
}

void CsvFile::close() { file->close(); }

ulong CsvFile::rowsCount() { return rowsInFile; }
ulong CsvFile::columnsCount() { return columnsInFile; }

QList<QStringList> CsvFile::getRows(int index, int count) {
  readRows(index, count);
  parseRows();

  return csvFields;
}
