#include "MessageBox.h"
#include <QMessageBox>

MessageBox::MessageBox(QWidget *parent) : QWidget{parent} {}

void MessageBox::DatabaseSuccess(QString message) {
  QMessageBox(QMessageBox::Icon::Information, QString(tr("Database success")),
              message)
      .exec();
}

void MessageBox::DatabaseError(QString error) {
  QMessageBox(QMessageBox::Icon::Critical, QString(tr("Database error")), error)
      .exec();
}

void MessageBox::Information(QString title, QString message) {
  QMessageBox(QMessageBox::Icon::Information, title, message).exec();
}

void MessageBox::Warning(QString title, QString message) {
  QMessageBox(QMessageBox::Icon::Warning, title, message).exec();
}

void MessageBox::Critical(QString title, QString message) {
  QMessageBox(QMessageBox::Icon::Critical, title, message).exec();
}
