#include "class.h"
#include "extern.h"

void save_log_data(int nplayers, int turn)
{
    FILE* log_file = fopen("Logfile.txt", "w");
    if(!turn){
        fprintf(log_file, "%d %d ", nplayers, turn);
    }
    else{
        fprintf(log_file, "%d %d ", nplayers, turn+1);
    }

    for(int i = 1; i < nplayers+1; i++)
    {
        fprintf(log_file, "%d ", i);
        if(players[i].get_is_register()) fprintf(log_file, "1 ");
        else fprintf(log_file, "0 ");

        fprintf(log_file, "%d ", players[i].get_card_num());
        for(int j = 0; j < players[i].get_card_num(); j++) {
            fprintf(log_file, "%d ", (players[i].get_card().at(j))->get_id() );
        }
    }
    fprintf(log_file, "%d ", table.get_num_rows());
    for(int i = 1; i <= table.get_num_rows(); i++) {
        fprintf(log_file, "%lu ", table.get_group().at(i).size());
        for(size_t j = 0; j < table.get_group().at(i).size(); j++) {
            fprintf(log_file, "%d ", table.get_group().at(i).at(j)->get_id());
        }
    }
    fclose(log_file);
}

void load_log_data(int& nplayers, int& turn)
{
	int trash;
	trash = 0;

    FILE* log_file = fopen("Logfile.txt", "r");
    trash = fscanf(log_file, "%d %d", &nplayers, &turn);

    for(int i = 1; i < nplayers+1; i++)
    {
        int now_player, is_register;
        trash = fscanf(log_file, "%d %d", &now_player, &is_register);
        if(is_register == 1) players[i].is_register = true;
        else players[i].is_register = false;

		players[i].hand_card.clear();
        trash = fscanf(log_file, "%d", &players[i].card_num);
        for(int j = 0; j < players[i].card_num; j++) {
            int card_id;
            trash = fscanf(log_file, "%d", &card_id);
            card* load_card = &all_cards[card_id];
            players[i].hand_card.push_back(load_card);
        }
    }

    trash = fscanf(log_file, "%d", &table.num_rows);
	vector <card*> buf;
	table.group.clear();
	buf.clear();
	table.group.push_back(buf);	
    for(int i = 1; i <= table.num_rows; i++) {
		buf.clear();
        int group_size;
        trash = fscanf(log_file, "%d", &group_size);
        for(int j = 0; j < group_size; j++) {
            int card_id;
            trash = fscanf(log_file, "%d", &card_id);
            card* newcard = &all_cards[card_id]; 
            buf.push_back(newcard);
        }
		table.group.push_back(buf);
    }
    fclose(log_file);

    for(int i = 1; i < nplayers+1; i++){
        for(int j = 0; j < players[i].card_num; j++){
            auto deck_it = find(deck.begin(), deck.end(), players[i].hand_card[j]->get_id());
            deck.erase(deck_it);
        }
    }
    for(int i = 1; i <= table.num_rows; i++){
        for(size_t j = 0; j < table.group[i].size(); j++){
            auto deck_it = find(deck.begin(), deck.end(), table.group[i][j]->get_id());
            deck.erase(deck_it);
        } 
    }
	//todo : rearrange deck
}
