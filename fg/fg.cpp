#include <vector>
#include <string>
#include <set>
#include <algorithm>

namespace fg {



using symbol=std::string;
using string=std::vector<symbol>;
using alphabet=std::set<symbol>;
using rule=std::pair<string,string>;
using formal_grammar=std::tuple<alphabet, alphabet, symbol, std::set<rule> >;

bool symbol_is_correct(const symbol& x){
    return
    !x.empty()&&std::find(x.cbegin(), x.cend(), '\n')==(std::prev(std::end(x),1));
}
bool string_is_correct(const string& x){

    return !x.empty()&&std::all_of(x.cbegin(), x.cend(), [](const symbol& sym){return symbol_is_correct(sym);});
}

bool alphabet_is_correct(const alphabet& x){

    return !x.empty()&&std::all_of(x.cbegin(), x.cend(), [](const symbol& sym){return symbol_is_correct(sym);});
}

bool rule_is_correct(const rule& r, const alphabet& V, const alphabet& W){
    return  string_is_correct(r.first)&&
            string_is_correct(r.second)&&
            std::all_of(r.first.cbegin(), r.first.cend(), [&, V, W](const symbol& sym){return V.find(sym)!=V.end()||W.find(sym)!=W.end();})&&
            std::all_of(r.second.cbegin(), r.second.cend(), [&, V, W](const symbol& sym){return V.find(sym)!=V.end()||W.find(sym)!=W.end();});
}

bool formal_grammar_is_correct(const formal_grammar& fg){
    return  alphabet_is_correct(std::get<0>(fg))&&
            alphabet_is_correct(std::get<1>(fg))&&
            std::find_first_of(std::get<0>(fg).begin(),std::get<0>(fg).end(),std::get<1>(fg).begin(),std::get<1>(fg).end())==std::get<0>(fg).end()&&
//            ONLY C++20
//            std::get<1>(fg).contains(std::get<2>(fg));
            std::get<1>(fg).find(std::get<2>(fg))!=std::get<1>(fg).end()&&
            std::all_of(std::get<3>(fg).cbegin(), std::get<3>(fg).cend(), [&,fg](const rule& r){return rule_is_correct(r, std::get<0>(fg),std::get<1>(fg));});
}

    void   substitution( string& str, const rule& r, std::size_t index ){
        assert(index+r.first.size()<=str.size()&&"SUBSTITUTION_FAULT");
    str.erase(std::next(str.begin(), index), std::next(str.begin(), index+r.first.size() ) );
    str.insert(std::next(str.begin(),index), r.second.begin(), r.second.end());
}

    std::vector<std::size_t> occurrence(const string& str, const rule& r){

        std::vector<std::size_t> ret;   
        auto begin=std::begin(str);
    while(std::search(begin,std::end(str),std::begin(r.first),std::end(r.first))!=std::end(str)){
        auto it=std::search(begin,std::end(str),std::begin(r.first),std::end(r.first));
        ret.push_back(std::distance(std::begin(str), it));
        begin=std::next(it,1);

    }
        return ret;
    }

    std::string fg_str_to_std_str(const string& x){
        std::string ret;
        for(auto _ : x) ret+=_;
        return ret;
    }


}

