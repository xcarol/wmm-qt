#include "csvfile.h"
#include <QFile>
#include <QIODevice>
#include <QRegularExpression>

bool CsvFile::isValidCsvFile(QString filename)
{
    QFile file = QFile(filename);

    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray bytes = file.read(10);

        for (int n = 0; n < bytes.length(); n++)
        {
            if (QChar::isPrint(bytes.at(n)) == false)
            {
                return false;
            }
        }
    }

    return true;
}

void CsvFile::parseRows()
{
    foreach (QString row, csvRows)
    {
        QStringList fields = row.split(",");
        QStringList parsedFields;

        foreach (QString rowField, fields)
        {
            parsedFields.append(rowField.replace(QRegularExpression("^\"|\"$|\\n$"), ""));
        }
        csvFields.append(parsedFields);
    }
}

bool CsvFile::read(QString filename)
{
    csvRows.clear();
    csvFields.clear();

    QFile file = QFile(filename);

    if (isValidCsvFile(filename) == false)
    {
        return false;
    }

    if (file.open(QIODevice::ReadOnly))
    {
        do
        {
            QByteArray line = file.readLine();
            if (line.length() == 0)
                break;
            csvRows.append(QString::fromStdString(line.toStdString()));
        } while (true);

        parseRows();
        return true;
    }

    return false;
}

QList<QStringList> CsvFile::getRows(int count)
{
    return csvFields.mid(0, count);
}
