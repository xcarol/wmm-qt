#include "DatabaseSettings.h"
#include "ui_DatabaseSettings.h"

DatabaseSettings::DatabaseSettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DatabaseSettings)
{
    ui->setupUi(this);
}

DatabaseSettings::~DatabaseSettings()
{
    delete ui;
}

void DatabaseSettings::init(QString hostname, int port, QString database, QString username, QString userpass)
{
    ui->HostEdit->setText(hostname);
    ui->databaseEdit->setText(database);
    ui->userEdit->setText(username);
    ui->portEdit->setText(QString::number(port));
    ui->passwordEdit->setText(userpass);
}

void DatabaseSettings::on_buttonBox_accepted()
{
    hostname = ui->HostEdit->text();
    port = ui->portEdit->text().toInt();
    database = ui->databaseEdit->text();
    username = ui->userEdit->text();
    userpass = ui->passwordEdit->text();
}

