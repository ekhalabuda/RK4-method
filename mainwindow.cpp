#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <functional>
#include <QtMath>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->lineEdit_2->setText(QString::number(0.1));
    I0 = 0.1;
    x0 = 0;
    ui->lineEdit_4->setText(QString::number(2));
    xn = 2;
    ui->lineEdit_5->setText(QString::number(0.01));
    h = 0.01;
    ui->lineEdit_6->setText(QString::number(0.00001));
    precision = 0.00001;
    ui->lineEdit_7->setText(QString::number(1000000));
    maxim_step = 1000000;
    ui->lineEdit_8->setText(QString::number(0.05));
    eps = 0.05;
    ui->lineEdit_9->setText(QString::number(1));
    R = 1;
    ui->lineEdit_10->setText(QString::number(1));
    V = 1;
    ui->lineEdit_11->setText(QString::number(1));
    L = 1;
    ui->textEdit->setText("Вариант №5 Задание №9 Команда №2 Халабуда \n"
                          "Метод Рунге-Кутта явный порядка p=4 \n");


    ui->tableWidget->setColumnCount(11);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "i" << "xi" << "hi" << "Vi" << "V^i" << "Vi-V^i" << "|ОЛП|" << "Деления" << "Удвоения" << "Ui" << "|Ui-Vi|");

    ui->tableWidget_2->setColumnCount(10);
    ui->tableWidget_2->setHorizontalHeaderLabels(QStringList() << "Итераций" << "xn" << "Vn"<< "b-xn" << "Макс. |ОЛП|" << "Делений" << "Удвоений" << "Макс. шаг" << "x при макс.шаге" << "Мин. шаг" << "x при мин.шаге" << "Макс. |ГП|" );

    ui->widget->xAxis->setLabel("Время х");
    ui->widget->yAxis->setLabel("Сила тока I");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{

    I0 = ui->lineEdit_2->text().toDouble();
    xn = ui->lineEdit_4->text().toDouble();
    h = ui->lineEdit_5->text().toDouble();
    precision = ui->lineEdit_6->text().toDouble();
    maxim_step = ui->lineEdit_7->text().toInt();
    eps = ui->lineEdit_8->text().toDouble();
    R = ui->lineEdit_9->text().toDouble();
    V = ui->lineEdit_10->text().toDouble();
    L = ui->lineEdit_11->text().toDouble();
    dif_step = ui->comboBox->currentIndex() == 0 ? true : false;
    std::string s = dif_step == 1 ? "с контролем локальной погрешности" : "без контроля локальной погрешности";
    QString qs = QString::fromStdString(s);

    ui->textEdit->setText("Вариант №5 Задание №9 Команда №2 Халабуда \n"
                          "Метод Рунге-Кутта явный порядка p=4 \n "
                          + qs + "\n \n"
                          "Начальное значение силы тока :"+QString::number(I0)+"A \n"
                          "Начальное время счёта :"+QString::number(x0)+"c \n"
                          "Конечное время счёта :"+QString::number(xn)+"c \n"
                          "Сопротивление :"+QString::number(R)+"Ом \n"
                          "Напряжение в цепи :"+QString::number(V)+"В \n"
                          "Коэффициент самоиндукции :"+QString::number(L)+"Гн \n");
}

void MainWindow::on_pushButton_3_clicked()
{
    // находим и строим численное решение
    double r = R;
    double vv = V;
    double l = L;
    std::function<double(double, double)> func = [r, vv, l](double x, double I){return (vv-r*I)/l;};
    std::pair<std::vector<double>, std::vector<double>> p = numcpp::RK4(x0, xn, I0, func, R, V, L, h, precision, maxim_step, dif_step, eps);

    QVector<double> x = QVector<double>::fromStdVector(p.first);
    QVector<double> v = QVector<double>::fromStdVector(p.second);

    double max_v = *max_element(p.second.begin(), p.second.end());
    double min_v = *min_element(p.second.begin(), p.second.end());
    //ui->widget->clearGraphs();
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget_2->clear();
    ui->tableWidget_2->setRowCount(0);
    ui->widget->xAxis->setRange(x0, xn);
    ui->widget->yAxis->setRange(min_v, max_v);

    ui->tableWidget->setColumnCount(11);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "i" << "xi" << "hi" << "Vi" << "V^i" << "Vi-V^i" << "|ОЛП|" << "Деления" << "Удвоения" << "Ui" << "|Ui-Vi|");

    ui->tableWidget_2->setColumnCount(10);
    ui->tableWidget_2->setHorizontalHeaderLabels(QStringList() << "Итераций" << "xn" << "Vn"<< "b-xn" << "Макс. |ОЛП|" << "Делений" << "Удвоений" << "Макс. шаг" << "x при макс.шаге" << "Мин. шаг" << "x при мин.шаге" << "Макс. |ГП|" );



    ui->widget->addGraph();
    ui->widget->graph(graph_count)->setPen(QPen(QColor("red")));
    ui->widget->graph(graph_count++)->addData(x, v);
    ui->widget->rescaleAxes();
    ui->widget->replot();


    std::string tmp;
    // Выводим таблицу
    int i = 0;
    std::ifstream table("Table.txt");
    while(getline(table, tmp)){
        ui->tableWidget->insertRow(i);
        std::vector<std::string> output;

        size_t j = 0;
        std::string p;
        while(tmp[j]){
            if(tmp[j]==' '){
                output.push_back(p);
                p = "";
            }
            else {
                p += tmp[j];
            }
            j++;
        }
         output.push_back(p);

        for(size_t j = 0; j < output.size(); j++){
            QTableWidgetItem *tbl = new QTableWidgetItem(QString::fromStdString(output[j]));
            ui->tableWidget->setItem(i, j, tbl);
        }
       i++;

    }

    table.close();
    // выводим справку метода
    std::ifstream final_data("Final_data.txt");
    getline(final_data, tmp);
    ui->tableWidget_2->insertRow(0);
    std::vector<std::string> output1;

    size_t j = 0;
    std::string p1;
    while(tmp[j]){
        if(tmp[j]==' '){
            output1.push_back(p1);
            p1 = "";
        }
        else {
            p1 += tmp[j];
        }
        j++;
    }
     output1.push_back(p1);

    for(size_t j = 0; j < output1.size(); j++){
        QTableWidgetItem *tbl = new QTableWidgetItem(QString::fromStdString(output1[j]));
        ui->tableWidget_2->setItem(0, j, tbl);
    }

    final_data.close();



}


void MainWindow::on_pushButton_4_clicked()
{
    // находим и строим численное решение
    double r = R;
    double vv = V;
    double l = L;
    std::function<double(double, double)> func = [r, vv, l](double x, double I){return (vv-r*I)/l;};
    std::pair<std::vector<double>, std::vector<double>> p = numcpp::RK4(x0, xn, I0, func, R, V, L, h, precision, maxim_step, dif_step, eps);

    QVector<double> x = QVector<double>::fromStdVector(p.first);
    //читаем из файла точное решение
    std::string tmp;
    QVector<double> u;
    std::ifstream exact_values("Exact_values.txt");
    while(getline(exact_values, tmp)){
        u.append(std::stod(tmp));
    }
    // строим его
    ui->widget->addGraph();
    ui->widget->graph(graph_count)->setPen(QPen(QColor("green")));
    ui->widget->graph(graph_count++)->addData(x, u);
    ui->widget->rescaleAxes();
    ui->widget->replot();
    exact_values.close();

}

void MainWindow::on_pushButton_5_clicked()
{
    ui->widget->clearGraphs();
    graph_count = 0;
    ui->widget->replot();
}
