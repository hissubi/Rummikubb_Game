#include "class.h"
#include "extern.h"
#include <ncurses.h>

player::player(){
	return;
}

player::player(int id_){
	player_id = id_;
	card_num = 0;
	is_register = false;
}

bool player::is_bigger_by_color(card* a, card* b){
    if(a->color > b->color) return true;
    if(a->color < b->color) return false;
    if(a->value > b->value) return true;
    return false;  
}

bool player::is_bigger_by_number(card* a, card* b){
    if(a->value > b->value) return true;
    if(a->value < b->value) return false;
    if(a->color > b->color) return true;
    return false;                
}

void player::sort_by_color(){
    for(int i=0; i<card_num; i++){
        for(int k=1; k<card_num; k++){
            if( is_bigger_by_color(hand_card[k-1], hand_card[k])  ){
                card* tmp = hand_card.at(k);
                hand_card.at(k) = hand_card.at(k-1);
                hand_card.at(k-1) = tmp;                            
            }                       
        }
    }
    for(int i=0; i<card_num; i++){
        hand_card[i]->print_card();
	    printw(" ");
		refresh();
    }
    printw("\n");
	refresh();
}

void player::sort_by_number(){
    for(int i=0; i<card_num; i++){
        for(int k=1; k<card_num; k++){
            if( is_bigger_by_number(hand_card[k-1], hand_card[k])  ){
                card* tmp = hand_card.at(k);
                hand_card.at(k) = hand_card.at(k-1);
                hand_card.at(k-1) = tmp;                                                   
            }                       
        }
    } 
    for(int i=0; i<card_num; i++){
        hand_card[i]->print_card();
        printw(" ");
		refresh();
    }
    printw("\n");
	refresh();
}

bool player::get_is_register(){
    return is_register;
}
void player::set_is_register(bool in){
	is_register = in;
}
int player::get_player_id(){
    return player_id;
}
void player::set_player_id(int id_){
	player_id = id_;
}
int player::get_card_num(){
	return card_num;
}
void player::set_card_num(int in){
	card_num = in;
}
vector<card*> player::get_card(){
    return hand_card;
}
void player::set_card(vector <card*> in){
	hand_card.clear();
	hand_card = in;
}
void player::add_card(card* _hand_card){
	hand_card.push_back(_hand_card);
	card_num++;
}
void player::copy(player p){
    is_register = p.get_is_register();
    player_id = p.get_player_id();
    card_num = p.get_card_num();
    if(card_num)
	{
		hand_card = p.get_card();
	}
	else
	{
		hand_card.clear();
	}
    /*
    hand_card.resize(card_num);
    for(int i = 0; i < card_num; i++){
        hand_card[i] = p.get_card().at(i);
    }*/
}

