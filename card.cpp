#include "class.h"
#include "extern.h"
#include <ncurses.h>

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

void card::print_card(){
	//set_color(color);
     //   cout << value;
	//set_color(4);
	attron(COLOR_PAIR(color+1));
	printw("%2d", value);
	attroff(COLOR_PAIR(color+1));
}

void card::set_loc(int loc_){
	loc = loc_;
}

int card::get_loc(){
	return loc;
}


