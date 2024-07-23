#include "gotodialog.h"
#include "ui_gotodialog.h"

GotoDialog::GotoDialog(QWidget *parent) : QDialog(parent), ui(new Ui::GotoDialog){

  ui->setupUi(this);
  ui->gotoButton->setEnabled(false);
  QRegExp re("[A-Za-z]{1}[1-9]{1}[0-9]{0,2}");
  ui->addressLineEdit->setValidator(new QRegExpValidator(re));
  connect(ui->addressLineEdit, &QLineEdit::textChanged, this, [&](){
    ui->gotoButton->setEnabled(ui->addressLineEdit->hasAcceptableInput());
  });
  connect(ui->gotoButton, &QPushButton::clicked, this, [&](){
    emit execGoto(ui->addressLineEdit->text());
    accept();
  });
  connect(ui->cancelButton, &QPushButton::clicked, this, &GotoDialog::reject);
}

GotoDialog::~GotoDialog(){
  delete ui;
}
