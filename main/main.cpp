#include "../views/MainWindow.h"

#include <QApplication>
#include <QLocale>
#include <QSettings>
#include <QTranslator>

int main(int argc, char *argv[]) {
  QApplication application(argc, argv);

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "wmm_" + QLocale(locale).name();
    if (translator.load(baseName)) {
      application.installTranslator(&translator);
      break;
    }
  }

  QSettings settings = QSettings("com.xicra", "wmm");
  MainWindow mainWindow;

  mainWindow.restoreGeometry(settings.value("geometry").toByteArray());
  mainWindow.restoreState(settings.value("state").toByteArray());
  
  
  mainWindow.show();

  int result = application.exec();

  if (mainWindow.isMaximized() == false) {
    settings.setValue("geometry", mainWindow.saveGeometry());
    settings.setValue("state", mainWindow.saveState());
  }

  return result;
}
