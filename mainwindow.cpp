#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    this->ouvreBase();
    this->actualiseIcon();
    this->chargerListeFilms();
    QPalette p=ui->listWidgetFilms->palette();
    p.setColor(QPalette::Highlight, Qt::black);
    ui->listWidgetFilms->setPalette(p);

}

MainWindow::~MainWindow()
{
    this->base.close();
    delete ui;
}


void MainWindow::ouvreBase()
{
    base=QSqlDatabase::addDatabase("QSQLITE");

    base.setDatabaseName("BDDx264SF.db");

    //ouverture de la base de données
    base.open();

    //si la base ne s'est pas ouverte correctement
    if(base.isOpen() != true){
            QMessageBox::critical(this, "Erreur critique", "Impossible d'ouvrir la base de données.");
        }else{
            this->verifBase();
    }
}

void MainWindow::verifBase()
{
    this->base.exec("select titre, knowDate, dateSortie, plustard from film;");
    qDebug()<<this->base.lastError().number();
    if (this->base.lastError().number()>0)//si la requete renvoie une erreur
    {
        this->base.exec("begin;"); //début transaction SQL
        this->base.exec("create table film (titre varchar(20),knowDate bool, dateSortie date, plustard bool);");
        this->base.exec("insert into film values(\"Dandelion\",\"false\",null,\"true\");");
        this->base.exec("commit;"); //fin transaction
        qDebug()<<"création de la base de donné";
    }
}

void MainWindow::chargerListeFilms()
{
    this->on_pushButtonNew_clicked();
    ui->listWidgetFilms->clear();
    //requete
    QSqlQuery req1;
    req1.exec("select titre from film where knowDate=\"true\" and plustard=\"false\" and dateSortie<=current_date order by dateSortie desc;");
        while(req1.next())
        {
            QListWidgetItem* newItem= new QListWidgetItem(req1.value(0).toString());
            QBrush bruch= QBrush(QImage(":/list/green.png"));
            bruch.setTransform(QTransform::fromScale(1,2));// adapté au S4 mini
            newItem->setBackground(bruch);

            ui->listWidgetFilms->addItem(newItem);
        }
    QSqlQuery req3;
    req3.exec("select titre from film where knowDate=\"true\" and plustard=\"false\" and dateSortie>current_date order by dateSortie asc;");
        while(req3.next())
        {
            QListWidgetItem* newItem= new QListWidgetItem(req3.value(0).toString());
            QBrush bruch= QBrush(QImage(":/list/yellow.png"));
            bruch.setTransform(QTransform::fromScale(1,2));// adapté au S4 mini
            newItem->setBackground(bruch);
            ui->listWidgetFilms->addItem(newItem);
        }
    QSqlQuery req2;
    req2.exec("select titre from film where plustard=\"true\" order by titre;");
        while(req2.next())
        {
            QListWidgetItem* newItem= new QListWidgetItem(req2.value(0).toString());
            QBrush bruch= QBrush(QImage(":/list/blue.png"));
            bruch.setTransform(QTransform::fromScale(1,2));// adapté au S4 mini
            newItem->setBackground(bruch);
            ui->listWidgetFilms->addItem(newItem);
        }
    QSqlQuery req4;
    req4.exec("select titre from film where knowDate=\"false\" and plustard=\"false\";");
        while(req4.next())
        {
            QListWidgetItem* newItem= new QListWidgetItem(req4.value(0).toString());
            QBrush bruch= QBrush(QImage(":/list/red.png"));
            bruch.setTransform(QTransform::fromScale(1,2));// adapté au S4 mini
            newItem->setBackground(bruch);
            ui->listWidgetFilms->addItem(newItem);
        }
}

