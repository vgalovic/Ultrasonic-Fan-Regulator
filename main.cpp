#include "dialog.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile styleSheetFile(":/qss/qss/MaterialDark.qss");
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    a.setStyleSheet(styleSheet);

    Dialog w;
    w.show();

    return a.exec();
}
