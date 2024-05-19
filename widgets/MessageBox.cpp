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
