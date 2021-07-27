#include "Database.h"

const QString Database::TABLE_COEFFS("coefficients");
const QString Database::TABLE_OPTIONS("options");
const QString Database::TABLE_PRICES("prices");
const QString Database::TABLE_PROFILES("profiles");

QSqlDatabase Database::getDatabase()
{
    QSettings settings(QDir::currentPath() + "/settings.ini", QSettings::IniFormat);
    QString pathToDatabase = settings.value("pathToDB").toString();
    return QSqlDatabase::database(pathToDatabase);
}

bool Database::initDatabase(const QString& DB_NAME)
{
    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DB_NAME);
    const bool ok = db.open();
    if (!ok)
    {
        QString error = db.lastError().text();
        QMessageBox::critical(0, "Ошибка", "Ошибка подключения к базе данных: " + error);
    }

    return ok;
}

int Database::getLastId(const QString& table)
{
    int lastId = -1;

    QSqlQuery query(getDatabase());
    query.exec("SELECT _id FROM " + table + " ORDER BY _id DESC LIMIT 1");

    if (query.first())
        lastId = query.value(0).toInt();

    return query.exec() ? lastId : -1;
}

std::vector<Profile> Database::getProfilesList()
{
    QSqlQuery query(getDatabase());
    query.exec("SELECT * FROM " + TABLE_PROFILES + " ORDER BY favorite DESC");
    query.last();
    int rowCount = query.at();
    
    std::vector<Profile> profilesList;
    if (rowCount != 0)
        profilesList.reserve(++rowCount);

    query.first();
    query.previous();

    while (query.next())
    {
        Profile profile(
            query.value(0).toInt(),
            query.value(1).toString(),
            query.value(2).toFloat(),
            query.value(3).toString(),
            query.value(4).toFloat(),
            query.value(5).toBool());
        profilesList.push_back(std::move(profile));
    }

    return profilesList;
}

bool Database::addProfile(const Profile& profile)
{
    QSqlQuery query(getDatabase());
    query.prepare("INSERT INTO " + TABLE_PROFILES + " (name, width, color, price, favorite)"
        "VALUES (:name, :width, :color, :price, :favorite);");
    query.bindValue(":name", profile.get_name());
    query.bindValue(":width", profile.get_width());
    query.bindValue(":color", profile.get_color());
    query.bindValue(":price", profile.get_price());
    query.bindValue(":favorite", profile.isFavorite());

    return query.exec();
}

bool Database::editProfile(const Profile& profile)
{
    QSqlQuery query(getDatabase());
    query.prepare("UPDATE " + TABLE_PROFILES +
        " SET name = ?, width = ?, color = ?, price = ?, favorite = ? " +
        " WHERE _id=" + QString::number(profile.get_id()));
    query.addBindValue(profile.get_name());
    query.addBindValue(profile.get_width());
    query.addBindValue(profile.get_color());
    query.addBindValue(profile.get_price());
    query.addBindValue(profile.isFavorite());

    return query.exec();
}

bool Database::setProfileFavorite(const int profileId, const bool favorite)
{
    QSqlQuery query(getDatabase());
    query.prepare("UPDATE " + TABLE_PROFILES +
        " SET favorite = ? " +
        " WHERE _id=" + QString::number(profileId));
    query.addBindValue(favorite);

    return query.exec();
}

Coefficients Database::getCoefficients()
{
    QSqlQuery query(getDatabase());
    query.exec("SELECT * FROM " + TABLE_COEFFS);

    query.first();

    Coefficients coefficients;
    coefficients.set_id(query.value(0).toInt());
    coefficients.set_netProfit(query.value(1).toFloat());
    coefficients.set_laborCosts(query.value(2).toFloat());
    coefficients.set_height_5_11(query.value(3).toFloat());
    coefficients.set_height_12_19(query.value(4).toFloat());
    coefficients.set_height_20_29(query.value(5).toFloat());
    coefficients.set_height_30_49(query.value(6).toFloat());
    coefficients.set_height_more50(query.value(7).toFloat());
    coefficients.set_advertAgency(query.value(8).toFloat());
    coefficients.set_otherClient(query.value(9).toFloat());

    return coefficients;
}

bool Database::setCoefficients(const Coefficients& coefficients)
{
    QSqlQuery query(getDatabase());
    query.prepare("UPDATE " + TABLE_COEFFS +
        " SET netProfit = ?, laborCosts = ?, height_5_11 = ?, height_12_19 = ?, height_20_29 = ?, " +
        " height_30_49 = ?, height_more50 = ?, advertisingAgency = ?, otherClient = ? ");
    query.first();
    query.addBindValue(coefficients.get_netProfit());
    query.addBindValue(coefficients.get_laborCosts());
    query.addBindValue(coefficients.get_height_5_11());
    query.addBindValue(coefficients.get_height_12_19());
    query.addBindValue(coefficients.get_height_20_29());
    query.addBindValue(coefficients.get_height_30_49());
    query.addBindValue(coefficients.get_height_more50());
    query.addBindValue(coefficients.get_advertAgency());
    query.addBindValue(coefficients.get_otherClient());

    return query.exec();
}

Prices Database::getPrices()
{
    QSqlQuery query(getDatabase());
    query.exec("SELECT * FROM " + TABLE_PRICES);

    query.first();

    return Prices(
        query.value(0).toInt(),
        query.value(1).toFloat(),
        query.value(2).toFloat(),
        query.value(3).toFloat());
}

bool Database::setPrices(const Prices& prices)
{
    QSqlQuery query(getDatabase());
    query.prepare("UPDATE " + TABLE_PRICES +
        " SET Acrylic = ?, Frezer = ?, PVC = ? ");
    query.first();
    query.addBindValue(prices.get_acrylic());
    query.addBindValue(prices.get_frezer());
    query.addBindValue(prices.get_pvc());

    return query.exec();
}

QString Database::getPassword()
{
    QSqlQuery query(getDatabase());
    query.exec("SELECT password FROM " + TABLE_OPTIONS);

    query.first();

    return QString(query.value(0).toString());
}

QString Database::getPassToReset()
{
    QSqlQuery query(getDatabase());
    query.exec("SELECT passwordToReset FROM " + TABLE_OPTIONS);

    query.first();

    return QString(query.value(0).toString());
}

bool Database::changePassword(const QString& newPassword)
{
    QSqlQuery query(getDatabase());
    query.prepare("UPDATE " + TABLE_OPTIONS +
        " SET password = ? ");
    query.first();
    query.addBindValue(newPassword);

    return query.exec();
}

bool Database::removeObject(const QString& table, const int id)
{
    QSqlQuery query(getDatabase());

    return query.exec("DELETE FROM " + table + " WHERE _id=" + QString::number(id));
}

