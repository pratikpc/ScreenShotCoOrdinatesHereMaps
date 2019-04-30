#include <QApplication>
#include <QDir>

#include "Screenshot.hxx"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Generate Html Path Directory vise to Map.html File
    // Assume in Current Working Directory
    QString const MapPath = QDir::currentPath() + "/" + "map.html";
    // Convert File path /home/file.txt to file Uri form
    QString const MapUri = QUrl::fromLocalFile(MapPath).toString();

    // Provide a Path to Html File Provided in Repo Here
    // This Html File Is important for Finding the Location
    // Point SampleTest to Location where
    // You Would Want to Store the Image
    Screenshot screenshot(MapUri,QStringLiteral("SampleTest.jpg"));
    QJsonArray array;
    array.append(QJsonArray({28.644800,77.216721}));
    array.append(QJsonArray({28.664800,77.226721}));
    array.append(QJsonArray({28.654800,77.213721}));
    screenshot.Perform("q", array);

    return app.exec();
}
