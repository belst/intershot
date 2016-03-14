#include "screenshot.h"

#include <QScreen>
#include <QGuiApplication>
#include <QRect>
#include <QPainter>
#include <QDebug>

Screenshot::Screenshot(QCommandLineParser* parser,
                       QString filename,
                       QWidget *parent)
    : QLabel(parent), parser(parser), filename(filename)
{
    connect(this, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mouseDown(QMouseEvent*)));
    connect(this, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseUp(QMouseEvent*)));

    shootScreen();
}


void Screenshot::shootScreen() {

    QScreen *screen = QGuiApplication::primaryScreen();

    if(screen)
        originalImage = screen->grabWindow(0);

    if(parser->isSet("s")) {
        setPixmap(originalImage);
        showFullScreen();
    } else {
        saveAndClose();
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

