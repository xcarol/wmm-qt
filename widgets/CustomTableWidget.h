#ifndef CUSTOMTABLEWIDGET_H
#define CUSTOMTABLEWIDGET_H

#include <QTableWidget>
#include <QSettings>

class CustomTableWidget : public QTableWidget {
  Q_OBJECT

private:
  QString sortSymbolsRegexp = " (▲|▼)$";
  QString sortSymbolDown = " ▼";
  QString sortSymbolUp = " ▲";

  bool resizeNastyReentering = false;
  int sortedColumn = -1;
  Qt::SortOrder sortOrder = Qt::SortOrder::AscendingOrder;

  QString settingTemplate = QString("%1_column_%2");
  QSettings settings = QSettings("com.xicra", "wmm");

  void saveColumnsWidths();
  void sortByColumn(int column);
  void sortHeaders(int column, Qt::SortOrder order);

protected:
  QString tableName = QString("");
  QString trimTimeFromDate = QString("T00:00:00.000");

  bool isNesting();
  void disableNesting();
  void enableNesting();
  void restoreColumnsWidths(QString tableName);

public:
  explicit CustomTableWidget(QWidget *parent = nullptr);
  void on_headerClicked(int index);
  void on_headerResized();

signals:
};

#endif // CUSTOMTABLEWIDGET_H
