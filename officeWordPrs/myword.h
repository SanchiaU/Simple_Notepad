#ifndef MYWORD_H
#define MYWORD_H

#include <QMainWindow>
#include<QPrintDialog>
#include<QPrinter>
#include<QPrintPreviewDialog>
#include <QSignalMapper>
#include <QAction>
#include <QMdiArea>
#include<QMdiSubWindow>
#include <QMessageBox>  // 为 aboutQt() 函数
#include <QFileInfo>    // 为 findMyChild() 函数
#include <QTextCursor>  // 如果用到 textCursor()

#include "mychild.h"
class MyChild;
class QAciton;
class QMenu;//菜单栏
class QComboBox;
class QFontComboBox;
class QMdiArea;//同时显示多个文档
class QMidSubWindow;//MDI框架,QMdiArea用于主容器中，用于容纳多个子容器QMdiSubWindow
class QSignalMapper;//收集无参信号

class MyWord : public QMainWindow
{
    Q_OBJECT

public:
    MyWord(QWidget *parent = nullptr);
    ~MyWord();
protected:
    void closeEvent(QCloseEvent *event);
private:
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void filePrint();
    void filePrintPreview();
    void printPreview(QPrinter *);

    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void about();

    void textBold();
    void textItalic();
    void textUnderline();
    void textAlign(QAction *a);
    void textStyle(int styleIndex);
    void textFamily(const QString &f);
    void textSize(const QString &p);
    void textColor();

    void updateMenus();
    void updateWindowMenu();
    void setActiveSubWindow(QWidget *window);
    MyChild *createMyChild();
private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void enabledText();
    void fontChanged(const QFont &f);
    void colorChanged(const QColor &c);
    void alignmentChanged(Qt::Alignment a);
    MyChild *activeMyChild();
    QMdiSubWindow *findMyChild(const QString &filename);

    QMdiArea *mdiArea;
    QSignalMapper *windowMapper;

    //定义菜单
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *formatMenu;
    QMenu *fontMenu;
    QMenu *alignMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;

    //工具栏
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *formatToolBar;
    QToolBar *comboToolBar;//
    QFontComboBox *comboFont;
    QComboBox *comboStyle;
    QComboBox *comboSize ;

    //菜单动作
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *printAct;
    QAction *printPreviewAct;
    QAction *exitAct;

    QAction *undoAct;
    QAction *redoAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;

    QAction *boldAct;
    QAction *italicAct;
    QAction *underlineAct;
    QAction *leftAlignAct;
    QAction *centerAct;
    QAction *rightAlignAct;
    QAction *justifyAct;
    QAction *colorAct;

    QAction *closeAct;
    QAction *closeAllAct;
    QAction *tileAct;
    QAction *cascadeAct;
    QAction *nextAct;
    QAction *previousAct;
    QAction *separatorAct;

    QAction *aboutAct;
    QAction *aboutQtAct;

};
#endif // MYWORD_H
