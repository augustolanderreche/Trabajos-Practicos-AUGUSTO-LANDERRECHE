#include "noteseditor.h"
#include "ui_noteseditor.h"

NotesEditor::NotesEditor(const QString& notes, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NotesEditor)
{
    ui->setupUi(this);
    ui->textEditNotes->setPlainText(notes);
}

NotesEditor::~NotesEditor()
{
    delete ui;
}

QString NotesEditor::getNotes() const
{
    return ui->textEditNotes->toPlainText();
}