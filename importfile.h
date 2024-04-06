#ifndef IMPORTFILE_H
#define IMPORTFILE_H

#include <QMainWindow>
#include "csvfile.h"

namespace Ui {
class ImportFile;
}

class ImportFile : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImportFile(QWidget *parent = nullptr);
    ~ImportFile();

private slots:
    void on_openFileButton_clicked();

    void on_firstRowCheckBox_stateChanged(int arg1);

    void on_dateColumnComboBox_currentIndexChanged(int index);

    void on_descriptionColumnComboBox_currentIndexChanged(int index);

    void on_amountColumnComboBox_currentIndexChanged(int index);

    void on_ImportButton_clicked();

    void on_banksComboBox_editTextChanged(const QString &arg1);

private:
    int headerRows = 0;
    int dateColumn = 0;
    int descriptionColumn = 0;
    int amountColumn = 0;
    QString bankName = "";

    Ui::ImportFile *ui;

    CsvFile csvFile = CsvFile();

    void updatePreview();
    void updateImportButtonState();
};

#endif // IMPORTFILE_H
