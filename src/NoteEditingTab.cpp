#include "NoteEditingTab.h"
#include "PersistenceManager.h"
#include "ui_NoteEditingTab.h"
#include <QMessageBox>
#include <QShortcut>

NoteEditingTab::NoteEditingTab(QWidget *parent) : QWidget(parent), ui(new Ui::NoteEditingTab)
{
    ui->setupUi(this);
    QObject::connect(ui->saveAndReturn, &QPushButton::clicked, this, &NoteEditingTab::saveNoteIfChanged);
    QObject::connect(ui->saveAndReturn, &QPushButton::clicked, this,
                     [this]() { emit exitEditingNote(*currentEditingNote); });
    QObject::connect(ui->deleteButton, &QPushButton::clicked, this, &NoteEditingTab::onDeleteNoteButtonPressed);
    QObject::connect(ui->returnWithoutSavingButton, &QPushButton::clicked, this,
                     &NoteEditingTab::onReturnWithoutSavingButtonPressed);
    QObject::connect(ui->saveButton, &QPushButton::clicked, this, &NoteEditingTab::saveNoteIfChanged);
    QObject::connect(new QShortcut(QKeySequence(QKeySequence::Save), this), &QShortcut::activated, this,
                     &NoteEditingTab::saveNoteIfChanged);
    QObject::connect(new QShortcut(QKeySequence(Qt::Key_Escape), this), &QShortcut::activated, this,
                     &NoteEditingTab::onReturnWithoutSavingButtonPressed);
}

NoteEditingTab::~NoteEditingTab()
{
    delete ui;
}

void NoteEditingTab::startEditingNewNote(NoteData *note)
{
    currentEditingNote = note;
    ui->titleEdit->setText(note->getTitle());
    ui->contentEdit->setText(note->getContent());
    lastSavedTitle = note->getTitle();
    lastSavedContent = note->getContent();
}

void NoteEditingTab::saveNoteIfChanged()
{
    if (!hasNoteChanged())
        return;

    currentEditingNote->setTitle(ui->titleEdit->text());
    currentEditingNote->setContent(ui->contentEdit->toPlainText());
    lastSavedTitle = currentEditingNote->getTitle();
    lastSavedContent = currentEditingNote->getContent();
    emit saveNote(*currentEditingNote);
}

bool NoteEditingTab::hasNoteChanged()
{
    if (lastSavedTitle == ui->titleEdit->text() and lastSavedContent == ui->contentEdit->toPlainText())
        return false;

    return true;
}

void NoteEditingTab::onDeleteNoteButtonPressed()
{
    auto reply = QMessageBox::question(this, "Delete note?", "Are you sure you want to delete this note?");
    if (reply == QMessageBox::No)
        return;
    emit deleteNote(*currentEditingNote);
}

void NoteEditingTab::onReturnWithoutSavingButtonPressed()
{
    if (!hasNoteChanged())
    {
        emit exitEditingNote(*currentEditingNote);
        return;
    }

    auto reply =
        QMessageBox::question(this, "Discard changes?", "Are you sure you want to discard unsaved changes and return?",
                              QMessageBox::Yes | QMessageBox::Cancel | QMessageBox::Save);

    if (reply == QMessageBox::Yes)
        emit exitEditingNote(*currentEditingNote);

    if (reply == QMessageBox::Save)
    {
        saveNoteIfChanged();
        emit exitEditingNote(*currentEditingNote);
    }
}
