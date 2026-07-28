#include "myword.h"
#include <QMdiArea>
#include <QScrollBar>
#include<QApplication>
#include<QMenu>
#include<QMenuBar>
#include <QObject>
#include<QToolBar>
#include<QComboBox>
#include<QFontComboBox>
#include <QtGlobal>
#include <QtWidgets>
#include <QFontDatabase>

const QString srcpaths=":/new/prefix1/images";

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

    createActions();//创建菜单栏、工具栏、状态栏等相关操作
    createMenus();//调用菜单实现
    createToolBars();//调用创建工具条
    updateMenus();

    move(600,100);
    resize(900,600);
    setWindowTitle(tr("Office办公自动化文字编辑软件 V2.1"));
    setUnifiedTitleAndToolBarOnMac(true);

}
MyWord::~MyWord() {}

void MyWord::closeEvent(QCloseEvent *event){
    mdiArea->closeAllSubWindows();
    if(mdiArea->currentSubWindow()){
        event->ignore();
    }else{
        event->accept();
    }

}

void MyWord::updateMenus(){
    bool hasMyChild = (activeMyChild()!=0);
    saveAct->setEnabled(hasMyChild);
    saveAsAct->setEnabled(hasMyChild);
    printAct->setEnabled(hasMyChild);
    printPreviewAct->setEnabled(hasMyChild);

    pasteAct->setEnabled(hasMyChild);
    closeAct->setEnabled(hasMyChild);
    closeAllAct->setEnabled(hasMyChild);
    closeAct->setEnabled(hasMyChild);
    closeAllAct->setEnabled(hasMyChild);
    tileAct->setEnabled(hasMyChild);
    cascadeAct->setEnabled(hasMyChild);
    nextAct->setEnabled(hasMyChild);
    previousAct->setEnabled(hasMyChild);
    separatorAct->setVisible(hasMyChild);

    //打开文档并且选中内容
    bool hasSelection = (activeMyChild()&&activeMyChild()->textCursor().hasSelection());
    cutAct->setEnabled(hasSelection);
    copyAct->setEnabled(hasSelection);

    boldAct->setEnabled(hasSelection);
    italicAct->setEnabled(hasSelection);
    underlineAct->setEnabled(hasSelection);
    leftAlignAct->setEnabled(hasSelection);
    centerAct->setEnabled(hasSelection);
    rightAlignAct->setEnabled(hasSelection);
    justifyAct->setEnabled(hasSelection);
    colorAct->setEnabled(hasSelection);
}

void MyWord::updateWindowMenu(){
    windowMenu->clear();
    windowMenu->addAction(closeAct);
    windowMenu->addAction(closeAllAct);
    windowMenu->addSeparator();

    windowMenu->addAction(tileAct);
    windowMenu->addAction(cascadeAct);
    windowMenu->addSeparator();

    windowMenu->addAction(nextAct);
    windowMenu->addAction(previousAct);
    windowMenu->addAction(separatorAct);

    QList<QMdiSubWindow*>windows = mdiArea->subWindowList();
    separatorAct->setVisible(!windows.isEmpty());
    //显示子窗口
    for(int i = 1;i<windows.size();++i){
        MyChild *child = qobject_cast<MyChild *>(windows.at(i)->widget());
        QString text;
        if(i<9){
            text = tr("&%1 %2").arg(child->userFriendlyCurrentFile());
        }else{
            text = tr("&%1 %2").arg(i+1).arg(child->userFriendlyCurrentFile());
        }
        QAction *action = windowMenu->addAction(text);
        action->setCheckable(true);
        action->setChecked(child==activeMyChild());
        connect(action,SIGNAL(triggerred),windowMapper,SLOT(map()));
        windowMapper->setMapping(action,windows.at(i));
    }
    enabledText();
}

