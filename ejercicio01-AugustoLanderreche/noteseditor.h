#ifndef NOTESEDITOR_H
#define NOTESEDITOR_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class NotesEditor;
}
QT_END_NAMESPACE

class NotesEditor : public QDialog
{
    Q_OBJECT

public:
    explicit NotesEditor(const QString& notes, QWidget *parent = nullptr);
    ~NotesEditor() override;

    QString getNotes() const;

private:
    Ui::NotesEditor *ui;
};

#endif // NOTESEDITOR_H