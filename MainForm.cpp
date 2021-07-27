#include "MainForm.h"

bool MainForm::_admin = false;
std::vector<Profile> MainForm::_profilesList;
Coefficients MainForm::_coefficients;
Prices MainForm::_prices;
QComboBox* MainForm::_profilesCombobox;

MainForm::MainForm(const QString& pathToDatabase, QWidget *parent)
    : QMainWindow(parent)
    , _pathToDatabase(pathToDatabase)
{
    ui.setupUi(this);
    setMenuBar();

    initDataFromDB();
    connect(_setDatabaseAction, &QAction::triggered, this, &MainForm::setPathToDatabaseSlot);
    connect(_showPathToDBAction, &QAction::triggered, this, &MainForm::showPathToDatabaseSlot);
    connect(_pricesAction, &QAction::triggered, this, &MainForm::pricesFormSlot);
    connect(_coeffAction, &QAction::triggered, this, &MainForm::coefficientsFormSlot);
    connect(_profilesAction, &QAction::triggered, this, &MainForm::profilesFormSlot);
    connect(_changePassAction, &QAction::triggered, this, &MainForm::changePasswordSlot);
    connect(_resetPassAction, &QAction::triggered, this, &MainForm::resetPasswordSlot);
    connect(_userAction, &QAction::triggered, this, &MainForm::changeAccessRightsSlot);
    connect(_adminAction, &QAction::triggered, this, &MainForm::changeAccessRightsSlot);
    connect(_aboutAction, &QAction::triggered, this, &MainForm::showAboutSlot);
    connect(_quitAction, &QAction::triggered, this, &MainForm::quitSlot);

    ui.oneLettRBtn->setChecked(true);
    ui.otherClientRBtn->setChecked(true);
    setLineEdits();
    this->setMinimumSize(QSize(440, 747));
    this->resize(QSize(440, 747));
}

Profile* MainForm::findProfile(int profileId)
{
    auto it = std::find_if(_profilesList.begin(), _profilesList.end(), [&profileId](const Profile& profile) {return profile.get_id() == profileId; });
    return (it == _profilesList.end()) ? nullptr : &(*it);
}

void MainForm::setProfilesComboBox()
{
    int index = 0;
    for (const auto& profile : _profilesList)
    {
        QVariant var;
        var.setValue(profile.get_id());
        ui.profilesComboBox->addItem(profile.get_name());
        ui.profilesComboBox->setItemData(index, var);
        ++index;
    }
    
    ui.profilesComboBox->setCurrentIndex(-1);
}

void MainForm::setLineEdits()
{
    ui.perimeterLineEdit->setValidator(new QIntValidator());
    ui.squareLineEdit->setValidator(new QIntValidator());
    ui.widthLineEdit->setValidator(new QIntValidator());
    ui.heightLineEdit->setValidator(new QIntValidator());
}

bool MainForm::checkInputData()
{
    if (ui.profilesComboBox->currentIndex() == -1)
    {
        QMessageBox::warning(this, "Внимание!", "Не выбран профиль");
        return false;
    }
    else if (!ui.oneLettRBtn->isChecked() && !ui.someLettRBtn->isChecked())
    {
        QMessageBox::warning(this, "Внимание!", "Не указано количество букв");
        return false;
    }
    else if (ui.perimeterLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Внимание!", "Не введен периметр буквы");
        return false;
    }
    else if (ui.squareLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Внимание!", "Не введена площадь буквы");
        return false;
    }
    else if (ui.widthLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Внимание!", "Не введена ширина буквы");
        return false;
    }
    else if (ui.heightLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Внимание!", "Не введена высота буквы");
        return false;
    }
    else if (ui.widthLineEdit->text().toInt() > 2400 && ui.oneLettRBtn->isChecked())
    {
        QMessageBox::warning(this, "Внимание!", "Значение \"Ширина\" не должна превышать 2400 мм");
        return false;
    }
    else if (ui.heightLineEdit->text().toInt() > 1200)
    {
        QMessageBox::warning(this, "Внимание!", "Значение \"Высота\" не должна превышать 1200 мм");
        return false;
    }
    else if (!ui.adverRBtn->isChecked() && !ui.otherClientRBtn->isChecked())
    {
        QMessageBox::warning(this, "Внимание!", "Не указан клиент");
        return false;
    }

    return true;
}

