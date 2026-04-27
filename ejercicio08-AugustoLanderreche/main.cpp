#include <QApplication>

#include "appconfig.h"
#include "appcontroller.h"
#include "eventlogger.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    const AppConfig config = AppConfig::cargar();
    EventLogger logger(config.rutaLog);
    logger.log("Aplicacion iniciada en modo offline");

    AppController controlador(config, logger);
    controlador.iniciar();

    return app.exec();
}
