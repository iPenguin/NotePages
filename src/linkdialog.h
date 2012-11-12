/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef LINKDIALOG_H
#define LINKDIALOG_H

#include <QDialog>
#include <QTreeWidget>

namespace Ui {
class LinkDialog;
}

class LinkDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LinkDialog(QTreeWidget *tree, QWidget *parent = 0);
    ~LinkDialog();
    
    QString linkText();
    void generateBasketList(QTreeWidget *tree, QTreeWidgetItem *item, int indent);

    void setLinkText(QString text);

signals:
    void linkCreated(QStringList link);

private:
    Ui::LinkDialog *ui;

private slots:
    void generateLink();

};

#endif // LINKDIALOG_H
