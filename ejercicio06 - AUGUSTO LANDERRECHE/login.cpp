#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);

    connect (ui->pbIngresar, SIGNAL(pressed()), this, SLOT(slot_validarUsuario()));
}

login::~login()
{
    delete ui;
}

void login::slot_validarUsuario(){

    if  (ui->leUsuario->text() == "admin" && ui->leClave->text() == "1234"){
        this->close();
    }else{
        ui->leClave->clear();
    }
}
