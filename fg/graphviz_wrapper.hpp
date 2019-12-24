#pragma once

#include <gvc.h>
#include <string>
#include <set>
#include <vector>


namespace graphviz_wrapper{

namespace  {
    inline char* str_to_c_style(const std::string& str){
        return const_cast<char*>(str.c_str());
    }
}

class context{
public:
 context(){
        gvc=gvContext();
 }
 context(context&& other){
     *this=std::forward<context&&>(other);
 }
 const context& operator=(context&& other) {
     destruct();
     gvc=other;
     other.gvc=nullptr;
     return *this;
 }
 operator GVC_t*()const{
        return gvc;
    }
    ~context(){
     destruct();
     }

private:

 void destruct(){
    if(gvc!=nullptr) {
        gvFreeContext(gvc);
    }
 }

    GVC_t* gvc;
};









class graph{
public:
    using node_iter=std::set<Agnode_t*>::iterator;
    using edge_iter=std::set<Agedge_t*>::iterator;

 graph(const std::string& graphName){
        g=agopen(str_to_c_style(graphName), Agstrictdirected, nullptr);
    }
 graph(graph&& other){
     *this=std::forward<graph&&>(other);
 }
 const graph& operator=(graph&& other){
     destruct();
     g=other;
     for(auto _ : other.edges) edges.insert(_);
     for(auto _ : other.nodes) nodes.insert(_);

     other.g=nullptr;
     other.edges.clear();
     other.nodes.clear();

     return *this;
 }

 operator Agraph_t*()const{
        return g;
    }

    ~graph(){
     destruct();
    }

   node_iter insert_node(const std::string& nodeName="\n"){
       return nodes.insert( agnode(g, str_to_c_style(nodeName), true)).first;
    }

   void erase_node(const node_iter& it){
       nodes.erase(it);
   }

   edge_iter insert_edge(const node_iter& tail, const node_iter& head, const std::string& edgeName="\n"){
       return edges.insert(agedge(g, *tail, *head, str_to_c_style(edgeName), true)).first;
   }

   void erase_edge(const edge_iter& it){
       edges.erase(it);
   }

 //params
   double get_ND_x(const node_iter& iter)const{
       return ND_coord(*iter).x;
   }
   double get_ND_y(const node_iter& iter)const{
       return GD_bb(g).UR.y-ND_coord(*iter).y;
   }
   double get_ND_w(const node_iter& iter)const{
       return ND_width(*iter)*72.;
   }
   double get_ND_h(const node_iter& iter)const{
       return ND_height(*iter)*72.;
   }

   //только после раскладки!
    std::string get_ND_text( const node_iter& iter)const{
       return ND_label(*iter)->text;
   }

    const std::set<Agnode_t *>& get_ND_set()const{
        return nodes;
    }


    const auto& get_ED_set()const{
        return edges;
    }

    struct point{
        double x=0;
        double y=0;
    };
    struct spline{
      bool sflag=0;
      bool eflag=0;
      point sp, ep;
      std::size_t size=0;
      std::vector<point> points;
    };


    std::vector<spline> get_ED_coord(const edge_iter& iter)const{
        std::vector<spline> ret;
        auto splines_old=ED_spl(*iter);
        for(std::size_t i=0; i<splines_old->size; ++i){
            const auto& s_old=splines_old->list[i];
            spline s_new;
            s_new.sflag=s_old.sflag;
            s_new.eflag=s_old.eflag;
            s_new.sp={s_old.sp.x,GD_bb(g).UR.y-s_old.sp.y};
            s_new.ep={s_old.ep.x,GD_bb(g).UR.y-s_old.ep.y};
            s_new.size=s_old.size;
            for(std::size_t j=0; j<s_old.size; ++j){
                s_new.points.push_back(point{s_old.list[j].x, GD_bb(g).UR.y - s_old.list[j].y});
            }
        ret.push_back(s_new);
        }
        return ret;
    }

    double get_ND_fontsize(const node_iter& iter)const{
        return ND_label(*iter)->fontsize;
    }

    std::string get_ND_fontname(const node_iter& iter)const{
        return ND_label(*iter)->fontname;
    }

    std::string get_ND_fontcolor(const node_iter& iter)const{
        return ND_label(*iter)->fontcolor;
    }

private:
   void destruct(){
       if(g!=nullptr){
          for(auto _ : edges) agdeledge(g,_);
          for(auto _ : nodes) agdelnode(g,_);
          agclose(g);
    }
   }

    Agraph_t* g;
    std::set<Agnode_t*> nodes;
    std::set<Agedge_t*> edges;
};













class Layout{
    public:
    Layout(const context& cntxt, const graph& grph, const std::string& mode="circo"): gvc(cntxt), g(grph){
        gvLayout(gvc, g, str_to_c_style(mode));
    }
    Layout(Layout&& other){
        *this=std::forward<Layout&&>(other);
    }
    const Layout& operator =(Layout&& other){
        destruct();

        gvc=other.gvc;
        g=other.g;

        other.gvc=nullptr;
        other.g=nullptr;
        return *this;
    }
    ~Layout(){
        destruct();
    }

private:
void destruct(){
        if(gvc!=nullptr&&g!=nullptr){
            gvFreeLayout(gvc, g);
        }
    }

    GVC_t* gvc;
    Agraph_t* g;
};


}