void MainWindow::actualiseIcon()
{
    if(ui->pushButtonLate->isChecked())
    {
        QIcon ico= QIcon(":/icon/active.png");
        //horloge bleu
        ui->pushButtonLate->setIcon(ico);
    }
    else
    {
        QIcon ico= QIcon(":/icon/desactive.png");
        //horloge blanche
        ui->pushButtonLate->setIcon(ico);
    }

    if(editerPasAjouter)
    {
        QIcon ico= QIcon(":/icon/edit.png");
        //icone modifier
        ui->pushButtonBingo->setIcon(ico);
    }
    else
    {
        QIcon ico= QIcon(":/icon/add.png");
        //icone ajouter
        ui->pushButtonBingo->setIcon(ico);
    }
}


void MainWindow::on_lineEditTitle_editingFinished()
{
    ui->pushButtonBingo->setFocus();
}

void MainWindow::on_radioButtonKnow_clicked()
{
    ui->radioButtonKnow->setChecked(true);
    if(ui->radioButtonKnow->isChecked())
    {
        ui->labelSlash1->setEnabled(true);
        ui->labelSlash2->setEnabled(true);
        ui->lineEditDay->setEnabled(true);
        ui->lineEditMonth->setEnabled(true);
        ui->lineEditYear->setEnabled(true);
        ui->pushButtonDateChoose->setEnabled(true);
    }
}

void MainWindow::on_radioButtonUnknow_clicked()
{
    ui->radioButtonUnknow->setChecked(true);
    if(ui->radioButtonUnknow->isChecked())
    {
        ui->labelSlash1->setEnabled(false);
        ui->labelSlash2->setEnabled(false);
        ui->lineEditDay->setEnabled(false);
        ui->lineEditMonth->setEnabled(false);
        ui->lineEditYear->setEnabled(false);
        ui->pushButtonDateChoose->setEnabled(false);
    }
}

void MainWindow::on_listWidgetFilms_clicked(const QModelIndex &index)
{
    editerPasAjouter=true;
    QString title= ui->listWidgetFilms->currentItem()->text();
    QSqlQuery req;
    req.exec("select titre, knowDate, dateSortie, plustard from film where titre=\""+title+"\";");
    req.first();
    ui->lineEditTitle->setText(req.value(0).toString());


    QDate date=QDate::fromString(req.value(2).toString(),"yyyy-MM-dd");
    ui->lineEditDay->setText(date.toString("dd"));
    ui->lineEditMonth->setText(date.toString("MM"));
    ui->lineEditYear->setText(date.toString("yyyy"));

    if(req.value(1)==true)
    {
        this->on_radioButtonKnow_clicked();
    }
    else
    {
        this->on_radioButtonUnknow_clicked();
    }
    if(req.value(3)==true)
    {
        ui->pushButtonLate->setChecked(true);
    }
    else
    {
        ui->pushButtonLate->setChecked(false);
    }
    this->actualiseIcon();

}

void MainWindow::on_pushButtonNew_clicked()
{
    ui->calendarWidget->setSelectedDate(QDate::currentDate());
    ui->lineEditDay->setText(ui->calendarWidget->selectedDate().toString("dd"));
    ui->lineEditMonth->setText(ui->calendarWidget->selectedDate().toString("MM"));
    ui->lineEditYear->setText(ui->calendarWidget->selectedDate().toString("yyyy"));
    ui->pushButtonOust->setEnabled(false);
    editerPasAjouter=false;
    ui->lineEditTitle->clear();
    this->on_radioButtonUnknow_clicked();
    ui->pushButtonLate->setChecked(false);
    this->actualiseIcon();
    ui->listWidgetFilms->clearSelection();
    ui->lineEditTitle->setFocus();
}

void MainWindow::on_pushButtonLate_clicked()
{
    this->actualiseIcon();
}