MyChild *MyWord::createMyChild(){
    MyChild *child = new MyChild;

    mdiArea->addSubWindow(child);
    connect(child,SIGNAL(copyAvailable(bool)),cutAct,SLOT(setEnabled(bool)));
    connect(child,SIGNAL(copyAvailable(bool)),cutAct,SLOT(setEnabled(bool)));

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
    openAct = new QAction(QIcon(srcpaths+"/fileopen.png"),tr("打开(&O)"),this);
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

    //退出操作
    exitAct = new QAction(tr("退出(X)"),this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip("退出Word文件应用程序");
    connect(exitAct,SIGNAL(triggered()),qApp,SLOT(closeAllWindows()));

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
    connect(closeAct,SIGNAL(triggered()),mdiArea,SLOT(closeActiveSubWindow()));

    closeAllAct = new QAction(tr("关闭所有(&A)"),this);
    closeAllAct->setStatusTip(tr("关闭活动word文档所有子窗口."));
    connect(closeAct,SIGNAL(triggered()),mdiArea,SLOT(closeActiveAllSubWindows()));

    tileAct = new QAction(tr("平铺(&C)"));
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
    connect(aboutQtAct,SIGNAL(triggered()),qApp,SLOT(aboutQt()));

}
void MyWord::createMenus(){

    //【文件】菜单
    fileMenu = menuBar()->addMenu(tr("文件(&F)"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(printAct);
    fileMenu->addAction(printPreviewAct);
    fileMenu->addAction(exitAct);

    //【编辑】菜单
    editMenu = menuBar()->addMenu(tr("编辑(&F)"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    //【格式】菜单
    formatMenu = menuBar()->addMenu(tr("格式(&O)"));
    fontMenu = formatMenu->addMenu(tr("字体(&D)"));
    fontMenu->addAction(boldAct);
    fontMenu->addAction(italicAct);
    fontMenu->addAction(underlineAct);

    alignMenu = formatMenu->addMenu(tr("段落"));
    alignMenu->addAction(leftAlignAct);
    alignMenu->addAction(centerAct);
    alignMenu->addAction(rightAlignAct);
    alignMenu->addAction(justifyAct);

    formatMenu->addAction(colorAct);

    //【窗口】菜单
    windowMenu = menuBar()->addMenu(tr("窗口(&W)"));
    updateWindowMenu();
    //connect(windowMenu,SIGNAL(aboutToShow()),this,SLOT(updateWindowMenu()));

    connect(windowMenu, &QMenu::aboutToShow, this, &MyWord::updateWindowMenu);
    menuBar()->addSeparator();

    //【帮助】菜单
    helpMenu = menuBar()->addMenu(tr("帮助(&H)"));
    helpMenu->addAction(aboutAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutQtAct);



}
void MyWord::createToolBars(){
    //文件
    fileToolBar = addToolBar(tr("文件"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(printAct);
    //编辑
    editToolBar = addToolBar(tr("编辑"));
    editToolBar->addAction(undoAct);
    editToolBar->addAction(redoAct);
    editToolBar->addSeparator();
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
    //格式
    editToolBar = addToolBar(tr("格式"));
    editToolBar->addAction(boldAct);
    editToolBar->addAction(italicAct);
    editToolBar->addAction(underlineAct);
    editToolBar->addAction(leftAlignAct);
    editToolBar->addAction(centerAct);
    editToolBar->addAction(rightAlignAct);
    editToolBar->addAction(justifyAct);
    editToolBar->addSeparator();
    editToolBar->addAction(colorAct);
    //组合工具栏
    addToolBarBreak(Qt::TopToolBarArea);
    comboToolBar = addToolBar(tr("组合选择"));
    comboStyle = new QComboBox();
    comboToolBar->addWidget(comboStyle);
    comboStyle->addItem("标准");
    comboStyle->addItem("项目符号(●)");
    comboStyle->addItem("项目符号(○)");
    comboStyle->addItem("项目符号(■)");
    comboStyle->addItem("编号(a.b.c)");
    comboStyle->addItem("编号(A.B.C)");
    comboStyle->addItem("编号(ⅰ.ⅱ.ⅲ)");
    comboStyle->addItem("编号(Ⅰ.Ⅱ.Ⅲ)");
    comboStyle->addItem("编号(段落加上标号或编号)");
    connect(comboStyle,SIGNAL(activated(int)),this,SLOT(textStyle(int)));

    comboFont = new QFontComboBox();
    comboToolBar->addWidget(comboFont);
    comboFont->setStatusTip("更改字体");
    connect(comboFont,SIGNAL(activated(int)),this,SLOT(textFamily(QString)));

    comboSize = new QComboBox();
    comboToolBar->addWidget(comboSize);
    comboSize->setEditable(true);
    comboSize->setStatusTip("更改字号");

    QFontDatabase db;
    QList<int> sizes = db.standardSizes();
    for(int size : sizes) {
        comboSize->addItem(QString::number(size));
    }

    //for(int size:db.smoothSizes()) comboSize->addItem(QString::number(size));


}
void MyWord::createStatusBar(){
    statusBar()->showMessage(tr("准备就绪状态."));
}
MyChild *MyWord::activeMyChild(){
    if(QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
        return qobject_cast<MyChild*>(activeSubWindow->widget());
    return 0;
}

QMdiSubWindow *MyWord::findMyChild(const QString &filename){
    QString canonicalFilePath = QFileInfo(filename).canonicalFilePath();
    foreach(QMdiSubWindow *window,mdiArea->subWindowList()){
        MyChild *myChild = qobject_cast<MyChild*>(window->widget());
        if(myChild->currentFile()==canonicalFilePath)
            return window;
    }
    return 0;
}

void MyWord::setActiveSubWindow(QWidget *window){
    if(!window)return;
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(window));
}

void MyWord::fileNew(){
    MyChild *child = createMyChild();
    child->newFile();
    child->show();
    enabledText();

}
void MyWord::fileOpen(){
    QString filename = QFileDialog::getOpenFileName(this,tr("打开"),QString(),tr("HTML 文档(*.htm *html);;所有文件(*.*)"));
    if(!filename.isEmpty()){
        QMdiSubWindow *existing = findMyChild(filename);
        if(existing){
            mdiArea->setActiveSubWindow(existing);
            return;
        }
        MyChild *child = createMyChild();
        if(child->loadFile(filename)){
            statusBar()->showMessage(tr("文件已经加载"),2000);
            child->show();
            enabledText();
        }else{
            child->close();
        }

    }
}
void MyWord::fileSave(){
    if(activeMyChild()&&activeMyChild()->save())
        statusBar()->showMessage(tr("word文档保存成功."),2000);

}
void MyWord::fileSaveAs(){
    if(activeMyChild()&&activeMyChild()->saveAs())
        statusBar()->showMessage(tr("Word文档另存为成功."));
}
void MyWord::undo(){
    if(activeMyChild())
        activeMyChild()->undo();
}
void MyWord::redo(){
    if(activeMyChild())
        activeMyChild()->redo();
}
void MyWord::cut(){
    if(activeMyChild())
        activeMyChild()->cut();
}
void MyWord::paste(){
    if(activeMyChild())
        activeMyChild()->paste();
}
void MyWord::enabledText(){
    boldAct->setEnabled(true);
    italicAct->setEnabled(true);
    underlineAct->setEnabled(true);
    leftAlignAct->setEnabled(true);
    centerAct->setEnabled(true);
    rightAlignAct->setEnabled(true);
    justifyAct->setEnabled(true);
    colorAct->setEnabled(true);
}
void MyWord::textBold(){
    QTextCharFormat fmt;
    fmt.setFontWeight(boldAct->isChecked()?QFont::Bold:QFont::Normal);
    if(activeMyChild())
        activeMyChild()->mergeFormationOnWordOrSelection(fmt);
}
void MyWord::textItalic(){
    QTextCharFormat fmt;
    fmt.setFontItalic(italicAct->isChecked());
    if(activeMyChild())
        activeMyChild()->mergeFormationOnWordOrSelection(fmt);
}
void MyWord::textUnderline(){
    QTextCharFormat fmt;
    fmt.setFontUnderline(underlineAct->isChecked());
    if(activeMyChild())
        activeMyChild()->mergeFormationOnWordOrSelection(fmt);
}
void MyWord::textAlign(QAction *a){
    if(activeMyChild()){
        if(a==leftAlignAct) activeMyChild()->setAlign(1);
        else if(a==centerAct) activeMyChild()->setAlign(2);
        else if(a==rightAlignAct) activeMyChild()->setAlign(3);
        else if(a==justifyAct) activeMyChild()->setAlign(4);
    }
}
void MyWord::textStyle(int styleIndex){
    if(activeMyChild())
        activeMyChild()->setStyle(styleIndex);
}
void MyWord::textFamily(const QString &f){
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    if(activeMyChild())
        activeMyChild()->mergeFormationOnWordOrSelection(fmt);
}
void MyWord::textSize(const QString &p){
    qreal pointsize = p.toFloat();
    if(p.toFloat()>0){
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointsize);
        if(activeMyChild())
            activeMyChild()->mergeFormationOnWordOrSelection(fmt);
    }
}
void MyWord::textColor(){
    if(activeMyChild()){
        QColor color = QColorDialog::getColor(activeMyChild()->textColor(),this);
        if(!color.isValid())return;
        QTextCharFormat fmt;
        fmt.setForeground(color);
        activeMyChild()->mergeFormationOnWordOrSelection(fmt);
        colorChanged(color);
    }
}
void MyWord::fontChanged(const QFont &f){
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));

    boldAct->setChecked(f.bold());
    italicAct->setChecked(f.italic());
    underlineAct->setChecked(f.underline());
}
void MyWord::colorChanged(const QColor &c){
    QPixmap pix(16,16);
    pix.fill(c);
    colorAct->setIcon(pix);
}
void MyWord::alignmentChanged(Qt::Alignment a){
    if(a&Qt::AlignLeft) leftAlignAct->setChecked(true);
    else if(a &Qt::AlignCenter) centerAct->setChecked(true);
    else if(a &Qt::AlignRight) rightAlignAct->setChecked(true);
    else if(a &Qt::AlignJustify) justifyAct->setChecked(true);
}
void MyWord::filePrint(){
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *pdlg = new QPrintDialog(&printer,this);
    if(activeMyChild()->textCursor().hasSelection())
        pdlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    pdlg->setWhatsThis(tr("打印文档"));
    if(pdlg->exec()==QDialog::Accepted)
        activeMyChild()->print(&printer);
    delete pdlg;
}
void MyWord::filePrintPreview(){
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer,this);
    connect(&preview,SIGNAL(paintRequested(QPrinter*)),SLOT(printPreview(QPrinter*)));
}
void MyWord::printPreview(QPrinter *printer){
    activeMyChild()->print(printer);
}
void MyWord::about(){
    QMessageBox::about(this,tr("关于"),tr("此软件是基于Qt5实现的文字处理软件！！！"));
}
