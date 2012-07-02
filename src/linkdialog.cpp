#include "linkdialog.h"
#include "ui_linkdialog.h"

#include <QString>
#include <QUrl>
#include <QDebug>

LinkDialog::LinkDialog(QTreeWidget *tree, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LinkDialog)
{
    ui->setupUi(this);
    connect(this, SIGNAL(accepted()), SLOT(generateLink()));
    generateBasketList(tree, 0, 0);

}

LinkDialog::~LinkDialog()
{
    delete ui;
}

void LinkDialog::generateBasketList(QTreeWidget *tree, QTreeWidgetItem *item, int indent)
{
    if(!item) {
        for(int i = 0; i < tree->topLevelItemCount(); ++i)
            generateBasketList(tree, tree->topLevelItem(i), indent);
    } else {

        //TODO: add some fancy deco stuff to make it look like a tree list.
        QString pad;
        int pageNumber = item->data(0, Qt::UserRole).toInt(); //page number.
        QString text = item->data(0, Qt::DisplayRole).toString();

        text.prepend(pad.fill(' ', indent *2));

        ui->pageLink->addItem(item->icon(0), text, QVariant(pageNumber));

        int subBasketCount = item->childCount();
        if(subBasketCount > 0) {
            indent++;
            for(int i = 0; i < subBasketCount; ++i) {
                generateBasketList(tree, item->child(i), indent);
            }
        }
    }
}

void LinkDialog::generateLink()
{

    QString link;
    if(ui->pageBttn->isChecked()) {

        int index = ui->pageLink->currentIndex();
        QString pageNumber = ui->pageLink->itemData(index, Qt::UserRole).toString();

        link = "npage://" + pageNumber;
    } else {
        QString ulink = ui->urlLink->text().toLower();
        QUrl url = QUrl(ulink, QUrl::TolerantMode);
        link = url.toString();
        if(!link.contains(QRegExp("^[a-zA-Z].*://")))
            link.prepend("http://");
    }
    emit linkCreated(link);
}

