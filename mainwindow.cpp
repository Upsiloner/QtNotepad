#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // add qss
    QFile file(":/qt.qss");
    file.open(QIODevice::ReadOnly);
    setStyleSheet(file.readAll());
    ui->setupUi(this);
    // init the widget
    this->init();
    // connect
    this->cont();


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::init()
{
    setFixedSize(1000,650);
    setWindowTitle("无标题 - 记事本");

    is_status=true;
    is_line=true;

    // set the status bar
    statusInfo = new QLabel("第 1 行，第 1 列   字数 0   ",this);
    ui->statusbar->addPermanentWidget(statusInfo);
    cglineInfo = new QLabel(" [ 自动换行 ] ",this);
    ui->statusbar->addWidget(cglineInfo);

    // set the font size
    //ui->textEdit->setText();
    //ui->textEdit->setFontPointSize(100);


}

void MainWindow::cont()
{
    // file
    connect(ui->open,SIGNAL(triggered()),this,SLOT(open_file_slot()));
    connect(ui->save,SIGNAL(triggered()),this,SLOT(save_file_slot()));
    connect(ui->exit,SIGNAL(triggered()),this,SLOT(exit_slot()));
    connect(ui->anothersave,SIGNAL(triggered()),this,SLOT(another_save_slot()));

    // edit
    connect(ui->font,SIGNAL(triggered()),this,SLOT(font_slot()));
    connect(ui->color,SIGNAL(triggered()),this,SLOT(color_slot()));
    connect(ui->copy,SIGNAL(triggered()),this,SLOT(copy_slot()));
    connect(ui->cut,SIGNAL(triggered()),this,SLOT(cut_slot()));
    connect(ui->paste,SIGNAL(triggered()),this,SLOT(paste_slot()));
    connect(ui->undo,SIGNAL(triggered()),this,SLOT(undo_slot()));
    connect(ui->clear,SIGNAL(triggered()),this,SLOT(clear_slot()));

    // watch
    connect(ui->status,SIGNAL(triggered()),this,SLOT(show_status_slot()));
    connect(ui->cgline,SIGNAL(triggered()),this,SLOT(auto_line_slot()));

    // help
    connect(ui->about,SIGNAL(triggered()),this,SLOT(about_slot()));

    // texteedit
    connect(ui->textEdit,SIGNAL(cursorPositionChanged()),this,SLOT(refresh_status_slot()));
}

// save content to the file
void MainWindow::save_text()
{
    QFile file(file_path);
    if(file.open(QIODevice::WriteOnly)){
        QTextStream out(&file);
        out << ui->textEdit->document()->toPlainText();
        file.close();
        this->setWindowTitle(file_path.mid(file_path.lastIndexOf('/')+1)+" - 记事本");
        file_content = ui->textEdit->document()->toPlainText();
        ui->statusbar->showMessage("已保存",3000);
    }
}

// read content to the textedit
void MainWindow::read_text(QString path)
{
    QFile file(path);
    file.open(QIODevice::ReadWrite);
    QByteArray bytes = file.readAll();
    ui->textEdit->setText(bytes);
    file.close();
}


//open file
void MainWindow::open_file_slot()
{
    file_path=QFileDialog::getOpenFileName(this,"打开文件","C:\\Users\\14583\\Desktop");
    if(!file_path.isEmpty()){
        this->read_text(file_path);
    }
    // reset the title
    if(file_path.lastIndexOf("\\") != -1){
        this->setWindowTitle(file_path.mid(file_path.lastIndexOf('\\')+1)+" - 记事本");
    }
    else{
        this->setWindowTitle(file_path.mid(file_path.lastIndexOf('/')+1)+" - 记事本");
    }
}

// save file
void MainWindow::save_file_slot()
{
    if(file_path.isEmpty()){  // there is no file opend
        // getSaveFileName to get the save path
        file_path = QFileDialog::getSaveFileName(this,tr("保存的文件"),tr("myText.txt"),
                    tr("Items files(*.items)"));
        if(!file_path.isEmpty()){
            this->save_text();
        }
    }
    else{    // a file is opend
        this->save_text();
    }
}

// exit the app
void MainWindow::exit_slot()
{
    if(!ui->textEdit->document()->isEmpty()){
        QMessageBox box(QMessageBox::Question,"是否退出","是否保存您之前的更改？");
        box.setStandardButtons (QMessageBox::Ok|QMessageBox::Ignore|QMessageBox::Cancel);
        box.setButtonText (QMessageBox::Ok,QString("保存"));
        box.setButtonText (QMessageBox::Ignore,QString("不保存"));
        box.setButtonText (QMessageBox::Cancel,QString("取消"));
        int ret = box.exec();
        if(ret == QMessageBox::Ok){
            if(file_path.isEmpty()){   // there is no file opend
                // getSaveFileName to get the save path
                file_path = QFileDialog::getSaveFileName(this,tr("保存的文件"),tr("myText.txt"),
                            tr("Items files(*.items)"));
                if(!file_path.isEmpty()){
                    this->save_text();
                }
            }else{     // a file is opend
                this->save_text();
            }

        }else if(ret == QMessageBox::Ignore){
            // not save, close the app
            this->close();
        }

    }
    else{
        this->close();
    }
}

void MainWindow::another_save_slot()
{
    file_path = QFileDialog::getSaveFileName(this,tr("保存的文件"),tr("myText.txt"),
                tr("Items files(*.items)"));
    this->save_text();
}

// show information about this app
void MainWindow::about_slot()
{
    about_widget *aboutInfo = new about_widget;
    aboutInfo->show();
}

// refresh the status bar
void MainWindow::refresh_status_slot()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    int row = cursor.blockNumber()+1;
    int colume = cursor.columnNumber()+1;
    int number = cursor.document()->toPlainText().length();
    statusInfo->setText("第 "+QString::number(row)+" 行，第 "+
                        QString::number(colume)+" 列   字数 "+
                        QString::number(number)+"   ");
}

// show status bar?
void MainWindow::show_status_slot()
{
    is_status=!is_status;
    ui->statusbar->setVisible(is_status);
    ui->status->setIconVisibleInMenu(is_status); 
}

// change lines by itself?
void MainWindow::auto_line_slot()
{
    if(is_line){
        ui->textEdit->setLineWrapMode(QTextEdit::NoWrap);
        is_line = false;
        cglineInfo->setText(" [ 非自动换行 ] ");
    }
    else{
        ui->textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
        is_line = true;
        cglineInfo->setText(" [ 自动换行 ] ");
    }
    ui->cgline->setIconVisibleInMenu(is_line);
}

// change font's style
void MainWindow::font_slot()
{
    QFont textFont = ui->textEdit->font();
    // get font dialog
    bool ok;
    QFont font = QFontDialog::getFont(&ok,textFont,this,"字体选择");

    if(ok){
        ui->textEdit->setFont(font);
    }
}

void MainWindow::color_slot()
{
    QColor textColor = ui->textEdit->textColor();
    // get color dialog
    QColor color = QColorDialog::getColor(textColor,this,"选择颜色");

    ui->textEdit->setTextColor(color);

}

void MainWindow::cut_slot()
{
    ui->textEdit->cut();
}

void MainWindow::paste_slot()
{
    ui->textEdit->paste();
}

void MainWindow::copy_slot()
{
    ui->textEdit->copy();
}

void MainWindow::undo_slot()
{
    ui->textEdit->undo();
}

void MainWindow::clear_slot()
{
    ui->textEdit->clear();
}

