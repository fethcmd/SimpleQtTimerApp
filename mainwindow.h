#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startStopButton_toggled(bool checked);
    void on_saveButton_clicked();
    void on_reStartButton_clicked();
    void updateTime();
    void getRecordFromSql();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
