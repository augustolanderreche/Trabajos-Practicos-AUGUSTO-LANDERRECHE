#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CanvasModel;
class CanvasView;
class SyncManager;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSaveClicked();
    void onSyncError(const QString &message);

private:
    CanvasModel *m_model;
    CanvasView *m_view;
    SyncManager *m_sync;
};

#endif // MAINWINDOW_H