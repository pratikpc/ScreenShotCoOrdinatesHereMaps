#pragma once

#ifndef SCREENSHOTTER_H
#define SCREENSHOTTER_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QUrlQuery>
#include <QWebEngineView>

#include <QJsonArray>
#include <QJsonDocument>

#include <iostream>

struct Screenshot: public QObject
{
    Q_OBJECT
private:
    QWebEngineView View;

public:
    QString Destination;
    QString Url;
    int Interval;

private:
    QSize PageDimensions() const
    {
        return View.page()->contentsSize().toSize();
    }

    constexpr static bool ColorWhite(QRgb const Color) noexcept
    {
        return qRed(Color) == 255 && qGreen(Color) == 255 && qBlue(Color) == 255;
    }

    bool VerifyIfLoadNotFinished(QImage const& Image) const noexcept
    {
        for(int i = 2; i < Image.width(); i = i + 100)
            for(int j = 2; j < Image.height(); j = j + 100)
                if(ColorWhite(Image.pixel(i,j)))
                    return true;

        return ColorWhite(Image.pixel(2,2))
               || ColorWhite(Image.pixel(Image.width()-2,2))
               || ColorWhite(Image.pixel(2,Image.height()-2))
               || ColorWhite(Image.pixel(Image.width()-2,Image.height()-2));
    }

private:
    QTimer * const TimerCheckIfScreenshotPossible;

private slots:
    void TakeScreenShot()
    {
        if(PageDimensions().isEmpty())
            return;

        QImage image(PageDimensions(),QImage::Format_ARGB32);
        QPainter painter(&image);

        View.page()->view()->render(&painter);

        if(VerifyIfLoadNotFinished(image))
        {
            qDebug() << "Color Downloading Being Performed";
            return;
        }
        qDebug() << "Color Download Done Succedded";

        // Save the File
        image.save(Destination);

        qDebug() << "File Saved " << Destination;

        disconnect(TimerCheckIfScreenshotPossible,SIGNAL(timeout()),this,SLOT(TakeScreenShot()));

        // Terminate Timer
        // Timer Use finished
        TimerCheckIfScreenshotPossible->stop();
        // Close the View
        View.close();
    }

private slots:
    void LoadedTakeScreenShot(bool const loaded)
    {
        if(!loaded)
            return;

        qDebug() << "Page Load Almost done";

        TimerCheckIfScreenshotPossible->start(Interval);
        connect(TimerCheckIfScreenshotPossible,SIGNAL(timeout()),this,SLOT(TakeScreenShot()));
    }

public:

    Screenshot(QString const& Url, QString const& Destination)
        : Destination{Destination},
        Url{Url},
        TimerCheckIfScreenshotPossible{new QTimer(&View)},
        Interval{2000/*2 Seconds*/}
    {
        View.setAttribute(Qt::WA_DontShowOnScreen);
        View.resize(800, 600);
        View.show();
    }
    void Perform(QString const& Key, QJsonArray const& Array)
    {
        QJsonDocument Value;
        Value.setArray(Array);

        QString const UrlWithData = Url + "?" + Key + "=" + QString(Value.toJson(QJsonDocument::JsonFormat::Compact));

        qDebug() << "Page Load Started " << UrlWithData;
        View.setUrl(UrlWithData);

        View.connect(View.page(), SIGNAL(loadFinished(bool)), this, SLOT(LoadedTakeScreenShot(bool)));
    }
};

#endif // SCREENSHOTTER_H
