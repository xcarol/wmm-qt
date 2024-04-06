#ifndef CSVFILE_H
#define CSVFILE_H

#include <QList>

class CsvFile {
private:
    QList<QString> csvRows;
    QList<QStringList> csvFields;

    void parseRows();

public:
    static bool isValidCsvFile(QString filename);

public:
    bool isEmpty() { return csvRows.isEmpty(); }
    bool read(QString filename);
    QList<QStringList> getRows(int);
};

#endif // CSVFILE_H
