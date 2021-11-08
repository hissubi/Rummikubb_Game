//#include "cmdkub.h"
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

int main(){
    char load;
    int num_players;
	int turn = 0;

	for(int i=0;i<8;i++){
		for(int j=1;j<=13;j++){
			int id = i*13+j;
            //cout << table.get_num_rows() << endl;
			all_cards[id] = card(id,i/2,j);
            //cout << table.get_num_rows() << endl;
			deck.push_back(id);
		}
	}
    //cout << table.get_num_rows() << endl;


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

	for(; turn < 100; turn++){	//what is MAX turn?
		int t = turn%num_players+1;
		cout << "Player " << t << " turn" << endl;
		while(1){
			players[t].print_hand_by_color();
			cout << "Action : draw_card, ?? ,finish" << endl;
			string input;
			cin >> input;
			if(input == "draw_card"){
				int new_card_id = pop_card_from_deck();
				players[t].add_card(new_card_id);
				players[t].print_hand_by_color();
				break;
			}
			/**** todo: other actions ****/
			if(input == "finish"){
				break;
			}
			if(players[t].get_card_num() == 0){
				cout << "Winner is Player " << t << "!" << endl;
				return 0;
			}
		}
        cout << table.get_num_rows() << endl;
        save_log_data(num_players, turn);
	}
	/**** todo : check who is winner ****/
	return 0;
}

void distribute_initial_card(int player_id){
	int count = 0;

	for(int i=0;i<init_card_num;i++){
		int new_card_id = pop_card_from_deck();
		players[player_id].add_card(new_card_id);
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
