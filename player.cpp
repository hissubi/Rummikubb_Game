#include "class.h"
#include "extern.h"


player::player(){
	return;
}

player::player(int id_){
	player_id = id_;
	card_num = 0;
}

void player::print_hand_by_color(){
	/**** todo : sort the cards ****/
	for(int i=0;i<card_num-1;i++){
		for(int j=i+1;j<card_num;j++){
			if((card_id[i]-1) % 13 > (card_id[j]-1) % 13){
				int temp = card_id[i];
				card_id[i] = card_id[j];
				card_id[j] = temp;
			}
		}
	}
	for(int i=0;i<card_num;i++){
		all_cards[card_id[i]].print_card();
		cout << " ";
	}
	cout << endl;
}

bool player::get_is_register(){
    return is_register;
}
int player::get_card_num(){
	return card_num;
}
vector<int> player::get_card(){
    return card_id;
}
void player::add_card(int _card_id){
	card_id.push_back(_card_id);
	card_num++;
}

