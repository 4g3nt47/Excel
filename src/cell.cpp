#include "cell.h"
#include <QDebug>

Cell::Cell(){
  qDebug() << "Creating a new cell...";
}

Cell::~Cell(){
  qDebug() << "Destroying a cell...";
}

QTableWidgetItem *Cell::clone() const{

  // Clone the prototype cell to create a new cell.
  qDebug() << "Cloning a cell...";
  Cell *cell = new Cell();
  cell->setText(this->text());
  return cell;
}

