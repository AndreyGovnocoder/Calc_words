#include "MainForm.h"
#include <QtWidgets/QApplication>
#include "Database.h"

QString _pathToDatabase;

bool checkDatabase()
{
    if (!QFile(QDir::currentPath() + "/settings.ini").exists())
    {
        if (!QFile(QDir::currentPath() + "/db/calcWords_database.db").exists())
            return false;
    }
    else
    {
        QSettings settings(QDir::currentPath() + "/settings.ini", QSettings::IniFormat);
        _pathToDatabase = settings.value("pathToDB").toString();
        if (QFile(_pathToDatabase).exists())
            return true;
    }

    _pathToDatabase = QDir::currentPath() + "/db/calcWords_database.db";
    if (!QFile(_pathToDatabase).exists())
        return false;

    QSettings settings(QDir::currentPath() + "/settings.ini", QSettings::IniFormat);
    settings.setValue("pathToDB", _pathToDatabase);

    return true;
}

bool setPathToDatabase()
{
    QString path = QFileDialog::getOpenFileName(0,
        "Указать базу данных",
        QDir::currentPath(),
        "Database(*.db)");
    if (path.isEmpty())
        return false;

    QSettings settings(QDir::currentPath() + "/settings.ini", QSettings::IniFormat);
    settings.setValue("pathToDB", path);
    _pathToDatabase = path;
    Database::initDatabase(path);
    return true;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Goose");
    QCoreApplication::setOrganizationDomain("Elizgerd@yandex.ru");
    QCoreApplication::setApplicationName("Calc_words");

    if (!checkDatabase())
    {
        QMessageBox::warning(0, "Внимание", "Отсутствует доступ к базе данных\nУкажите путь к базе данных");
        if (!setPathToDatabase())
            return 0;
    }
    
    MainForm mainForm(_pathToDatabase);
    mainForm.setWindowTitle("Калькулятор букв");
    mainForm.show();
    return app.exec();
}
