#include "databasesettingsdialog.h"
#include "./build/wmm_autogen/include/ui_databasesettingsdialog.h"

DatabaseSettingsDialog::DatabaseSettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DatabaseSettingsDialog)
{
    ui->setupUi(this);
}

DatabaseSettingsDialog::~DatabaseSettingsDialog()
{
    delete ui;
}

void DatabaseSettingsDialog::init(QString hostname, QString database, QString username, QString userpass)
{
    ui->HostEdit->setText(hostname);
    ui->databaseEdit->setText(database);
    ui->userEdit->setText(username);
    ui->passwordEdit->setText(userpass);
}

void DatabaseSettingsDialog::on_buttonBox_accepted()
{
    hostname = ui->HostEdit->text();
    database = ui->databaseEdit->text();
    username = ui->userEdit->text();
    userpass = ui->passwordEdit->text();
}

