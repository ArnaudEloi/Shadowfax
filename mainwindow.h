#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <QMouseEvent>
#include <QListWidgetItem>
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:


    void on_lineEditTitle_editingFinished();

    void on_radioButtonKnow_clicked();

    void on_radioButtonUnknow_clicked();

    void on_listWidgetFilms_clicked(const QModelIndex &index);

    void on_pushButtonNew_clicked();

    void on_pushButtonLate_clicked();

    void on_pushButtonBingo_clicked();

    void on_pushButtonOust_clicked();

    void on_listWidgetFilms_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_listWidgetFilms_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButtonDateOk_clicked();

    void on_pushButtonDateChoose_clicked();

    void on_pushButtonDateInc_clicked();

    void on_pushButtonDateDec_clicked();

    void on_calendarWidget_currentPageChanged(int year, int month);

private:
    Ui::MainWindow *ui;
    QSqlDatabase base;
    void ouvreBase();
    void verifBase();
    void fermeBase();
    void chargerListeFilms();
    void actualiseIcon();
    bool editerPasAjouter=false;
    void actualiseCalendar();
};

#endif // MAINWINDOW_H
