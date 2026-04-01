#include <QApplication>
#include "monitoringdashboard.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MonitoringDashboard dashboard;
    dashboard.show();

    return app.exec();
}
