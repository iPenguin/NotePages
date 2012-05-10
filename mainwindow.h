#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDomElement>
#include <QTreeWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
//File
    void open();
    void closeFile();
    void quit();
    void save();

//Edit
    void copy();
    void cut();
    void paste();

    void loadFile(QString folder);

    void pageSelected(QTreeWidgetItem* page);

private:
    Ui::MainWindow *ui;

    void load();


    void saveFile(QString fileName);

    QTreeWidgetItem* loadPage(QDomElement element);

    void openPage(int pageNumber);

    void setupMenubars();

    QString m_wikiFile;
    QString m_name;
    QString mPath;
    QString mLastDirectory;
};

#endif // MAINWINDOW_H
