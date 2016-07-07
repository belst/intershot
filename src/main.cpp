#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <ctime>
#include <time.h>
#include "screenshot.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QCommandLineParser* parser = new QCommandLineParser;
    parser->addHelpOption();
    parser->addVersionOption();

    parser->addPositionalArgument("file", "Save Location (can use strftime args)");
    QCommandLineOption selectOption({"s", "select"}, "Select an area on the screen");
    parser->addOption(selectOption);
    QCommandLineOption execOption({"e", "exec"}, "Execute command on the created file (use $f as filename)", "execstring");
    parser->addOption(execOption);

    parser->process(app);
    
    const QStringList args = parser->positionalArguments();
    char mbstr[100];
    std::time_t t = std::time(NULL);
    struct tm* tm = localtime(&t);

    QString filename = "";
    if(args.isEmpty()) {
        std::strftime(mbstr, sizeof(mbstr), "%F-%T.png", tm);
        if(parser->isSet(selectOption))
            filename = "selection_";
        else
            filename = "screen_";
        filename.append(mbstr);
    } else {
        std::strftime(mbstr, sizeof(mbstr), args.first().toStdString().c_str(), tm);
        filename = mbstr;
    }

    Screenshot main(parser, filename);

    app.exec();
}