void MainForm::calculate()
{
    if (!checkInputData())
        return;

    int perimeter = ui.perimeterLineEdit->text().toInt();
    int square = ui.squareLineEdit->text().toInt();
    int width = ui.widthLineEdit->text().toInt();
    int height = ui.heightLineEdit->text().toInt();
    float priceProfile = findProfile(ui.profilesComboBox->currentData().toInt())->get_price();
    float coeffLetterHeight;
    float acrylic = square * _acrThickness * _acrDensity; //стоимость акрила: площадь*толщина*плотность
    float sPVC = width * height; //площадь ПВХ

    if (ui.oneLettRBtn->isChecked() && height < width)
    {
        //меняем местами высоту и ширину
        height = ui.widthLineEdit->text().toInt();
        width = ui.heightLineEdit->text().toInt();
    }

    int heightSM = height / 10; //высота в сантиметрах

    //получаем коэффициент по высоте буквы
    if (heightSM < 5)
        coeffLetterHeight = 2;
    else if (heightSM >= 5 && heightSM <= 11)
        coeffLetterHeight = _coefficients.get_height_5_11();
    else if (heightSM >= 12 && heightSM <= 19)
        coeffLetterHeight = _coefficients.get_height_12_19();
    else if (heightSM >= 20 && heightSM <= 29)
        coeffLetterHeight = _coefficients.get_height_20_29();
    else if (heightSM >= 30 && heightSM <= 49)
        coeffLetterHeight = _coefficients.get_height_30_49();
    else if (heightSM >= 50)
        coeffLetterHeight = _coefficients.get_height_more50();
    else
        coeffLetterHeight = 0;

    //считаем стоимсоть по материалам
    float resultAcrylic = acrylic * (_prices.get_acrylic() / 1000); //цена за акрил (переводим в граммы)
    float resultFrezer = (perimeter * (_prices.get_frezer() / 1000)) * 3; //цена за фрезеровку (переводим в милиметры), 3 прохода фрезой
    float frezerBackdrop = perimeter * (_prices.get_frezer() / 1000); //стоимость фрезеровки задника (переводим в милиметры)
    float resultPVC = sPVC * (_prices.get_pvc() / 1000000); //цена за ПВХ (переводим в кв. милиметры)
    float pvcBackdrop = resultPVC; //стоимость ПВХ задника
    float resultProfile = perimeter * (priceProfile / 1000); //стоимсоть по профилю (переводим в милиметры)
    float totalResult = resultProfile + resultAcrylic + resultFrezer + resultPVC; //сумма стоимостей материалов
    totalResult = totalResult * coeffLetterHeight * _coefficients.get_laborCosts() * _coefficients.get_netProfit(); //cумма с коэффициентами

    //коэффициент по заказчику
    if (ui.adverRBtn->isChecked())
        totalResult = (totalResult * _coefficients.get_advertAgency()) / 100;
    else if (ui.otherClientRBtn->isChecked())
        totalResult = (totalResult * _coefficients.get_otherClient()) / 100;

    if (ui.backdropCheckBox->isChecked())
    {
        resultFrezer = resultFrezer + frezerBackdrop; //добавляем к цене за фрезеровку еще и фрезеровку задника
        resultPVC = resultPVC + pvcBackdrop; //к цене за ПВХ добавляем ПВХ задника
        totalResult = totalResult + frezerBackdrop + pvcBackdrop; //добавляем к общей цене цены задников
    }

    float resultMaterial = resultProfile + resultAcrylic + resultFrezer + resultPVC; //сумма за материалы

    ui.resultLineEdit->setText("Общая стоимость: " + QString::number(totalResult, 'f', 2) + " рублей");
    ui.coeffValueLabel->setText(QString::number(coeffLetterHeight, 'f', 2));
    ui.sideValueLabel->setText(QString::number(resultProfile, 'f', 2));
    ui.acrylicValueLabel->setText(QString::number(resultAcrylic, 'f', 2));
    ui.millingValueLabel->setText(QString::number(resultFrezer, 'f', 2));
    ui.pvcValueLabel->setText(QString::number(resultPVC, 'f', 2));
    ui.totalMaterialCostValueLabel->setText(QString::number(resultMaterial, 'f', 2));
}

void MainForm::initDataFromDB()
{
    Database::initDatabase(_pathToDatabase);
    _profilesList = Database::getProfilesList();
    _coefficients = Database::getCoefficients();
    _prices = Database::getPrices();
    _password = Database::getPassword();
    _passToReset = Database::getPassToReset();
    if (_password == _passToReset)
        ui.changePassAction->setText("Установить пароль");
    _profilesCombobox = ui.profilesComboBox;
    setProfilesComboBox();
}

