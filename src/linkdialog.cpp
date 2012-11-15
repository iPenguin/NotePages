/*********************************************************************************************\
    Note Pages - the note taking and cross reference application 
    Copyright (c) 2012 Brian C. Milco <bcmilco AT gmail DOT com>       

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; version 2
    of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
\*********************************************************************************************/
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

void LinkDialog::setLinkText(QString text)
{
    ui->linkText->setText(text);
}

void LinkDialog::generateLink()
{
    QStringList link;
    QString linkUrl;
    if(ui->pageBttn->isChecked()) {

        int index = ui->pageLink->currentIndex();
        QString pageNumber = ui->pageLink->itemData(index, Qt::UserRole).toString();

        linkUrl = "npage://" + pageNumber;
    } else {
        QString ulink = ui->urlLink->text().toLower();
        QUrl url = QUrl(ulink, QUrl::TolerantMode);
        linkUrl = url.toString();
        if(!linkUrl.contains(QRegExp("^[a-zA-Z].*://")))
            linkUrl.prepend("http://");
    }

    //pass along the pretty text and the url.
    link.append(ui->linkText->text());
    link.append(linkUrl);
    emit linkCreated(link);
}

