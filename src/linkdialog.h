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

signals:
    void linkCreated(QString link);

private:
    Ui::LinkDialog *ui;

private slots:
    void generateLink();

};

#endif // LINKDIALOG_H
