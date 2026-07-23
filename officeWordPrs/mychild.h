#ifndef MYCHILD_H
#define MYCHILD_H

#include <QTextEdit>

class MyChild : public QTextEdit
{
    Q_OBJECT
public:
    MyChild();
    // 新建、导入、保存、另存为文件
    void newFile();
    bool loadFile(const QString &fileName);
    bool save();
    bool saveAs();
    bool saveFile(QString filename);
    QString userFriendlyCurrentFile();
    QString currentFile(){return curFile;}
    void mergeFormationOnWordOrSelection(const QTextCharFormat &format);//格式字体设置
    void setAlign(int align);
    void setStyle(int style);
protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void documentWasModified();

private:
    QString curFile;
    bool isUntitled;
    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
};

#endif // MYCHILD_H
