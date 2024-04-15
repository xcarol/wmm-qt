#include "BrowseData.h"
#include "ui_BrowseData.h"

BrowseData::BrowseData(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BrowseData)
{
    ui->setupUi(this);
}

BrowseData::~BrowseData()
{
    delete ui;
}
