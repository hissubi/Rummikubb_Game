#include "class.h"
#include "extern.h"


player::player(){
	return;
}

player::player(int id_){
	player_id = id_;
	card_num = 0;
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
    for(size_t i=0; i<hand_card.size(); i++){
        for(size_t k=1; k<hand_card.size(); k++){
            if( is_bigger_by_color(hand_card[k-1], hand_card[k])  ){
                //cout << "before : " << hand_card[k-1]->id << " " << hand_card[k]->id << endl;
                card* tmp = hand_card.at(k);
                hand_card.at(k) = hand_card.at(k-1);
                hand_card.at(k-1) = tmp;                            
                //cout << "after : " << hand_card[k-1]->id << " " << hand_card[k]->id << endl;
            }                       
        }
    } 
    for(int i=0; i<card_num; i++){
        all_cards[hand_card[i]->id].print_card();
        cout << " ";                    
    }
    cout << endl; 
}

void player::sort_by_number(){
    for(int i=0; i<card_num; i++){
        for(int k=1; k<card_num; k++){
            if( is_bigger_by_number(hand_card[k-1], hand_card[k])  ){
                card* tmp = hand_card[k];
                hand_card[k] = hand_card[k-1];
                hand_card[k-1] = tmp;                                                   
            }                       
        }
    } 
    for(int i=0; i<card_num; i++){
        all_cards[hand_card[i]->id].print_card();
        cout << " ";                    
    }
    cout << endl; 
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

