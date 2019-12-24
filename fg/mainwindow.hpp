#pragma once

#include <QWidget>
#include <QScopedPointer>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainterPath>
#include <QGraphicsTextItem>
#include "graphviz_wrapper.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
private:
    QScopedPointer<Ui::MainWindow> ui;
    QScopedPointer<QGraphicsScene> scene;
    graphviz_wrapper::context context;
protected slots:
    void button_clicked();
    void clean_button_clicked();
};

