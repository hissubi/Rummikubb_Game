#include "class.h"
#include "extern.h"


player::player(){
	return;
}

player::player(int id_){
	player_id = id_;
	card_num = 0;
}

void player::sort_by_color(){
	/*vector<card*> res;
    card* tmp;
    for(size_t i=0; i<hand_card.size(); i++){
        int MIN=999;
        for(size_t i=0; i<hand_card.size(); i++){
            
        }
    } update sooooon~ */ 
    // sort and print "hand_card" //
}

void player::sort_by_number(){
    // sort and print "hand_card" //
}

bool player::get_is_register(){
    return is_register;
}
int player::get_player_id(){
    return player_id;
}
int player::get_card_num(){
	return card_num;
}
vector<card*> player::get_card(){
    return hand_card;
}
void player::add_card(card* _hand_card){
	hand_card.push_back(_hand_card);
	card_num++;
}
void player::copy(player p){
    is_register = p.get_is_register();
    player_id = p.get_player_id();
    card_num = p.get_card_num();
    hand_card.resize(card_num);
    for(int i = 0; i < card_num; i++){
        hand_card[i] = p.get_card().at(i);
    }
}