void MainForm::setMenuBar()
{
    _userAction->setCheckable(true);
    _userAction->setChecked(true);
    _adminAction->setCheckable(true);

    QActionGroup* accessGroup = new QActionGroup(this);
    accessGroup->addAction(_userAction);
    accessGroup->addAction(_adminAction);
    
    _optionMenu->addAction(_refreshAction);
    _optionMenu->addMenu(_databaseMenu);
    _optionMenu->addSeparator();
    _optionMenu->addAction(_quitAction);
    _databaseMenu->addAction(_setDatabaseAction);
    _databaseMenu->addAction(_showPathToDBAction);
    _accessMenu->addMenu(_passwordMenu);
    _accessMenu->addMenu(_accessRightsMenu);
    _passwordMenu->addAction(_changePassAction);
    _passwordMenu->addAction(_resetPassAction);
    _accessRightsMenu->addAction(_userAction);
    _accessRightsMenu->addAction(_adminAction);
    ui.menuBar->addMenu(_optionMenu);
    ui.menuBar->addAction(_pricesAction);
    ui.menuBar->addAction(_coeffAction);
    ui.menuBar->addAction(_profilesAction);
    ui.menuBar->addMenu(_accessMenu);
    ui.menuBar->addAction(_aboutAction);
}

void MainForm::pricesFormSlot()
{
    PricesForm* pricesForm = new PricesForm;
    pricesForm->setWindowFlags( Qt::MSWindowsFixedSizeDialogHint);
    pricesForm->setWindowTitle("Цены");
    pricesForm->setWindowModality(Qt::WindowModality::ApplicationModal);
    pricesForm->setAttribute(Qt::WA_DeleteOnClose);
    if (!_admin)
        pricesForm->saveButton->setVisible(false);
    pricesForm->show();
}

void MainForm::coefficientsFormSlot()
{
    CoefficientsForm* coeffForm = new CoefficientsForm;
    coeffForm->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    coeffForm->setWindowTitle("Коэффициенты");
    coeffForm->setWindowModality(Qt::WindowModality::ApplicationModal);
    coeffForm->setAttribute(Qt::WA_DeleteOnClose);
    if (!_admin)
        coeffForm->saveButton->setVisible(false);
    coeffForm->show();
}

void MainForm::profilesFormSlot()
{
    ProfilesForm* profilesForm = new ProfilesForm;
    profilesForm->setWindowTitle("Профили");
    profilesForm->setWindowModality(Qt::WindowModality::ApplicationModal);
    profilesForm->setAttribute(Qt::WA_DeleteOnClose);
    profilesForm->show();
}

void MainForm::calcButtonSlot()
{
    calculate();
}

void MainForm::refreshSlot()
{
    _profilesList = Database::getProfilesList();
    ui.profilesComboBox->clear();
    setProfilesComboBox();
}

void MainForm::squareLineEditChangedSlot()
{
    float acrylic = ui.squareLineEdit->text().toFloat() * 0.4 * 1.2;
    ui.acrylicLabel->setText(QString::number(acrylic) + " гр");
    if (ui.squareLineEdit->text().isEmpty())
        ui.acrylicLabel->clear();
}

void MainForm::widthHeightLineEditsChangeSlot()
{
    if (!ui.widthLineEdit->text().isEmpty() && !ui.heightLineEdit->text().isEmpty())
    {
        float sPVC = ui.widthLineEdit->text().toFloat() * ui.heightLineEdit->text().toFloat();
        ui.sPVCLabel->setText(QString::number(sPVC / 100) + " кв. см");
    }
    else
        ui.sPVCLabel->clear();
}

void MainForm::changeAccessRightsSlot()
{
    if (_adminAction->isChecked())
    {
        bool ok;
        QString inputPass = QInputDialog::getText(this, "Аутентификация",
            "Введите пароль", QLineEdit::Password, nullptr, &ok);

        if (ok && !inputPass.isEmpty())
        {
            if (inputPass == _password)
                _admin = true;
            else
                QMessageBox::warning(this, "Внимание", "Неверный пароль");
        }
        else
            _admin = false;
    }
    else if (_userAction->isChecked())
        _admin = false;

    if (_admin)
    {
        this->setMinimumSize(QSize(440, 747));
        this->resize(QSize(440, 747));
        ui.detailsGroupBox->setVisible(true);
        _adminAction->setChecked(true);
    }
    else
    {
        _userAction->setChecked(true);
    }
}

