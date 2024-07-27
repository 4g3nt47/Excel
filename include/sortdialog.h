#ifndef SORTDIALOG_H
#define SORTDIALOG_H

#include <QDialog>

namespace Ui {
  class SortDialog;
}

class SortDialog : public QDialog{

  Q_OBJECT

  public:

    explicit SortDialog(QWidget *parent = nullptr);
    ~SortDialog();

  private:

    Ui::SortDialog *ui;

  signals:

    void execSort(Qt::SortOrder order);

};

#endif // SORTDIALOG_H
