#include "BrowseDataView.h"
#include "ui_BrowseDataView.h"
#include "../lib/Database.h"
#include <qcontainerfwd.h>

BrowseDataView::BrowseDataView(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BrowseDataView)
{
    ui->setupUi(this);
    updateBankBalanceTable();
}

BrowseDataView::~BrowseDataView()
{
    delete ui;
}

void BrowseDataView::updateBankBalanceTable()
{
    Database database = Database();

    QList<QStringList> banks = database.getBanksBalance();

}
