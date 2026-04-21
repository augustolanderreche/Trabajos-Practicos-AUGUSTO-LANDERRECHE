#include "login.h"

login::login() {
    lUsuario = new QLabel ("Usuario");
    lClave = new QLabel ("Clave");
    leUsuario = new QLineEdit;
    leClave = new QLineEdit;
    leClave->setEchoMode(QLineEdit::Password);

    pbIngresar = new QPushButton("Ingresar");
    layout = new QGridLayout;

    layout->addWidget(lUsuario, 0, 0, 1, 1);
    layout->addWidget(lClave, 1, 0, 1, 1);
    layout->addWidget(leUsuario, 0, 1, 1, 2);
    layout->addWidget(leClave, 1, 1, 1, 2);
    layout->addWidget(pbIngresar, 2, 1, 1, 1);

    this->setLayout(layout);

    connect(pbIngresar, SIGNAL(pressed()), this, SLOT(slot_validarUsuario()));
    connect(leClave, SIGNAL(returnPressed()), this, SLOT(slot_validarUsuario()));


}

void login::slot_validarUsuario(){

    if(this->leUsuario->text() == "admin" && this->leClave->text() == "1234") {
     this->close();
    } else {
    this->leClave->clear();
    }

}