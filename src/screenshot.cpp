#include "screenshot.h"

#include <QScreen>
#include <QGuiApplication>
#include <QRect>
#include <QPainter>
#include <QProcess>
#include <QDebug>

Screenshot::Screenshot(QCommandLineParser* parser,
                       QString filename,
                       QWidget *parent)
    : QLabel(parent), parser(parser), filename(filename)
{
#ifdef Q_OS_WIN
    int status = system("which slop > nul");
#else
    int status = system("which slop > /dev/null");
#endif
    if(WEXITSTATUS(status) != EXIT_SUCCESS) {
        use_slop = false;
        connect(this, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mouseDown(QMouseEvent*)));
        connect(this, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseUp(QMouseEvent*)));
    } else {
        use_slop = true;
    }

    shootScreen();
}


void Screenshot::shootScreen() {

    QScreen *screen = QGuiApplication::primaryScreen();

    if(screen)
        originalImage = screen->grabWindow(0);

    if(parser->isSet("s")) {
        setPixmap(originalImage);
        showFullScreen();
        if(use_slop) {
            slop();
        }
    } else {
        saveAndClose();
    }
}

void Screenshot::slop() {
    QProcess slop;
    slop.start("slop");
    slop.waitForFinished(-1);
    unsigned x, y, w, h;
    if(slop.exitStatus() == QProcess::NormalExit) {
        QString out = slop.readAllStandardOutput();
        for(QString str : out.split(QRegExp("[\r\n]"), QString::SkipEmptyParts)) {
            if(str.startsWith("X"))
                x = str.split("=")[1].toInt();
            else if(str.startsWith("Y"))
                y = str.split("=")[1].toInt();
            else if(str.startsWith("W"))
                w = str.split("=")[1].toInt();
            else if(str.startsWith("H"))
                h = str.split("=")[1].toInt();

        }

        QRect rect(x, y, w, h);
        originalImage = originalImage.copy(rect);
        saveAndClose();
    } else {
        exit(slop.exitCode());
    }
}

void Screenshot::mouseDown(QMouseEvent* event) {
    if(!parser->isSet("s"))
        return;

    upperleft = event->globalPos();
}

void Screenshot::mouseUp(QMouseEvent* event) {
    if(!parser->isSet("s"))
        return;

    lowerright = event->globalPos();
    QRect rect(upperleft, lowerright);
    if(!rect.isValid())
        qFatal("Invalid Screen region");

    originalImage = originalImage.copy(rect);
    saveAndClose();
}

void Screenshot::saveAndClose() {
    int retCode = 0;
    originalImage.save(filename);
    if(parser->isSet("exec")) {
        QString exec = parser->value("exec").replace("$f", filename).replace(QRegExp("\\$."), "$");
        retCode = system(exec.toStdString().c_str());
        retCode = WEXITSTATUS(retCode);
    }
    exit(retCode);
}

void Screenshot::mousePressEvent(QMouseEvent *event)
{
    emit mousePress(event);
}

void Screenshot::mouseReleaseEvent(QMouseEvent *event)
{
    emit mouseRelease(event);
}

