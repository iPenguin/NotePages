/********************************************************\
| Copyright (c) 2012 Brian C. Milco <bcmilco@gmail.com>  |
\********************************************************/
#ifndef SETTINGSUI_H
#define SETTINGSUI_H

#include <QDialog>

class QAbstractButton;
class QPixmap;

namespace Ui {
    class SettingsDialog;
}

class SettingsUi : public QDialog
{
    Q_OBJECT
public:
    SettingsUi(QWidget* parent);
    ~SettingsUi();

    int exec();
public slots:
    void selectFolder();

private slots:

    void buttonClicked(QAbstractButton* button);

private:

    //TODO: Using the same code in MainWindow
    QPixmap drawColorBox(QColor color, QSize size);

    void setupDialogWidgets();
    void resetDialogWidgets();
    void saveDialogWidgets();
    
    void load(QObject* w);
    void loadDefualt(QObject* w);
    void save(QObject* w);

    bool isSettingsWidget(QObject* obj);
    
    Ui::SettingsDialog* ui;

    QColor mPrimaryColor;
    QColor mAlternateColor;
    QColor mDotColor;
    QColor mKeywordColor;
    
};

#endif //SETTINGSUI_H
