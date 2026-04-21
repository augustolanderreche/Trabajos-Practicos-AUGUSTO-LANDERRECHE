#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>

class login : public QWidget
{

    Q_OBJECT

public:
    login();

private:
    QLabel *lUsuario, *lClave;
    QLineEdit *leUsuario, *leClave;
    QPushButton *pbIngresar;
    QGridLayout *layout;

private slots:
    void slot_validarUsuario();
};

#endif // LOGIN_H
