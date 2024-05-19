#ifndef MESSAGEBOXWIDGETS_H
#define MESSAGEBOXWIDGETS_H

#include <QWidget>

class MessageBox : public QWidget
{
    Q_OBJECT
public:
    explicit MessageBox(QWidget *parent = nullptr);

    static void DatabaseSuccess(QString message);
    static void DatabaseError(QString error);
signals:
};

#endif // MESSAGEBOXWIDGETS_H
