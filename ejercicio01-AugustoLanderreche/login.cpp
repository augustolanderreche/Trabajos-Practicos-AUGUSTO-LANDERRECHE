#include "login.h"
#include "ui_login.h"
#include <QCryptographicHash>

login::login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login)
    , dataStore(new DataStore())
    , sessionManager(new SessionManager())
{
    ui->setupUi(this);
    connect(ui->pushButtonLogin, &QPushButton::clicked, this, &login::on_pushButtonLogin_clicked);
    connect(ui->pushButtonRegister, &QPushButton::clicked, this, &login::on_pushButtonRegister_clicked);

    QString style = R"(
        QWidget { background-color: #f4f6fb; }
        QLabel { color: #1e293b; }
        QLineEdit { background: #ffffff; color: #1e293b; border: 1px solid #cbd5e1; border-radius: 8px; padding: 6px; }
        QPushButton { background-color: #0f172a; color: #ffffff; border-radius: 8px; min-height: 32px; padding: 6px 12px; font-weight: 600; }
        QPushButton:hover { background-color: #1e293b; }
        QCheckBox { color: #334155; }
        QLabel#labelError { color: #dc2626; font-weight: 600; }
    )";
    this->setStyleSheet(style);
}

login::~login()
{
    delete ui;
    delete dataStore;
    delete sessionManager;
}

void login::on_pushButtonLogin_clicked()
{
    QString username = ui->lineEditUsername->text().trimmed();
    QString password = ui->lineEditPassword->text();

    if (username.isEmpty() || password.isEmpty()) {
        ui->labelError->setText("Usuario y contraseña son requeridos.");
        return;
    }

    QList<User> users = dataStore->loadUsers();
    QString hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

    bool authenticated = false;
    for (const User& user : users) {
        if (user.username == username && user.passwordHash == hash) {
            authenticated = true;
            break;
        }
    }

    if (authenticated) {
        if (ui->checkBoxRemember->isChecked()) {
            sessionManager->saveSession(username);
        }
        ui->labelError->setText("");
        emit loginSuccessful(username);
    } else {
        ui->labelError->setText("Usuario o contraseña incorrectos.");
    }
}

void login::on_pushButtonRegister_clicked()
{
    QString username = ui->lineEditUsername->text().trimmed();
    QString password = ui->lineEditPassword->text();

    if (username.isEmpty() || password.isEmpty()) {
        ui->labelError->setText("Usuario y contraseña son requeridos.");
        return;
    }

    if (password.length() < 4) {
        ui->labelError->setText("La contraseña debe tener al menos 4 caracteres.");
        return;
    }

    QList<User> users = dataStore->loadUsers();
    for (const User& user : users) {
        if (user.username == username) {
            ui->labelError->setText("El usuario ya existe.");
            return;
        }
    }

    User newUser;
    newUser.username = username;
    newUser.passwordHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
    users.append(newUser);
    dataStore->saveUsers(users);

    ui->labelError->setStyleSheet("color: green;");
    ui->labelError->setText("Usuario registrado correctamente. Inicia sesión.");
}
