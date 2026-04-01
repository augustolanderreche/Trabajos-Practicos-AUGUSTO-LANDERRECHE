#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QString>
#include "datastore.h"
#include "sessionmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class login;
}
QT_END_NAMESPACE

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login() override;

signals:
    void loginSuccessful(const QString& username);

private slots:
    void on_pushButtonLogin_clicked();
    void on_pushButtonRegister_clicked();

private:
    Ui::login *ui;
    DataStore* dataStore;
    SessionManager* sessionManager;
};
#endif // LOGIN_H
