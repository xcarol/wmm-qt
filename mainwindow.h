#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "csvfile.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openFileButton_clicked();

    void on_firstRowCheckBox_stateChanged(int arg1);

    void on_dateColumnComboBox_currentIndexChanged(int index);

    void on_conceptColumnComboBox_currentIndexChanged(int index);

    void on_amountColumnComboBox_currentIndexChanged(int index);

    void on_ImportButton_clicked();

private:
    Ui::MainWindow *ui;

    CsvFile csvFile = CsvFile();

    void updatePreview();
};
#endif // MAINWINDOW_H
