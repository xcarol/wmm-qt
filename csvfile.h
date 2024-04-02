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
    bool read(QString filename);
};

#endif // CSVFILE_H
