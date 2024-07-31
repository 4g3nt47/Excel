#include "cell.h"

Cell::Cell(){

}

Cell::~Cell(){

}

QTableWidgetItem *Cell::clone() const{

  // Clone the prototype cell to create a new cell.
  Cell *cell = new Cell();
  cell->setText(this->text());
  cell->setFlags(this->flags());
  return cell;
}
