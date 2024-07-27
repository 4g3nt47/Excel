#include "sortdialog.h"
#include "ui_sortdialog.h"

SortDialog::SortDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SortDialog){

  ui->setupUi(this);
  connect(ui->sortButton, &QPushButton::clicked, this, [&](){
    Qt::SortOrder order = ui->sortComboBox->currentText().startsWith("A") ? Qt::AscendingOrder : Qt::DescendingOrder;
    emit execSort(order);
    accept();
  });
  connect(ui->cancelButton, &QPushButton::clicked, this, &SortDialog::reject);
}

SortDialog::~SortDialog(){
  delete ui;
}
