#ifndef GOTODIALOG_H
#define GOTODIALOG_H

#include <QDialog>
#include <QRegExp>
#include <QRegExpValidator>

namespace Ui {
  class GotoDialog;
}

class GotoDialog : public QDialog{

  Q_OBJECT

  public:

    explicit GotoDialog(QWidget *parent = nullptr);
    ~GotoDialog();

  private:

    Ui::GotoDialog *ui;

  signals:

    void execGoto(const QString &address);

};

#endif // GOTODIALOG_H
