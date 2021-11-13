#include "cmdkub.h"
#include <time.h>
#include <string>
#include <random>
#include <algorithm>
#include "class.h"
#include <iterator>
#include <iostream>
#include <vector>

#define init_card_num	14

void set_color(int color);
void distribute_initial_card(int player_id);
int pop_card_from_deck();
void save_log_data(int nplayers, int turn);
void load_log_data(int& nplayers, int& turn);

using namespace std;

player players[5];
vector<int> deck;
card all_cards[13*8+3];
board table;
checkpoint saved_checkpoint;

int main(){
    char load;
    int num_players;
	int turn = 0;

	for(int i=0;i<8;i++){
		for(int j=1;j<=13;j++){
			int id = i*13+j;
			all_cards[id] = card(id,i/2,j);
			deck.push_back(id);
		}
	}

	random_device rd;
	mt19937 g(rd());
	shuffle(deck.begin(), deck.end(),g);

	cout << "Welcome to Rummkub!" << endl;
    while(1)
    {
        cout << "Do you want to load the game? (y/n)" << endl;
        cin >> load;
        if(load == 'y') {
            
            load_log_data(num_players, turn);
            cout << "Load game state!" << endl;
            break;
        }
        else if(load == 'n') {
            while(1)
            {
	            cout << "Enter Players number" << endl;
	            cin >> num_players;
	            if(num_players < 2){
		            cout << "There must be 2 or more players!" << endl;
	            }
	            else if(num_players > 4){
		            cout << "Maximum players is 4!" << endl;
	            }
                else break;
            }
            saved_checkpoint.set_nplayer(num_players);
	        for(int i=1;i<=num_players;i++){
		        players[i] = player(i);
		        distribute_initial_card(i);
	        }
            break;
        }
        else {
            cout << "Invalid input! Try again." << endl;
        }
    }
    
    saved_checkpoint.copy_all(players, table);
    save_log_data(num_players, turn);
	
    for(;; turn++){	
		int t = turn%num_players+1;
		cout << "Player " << t << " turn" << endl;
		bool sort_type = true;
		while(1){
			table.print_board();
			cout << endl;
			if(sort_type == true){
				players[t].sort_by_color();
			}
			else{
				players[t].sort_by_number();
			}
			cout << "Action : draw_card, move_card, checkpoint, ";
			if(sort_type){
				cout << "sort_by_number, ";
			}
			else{
				cout << "sort_by_color, ";
			}
			cout << "finish" << endl;
			string input;
			cin >> input;
			if(input == "draw_card"){
				int new_card_id = pop_card_from_deck();
                //card* new_card = new card(new_card_id, new_card_id%4, new_card_id%13);
				card* new_card = &all_cards[new_card_id];
				cout << "drawed ";
				all_cards[new_card_id].print_card();
				cout << endl;

				players[t].add_card(new_card);
				if(sort_type){
					players[t].sort_by_color();
				}
				else{
					players[t].sort_by_number();
				}
				break;
			}
			else if(input == "move_card"){
				cout << "from : ";
				int fgid, foff;
				cin >> fgid >> foff;
				cout << "to : ";
				int tgid, toff;
				cin >> tgid >> toff;
				bool valid_input = true;
				if(fgid > table.get_num_rows() || tgid > table.get_num_rows()+1){
					cout << "Invalid group ID" << endl;
					valid_input = false;
				}
				if(fgid < 0 || tgid < 0){
					cout << "Group ID must be positive or 0" << endl;
					valid_input = false;
				}
				if(toff < 0 || foff < 0){
					cout << "Card offset must be positive or 0" << endl;
					valid_input = false;
				}
				if(fgid == 0 && foff > players[t].get_card_num()){
					cout << "There is no card " << foff << " at player's hand" << endl;
					valid_input = false;
				}
				if(tgid == 0){
					cout << "You can't move card to your hand!" << endl;
					valid_input = false;
				}
				if(!valid_input){
					continue;
				}
				if(fgid == tgid && foff == toff){
					cout << "Interesting..." << endl;
					//actually do nothing
				}
				//actual code start
				vector <vector <card *>> temp = table.get_group();
				temp[0] = players[t].get_card();
				card *moving_card = temp[fgid][foff];
				if(tgid == table.get_num_rows()+1){
					int new_num_rows = table.get_num_rows() + 1;
					vector<card *> buf;
					buf.push_back(moving_card);
					temp.push_back(buf);
					table.set_num_rows(new_num_rows);
				}
				else{
					temp[tgid].insert(temp[tgid].begin()+toff,moving_card);
				}
				temp[fgid].erase(temp[fgid].begin()+foff);
				players[t].set_card(temp[0]);
				players[t].set_card_num(temp[0].size());
				temp[0].clear();
				table.set_group(temp);
			}
            else if(input == "checkpoint"){
                if(table.check_valid_fin()){
                    cout << "Load previous table..." << endl;
                    players[t].copy(saved_checkpoint.get_player_data(t)); 
                    table.copy(saved_checkpoint.get_board_data());
                }
                else{
                    saved_checkpoint.copy(t, players[t], table);
                    cout << "Checkpoint saved!" << endl;
                }
            }
			else if(input == "finish"){
				//todo : check if board is valid, if invalid, return to checkpoint
				if(table.check_valid_fin()){
                    cout << "Load previous table..." << endl;
                    players[t].copy(saved_checkpoint.get_player_data(t)); 
                    table.copy(saved_checkpoint.get_board_data());
                }
                else{
                    saved_checkpoint.copy(t, players[t], table);
                    cout << "Checkpoint saved!" << endl;
                    break;
                }       
			}
			else if(input == "sort_by_number"){
				sort_type = false;
			}
			else if(input == "sort_by_color"){
				sort_type = true;
			}
			else{
				cout << "Invalid input! Try again." << endl;
			}
		}
        if(players[t].get_card_num() == 0){
			cout << "Winner is Player " << t << "!" << endl;
			return 0;
		}

        save_log_data(num_players, turn);
	}
	/**** todo : check who is winner ****/
	return 0;
}

void distribute_initial_card(int player_id){
	for(int i=0;i<init_card_num;i++){
		int new_card_id = pop_card_from_deck();
	    //card* new_card = new card(new_card_id, new_card_id%4, new_card_id%13);
		card *new_card = &all_cards[new_card_id];
		players[player_id].add_card(new_card);
	}
	return;
}

void set_color(int color){
        if(color == 0){ //red
                printf("%c[1;31m",27);
        }
        else if(color == 1){
                printf("%c[1;34m",27);
        }
        else if(color == 2){
                printf("%c[1;33m",27);
        }
        else if(color == 3){
                printf("%c[1;36m",27);
        }
	else{
                printf("%c[1;37m",27);
	}
}

int pop_card_from_deck(){
	int ret;
	ret = deck[deck.size()-1];
	deck.pop_back();
	return ret;	
}
