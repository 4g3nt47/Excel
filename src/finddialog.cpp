#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent) : QDialog(parent), ui(new Ui::FindDialog){

  ui->setupUi(this);
  ui->findButton->setEnabled(false);
  connect(ui->keywordLineEdit, &QLineEdit::textChanged, this, [&](const QString &kw){
    ui->findButton->setEnabled(!kw.isEmpty());
  });
  connect(ui->findButton, &QPushButton::clicked, this, [&](){
    emit execFind(ui->keywordLineEdit->text(), ui->csCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
    accept();
  });
  connect(ui->cancelButton, &QPushButton::clicked, this, &FindDialog::reject);
}

FindDialog::~FindDialog(){
  delete ui;
}
