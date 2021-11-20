#include "class.h"
checkpoint:: checkpoint(){
    player_data = NULL;
    nplayer = 0;
}
checkpoint:: checkpoint(int nplayer_){
    nplayer = nplayer_;
    player_data = new player[nplayer+1];
}

void checkpoint:: set_nplayer(int nplayer_){
    if(player_data != NULL) delete [] player_data;
	player_data = new player[nplayer_+1];
    nplayer = nplayer_;
	for(int i = 0; i < nplayer+1; i++)
	{
		player_data[i].set_player_id(i);
		player_data[i].set_is_register(false);
		player_data[i].set_card_num(0);
		player_data[i].get_card().clear();
	}
}
player checkpoint:: get_player_data(int player_num){
    return player_data[player_num];
}
board checkpoint:: get_board_data(){
    return board_data;
}

void checkpoint:: copy(int player_num, player p, board b){
    player_data[player_num].copy(p);
    board_data.copy(b);
}
void checkpoint:: copy_all(player* p, board b){
    for(int i = 1; i < nplayer+1; i++){
        player_data[i].copy(p[i]);
    }
    board_data.copy(b);
}
