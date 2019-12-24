#pragma once

#include <vector>
#include <string>
#include <set>

namespace fg {


using symbol=std::string;
using string=std::vector<symbol>;
using alphabet=std::set<symbol>;
using rule=std::pair<string,string>;
using formal_grammar=std::tuple<alphabet, alphabet, symbol, std::set<rule> >;

bool symbol_is_correct(const symbol& x);
bool string_is_correct(const string& x);
bool rule_is_correct(const rule& r, const alphabet& V, const alphabet& W);
bool alphabet_is_correct(const alphabet& x);
bool formal_grammar_is_correct(const formal_grammar& fg);



    void    substitution( string& str, const rule& r, std::size_t index );
    std::vector<std::size_t> occurrence(const string& str, const rule& r);
    std::string fg_str_to_std_str(const fg::string& x);
}



//workspace

//boost_text

//github boost text rope
//size t левое поддерево
//unique_ptr<char[]>
/*
    struct{}
    тегированные указатели
*/


//tuple заменить
//cmake
