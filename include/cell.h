/**
 * @file cell.h
 * @brief The Cell class that extends QTableWidgetItem. Nothing special in this case, but could be used
 * to extend the functionalities of the cells. The way to use this is to pass a new instance to the
 * setPrototype() function of the QTableWidget class.
 */

#ifndef CELL_H
#define CELL_H

#include <QTableWidgetItem>

class Cell : public QTableWidgetItem{

  public:

    Cell();
    ~Cell();
    QTableWidgetItem *clone() const; // This will be called any time the QWidgetTable needs a new cell.

};

#endif // CELL_H
