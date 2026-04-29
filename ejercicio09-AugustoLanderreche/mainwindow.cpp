#include "mainwindow.h"
#include <QStatusBar>

MainWindow::MainWindow(AdminDB *adminDB, QWidget *parent)
    : QMainWindow(parent)
    , adminDB(adminDB)
{
    setWindowTitle("Pintura - Ejercicio 09");
    resize(900, 650);

    pintura = new Pintura(adminDB, this);
    setCentralWidget(pintura);

    connect(pintura, &Pintura::estadoCambiado,
            this, [this](const QString &msg){ statusBar()->showMessage(msg); });

    pintura->cargarTrazosDB();

    statusBar()->showMessage(
        "R=Rojo  G=Verde  B=Azul  Rueda=Grosor  Esc=Borrar  Ctrl+Z=Deshacer"
    );
}
