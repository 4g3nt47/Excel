/**
 * @file cell.h
 * @brief The Cell class that extends QTableWidgetItem. Nothing special in this case, but could be used
 * to extend the functionalities of the cells.
 */

#ifndef CELL_H
#define CELL_H

#include <QTableWidgetItem>

class Cell : public QTableWidgetItem{

  public:

    Cell();
    ~Cell();

};

#endif // CELL_H
