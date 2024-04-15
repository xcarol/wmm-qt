#ifndef BROWSEDATA_H
#define BROWSEDATA_H

#include <QMainWindow>

namespace Ui {
class BrowseData;
}

class BrowseData : public QMainWindow
{
    Q_OBJECT

public:
    explicit BrowseData(QWidget *parent = nullptr);
    ~BrowseData();

private:
    Ui::BrowseData *ui;
};

#endif // BROWSEDATA_H
