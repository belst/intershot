#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QObject>
#include <QLabel>
#include <QPixmap>
#include <QPoint>
#include <QMouseEvent>
#include <QCommandLineParser>

class Screenshot : public QLabel
{
    Q_OBJECT
public:
    explicit Screenshot(QCommandLineParser* parser,
                        QString filename,
                        QWidget *parent = 0);

signals:
    void mousePress(QMouseEvent*);
    void mouseRelease(QMouseEvent*);

public slots:
    void shootScreen();
    void mouseDown(QMouseEvent*);
    void mouseUp(QMouseEvent*);

private:
    QCommandLineParser* parser;
    QPixmap originalImage;
    QPoint upperleft;
    QPoint lowerright;
    QString filename;
    void saveAndClose();
    void parseString(QString&);

};

#endif // SCREENSHOT_H
