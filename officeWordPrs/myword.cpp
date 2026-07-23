#include "myword.h"
const QString srcpaths=":/new/prefox1/images";
#include <QMdiArea>
#include <QScrollBar>
MyWord::MyWord(QWidget *parent)
    : QMainWindow(parent)
{
    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setCentralWidget(mdiArea);
    connect(mdiArea,SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this,SLOT(updateMenus()));

    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(setActiveSubWindow(QWidget*)));

    createActions();

    move(600,100);
    resize(900,600);
    setWindowTitle(tr("Office办公自动化文字编辑软件 V2.1"));
    setUnifiedTitleAndToolBarOnMac(true);

}
MyWord::~MyWord() {}

void MyWord::closeEvent(QCloseEvent *event){}

void MyWord::updateMenus(){}

void MyWord::updateWindowMenu(){}

MyChild *MyWord::createMyChild(){
    MyChild *child = new MyChild;
    return child;
}

void MyWord::createActions(){
    //创建菜单操作
    //新建
    newAct = new QAction(QIcon(srcpaths+"/filenew.png"),tr("新建(&N)"),this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setToolTip("新建");
    newAct->setStatusTip(tr("创建一个新的word文档。"));
    connect(newAct,SIGNAL(triggered()),this,SLOT(fileNew()));
    //打开
    openAct = new QAction(QIcon(srcpaths+"/filenew.png"),tr("打开(&O)"),this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setToolTip("打开");
    openAct->setStatusTip(tr("打开已经存在的word文档。"));
    connect(openAct,SIGNAL(triggered()),this,SLOT(fileOpen()));
    //保存
    saveAct = new QAction(QIcon(srcpaths+"/filesave.png"),tr("保存(&S)"),this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setToolTip("保存");
    saveAct->setStatusTip(tr("将当前word文档存盘."));
    connect(saveAct,SIGNAL(triggered()),this,SLOT(fileSave()));
    //另存为
    saveAsAct = new QAction(tr("另存为(&A)..."),this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    //saveAsAct->setToolTip("另存为");
    saveAsAct->setStatusTip(tr("用另一个新的文件名称，保存当前word文档."));
    connect(saveAsAct,SIGNAL(triggered()),this,SLOT(fileSaveAs()));
    //打印
    printAct = new QAction(QIcon(srcpaths+"/filesave.png"),tr("打印(&S)"),this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setToolTip("打印");
    printAct->setStatusTip(tr("打印当前word文档."));
    connect(printAct,SIGNAL(triggered()),this,SLOT(filePrint()));
    //打印预览
    printPreviewAct = new QAction(tr("打印预览..."),this);
    printPreviewAct->setStatusTip(tr("预览当前需打印word文档."));
    connect(printPreviewAct,SIGNAL(triggered()),this,SLOT(filePrintPreview()));

    //编辑菜单动作
    //撤销
    undoAct = new QAction(QIcon(srcpaths+"/editundo.png"),tr("撤销(&U)"),this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setToolTip("撤销");
    undoAct->setStatusTip(tr("撤销当前Word文档操作."));
    connect(undoAct,SIGNAL(triggered()),this,SLOT(undo()));

    //重做
    redoAct = new QAction(QIcon(srcpaths+"/editredo.png"),tr("重做(&U)"),this);
    redoAct->setShortcuts(QKeySequence::Undo);
    redoAct->setToolTip("重做");
    redoAct->setStatusTip(tr("回复之前的Word文档操作."));
    connect(redoAct,SIGNAL(triggered()),this,SLOT(redo()));

    //剪切
    cutAct = new QAction(QIcon(srcpaths+"/editcut.png"),tr("剪切(&T)"),this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setToolTip("剪切");
    cutAct->setStatusTip(tr("剪切当前选中的内容，将它存放到剪贴板."));
    connect(cutAct,SIGNAL(triggered()),this,SLOT(Cut()));

    //复制
    copyAct = new QAction(QIcon(srcpaths+"/editcopy.png"),tr("复制(&C)"),this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setToolTip("复制");
    copyAct->setStatusTip(tr("复制当前所选中的内容，将它存放到剪贴板."));
    connect(copyAct,SIGNAL(triggered()),this,SLOT(copy()));

}
void MyWord::createMenus(){

}
void MyWord::createToolBars(){

}
void MyWord::createStatusBar(){

}
MyChild *MyWord::activeMyChild(){}

QMidSubWindow *MyWord::findMyChild(const QString &filename){
    return 0;
}

void MyWord::setActiveSubWindow(QWidget *window){}

void MyWord::fileNew(){}
void MyWord::fileOpen(){}
void MyWord::fileSave(){}
void MyWord::fileSaveAs(){}
void MyWord::undo(){}
void MyWord::redo(){}
void MyWord::cut(){}
void MyWord::paste(){}
void MyWord::enabledText(){}
void MyWord::textBold(){}
void MyWord::textItalic(){}
void MyWord::textAlign(QAction *a){}
void MyWord::textStyle(int styleIndex){}
void MyWord::textFamily(const QString &f){}
void MyWord::textSize(const QString &p){}
void MyWord::textColor(){}
void MyWord::fontChanged(const QFont &f){}
void MyWord::colorChanged(const QColor &c){}
void MyWord::alignmentChanged(Qt::Alignment a){}
void MyWord::filePrint(){}
void MyWord::filePrintPreview(){}
void MyWord::printPreview(QPrinter *){}
void MyWord::about(){}
