#include "excel.h"

Excel::Excel(QWidget *parent) : QTableWidget(parent) {

}

Excel::~Excel(){

}

void Excel::setupExcel(){

  setItemPrototype(new Cell());
  setSelectionMode(QTableWidget::ContiguousSelection);
  clearCells();
  setCurrentFile("");
  connect(this, &Excel::itemChanged, this, &Excel::somethingChanged);
}

QString Excel::getCurrentFile(){
  return currentFile;
}

QString Excel::getBaseFilename(const QString &filename){
  return QFileInfo(filename).fileName();
}

bool Excel::canCloseDocument(){

  if (isWindowModified()){
    int r = QMessageBox::warning(this, tr("Excel"), tr("You have some unsaved changes!\nWould you like to save?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (r == QMessageBox::Yes)
      return saveFile();
    if (r == QMessageBox::No)
      return true;
    return false;
  }
  return true;
}

void Excel::setCurrentFile(const QString &filename){
  currentFile = filename;
  emit updateWindowTitle(tr("%1[*] - Excel").arg(filename.isEmpty() ? tr("Untitled") : getBaseFilename(filename)));
}

void Excel::clearCells(){

  setRowCount(0);
  setColumnCount(0);
  setRowCount(Excel::ROW_COUNT);
  setColumnCount(Excel::COL_COUNT);
  for (int i = 0; i < Excel::COL_COUNT; i++){
    Cell *cell = new Cell();
    cell->setText(QString(QChar('A' + i)));
    setHorizontalHeaderItem(i, cell);
  }
  setCurrentCell(0, 0);
}

void Excel::setDocumentModified(bool modified){
  setWindowModified(modified);
  emit documentModified(modified);
}

bool Excel::openFile(){

  if (!canCloseDocument())
    return false;
  QString filename = QFileDialog::getOpenFileName(this, tr("Open Excel document"), ".", tr("Excel files (*.exl)"));
  if (filename.isEmpty())
    return false;
  setCurrentFile(filename);
  if (readFromDisk(filename)){
    setDocumentModified(false);
    emit showStatusMessage(tr("File loaded sucessfully!"));
    return true;
  }
  emit showStatusMessage(tr("Error loading file!"));
  return false;
}

bool Excel::newFile(){

  if (!canCloseDocument())
    return false;
  clearCells();
  setCurrentFile("");
  return true;
}

bool Excel::saveFile(){

  if (isWindowModified() == false && currentFile.isEmpty() == false)
    return true;
  QString filename = currentFile;
  if (filename.isEmpty())
    filename = QFileDialog::getSaveFileName(this, tr("Save Excel document"), ".", tr("Excel files (*.exl)"));
  if (!filename.isEmpty()){
    setCurrentFile(filename);
    if (writeToDisk(filename)){
      setDocumentModified(false);
      emit showStatusMessage(tr("File saved successfully!"));
      return true;
    }
  }
  emit showStatusMessage(tr("Error saving file!"));
  return false;
}

bool Excel::saveFileAs(){

  QString filename = QFileDialog::getSaveFileName(this, tr("Save Excel document as"), ".", tr("Excel files (*.exl)"));
  if (!filename.isEmpty()){
    setCurrentFile(filename);
    setDocumentModified(true);
    return saveFile();
  }
  emit showStatusMessage(tr("Error saving file!"));
  return false;
}

void Excel::copy(){

  QTableWidgetSelectionRange range = getSelectedRange();
  QString data = "";
  for (int row = range.topRow(); row <= range.bottomRow(); row++){
    for (int col = range.leftColumn(); col <= range.rightColumn(); col++){
      Cell *cell = static_cast<Cell *>(item(row, col));
      data += (cell ? cell->text() : "");
      if (col == range.rightColumn())
        data += (row == range.bottomRow() ? "" : "\n");
      else
        data += "\t";
    }
  }
  QApplication::clipboard()->setText(data);
  emit showStatusMessage(tr("%1 rows and %2 columns copied to clipboard!").arg(range.rowCount()).arg(range.columnCount()));
}

void Excel::paste(){

  QString str = QApplication::clipboard()->text();
  QStringList rows = str.split("\n");
  int startRow = currentRow();
  int startCol = currentColumn();
  for (int row = startRow; row < (startRow + rows.count()) && row < ROW_COUNT; row++){
    QStringList rowData = rows.at(row - startRow).split("\t");
    for (int col = startCol; col < (startCol + rowData.count()) && col < COL_COUNT; col++){
      Cell *cell = static_cast<Cell *>(item(row, col));
      if (!cell){
        cell = new Cell();
        setItem(row, col, cell);
      }
      cell->setText(rowData.at(col - startCol));
    }
  }
  somethingChanged();
}

void Excel::cut(){

  copy();
  deleteSelected();
  somethingChanged();
}

void Excel::deleteSelected(){

  QTableWidgetSelectionRange range = getSelectedRange();
  for (int row = range.topRow(); row <= range.bottomRow(); row++){
    for (int col = range.leftColumn(); col <= range.rightColumn(); col++){
      Cell *cell = static_cast<Cell *>(item(row, col));
      if (cell){
        delete cell;
        setItem(row, col, nullptr);
      }
    }
  }
  somethingChanged();
}

void Excel::somethingChanged(){
  setDocumentModified(true);
}

void Excel::findCell(const QString &keyword, Qt::CaseSensitivity cs){

  for (int row = 0; row < ROW_COUNT; row++){
    for (int col = 0; col < COL_COUNT; col++){
      Cell *cell = static_cast<Cell *>(item(row, col));
      if (cell){
        if (cell->text().contains(keyword, cs)){
          setCurrentCell(row, col);
          emit showStatusMessage(tr("Keyword found!"));
          return;
        }
      }
    }
  }
  emit showStatusMessage(tr("Keyword not found :("));
}

void Excel::gotoCell(const QString &address){

  int col = address.at(0).toUpper().unicode() - QChar('A').unicode();
  int row = address.mid(1, address.size() - 1).toInt() - 1;
  if (row < 0 || row >= ROW_COUNT || col < 0 || col >= COL_COUNT){
    emit showStatusMessage(tr("Invalid cell address!"));
    return;
  }
  setCurrentCell(row, col);
}

QTableWidgetSelectionRange Excel::getSelectedRange(){

  QList<QTableWidgetSelectionRange> ranges = selectedRanges();
  if (ranges.isEmpty())
    return QTableWidgetSelectionRange();
  return ranges.first();
}

bool Excel::writeToDisk(const QString &filename){

  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly)){
    QMessageBox::warning(this, tr("Excel"), tr("Error opening file!\nError: %1").arg(file.errorString()));
    return false;
  }
  QDataStream ds(&file);
  ds << MAGIC_BYTES;
  QApplication::setOverrideCursor(Qt::WaitCursor);
  for (int row = 0; row < ROW_COUNT; row++){
    for (int col = 0; col < COL_COUNT; col++){
      Cell *c = static_cast<Cell *>(item(row, col));
      if (c && c->text().isEmpty() == false)
        ds << quint16(row) << quint16(col) << c->text();
    }
  }
  QApplication::restoreOverrideCursor();
  file.close();
  return true;
}

bool Excel::readFromDisk(const QString &filename){

  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly)){
    QMessageBox::warning(this, tr("Excel"), tr("Error opening file!\nError: %1").arg(file.errorString()));
    return false;
  }
  QDataStream ds(&file);
  quint32 mgk;
  ds >> mgk;
  if (mgk != MAGIC_BYTES){
    QMessageBox::warning(this, tr("Excel"), tr("The selecte file is not a valid Excel file!"));
    file.close();
    return false;
  }
  quint16 row, col;
  QString val;
  QApplication::setOverrideCursor(Qt::WaitCursor);
  clearCells();
  while (!ds.atEnd()){
    ds >> row >> col >> val;
    Cell *c = new Cell();
    c->setText(val);
    setItem(row, col, c);
  }
  file.close();
  QApplication::restoreOverrideCursor();
  return true;
}

void Excel::about(){
  QMessageBox::about(this, tr("About Editor"), tr("<h2>Excel v1.0</h2>"
                                                  "<p>This is a demo program showing how to use QTableWidget to work with grids of data</p>"
                                                  "<p>Copyright &copy; 2024 <a href='https://github.com/4g3nt47'>Umar Abdul</a></p>"));
}