#include "mychild.h"

#include<QtWidgets>

MyChild::MyChild() {
    setAttribute(Qt::WA_DeleteOnClose);
}

void MyChild::newFile(){
    static int sequenceNumber = 1;
    isUntitled = true;
    curFile = tr("Word文档=%1").arg(sequenceNumber++);
    setWindowTitle(curFile);
}

bool MyChild::loadFile(const QString &fileName){

    if(!fileName.isEmpty()){
        if(!QFile::exists(fileName))
            return false;

        QFile file(fileName);
        QByteArray data = file.readAll();

        QTextCodec *codec = Qt::codecForHtml(data);
        QString str = codec->toUnicode(data);

        if(Qt::mightBeRichText(str)){//富文本
            this->setHtml(str);
        }else{
            str = QString::fromLocal8Bit(data);
            this->setPlainText(str);
        }
        setCurrentFile(fileName);
        connect(document(),SIGNAL(contentsChange()),this,SLOT(documentWasModified()));
    }
    return true;
}

bool MyChild::save(){
    if(isUntitled){
        return saveAs();
    }else{
        return saveFile(curFile);
    }
}

bool MyChild::saveAs(){
    QString fileName = QFileDialog::getSaveFileName(this,tr("另存为"),curFile,tr("HTML 文档(*.html *.html);;所有文件(*.*)"));
    if(fileName.isEmpty())return false;
    return saveFile(fileName);
}

bool MyChild::saveFile(QString fileName){
    if(!(fileName.endsWith(".htm",Qt::CaseInsensitive)||fileName.endsWith(".html",Qt::CaseInsensitive))){
        fileName+=".html";
    }
    QTextDocumentWriter writer(fileName);
    bool success = writer.write(this->document());
    if(success) setCurrentFile(fileName);
    return success;
}


QString MyChild::userFriendlyCurrentFile(){
    return strippedName(curFile);
}

void MyChild::mergeFormationOnWordOrSelection(const QTextCharFormat &format){
    QTextCursor cursor = this->textCursor();
    if(!cursor.hasSelection()) cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    this->mergeCurrentCharFormat(format);
}//格式字体设置

void MyChild::setAlign(int align){
    if(align==1) this->setAlignment(Qt::AlignLeft|Qt::AlignAbsolute);
    else if(align==2) this->setAlignment(Qt::AlignCenter);
    else if(align==3) this->setAlignment(Qt::AlignRight|Qt::AlignAbsolute);
    else if(align==4)this->setAlignment(Qt::AlignJustify);

}

void MyChild::setStyle(int style){
    //多行文本框文本光标插入文本
    QTextCursor cursor = this->textCursor();
    if(style!=0){
        QTextListFormat::Style stylename = QTextListFormat::ListDisc;
        switch (style) {
        case 1:
            stylename = QTextListFormat::ListDisc;
            break;
        case 2:
            stylename = QTextListFormat::ListCircle;
            break;
        case 3:
            stylename = QTextListFormat::ListSquare;
            break;
        case 4:
            stylename = QTextListFormat::ListDecimal;
            break;
        case 5:
            stylename = QTextListFormat::ListLowerAlpha;
            break;
        case 6:
            stylename = QTextListFormat::ListUpperAlpha;
            break;
        case 7:
            stylename = QTextListFormat::ListLowerRoman;
            break;
        case 8:
            stylename = QTextListFormat::ListUpperRoman;
            break;
        default:
            break;
        }
        cursor.beginEditBlock();
        QTextBlockFormat blockFmt = cursor.blockFormat();
        QTextListFormat listFmt;
        if(cursor.currentList()){
            listFmt = cursor.currentList()->format();
        }else{
            listFmt.setIndent(blockFmt.indent()+1);
            blockFmt.setIndent(0);
            cursor.setBlockFormat(blockFmt);
        }
        listFmt.setStyle(stylename);
        cursor.createList(listFmt);
        cursor.endEditBlock();
    }else{
        QTextBlockFormat bfmt;
        bfmt.setObjectIndex(-1);
        cursor.mergeBlockFormat(bfmt);
    }
}

void MyChild::closeEvent(QCloseEvent *event){
    if(maybeSave()){
        event->accept();
    }else{
        event->ignore();
    }
}

void MyChild::documentWasModified(){
    //设置改变的时候，设置窗口已经修改
    setWindowModified(document()->isModified());
}


bool MyChild::maybeSave(){
    if(!document()->isModified())return true;

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this,tr("Qt word"),tr("文件'%1'已经被修改，是否保存吗？").arg(userFriendlyCurrentFile()),QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
    if(ret == QMessageBox::Save)return save();
    else if(ret==QMessageBox::Cancel)return false;
    return true;
}
void MyChild::setCurrentFile(const QString &fileName){
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile()+"[*]");
}
QString MyChild::strippedName(const QString &fullFileName){
    return QFileInfo(fullFileName).fileName();
}
