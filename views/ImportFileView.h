#ifndef IMPORTFILE_H
#define IMPORTFILE_H

#include <QMainWindow>
#include "../lib/CsvFile.h"

namespace Ui {
class ImportFileView;
}

class ImportFileView : public QMainWindow
{
    Q_OBJECT

public:
    explicit ImportFileView(QWidget *parent = nullptr);
    ~ImportFileView();

private slots:
    void on_openFileButton_clicked();

    void on_firstRowCheckBox_stateChanged(int arg1);

    void on_dateColumnComboBox_currentIndexChanged(int index);

    void on_descriptionColumnComboBox_currentIndexChanged(int index);

    void on_amountColumnComboBox_currentIndexChanged(int index);

    void on_ImportButton_clicked();

    void on_banksComboBox_editTextChanged(const QString &arg1);

    void on_banksComboBox_currentIndexChanged(int index);

    void on_banksComboBox_currentTextChanged(const QString &arg1);

private:
    int indexOffset = 1;
    int headerRows = 0;
    int dateColumn = 0;
    int descriptionColumn = 0;
    int amountColumn = 0;
    QString bankName = "";

    Ui::ImportFileView *ui;

    CsvFile csvFile = CsvFile();

    void updatePreview();
    void updateImportButtonState();
};

#endif // IMPORTFILE_H
