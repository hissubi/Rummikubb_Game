#include "class.h"
#include "extern.h"
#include <ncurses.h>
#include <wctype.h>
#include <wchar.h>
#include <locale.h>

card::card(){
        return;
}

card::card(int id_, int color_, int value_){
        id = id_;
        color = color_;
        value = value_;
	loc = -1;
}

int card::get_id(){
    return id;
}

int card::get_value(){
	return value;
}

void card::print_card(){
	attron(COLOR_PAIR(color+1));
	if(value == joker_value){
		//attron(A_BOLD);
		setlocale(LC_ALL,"");
		//printw(" ");
		const wchar_t* joker = L"\x1f608";
		addwstr(joker);
		//attroff(A_BOLD);
	}
	else printw("%2d", value);
	attroff(COLOR_PAIR(color+1));
}

void card::set_loc(int loc_){
	loc = loc_;
}

int card::get_loc(){
	return loc;
}


