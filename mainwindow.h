#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QTextEdit>
#include <QMessageBox>
#include <QFontDialog>
#include <QLabel>
#include <QTextCursor>
#include <QSettings>
#include <QColor>
#include <QColorDialog>
#include "about_widget.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    // open file
    void open_file_slot();
    // save file
    void save_file_slot();
    // exit the software
    void exit_slot();
    // another save
    void another_save_slot();
    // about information
    void about_slot();
    // when line and row changed
    void refresh_status_slot();
    // show statusbar or not?
    void show_status_slot();
    // auto change line?
    void auto_line_slot();

    // choose font
    void font_slot();
    // choose color
    void color_slot();

    // cut
    void cut_slot();
    // paste
    void paste_slot();
    // copy
    void copy_slot();
    // undo
    void undo_slot();
    // clear
    void clear_slot();

private:
    QString file_path;
    QString save_path;
    QString file_content;
    QLabel *statusInfo;
    QLabel *cglineInfo;
    bool is_status;
    bool is_line;
    QSettings *setting;   // config file

    // init the widget
    void init();
    // connect the signals and the slots
    void cont();
    // save text to file
    void save_text();
    // read text
    void read_text(QString path);
};
#endif // MAINWINDOW_H
