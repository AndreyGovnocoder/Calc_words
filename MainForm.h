#pragma once

#include <QtWidgets/QMainWindow>
#include <QInputDialog>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include "ui_MainForm.h"
#include "Profile.h"
#include "Database.h"
#include "ProfilesForm.h"
#include "CoefficientsForm.h"
#include "PricesForm.h"
#include "ChangePassDialog.h"
#include "AboutForm.h"
#include "PrintTechTaskForm.h"

class MainForm : public QMainWindow
{
    Q_OBJECT

public:
    MainForm(const QString& pathToDatabase, QWidget *parent = Q_NULLPTR);

    static std::vector<Profile> _profilesList;
    static Coefficients _coefficients;
    static Prices _prices;
    static Profile* findProfile(int profileId);
    static QComboBox* _profilesCombobox;
    static bool _admin;
    QString _pathToDatabase;

private:
    Ui::MainFormClass ui;
    
    QAction* _refreshAction = new QAction("Обновить",this);
    QAction* _setDatabaseAction = new QAction("Указать базу данных", this);
    QAction* _showPathToDBAction = new QAction("Текущий путь к базе данных", this);
    QAction* _pricesAction = new QAction("Цены", this);
    QAction* _coeffAction = new QAction("Коэффициенты", this);
    QAction* _profilesAction = new QAction("Профили", this);
    QAction* _changePassAction = new QAction("Сменить пароль", this);
    QAction* _resetPassAction = new QAction("Сбросить пароль", this);
    QAction* _userAction = new QAction("Пользователь", this);
    QAction* _adminAction = new QAction("Администратор", this);
    QAction* _aboutAction = new QAction("О программе", this);
    QAction* _quitAction = new QAction("Выход", this);
    QMenu* _optionMenu = new QMenu("Опции", this);
    QMenu* _databaseMenu = new QMenu("База данных", this);
    QMenu* _accessMenu = new QMenu("Безопасность", this);
    QMenu* _passwordMenu = new QMenu("Пароль", this);
    QMenu* _accessRightsMenu = new QMenu("Уровень доступа", this);
    const float _acrThickness = 0.4; //толщина акрила
    const float _acrDensity = 1.2; //плотность акрила
    QString _password;
    QString _passToReset;

    void initDataFromDB();
    void setProfilesComboBox();
    void setLineEdits();
    bool checkInputData();
    void calculate();
    void setMenuBar();
    
private slots:
    void pricesFormSlot();
    void coefficientsFormSlot();
    void profilesFormSlot();
    void calcButtonSlot();
    void refreshSlot();
    void squareLineEditChangedSlot();
    void widthHeightLineEditsChangeSlot();
    void changeAccessRightsSlot();
    void resetPasswordSlot();
    void changePasswordSlot();
    void setPathToDatabaseSlot();
    void showPathToDatabaseSlot();
    void showAboutSlot();
    void printTechTaskSlot();
    void quitSlot();
};
