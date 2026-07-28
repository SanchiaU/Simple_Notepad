#include "myword.h"
#include <QMdiArea>
#include <QScrollBar>
#include<QApplication>

const QString srcpaths=":/new/prefox1/images";

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
    //【创建】菜单操作
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

    //【编辑】菜单动作
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
    redoAct->setStatusTip(tr("恢复之前的Word文档操作."));
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

    //粘贴
    pasteAct = new QAction(QIcon(srcpaths+"/editpaste.png"),tr("粘贴(&C)"),this);
    pasteAct->setShortcuts(QKeySequence::Copy);
    pasteAct->setToolTip("粘贴");
    pasteAct->setStatusTip(tr("粘贴当前所选中的内容."));
    connect(pasteAct,SIGNAL(triggered()),this,SLOT(paste()));

    //【格式】菜单操作
    //加粗
    boldAct = new QAction(QIcon(srcpaths+"/textbold.png"),tr("加粗(&B)"),this);
    boldAct->setCheckable(true);
    boldAct->setShortcut(Qt::CTRL+Qt::Key_B);
    boldAct->setToolTip("加粗");
    boldAct->setStatusTip(tr("将所选中文字进行加粗处理"));
    QFont bold;
    bold.setBold(true);
    boldAct->setFont(bold);
    connect(boldAct,SIGNAL(triggered()),this,SLOT(textBold()));

    //倾斜
    italicAct = new QAction(QIcon(srcpaths+"/textitalic.png"),tr("倾斜(&I)"),this);
    italicAct->setCheckable(true);
    italicAct->setShortcut(Qt::CTRL+Qt::Key_I);
    italicAct->setToolTip("倾斜");
    italicAct->setStatusTip(tr("将所选文字进行倾斜处理"));
    QFont italic;
    italic.setItalic(true);
    italicAct->setFont(bold);
    connect(italicAct,SIGNAL(triggered()),this,SLOT(textItalic()));

    //下划线
    underlineAct = new QAction(QIcon(srcpaths+"/textunder.png"),tr("下划线(&I)"),this);
    underlineAct->setCheckable(true);
    underlineAct->setShortcut(Qt::CTRL+Qt::Key_U);
    underlineAct->setToolTip("下划线");
    underlineAct->setStatusTip(tr("将所选文字进行添加下划线处理"));
    QFont underline;
    italic.setItalic(true);
    underlineAct->setFont(underline);
    connect(underlineAct,SIGNAL(triggered()),this,SLOT(textUnderline()));

    //【格式】-->（段落）菜单动作
    QActionGroup *grp = new QActionGroup(this);
    connect(grp,SIGNAL(triggered(QAction*)),this,SLOT(textAlign(QAction*)));

    if(QApplication::isLeftToRight()){
        leftAlignAct = new QAction(QIcon(srcpaths+"/textleft.png"),tr("左对齐(&L)"),grp);
        centerAct = new QAction(QIcon(srcpaths+"/textcenter.png"),tr("居中对齐(&E)"),grp);
        rightAlignAct = new QAction(QIcon(srcpaths+"/textright.png"),tr("右对齐(&E)"),grp);
    }else{
        rightAlignAct = new QAction(QIcon(srcpaths+"/textright.png"),tr("右对齐(&E)"),grp);
        centerAct = new QAction(QIcon(srcpaths+"/textcenter.png"),tr("居中对齐(&E)"),grp);
        leftAlignAct = new QAction(QIcon(srcpaths+"/textleft.png"),tr("左对齐(&L)"),grp);
    }
    justifyAct = new QAction(QIcon(srcpaths+"/textjustify.png"),tr("两端对齐(&J)"),grp);

    leftAlignAct->setShortcut(Qt::CTRL+Qt::Key_L);
    leftAlignAct->setCheckable(true);
    leftAlignAct->setToolTip("对齐");
    leftAlignAct->setStatusTip(tr("将选择文字进行左对齐."));

    centerAct->setShortcut(Qt::CTRL+Qt::Key_E);
    centerAct->setCheckable(true);
    centerAct->setToolTip("对齐");
    centerAct->setStatusTip(tr("将选择文字进行居中对齐."));

    rightAlignAct->setShortcut(Qt::CTRL+Qt::Key_R);
    rightAlignAct->setCheckable(true);
    rightAlignAct->setToolTip("对齐");
    rightAlignAct->setStatusTip(tr("将选择文字进行右对齐."));

    justifyAct->setShortcut(Qt::CTRL+Qt::Key_J);
    justifyAct->setCheckable(true);
    justifyAct->setToolTip("对齐");
    justifyAct->setStatusTip(tr("将选择文字进行两端对齐."));

    QPixmap pix(16,16);
    pix.fill(Qt::red);
    colorAct = new QAction(pix,tr("颜色(&C)..."),this);
    colorAct->setToolTip("颜色");
    colorAct->setStatusTip(tr("将你选择的文字，设置对应的颜色."));
    connect(colorAct,SIGNAL(triggered()),this,SLOT(textColor()));

    //【窗口】菜单
    closeAct = new QAction(tr("关闭(&O)"),this);
    closeAct->setStatusTip(tr("关闭活动word文档子窗口."));
    connect(closeAct,SIGNAL(triggered()),mdiArea,SLOT(closeActiveSubWindows()));

    closeAct = new QAction(tr("关闭所有(&A)"),this);
    closeAct->setStatusTip(tr("关闭活动word文档所有子窗口."));
    connect(closeAct,SIGNAL(triggered()),mdiArea,SLOT(closeActiveAllSubWindows()));

    tileAct = new QAction(tr("平铺(&C)"),this);
    tileAct->setStatusTip(tr("平铺子窗口."));
    connect(tileAct,SIGNAL(triggered()),mdiArea,SLOT(tileSubWindows()));

    cascadeAct = new QAction(tr("层叠(&T)"),this);
    cascadeAct->setStatusTip(tr("层叠子窗口."));
    connect(cascadeAct,SIGNAL(triggered()),mdiArea,SLOT(cascadeSubWindows()));

    nextAct = new QAction(tr("下一个(&T)"),this);
    nextAct->setShortcuts(QKeySequence::NextChild);
    nextAct->setStatusTip(tr("移动焦点到下一个子窗口."));
    connect(nextAct,SIGNAL(triggered()),mdiArea,SLOT(activateNextSubWindow()));

    previousAct = new QAction(tr("前一个(&T)"),this);
    previousAct->setShortcuts(QKeySequence::PreviousChild);
    previousAct->setStatusTip(tr("移动焦点到前一个子窗口."));
    connect(previousAct,SIGNAL(triggered()),mdiArea,SLOT(activatePreviousSubWindow()));

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);

    //【帮助】菜单
    aboutAct = new QAction(tr("关于(&A)"),this);
    aboutAct->setStatusTip("关于Office Word相关信息");
    connect(aboutAct,SIGNAL(triggered()),this,SLOT(aboutQt()));

    aboutQtAct = new QAction(tr("关于Qt(&Q)"),this);
    aboutQtAct->setStatusTip("关于Qt库相关信息");
    connect(aboutQtAct,SIGNAL(triggered()),this,SLOT(aboutQt()));

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