void MainForm::resetPasswordSlot()
{
    bool ok;
    QString inputSpecPass = QInputDialog::getText(this, "Сброс пароля",
        "Введите спец. пароль", QLineEdit::Password, nullptr, &ok);

    if (inputSpecPass.isEmpty())
        return;

    if (inputSpecPass == _passToReset)
    {
        if (Database::changePassword(_passToReset))
        {
            _password = _passToReset;
            ui.changePassAction->setText("Установить пароль");
            QMessageBox::information(this, "Сброс пароля", "Пароль успешно сброшен");
        }
    }
    else
        QMessageBox::warning(this, "Внимание", "Неверный пароль");
}

void MainForm::changePasswordSlot()
{
    ChangePassDialog dialog;

    if (_password == _passToReset)
    {
        dialog.setWindowTitle("Установка нового пароля");
        dialog.oldPassLabel->setVisible(false);
        dialog.oldPassLineEdit->setVisible(false);
        if (dialog.exec())
        {
            if (dialog.newPassLineEdit->text() == dialog.repeatNewPassLineEdit->text())
            {
                if (Database::changePassword(dialog.newPassLineEdit->text()))
                {
                    _password = dialog.newPassLineEdit->text();
                    QMessageBox::information(this, "Установка нового пароля", "Новый пароль успешно установлен");
                    ui.changePassAction->setText("Сменить пароль");
                }
            }
            else
                QMessageBox::warning(this, "Внимание", "Неверный повторный ввод нового пароля");
        }
    }
    else
    {
        dialog.setWindowTitle("Смена пароля");
        if (dialog.exec())
        {
            if (dialog.oldPassLineEdit->text() == _password)
            {
                if (dialog.newPassLineEdit->text() == dialog.repeatNewPassLineEdit->text())
                {
                    if (Database::changePassword(dialog.newPassLineEdit->text()))
                    {
                        _password = dialog.newPassLineEdit->text();
                        QMessageBox::information(this, "Смена пароля", "Пароль успешно изменен");
                    }
                }
                else
                    QMessageBox::warning(this, "Внимание", "Неверный повторный ввод нового пароля");
            } 
            else
                QMessageBox::warning(this, "Внимание", "Неверный старый пароль");
        }   
    }
}

void MainForm::setPathToDatabaseSlot()
{
    QString path = QFileDialog::getOpenFileName(this,
        tr("Указать базу данных"),
        QDir::currentPath(),
        tr("Database(*.db)"));
    if (path.isEmpty())
        return;

    QSettings settings(QDir::currentPath() + "/settings.ini", QSettings::IniFormat);
    settings.setValue("pathToDB", path);
    _pathToDatabase = path;
    Database::initDatabase(path);
    initDataFromDB();
}

void MainForm::showPathToDatabaseSlot()
{
    if (_pathToDatabase.isEmpty())
        QMessageBox::information(this, "Текущий путь к базе данных", "База данных не указана");
    else
        QMessageBox::information(this, "Текущий путь к базе данных", _pathToDatabase);
}

void MainForm::showAboutSlot()
{
    AboutForm aboutForm;
    aboutForm.setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    aboutForm.setWindowTitle("О программе");
    aboutForm.setWindowModality(Qt::WindowModality::ApplicationModal);
    aboutForm.exec();
}

void MainForm::printTechTaskSlot()
{
    QString pathToImage = QFileDialog::getOpenFileName(
        this,
        "Выберите изображение",
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
        "Images(*.jpg *.png *.jpeg *.bmp)");
    if (pathToImage.isEmpty())
        return;

    PrintTechTaskForm* printForm = new PrintTechTaskForm(pathToImage);
    printForm->setWindowModality(Qt::WindowModality::ApplicationModal);
    printForm->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    printForm->setWindowTitle("Печать технического задания");
    printForm->setData(
        findProfile(ui.profilesComboBox->currentData().toInt()), 
        ui.backdropCheckBox->isChecked(), 
        ui.otherClientRBtn->isChecked());

    QPixmap pixmap(pathToImage);
    QPixmap scaled_img = pixmap.scaled(this->width(), this->height(), Qt::KeepAspectRatio);
    double divider = 1;
    while (scaled_img.height() > 350)
    {
        divider += 0.2;
        scaled_img = pixmap.scaled(this->width() / divider, this->height() / divider, Qt::KeepAspectRatio);
    }

    printForm->image_lbl->setPixmap(scaled_img);
    printForm->show();
}

void MainForm::quitSlot()
{
    close();
}
