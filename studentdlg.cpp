#include "studentdlg.h"
#include "ui_studentdlg.h"
#include<QMessageBox>
StudentDlg::StudentDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StudentDlg)
{
    ui->setupUi(this);
    //创建数据库
    CreateDatabaseFunc();
    //创建表，第一次执行时使用即可
//    CreateTableFunc();
    //进入时直接加载整个表
    QueryTableFunc(QString("SELECT id, name, score FROM student"));
}

StudentDlg::~StudentDlg()
{
    delete ui;
}
//链接数据库
void StudentDlg::CreateDatabaseFunc()
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    //地址
    db.setHostName("127.0.0.1");
    //用户名
    db.setUserName("root");
    //密码
    db.setPassword("zk123456");
    //数据库名
    db.setDatabaseName("qtmysql");
    if(!db.open()){
        QMessageBox::warning(this,"错误",db.lastError().text());
        return;
    }
    else {
         qDebug() << "成功链接到数据库";
    }
}
//创建表
void StudentDlg::CreateTableFunc()
{
    QSqlQuery createquery;
    QString strsql=QString("create table student("
                           "id int primary key not null,"
                           "name text not null,"
                           "score real not null)"
                           );
    if(!createquery.exec(strsql)){
        QMessageBox::warning(this,"错误",createquery.lastError().text());
        return;
    }
    else {
         qDebug() << "成功创建表";
    }
}
//将数据放在表中，参数是查询的语句
void StudentDlg::QueryTableFunc(QString queryStr)
{
    QSqlQuery sqlQuery;
    // 执行语句
    if (!sqlQuery.exec(queryStr)) {
        QMessageBox::warning(this, "错误", "执行语句失败：" + sqlQuery.lastError().text());
        return;
    }

    // 清空现有的表格内容，以防重复插入
    ui->tableWidget->setRowCount(0);

    // 遍历结果，将每一行数据添加到tableWidget中
    while (sqlQuery.next()) {
        int id = sqlQuery.value(0).toInt(); // 假设id是整型
        QString name = sqlQuery.value(1).toString(); // 假设name是字符串
        double score = sqlQuery.value(2).toDouble(); // 假设score是浮点型

        // 在tableWidget中插入新的一行
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rowCount);

        // 设置单元格的值
        QTableWidgetItem *newItemID = new QTableWidgetItem(QString::number(id));
        QTableWidgetItem *newItemName = new QTableWidgetItem(name);
        QTableWidgetItem *newItemScore = new QTableWidgetItem(QString::number(score));

        // 设置每个单元格内容居中对齐
        newItemID->setTextAlignment(Qt::AlignCenter);
        newItemName->setTextAlignment(Qt::AlignCenter);
        newItemScore->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->setItem(rowCount, 0, newItemID);
        ui->tableWidget->setItem(rowCount, 1, newItemName);
        ui->tableWidget->setItem(rowCount, 2, newItemScore);
    }
}

//排序
void StudentDlg::on_pushButtonSort_clicked()
{
    int condIndex=ui->comboBoxCond->currentIndex();
    QString condStr;
    if(condIndex==0){
        condStr="asc";
        }
    else{
        condStr="desc";
    }
    QString sqlQuery=QString("select * from student "
                             "order by %1 %2").arg(ui->comboBoxValue->currentText()).arg(condStr);
    QueryTableFunc(sqlQuery);
}

void StudentDlg::on_pushButton_INSERT_clicked()
{
    QSqlQuery sqlquery;
    int id=ui->lineEdit_ID->text().toInt();
    if(id==0){
        QMessageBox::warning(this,"错误","学生学号不能为0！");
        return;
    }
    QString name=ui->lineEdit_NAME->text();
    if(name==""){
        QMessageBox::warning(this,"错误","学生姓名不能为空！");
        return;
    }
    double score=ui->lineEdit_SCORE->text().toDouble();
    if(score<0||score>100){
        QMessageBox::warning(this,"错误","学生成绩范围为（0-100）！");
        return;
    }
    QString strs=QString("insert into student "
                         "values(%1,'%2',%3)").arg(id).arg(name).arg(score);
    if(!sqlquery.exec(strs)){
        QMessageBox::warning(this,"错误","插入数据失败");
        return;
    }
    QueryTableFunc(QString("SELECT id, name, score FROM student"));
}

void StudentDlg::on_pushButton_DELETE_clicked()
{
    QSqlQuery sqlquery;
    int id=ui->lineEdit_ID->text().toInt();
    if(id==0){
        QMessageBox::warning(this,"错误","学生学号不能为0！");
        return;
    }
     //查询数据库中是否有这个数据
    QString str=QString("Select id from student "
                        "where id=%1").arg(id);
    if(sqlquery.exec(str)) {
        if(sqlquery.next()) {
                QString queryStr=QString("Delete from student "
                                     "where id=%1").arg(id);
                if(!sqlquery.exec(queryStr)){
                    QMessageBox::warning(this,"错误","删除数据失败");
                    return;
                }
                else {
                     QMessageBox::information(this,"成功","删除数据成功");
                     QueryTableFunc(QString("SELECT id, name, score FROM student"));
                }
            }
        else{
            QMessageBox::information(this, "信息", "未找到该学生学号对应的记录，无需删除");
            return;
        }
    }
}

void StudentDlg::on_pushButton_UPDATE_clicked()
{
    QSqlQuery sqlQuery;
    int id=ui->lineEdit_ID->text().toInt();
    if(id==0){
        QMessageBox::warning(this,"错误","学生学号不能为0！");
        return;
    }
    QString name=ui->lineEdit_NAME->text();
    if(name==""){
        QMessageBox::warning(this,"错误","学生姓名不能为空！");
        return;
    }
    double score=ui->lineEdit_SCORE->text().toDouble();
    if(score<0||score>100){
        QMessageBox::warning(this,"错误","学生成绩范围为（0-100）！");
        return;
    }
    QString queryStr = QString("update student "
                               "set name='%1',score=%2 "
                               "where id=%3").arg(name).arg(score).arg(id);

   QueryTableFunc(queryStr);
   QueryTableFunc(QString("SELECT id, name, score FROM student"));
}
void StudentDlg::on_pushButton_SEARCH_clicked()
{
    QSqlQuery sqlQuery;
    QString queryStr;
    int id=ui->lineEdit_ID->text().toInt();
    //如果为空，就查询全部数据
    if(id==0){
        queryStr=QString("SELECT id, name, score FROM student");
    }
   // 准备SQL查询语句，例如查询所有学生的信息
   queryStr = QString("SELECT id, name, score FROM student "
                      "where id=%1").arg(id);
   QueryTableFunc(queryStr);
}
