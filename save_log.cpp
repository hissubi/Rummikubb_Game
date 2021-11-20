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
    for(int i = 0; i < table.get_num_rows(); i++) {
        fprintf(log_file, "%lu ", table.get_group().at(i).size());
        for(size_t j = 0; j < table.get_group().at(i).size(); j++) {
            fprintf(log_file, "%d ", table.get_group().at(i).at(j)->get_id());
        }
    }
    fclose(log_file);
}

void load_log_data(int& nplayers, int& turn)
{
    FILE* log_file = fopen("Logfile.txt", "r");
    fscanf(log_file, "%d %d", &nplayers, &turn);

    for(int i = 1; i < nplayers+1; i++)
    {
        int now_player, is_register;
        fscanf(log_file, "%d %d", &now_player, &is_register);
        if(is_register) players[i].is_register = true;
        else players[i].is_register = false;

        fscanf(log_file, "%d", &players[i].card_num);
        for(int j = 0; j < players[i].card_num; j++) {
            int card_id;
            fscanf(log_file, "%d", &card_id);
            card* load_card = &all_cards[card_id];
            players[i].hand_card.push_back(load_card);
        }
    }

    fscanf(log_file, "%d", &table.num_rows);
    table.group.resize(table.num_rows);
    for(int i = 0; i < table.num_rows; i++) {
        int group_size;
        fscanf(log_file, "%d", &group_size);
        table.group[i].resize(group_size);
        for(int j = 0; j < group_size; j++) {
            int card_id;
            fscanf(log_file, "%d", &card_id);
            card* newcard = &all_cards[card_id]; 
            table.group[i].push_back(newcard);
        }
    }
    saved_checkpoint.copy_all(players, table);
    fclose(log_file);

}
