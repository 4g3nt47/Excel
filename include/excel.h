/**
 * @file excel.h
 * @brief The Excel class. Extends the QTableWidget class, and provides most of the application logic.
 * @version 1.0
 * @date 23/07/2024
 * @author https://github.com/4g3nt47
 */

#ifndef EXCEL_H
#define EXCEL_H

#include <QApplication>
#include <QTableWidget>
#include <QHeaderView>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QIODevice>
#include <QDataStream>
#include <QMessageBox>
#include <QClipboard>
#include <QStringList>
#include "cell.h"

class Excel : public QTableWidget{

  Q_OBJECT

  public:

    Excel(quint16 rowCount, quint16 colCount, QStringList *colLabels = nullptr, QList<int> *readOnlyCols = nullptr, QWidget *parent = nullptr);

    ~Excel();

    void setupExcel();

    QString getCurrentFile();

    QString getBaseFilename(const QString &filename);

    bool canCloseDocument();

  private:

    const quint32 MAGIC_BYTES = 0xdeadbeef; // Custom magic bytes to identify valid Excel files.
    const quint16 ROW_COUNT;
    const quint16 COL_COUNT;
    QStringList *colLabels;
    QList<int> *readOnlyCols;

    QString currentFile;

    void setCurrentFile(const QString &filename);

    void clearCells();

    void setDocumentModified(bool modified);


  public slots:

    bool openFile();

    bool newFile();

    bool saveFile();

    bool saveFileAs();

    void copy();

    void paste();

    void cut();

    void deleteSelected();

    void somethingChanged(QTableWidgetItem *item = nullptr);

    void findCell(const QString &keyword, Qt::CaseSensitivity cs = Qt::CaseInsensitive);

    void gotoCell(const QString &address);

    void sortSelectedRows(Qt::SortOrder order);

    void about();

  public:

    QTableWidgetSelectionRange getSelectedRange();
    bool writeToDisk(const QString &filename);
    bool readFromDisk(const QString &filename);

  signals:

    void updateWindowTitle(const QString &newTitle);

    void documentModified(bool modified);

    void showStatusMessage(const QString &msg, int duration = 2000);

};

#endif // EXCEL_H
