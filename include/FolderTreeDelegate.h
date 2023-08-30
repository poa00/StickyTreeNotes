#ifndef FOLDERTREEDELEGATE_H
#define FOLDERTREEDELEGATE_H

#include "FolderTreeItem.h"
#include <QAbstractItemView>
#include <QStyledItemDelegate>

class FolderTreeDelegate : public QStyledItemDelegate
{
    Q_OBJECT

  public:
    FolderTreeDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    enum DropIndicatorPosition
    {
        OnItem,
        AboveItem,
        BelowItem,
        OnViewport
    };
    void setIndicator(DropIndicatorPosition newIndicator);
    void setDropIndex(const QModelIndex &newDropIndex);

  private:
    void paintBackground(QPainter *painter, const QStyleOptionViewItem &option, const FolderTreeItem *item) const;
    void paintIcon(QPainter *painter, const QStyleOptionViewItem &option, const FolderTreeItem *item) const;
    void paintName(QPainter *painter, const QStyleOptionViewItem &option, const FolderTreeItem *item) const;
    void paintNotesInsideCount(QPainter *painter, const QStyleOptionViewItem &option, const FolderTreeItem *item) const;
    void paintBranchArrow(QPainter *painter, const QStyleOptionViewItem &option, const FolderTreeItem *item) const;

    QModelIndex dropIndex;
    DropIndicatorPosition dropIndicatorindicator;
};

#endif // FOLDERTREEDELEGATE_H
