#ifndef CSVFILE_H
#define CSVFILE_H

#include <QFile>
#include <QList>

#define MAX_ROWS 10

class CsvFile {
private:
  QFile *file = NULL;
  ulong rowsInFile = 0;
  ulong columnsInFile = 0;
  QList<QString> csvRows;
  QList<QStringList> csvFields;

  void readRows(int index = 0, int count = 0);
  void parseRows();

public:
  static bool isValidCsvFile(QString filename);

public:
  ~CsvFile();
  bool isEmpty() { return csvRows.isEmpty(); }
  bool open(QString filename);
  void close();
  ulong rowsCount();
  ulong columnsCount();
  QList<QStringList> getRows(int index = 0, int count = 1);
};

#endif // CSVFILE_H
