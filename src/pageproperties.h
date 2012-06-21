#ifndef PAGEPROPERTIES_H
#define PAGEPROPERTIES_H

#include <QWidget>

namespace Ui {
class PageProperties;
}

class PageProperties : public QWidget
{
    Q_OBJECT
    
public:
    explicit PageProperties(QWidget *parent = 0);
    ~PageProperties();
    
private:
    Ui::PageProperties *ui;
};

#endif // PAGEPROPERTIES_H
