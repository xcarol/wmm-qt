#include "DatabaseSettings.h"
#include "../lib/Database.h"
#include "ui_DatabaseSettings.h"
#include <QMessageBox>

DatabaseSettings::DatabaseSettings(QWidget *parent)
    : QDialog(parent), ui(new Ui::DatabaseSettings) {
  ui->setupUi(this);
}

DatabaseSettings::~DatabaseSettings() { delete ui; }

void DatabaseSettings::init(QString hostname, int port, QString database,
                            QString username, QString userpass) {
  ui->HostEdit->setText(hostname);
  ui->databaseEdit->setText(database);
  ui->userEdit->setText(username);
  ui->portEdit->setText(QString::number(port));
  ui->passwordEdit->setText(userpass);
}

void DatabaseSettings::updateAttributes() {
  hostname = ui->HostEdit->text();
  port = ui->portEdit->text().toInt();
  database = ui->databaseEdit->text();
  username = ui->userEdit->text();
  userpass = ui->passwordEdit->text();
}

void DatabaseSettings::on_buttonBox_accepted() { updateAttributes(); }

bool DatabaseSettings::checkDatabaseConnection() {
  Database database = Database();

  QString currentHostname = database.getHostname();
  int currentPort = database.getPort();
  QString currentUsername = database.getUsername();
  QString currentUserpass = database.getUserpass();

  updateAttributes();

  database.setHostname(hostname);
  database.setPort(port);
  database.setUsername(username);
  database.setUserpass(userpass);

  bool success = database.checkConnection();
  if (success == false) {
    QMessageBox(
        QMessageBox::Icon::Warning, QString("Database connection problem"),
        QString("Error %1 accessing database").arg(database.getLastErrorText()))
        .exec();
  }

  database.setHostname(currentHostname);
  database.setPort(currentPort);
  database.setUsername(currentUsername);
  database.setUserpass(currentUserpass);

  return success;
}

void DatabaseSettings::on_databaseStatusButton_clicked() {
  if (checkDatabaseConnection()) {
    QMessageBox(QMessageBox::Icon::Information, QString("Database connection"),
                "Connected successfully to de database.")
        .exec();
  }
}
