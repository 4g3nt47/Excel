#include "excel.h"

Excel::Excel(quint16 rowCount, quint16 colCount, QStringList *colLabels, QList<int> *readOnlyCols, QWidget *parent) : QTableWidget(parent), ROW_COUNT(rowCount), COL_COUNT(colCount){
  this->colLabels = colLabels;
  this->readOnlyCols = readOnlyCols;
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
    int r = QMessageBox::warning(this, tr("ResultSender"), tr("You have some unsaved changes!\nWould you like to save?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
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
  emit updateWindowTitle(tr("%1[*] - ResultSender").arg(filename.isEmpty() ? tr("Untitled") : getBaseFilename(filename)));
}

void Excel::clearCells(){

  setRowCount(0);
  setColumnCount(0);
  setRowCount(ROW_COUNT);
  setColumnCount(COL_COUNT);
  for (int i = 0; i < COL_COUNT; i++){
    Cell *cell = new Cell();
    if (colLabels)
      cell->setText(colLabels->at(i));
    else
      cell->setText(QString(QChar('A' + i)));
    setHorizontalHeaderItem(i, cell);
  }
  horizontalHeader()->setStretchLastSection(true);
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
  setDocumentModified(false);
  return true;
}

bool Excel::saveFile(){

  if (isWindowModified() == false && currentFile.isEmpty() == false)
    return true;
  QString filename = currentFile;
  if (filename.isEmpty())
    filename = QFileDialog::getSaveFileName(this, tr("Save Excel document"), ".", tr("Excel files (*.exl)"));
  if (!filename.isEmpty()){
    if (!filename.endsWith(".exl"))
      filename.append(".exl");
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
}

void Excel::somethingChanged(QTableWidgetItem *item){

  if (item && readOnlyCols && readOnlyCols->contains(item->column())){
    if (item->flags() & Qt::ItemIsEditable){
      disconnect(this, &Excel::itemChanged, this, &Excel::somethingChanged); // Brief disconnect to avoid double-call
      item->setFlags(item->flags() ^ Qt::ItemIsEditable);
      connect(this, &Excel::itemChanged, this, &Excel::somethingChanged);
    }
  }
  setDocumentModified(true);
}

void Excel::findCell(const QString &keyword, Qt::CaseSensitivity cs){

  for (int row = 0; row < ROW_COUNT; row++){
    for (int col = 0; col < COL_COUNT; col++){
      Cell *cell = static_cast<Cell *>(item(row, col));
      if (!cell)
        continue;
      if (cell->text().contains(keyword, cs)){
        setCurrentCell(row, col);
        emit showStatusMessage(tr("Keyword found!"));
        return;
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

void Excel::sortSelectedRows(Qt::SortOrder order){

  // 1. Validate selection.
  QTableWidgetSelectionRange range = getSelectedRange();
  if (range.rowCount() < 2){
    emit showStatusMessage("You must select 2 or more rows before sorting!");
    return;
  }
  QApplication::setOverrideCursor(Qt::WaitCursor);
  // 2. Fetch all the cell data.
  QList<QStringList> rows;
  for (int i = range.topRow(); i <= range.bottomRow(); i++){
    QStringList row;
    for (int j = range.leftColumn(); j <= range.rightColumn(); j++){
      Cell *cell = static_cast<Cell *>(item(i, j));
      if (!cell){
        cell = new Cell();
        setItem(i, j, cell);
      }
      row.append(cell->text());
    }
    rows.append(row);
  }
  // 3. Sort them.
  std::stable_sort(rows.begin(), rows.end(), [&](const QStringList &row1, const QStringList &row2){
    int col_count = row1.count();
    for (int i = 0; i < col_count; i++){
      if (row1[i] != row2[i]){
        if (order == Qt::AscendingOrder)
          return row1[i] < row2[i];
        else
          return row2[i] < row1[i];
      }
    }
    return false;
  });
  // 4. Load them back into the cells.
  for (int i = range.topRow(); i <= range.bottomRow(); i++){
    for (int j = range.leftColumn(); j <= range.rightColumn(); j++){
      Cell *cell = static_cast<Cell *>(item(i, j));
      cell->setText(rows[i - range.topRow()][j - range.leftColumn()]);
    }
  }
  // 5. Done :)
  QApplication::restoreOverrideCursor();
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
    QMessageBox::warning(this, tr("ResultSender"), tr("Error opening file!\nError: %1").arg(file.errorString()));
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
    QMessageBox::warning(this, tr("ResultSender"), tr("Error opening file!\nError: %1").arg(file.errorString()));
    return false;
  }
  QDataStream ds(&file);
  quint32 mgk;
  ds >> mgk;
  if (mgk != MAGIC_BYTES){
    QMessageBox::warning(this, tr("ResultSender"), tr("The selecte file is not a valid Excel file!"));
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
  QMessageBox::about(this, tr("About ResultSender"), tr("<h2>ResultSender v1.0</h2>"
                                                  "<p>ResultSender is a program for sending student results through email with customized, encrypted PDF attachment.</p>"
                                                  "<p>Copyright &copy; 2024 <a href='https://github.com/4g3nt47'>Umar Abdul</a></p>"));
}
