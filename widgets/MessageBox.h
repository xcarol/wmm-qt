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

    static void Information(QString title, QString message);
    static void Warning(QString title, QString message);
    static void Critical(QString title, QString message);
signals:
};

#endif // MESSAGEBOXWIDGETS_H
