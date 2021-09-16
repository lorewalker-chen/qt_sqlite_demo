#include "main_window.h"

#include <QApplication>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    QSqlDatabase database;

    //建立连接
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        database = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("MyDataBase.db");
    }

    //打开数据库
    if (!database.open()) {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    } else {
        qDebug() << "Succeed to connect database." ;
    }

    //创建表格
    QSqlQuery sql_query;
    if (!sql_query.exec("create table student(id int primary key, name text, age int)")) {
        qDebug() << "Error: Fail to create table." << sql_query.lastError();
    } else {
        qDebug() << "Table created!";
    }

    //插入数据
    if (!sql_query.exec("INSERT INTO student VALUES(1, \"AAA\", 23)")) {
        qDebug() << sql_query.lastError();
    }
    if (!sql_query.exec("INSERT INTO student VALUES(2, \"BBB\", 24)")) {
        qDebug() << sql_query.lastError();
    }

    //修改数据
    sql_query.exec("update student set name = \"QQQ\" where id = 2");
    if (!sql_query.exec()) {
        qDebug() << sql_query.lastError();
    } else {
        qDebug() << "updated!";
    }

    //查询数据
    sql_query.exec("select * from student");
    if (!sql_query.exec()) {
        qDebug() << sql_query.lastError();
    } else {
        while (sql_query.next()) {
            int id = sql_query.value(0).toInt();
            QString name = sql_query.value(1).toString();
            int age = sql_query.value(2).toInt();
            qDebug() << QString("id:%1    name:%2    age:%3").arg(id).arg(name).arg(age);
        }
    }

    //删除数据
    sql_query.exec("delete from student where id = 1");
    if (!sql_query.exec()) {
        qDebug() << sql_query.lastError();
    }

    //删除表格
    sql_query.exec("drop table student");
    if (sql_query.exec()) {
        qDebug() << sql_query.lastError();
    }

    //关闭数据库
    database.close();

    return a.exec();
}
