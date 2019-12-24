#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "fg.hpp"
#include "graphviz_wrapper.hpp"


fg::string input_to_fg_str(std::string in){

    if(in.size()==0) throw std::runtime_error("ERROR! The input is empty!");
    fg::string ret;
    while(std::find(std::begin(in), std::end(in), '\n')!=std::end(in)){

    auto it=std::find(std::begin(in), std::end(in), '\n');
    fg::symbol sym;
    sym.insert(std::begin(sym), std::begin(in), std::next(it,1));
    in.erase(std::begin(in), std::next(it,1));
    ret.push_back(sym);
    }
    if(in.size()!=0) throw std::runtime_error("ERROR! The last symbol is not null-terminated!") ;
    return ret;
}

fg::alphabet fg_str_to_alph(fg::string str){
    fg::alphabet ret;
    for(auto _ : str){
        if(ret.find(_)!=std::end(ret)) throw std::runtime_error("ERROR! The same symbols in an alphabet are prohibited!");
        ret.insert(_);
    }
    str.clear();
    return ret;
}


 std::vector<std::string> set_of_rules_input_to_vec_of_rule_str(std::string in) {
    if(in.size()==0) throw std::runtime_error("ERROR! The input is empty!");
    const static  std::string rule_separator({'#','\n'});

     std::vector<std::string> ret;
    while(std::search(std::begin(in),std::end(in),std::begin(rule_separator),std::end(rule_separator))!=std::end(in)){

        auto it=std::search(std::begin(in),std::end(in),std::begin(rule_separator),std::end(rule_separator));
        std::string rule_str;
        rule_str.insert(std::begin(rule_str), std::begin(in), it);
        in.erase(std::begin(in),std::next(it,2));
        ret.push_back(rule_str);
    }
    if(!in.empty()) throw std::runtime_error("ERROR! The last rule is not terminated with #\n");
    return ret;
}

    fg::rule rule_str_to_rule(std::string str){
        if(str.size()==0) throw std::runtime_error("ERROR! The input is empty!");
        const static std::string l_r_separator({'-','>','\n'});

        auto it=std::search(std::begin(str),std::end(str),std::begin(l_r_separator),std::end(l_r_separator));
        if(it==std::end(str)) throw std::runtime_error("ERROR! No -> sepatator between left and right parts of a rule!");
        if(std::search(std::next(it,1),std::end(str),std::begin(l_r_separator),std::end(l_r_separator))!=std::end(str)) throw std::runtime_error("ERROR! More than 1 -> separator in a rule is prohibited!");
        fg::rule ret{input_to_fg_str(str.substr(0, std::distance(std::begin(str),it))), input_to_fg_str(str.substr(std::distance(std::begin(str),std::next(it,3)),std::distance(std::begin(str),std::end(str))))};
        str.clear();
    return ret;
    }

    std::set<fg::rule> vect_of_rule_str_to_set_of_rules(std::vector<std::string> vec){
        std::set<fg::rule> ret;
        for(auto _ : vec){
            auto rule=rule_str_to_rule(_);
            if(ret.find(rule)!=std::end(ret)) throw std::runtime_error("ERROR! The same rules in s set of rules are prohibited!");
            ret.insert(rule);
        }
        return ret;
    }

    fg::symbol input_to_fg_symbol(std::string in){
        if(in.size()==0) throw std::runtime_error("ERROR! The input is empty!");
        if(*(std::prev(std::end(in),1))!='\n') throw std::runtime_error("ERROR! The symbol is not null-terminated!");
        if(std::find(std::begin(in),std::end(in),'\n')!=std::prev(std::end(in),1)) throw std::runtime_error("ERROR! The symbol must contain only one null-terminator!");
        return in;
    }

    void fg_check(const fg::formal_grammar& fg){
        if(std::find_first_of(std::get<0>(fg).begin(),std::get<0>(fg).end(),std::get<1>(fg).begin(),std::get<1>(fg).end())!=std::get<0>(fg).end()) throw std::runtime_error("ERROR! V and W must not have any same symbols!");
        if(std::get<1>(fg).find(std::get<2>(fg))==std::get<1>(fg).end()) throw std::runtime_error("ERROR! W must contain I!");
    if(!std::all_of(std::get<3>(fg).cbegin(), std::get<3>(fg).cend(), [&,fg](const fg::rule& r){return fg::rule_is_correct(r, std::get<0>(fg),std::get<1>(fg));})) throw std::runtime_error("ERROR! Any symbol in R must be a member of W or V!");
    }









MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow), scene(new QGraphicsScene())
{

    ui->setupUi(this);
    ui->graphicsView->setScene(scene.data());
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::button_clicked);
    connect(ui->clean_Button, &QPushButton::clicked, this, &MainWindow::clean_button_clicked);
}





MainWindow::~MainWindow() = default;






void MainWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}





void MainWindow::clean_button_clicked(){
    ui->textEdit->clear();
    ui->textEdit_2->clear();
    ui->textEdit_3->clear();
    ui->textEdit_4->clear();
    scene->clear();
    ui->spinBox->clear();

}






void graph_builder( graphviz_wrapper::graph& g, const fg::formal_grammar& fg, const std::size_t& height){

    std::vector<std::pair<graphviz_wrapper::graph::node_iter,fg::string>> last_line, new_line;
    last_line.push_back(std::pair<graphviz_wrapper::graph::node_iter,fg::string>{g.insert_node(std::get<2>(fg)),fg::string{std::get<2>(fg)}});

    for(std::size_t i=0; i<height; ++i){
        for( auto _ : last_line){
        for(auto rule : std::get<3>(fg)){
            auto occurrence=fg::occurrence(_.second, rule);
            for(auto index : occurrence){
                     fg::string new_str(_.second);
                fg::substitution(new_str, rule, index);

                const auto& nd_set=g.get_ND_set();

                auto old_size=nd_set.size();
                auto new_it=g.insert_node(fg::fg_str_to_std_str(new_str));

                if(old_size!=nd_set.size()){
                    new_line.push_back(std::pair<graphviz_wrapper::graph::node_iter,fg::string>{new_it,new_str});
                    g.insert_edge(_.first,new_it);
                }
            }
        }
    }

    last_line.clear();
    std::swap(last_line, new_line);

    }
}

void graph_render(const graphviz_wrapper::graph& g, const QScopedPointer<QGraphicsScene>& scene){
    scene->clear();

    auto nd_set=g.get_ND_set();
    for(auto i=std::begin(nd_set); i!=std::end(nd_set); ++i){

        scene->addRect(g.get_ND_x(i)-g.get_ND_w(i)/2, g.get_ND_y(i)-g.get_ND_h(i)/2 ,g.get_ND_w(i),g.get_ND_h(i),QPen(Qt::blue),QBrush(Qt::yellow));

    auto t=new QGraphicsTextItem();
    t->setPos(g.get_ND_x(i)-g.get_ND_w(i)/2, g.get_ND_y(i)-g.get_ND_h(i)/2);
    t->setPlainText(g.get_ND_text(i).c_str());
    scene->addItem(t);
    }

    auto ed_set=g.get_ED_set();
    for(auto i=std::begin(ed_set); i!=std::end(ed_set); ++i){
        QPainterPath line;
    auto ed_coord=g.get_ED_coord(i);
        for(auto _ : ed_coord){
            if(_.sflag){
                line.moveTo(_.sp.x, _.sp.y);
                line.lineTo(_.points[0].x,_.points[0].y);
            }else line.moveTo(_.points[0].x, _.points[0].y);

            for(std::size_t j=0; j<_.size; j+=3 )
                line.cubicTo(_.points[0].x,_.points[0].y,_.points[1].x,_.points[1].y,_.points[2].x,_.points[2].y);

            if(_.eflag) line.lineTo(_.ep.x, _.ep.y);
            }
        scene->addPath(line, QPen(Qt::blue));
      }
}






void MainWindow::button_clicked()try{

    fg::formal_grammar fg;
    {
    fg::alphabet V(fg_str_to_alph(input_to_fg_str(std::string(ui->textEdit->toPlainText().toUtf8().constData()))));

    fg::alphabet W(fg_str_to_alph(input_to_fg_str(std::string(ui->textEdit_2->toPlainText().toUtf8().constData()))));

    fg::symbol   I(input_to_fg_symbol(ui->textEdit_3->toPlainText().toUtf8().constData()));

    std::set<fg::rule> R(vect_of_rule_str_to_set_of_rules(set_of_rules_input_to_vec_of_rule_str(ui->textEdit_4->toPlainText().toUtf8().constData())));

    fg={V,W,I,R};

    fg_check(fg);
    }


    graphviz_wrapper::graph g("FG_TREE");

    std::size_t height=ui->spinBox->value();

    graph_builder(g, fg, height);


    //1) вставка узлов
    //2) раскладка
    //3) получение текста из узла;

    graphviz_wrapper::Layout l(context,g);

    graph_render(g, scene);



}
catch (const std::exception& e){
    QMessageBox::information(this,"error",e.what());
}
catch (...){
clean_button_clicked();
QMessageBox::information(this, "UNKNOWN ERROR", "ERROR");
std::terminate();
}
