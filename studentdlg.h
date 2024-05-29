#ifndef STUDENTDLG_H
#define STUDENTDLG_H

#include <QDialog>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include<QSqlError>
#include<QDebug>
#include<QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class StudentDlg; }
QT_END_NAMESPACE

class StudentDlg : public QDialog
{
    Q_OBJECT

public:
    StudentDlg(QWidget *parent = nullptr);
    ~StudentDlg();
private:
    void CreateDatabaseFunc();
    void CreateTableFunc();
    void QueryTableFunc(QString queryStr);
    QSqlDatabase sqldb;
    QSqlQueryModel sqlmodel;

private slots:
    void on_pushButtonSort_clicked();

    void on_pushButton_INSERT_clicked();

    void on_pushButton_DELETE_clicked();

    void on_pushButton_UPDATE_clicked();

    void on_pushButton_SEARCH_clicked();

private:
    Ui::StudentDlg *ui;

};
#endif // STUDENTDLG_H
