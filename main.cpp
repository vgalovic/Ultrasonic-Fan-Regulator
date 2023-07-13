/*!
* \file main.cpp
* \author Vladimir Galovć
* \date 21 - 05 - 2023
* \version 1.0
*/

#include "dialog.h"

#include <QApplication>

#include <QFile>

/*!
 * \brief main
 * \param argc
 * \param argv
 * \return
 * The main() function creates an application instance, sets a style sheet for the application's visual appearance,
 * creates and displays a main dialog window, and starts the event loop for event handling.
 */
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
