#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){

  excel = new Excel(250, 26, nullptr, nullptr, this);
  connect(excel, &Excel::updateWindowTitle, this, &MainWindow::setWindowTitle);
  connect(excel, &Excel::documentModified, this, &MainWindow::setWindowModified);
  connect(excel, &Excel::showStatusMessage, this, &MainWindow::showStatusMessage);
  connect(excel, &Excel::currentCellChanged, this, &MainWindow::updateStatusBar);
  connect(excel, &Excel::itemChanged, this, &MainWindow::updateStatusBar);

  openAction = new QAction(tr("&Open"), this);
  openAction->setIcon(QIcon(":/images/open.png"));
  openAction->setShortcut(QKeySequence::Open);
  openAction->setStatusTip(tr("Open a file"));
  connect(openAction, &QAction::triggered, excel, &Excel::openFile);

  newAction = new QAction(tr("&New"), this);
  newAction->setIcon(QIcon(":/images/new.png"));
  newAction->setShortcut(QKeySequence::New);
  newAction->setStatusTip(tr("Create a new file"));
  connect(newAction, &QAction::triggered, excel, &Excel::newFile);

  saveAction = new QAction(tr("&Save"), this);
  saveAction->setIcon(QIcon(":/images/save.png"));
  saveAction->setShortcut(QKeySequence::Save);
  saveAction->setStatusTip(tr("Save file"));
  connect(saveAction, &QAction::triggered, excel, &Excel::saveFile);

  saveAsAction = new QAction(tr("Save as"), this);
  saveAsAction->setIcon(QIcon(":/images/save.png"));
  saveAsAction->setShortcut(QKeySequence::SaveAs);
  saveAsAction->setStatusTip(tr("Save file as"));
  connect(saveAsAction, &QAction::triggered, excel, &Excel::saveFileAs);

  exitAction = new QAction(tr("&Exit"), this);
  exitAction->setIcon(QIcon(":/images/exit.png"));
  exitAction->setShortcut(QKeySequence::Close);
  exitAction->setStatusTip(tr("Exit the application"));
  connect(exitAction, &QAction::triggered, this, &MainWindow::close);

  copyAction = new QAction(tr("&Copy"), this);
  copyAction->setIcon(QIcon(":/images/copy.png"));
  copyAction->setShortcut(QKeySequence::Copy);
  copyAction->setStatusTip(tr("Copy selection to clipboard"));
  connect(copyAction, &QAction::triggered, excel, &Excel::copy);

  pasteAction = new QAction(tr("Paste"), this);
  pasteAction->setIcon(QIcon(":/images/paste.png"));
  pasteAction->setShortcut(QKeySequence::Paste);
  pasteAction->setStatusTip(tr("Paste data from clipboard"));
  connect(pasteAction, &QAction::triggered, excel, &Excel::paste);

  cutAction = new QAction(tr("Cut"), this);
  cutAction->setIcon(QIcon(":/images/cut.png"));
  cutAction->setShortcut(QKeySequence::Cut);
  cutAction->setStatusTip(tr("Cut selection to clipboard"));
  connect(cutAction, &QAction::triggered, excel, &Excel::cut);

  deleteAction = new QAction(tr("&Delete"), this);
  deleteAction->setIcon(QIcon(":/images/delete.png"));
  deleteAction->setShortcut(QKeySequence::Delete);
  deleteAction->setStatusTip("Delete selected cells");
  connect(deleteAction, &QAction::triggered, excel, &Excel::deleteSelected);

  findAction = new QAction(tr("&Find"), this);
  findAction->setIcon(QIcon(":/images/find.png"));
  findAction->setShortcut(QKeySequence::Find);
  findAction->setStatusTip("Locate a keyword");
  connect(findAction, &QAction::triggered, this, &MainWindow::showFindDialog);

  gotoAction = new QAction(tr("&Goto"), this);
  gotoAction->setIcon(QIcon(":/images/goto.png"));
  gotoAction->setShortcut(tr("Ctrl+G"));
  gotoAction->setStatusTip(tr("Jump to a cell adrress"));
  connect(gotoAction, &QAction::triggered, this, &MainWindow::showGotoDialog);

  sortAction = new QAction(tr("Sort"));
  sortAction->setIcon(QIcon(":/images/sort.png"));
  sortAction->setStatusTip(tr("Sort selected rows"));
  connect(sortAction, &QAction::triggered, this, &MainWindow::showSortDialog);

  QAction *aboutAction = new QAction(tr("&About"));
  aboutAction->setStatusTip(tr("About Excel"));
  connect(aboutAction, &QAction::triggered, excel, &Excel::about);

  QAction *aboutQt = new QAction(tr("About Qt"));
  aboutQt->setStatusTip(tr("About the Qt framework"));
  connect(aboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);

  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAction);
  fileMenu->addAction(openAction);
  fileMenu->addAction(saveAction);
  fileMenu->addAction(saveAsAction);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAction);

  QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
  editMenu->addAction(cutAction);
  editMenu->addAction(copyAction);
  editMenu->addAction(pasteAction);
  editMenu->addAction(deleteAction);
  editMenu->addSeparator();
  editMenu->addAction(sortAction);
  editMenu->addSeparator();
  editMenu->addAction(findAction);
  editMenu->addAction(gotoAction);

  QMenu *aboutMenu = menuBar()->addMenu(tr("About"));
  aboutMenu->addAction(aboutAction);
  aboutMenu->addAction(aboutQt);

  QToolBar *fileToolBar = addToolBar(tr("&File"));
  fileToolBar->addAction(newAction);
  fileToolBar->addAction(openAction);
  fileToolBar->addAction(saveAction);

  QToolBar *editToolBar = addToolBar(tr("&Edit"));
  editToolBar->addAction(cutAction);
  editToolBar->addAction(copyAction);
  editToolBar->addAction(pasteAction);
  editToolBar->addAction(deleteAction);
  editToolBar->addSeparator();
  editToolBar->addAction(sortAction);
  editToolBar->addSeparator();
  editToolBar->addAction(findAction);
  editToolBar->addAction(gotoAction);

  addressLabel = new QLabel(" wwww ", this);
  addressLabel->setMinimumWidth(addressLabel->sizeHint().width());
  addressLabel->setAlignment(Qt::AlignHCenter);
  statusBar()->addWidget(addressLabel);
  valueLabel = new QLabel(this);
  statusBar()->addWidget(valueLabel, 1);

  excel->addAction(cutAction);
  excel->addAction(copyAction);
  excel->addAction(pasteAction);
  excel->addAction(deleteAction);
  excel->setContextMenuPolicy(Qt::ActionsContextMenu);

  updateStatusBar();
  setWindowIcon(QIcon(":/images/icon.png"));
  setCentralWidget(excel);
  setAttribute(Qt::WA_DeleteOnClose);
  setMinimumSize(800, 550);

  excel->setupExcel();
  findDialog = nullptr;
  gotoDialog = nullptr;
  sortDialog = nullptr;
}

