#include <QApplication>
#include "mainwindow.hpp"

int main(int argc, char* argv[]) try{

    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
catch(...){ std::terminate();}
