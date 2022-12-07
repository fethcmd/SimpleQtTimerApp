#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QDateTime>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

QSqlDatabase db;
QTimer timer;
QDateTime dateTime;

int ms=0;
int sec=0;
int min=0;
int hour=0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&timer,SIGNAL(timeout()),this,SLOT(updateTime()));
    ui->dateEdit->setDateTime(dateTime.currentDateTime());

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database");
    db.open();
    db.exec("CREATE TABLE IF NOT EXISTS "
            "datatable(hour INTEGER,minute INTEGER,second INTEGER,savedate VARCHAR,savetime VARCHAR)");
    getRecordFromSql();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startStopButton_toggled(bool checked)
{
    if(checked){
        timer.start(1000);
        ui->startStopButton->setText("Stop");
    }else{
        timer.stop();
        ui->startStopButton->setText("Start");
    }
}

void MainWindow::on_saveButton_clicked()
{
    QSqlQuery q(db);
    q.prepare("INSERT INTO datatable(hour,minute,second,savedate,savetime) VALUES(?,?,?,?,?)");
    q.addBindValue(hour);
    q.addBindValue(min);
    q.addBindValue(sec);
    q.addBindValue(dateTime.currentDateTime().toString("dd.MM.yyyy"));
    q.addBindValue(dateTime.currentDateTime().toString("hh:mm:ss"));
    q.exec();
    getRecordFromSql(); //REFRESH LIST
    on_reStartButton_clicked(); //CLEAR CURRENT TIMER
}

void MainWindow::on_reStartButton_clicked()
{
    sec=0;
    min=0;
    hour=0;
    ui->timeEdit->setTime(QTime(hour,min,sec,ms));
}

void MainWindow::updateTime(){
    sec++;
    ui->timeEdit->setTime(QTime(hour,min,sec,ms));
    if (sec==60){
        sec=0;
        min++;
    }
    if(min==60){
        min=0;
        hour++;
    }if(hour==24){
        hour=0;
    }
}

void MainWindow::getRecordFromSql(){
    QSqlQuery q (db);
    q.prepare("SELECT * FROM datatable WHERE savedate = ? ");
    q.exec();
    QSqlRecord rec = q.record();
    int hour = rec.indexOf("hour");
    int min = rec.indexOf("minute");
    int sec = rec.indexOf("second");
    ui->listWidget->clear();
    while(q.next()){
        ui->listWidget->addItem(" "+q.value(hour).toString()+" hour "+q.value(min).toString()+" min "+q.value(sec).toString()+" sec");
    }
}