MainWindow::~MainWindow(){

}

void MainWindow::updateStatusBar(){

  addressLabel->setText(tr("%1%2").arg(QChar('A' + excel->currentColumn())).arg(excel->currentRow() + 1));
  Cell *cell = static_cast<Cell *>(excel->item(excel->currentRow(), excel->currentColumn()));
  if (cell)
    valueLabel->setText(cell->text());
  else
    valueLabel->setText("");
}

void MainWindow::showFindDialog(){

  if (!findDialog){
    findDialog = new FindDialog(this);
    connect(findDialog, &FindDialog::execFind, excel, &Excel::findCell);
  }
  findDialog->exec();
}

void MainWindow::showGotoDialog(){

  if (!gotoDialog){
    gotoDialog = new GotoDialog(this);
    connect(gotoDialog, &GotoDialog::execGoto, excel, &Excel::gotoCell);
  }
  gotoDialog->exec();
}

void MainWindow::showSortDialog(){

  if (!sortDialog){
    sortDialog = new SortDialog(this);
    connect(sortDialog, &SortDialog::execSort, excel, &Excel::sortSelectedRows);
  }
  sortDialog->exec();
}

void MainWindow::showStatusMessage(const QString &msg, int duration){
  statusBar()->showMessage(msg, duration);
}

void MainWindow::closeEvent(QCloseEvent *event){

  if (excel->canCloseDocument())
    event->accept();
  else
    event->ignore();
}
