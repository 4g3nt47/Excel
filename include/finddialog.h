#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

namespace Ui {
  class FindDialog;
}

class FindDialog : public QDialog{

  Q_OBJECT

  public:

    explicit FindDialog(QWidget *parent = nullptr);
    ~FindDialog();

  private:

    Ui::FindDialog *ui;

  signals:

    void execFind(QString keyword, Qt::CaseSensitivity cs = Qt::CaseInsensitive);

};

#endif // FINDDIALOG_H
