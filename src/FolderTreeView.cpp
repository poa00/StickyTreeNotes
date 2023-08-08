#include "FolderTreeView.h"

#include "FolderData.h"
#include "PersistenceManager.h"
#include <QMenu>
#include <QMessageBox>
#include <QModelIndex>

FolderTreeView::FolderTreeView(QWidget *parent) : QTreeView(parent), delegate(this)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, &FolderTreeView::customContextMenuRequested, this,
                     &FolderTreeView::onCustomContextMenuRequested);
    setStyleSheet("FolderTreeView{border: none; selection-color: transparent; selection-background-color: transparent;}"
                  "QTreeView::branch { background: transparent; } ");
    setItemDelegate(&delegate);
}

void FolderTreeView::setModel(QAbstractItemModel *model)
{
    QTreeView::setModel(model);
    if (this->model()->rowCount() > 0)
        setCurrentIndex(this->model()->index(0, 0));
}

int FolderTreeView::getCurrentFolderSelectedId() const
{
    FolderData *selectedFolder = static_cast<FolderData *>(currentIndex().internalPointer());
    if (selectedFolder)
        return selectedFolder->getId();
    else
        return SpecialFolderId::AllNotesFolder;
}

void FolderTreeView::onCustomContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = QTreeView::indexAt(pos);
    if (!(index.flags() & Qt::ItemIsEditable))
        index = QModelIndex(); // Treat uneditable item as when the mouse isn't over any item

    QMenu *menu = new QMenu(this);
    QAction *createSubfolder = new QAction(index.isValid() ? "Create subfolder" : "Create new folder");
    QObject::connect(createSubfolder, &QAction::triggered, this,
                     [this, index]() { this->model()->insertRow(0, index); });
    menu->addAction(createSubfolder);

    if (index.isValid())
    {
        QAction *deleteFolder = new QAction("Delete folder");
        QObject::connect(deleteFolder, &QAction::triggered, this, [this, index]() { this->deleteFolder(index); });
        menu->addAction(deleteFolder);

        QAction *renameFolder = new QAction("Rename folder");
        QObject::connect(renameFolder, &QAction::triggered, this, [this, index]() { this->edit(index); });
        menu->addAction(renameFolder);
    }

    menu->exec(mapToGlobal(pos));
}

void FolderTreeView::deleteFolder(const QModelIndex &index)
{
    auto reply =
        QMessageBox::question(this, "Delete folder?",
                              "Are you sure you want to delete this folder? All notes and subfolders will be removed.");
    if (reply == QMessageBox::No)
        return;

    model()->removeRow(index.row(), index.parent());
}

void FolderTreeView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    QTreeView::selectionChanged(selected, deselected);
    auto selectedIndexes = selected.indexes();
    if (selectedIndexes.size() > 0)
    {
        FolderData *selectedFolder = static_cast<FolderData *>(selectedIndexes[0].internalPointer());
        emit newFolderSelected(selectedFolder->getId());
    }
}
