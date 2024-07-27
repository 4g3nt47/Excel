/**
 * @file mainwindow.h
 * @brief The application's main window.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QFont>
#include <QDebug>
#include <QMainWindow>
#include <QCloseEvent>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QIcon>
#include <QAction>
#include <QLabel>
#include "excel.h"
#include "finddialog.h"
#include "gotodialog.h"
#include "sortdialog.h"

class MainWindow : public QMainWindow{

  Q_OBJECT

  private:

    QAction *openAction, *newAction, *saveAction, *saveAsAction, *exitAction;
    QAction *copyAction, *pasteAction, *cutAction, *deleteAction, *findAction, *gotoAction, *sortAction;
    Excel *excel;
    QLabel *addressLabel, *valueLabel;
    FindDialog *findDialog;
    GotoDialog *gotoDialog;
    SortDialog *sortDialog;

  public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateStatusBar();

  public slots:

    void showFindDialog();
    void showGotoDialog();
    void showSortDialog();
    void showStatusMessage(const QString &msg, int duration = 2000);

  protected:

    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
