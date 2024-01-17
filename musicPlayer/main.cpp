#include "mainwindow.h"
#include<QCommandLineParser>
#include <QApplication>
#include<QDesktopServices>
#include<QFileInfo>
#include<QProcess>
#include<QString>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("My Media Player");

    QCommandLineParser parser;
    parser.addPositionalArgument("file", "The file to open.");



       //Set the app style sheet
    QFile styleSheetFile(":/Toolery.qss");
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    a.setStyleSheet(styleSheet);
    MainWindow w;
    parser.process(a);
    const QStringList args = parser.positionalArguments();
    if (args.size() > 0) {
        QString fileName = args.at(0);
        QString suffix = QFileInfo(fileName).suffix().toLower();
        if(suffix == "mp3" || suffix == "mp4" || suffix == "mkv"){
            w.handle_open_with(fileName);
        }
    }
    QWidget* focused_widget = a.focusWidget();
    w.setWindowTitle("Busters Media Player");
    w.show();


    return a.exec();
}

