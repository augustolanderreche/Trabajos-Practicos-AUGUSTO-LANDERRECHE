#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "admindb.h"
#include "pintura.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(AdminDB *adminDB, QWidget *parent = nullptr);

private:
    Pintura *pintura;
    AdminDB *adminDB;
};

#endif // MAINWINDOW_H
