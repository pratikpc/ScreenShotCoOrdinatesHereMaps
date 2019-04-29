#include <QApplication>

#include "Screenshot.hxx"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Provide a Path to Html File Provided in Repo Here
    // This Html File Is important for Finding the Location

    // Point SampleTest to Location where
    // You Would Want to Store the Image
    Screenshot screenshot(QStringLiteral("map.html"),QStringLiteral("SampleTest.jpg"));
    QJsonArray array;
    array.append(QJsonArray({28.644800,77.216721}));
    array.append(QJsonArray({28.664800,77.226721}));
    array.append(QJsonArray({28.654800,77.213721}));
    screenshot.Perform("q", array);


    return app.exec();
}
