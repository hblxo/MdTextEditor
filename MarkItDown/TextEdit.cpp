#include "TextEdit.h"

TextEdit::TextEdit()
{

//    this->setContextMenuPolicy(Qt::CustomContextMenu);

//    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
//        this, SLOT(ShowContextMenu(const QPoint&)));

}

TextEdit::TextEdit(QWidget *parent)
{
    this->setParent(parent);
}

//void    TextEdit::ShowContextMenu(const QPoint& pos) // this is a slot
//{
//    // for most widgets
//    QPoint globalPos = this->mapToGlobal(pos);
//    // for QAbstractScrollArea and derived classes you would use:
//    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

//    QMenu myMenu;
//    myMenu.addAction(ui->actionCopy);
//    // ...

//    QAction* selectedItem = myMenu.exec(globalPos);
//    if (selectedItem)
//    {
//        // something was chosen, do stuff
//    }
//    else
//    {
//        // nothing was chosen
//    }
//}