void MainWindow::on_pushButtonBingo_clicked()
{

    if(!ui->lineEditTitle->text().isEmpty())
    {
        //suppression des premiers espaces
        while(ui->lineEditTitle->text().at(0)==' ')
        {
                ui->lineEditTitle->setText(ui->lineEditTitle->text().remove(0,1));

                if(ui->lineEditTitle->text().isEmpty())
                {
                    return;
                }
        }
        QString texteDeBase=ui->lineEditTitle->text();


    QString knowledge="false";
    if(ui->radioButtonKnow->isChecked())
        knowledge="true";
    QString lateIsTheHour="false";
    if(ui->pushButtonLate->isChecked())
        lateIsTheHour="true";

    QDate DateSortie=QDate(ui->lineEditYear->text().toInt(),ui->lineEditMonth->text().toInt(),ui->lineEditDay->text().toInt());
    QString newDate=DateSortie.toString("yyyy-MM-dd");

    if(editerPasAjouter)
    {
        QString title= ui->listWidgetFilms->currentItem()->text();
        //modifier
        QSqlQuery req;
        req.exec("update film set titre=\""+texteDeBase+"\", knowDate=\""+knowledge+"\", dateSortie=\""+newDate+"\", plustard=\""+lateIsTheHour+"\" where titre=\""+title+"\";");
    }
    else
    {
        //ajouter
        QSqlQuery req;
        req.exec("insert into film values(\""+texteDeBase+"\",\""+knowledge+"\",\""+newDate+"\",\""+lateIsTheHour+"\");");
    }

    this->chargerListeFilms();
    }
}



void MainWindow::on_pushButtonOust_clicked()
{
       QString title= ui->listWidgetFilms->currentItem()->text();
       QSqlQuery req;
       req.exec("delete from film where titre=\""+title+"\";");
       this->chargerListeFilms();
       ui->pushButtonOust->setEnabled(false);
}

void MainWindow::on_listWidgetFilms_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    ui->pushButtonOust->setEnabled(false);
}

void MainWindow::on_listWidgetFilms_itemDoubleClicked(QListWidgetItem *item)
{
    ui->pushButtonOust->setEnabled(true);
}

void MainWindow::on_pushButtonDateOk_clicked()
{
    ui->lineEditDay->setText(ui->calendarWidget->selectedDate().toString("dd"));
    ui->lineEditMonth->setText(ui->calendarWidget->selectedDate().toString("MM"));
    ui->lineEditYear->setText(ui->calendarWidget->selectedDate().toString("yyyy"));
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButtonDateChoose_clicked()
{
    QString affDate=ui->lineEditYear->text()+"-"+ui->lineEditMonth->text()+"-"+ui->lineEditYear->text();
    ui->calendarWidget->setSelectedDate(QDate::fromString(affDate,"yyyy-MM-dd"));
    this->actualiseCalendar();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::actualiseCalendar()
{
    QString mois="Ce mois-ci";

    switch(ui->calendarWidget->monthShown())
    {
    case 1:{
        mois="Janvier";
    break;
    }
    case 2:{
        mois="Février";
    break;
    }
    case 3:{
        mois="Mars";
    break;
    }
    case 4:{
        mois="Avril";
    break;
    }
    case 5:{
        mois="Mai";
    break;
    }
    case 6:{
        mois="Juin";
    break;
    }
    case 7:{
        mois="Juillet";
    break;
    }
    case 8:{
        mois="Août";
    break;
    }
    case 9:{
        mois="Septembre";
    break;
    }
    case 10:{
        mois="Octobre";
    break;
    }
    case 11:{
        mois="Novembre";
    break;
    }
    case 12:{
        mois="Décembre";
    break;
    }
    Default:{
        mois="???";
    }
    }
    ui->labelDate->setText(mois+" "+QString::number(ui->calendarWidget->yearShown()));
}

void MainWindow::on_pushButtonDateInc_clicked()
{
    ui->calendarWidget->showNextMonth();
    this->actualiseCalendar();
}

void MainWindow::on_pushButtonDateDec_clicked()
{
    ui->calendarWidget->showPreviousMonth();
    this->actualiseCalendar();
}

void MainWindow::on_calendarWidget_currentPageChanged(int year, int month)
{
    this->actualiseCalendar();
}
