#include "cmdkub.h"
#include <time.h>
#include <string>
#include <random>
#include <algorithm>
#include "class.h"
#include <iterator>
#include <iostream>
#include <vector>
#include <ncurses.h>

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
    char load[5];
    int num_players;
	int turn = 0;
	char ncurse_tmp[10];

	// Ncurses Start //
	int curr_x[10];
	int curr_y[10];
	WINDOW* win = initscr();
	echo();
	start_color();
	init_color(COLOR_BLACK, 82, 0, 51);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_GREEN, COLOR_BLACK);
	init_pair(6, COLOR_WHITE, COLOR_BLACK);

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

	attron(A_BOLD | COLOR_PAIR(5));
	printw("\n     Welcome to Rummikub !!");
    attron(A_BLINK);
	printw("  (^_^)\n");
	attroff(A_BOLD | A_BLINK | COLOR_PAIR(5));
	refresh();
    while(1)
    {
        printw("\n");
		printw("Do you want to load the game? (y/n) ");
		refresh();
		getstr(load);
		printw("\n"); refresh();
        if(load[0] == 'y') {
            
            load_log_data(num_players, turn);
            printw("Load game state!\n");
			printw("\n"); refresh();
			break;
        }
        else if(load[0] == 'n') {
            while(1)
            {
	            printw("Enter Players number : ");
				refresh();
				getstr(ncurse_tmp);
				printw("\n"); refresh();
				num_players = atoi(ncurse_tmp);
				if(num_players < 2){
	            	printw("There must be 2 or more players!\n");
					refresh();
				}
	            else if(num_players > 4){
		            cout << "Maximum players is 4!" << endl;
	            	printw("Maximum players is 4!\n");
					refresh();
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
			printw("Invalid input! Try again.\n");
			refresh();
        }
    }

// Start Game ===========================================================//
    
	saved_checkpoint.copy_all(players, table);
    save_log_data(num_players, turn);
	
    for(;; turn++){	

		int t = turn%num_players+1;
			
		while(1){
			clear();
			attron(A_BOLD | COLOR_PAIR(5));
			printw("\n     Welcome to Rummikub !!");
		   	attron(A_BLINK);
			printw("  (^_^)\n");
			attroff(A_BOLD | A_BLINK | COLOR_PAIR(5));
			refresh();
			printw("\n\n**********************\n");
			printw("    Player %d turn\n", t);
			printw("**********************\n\n");
			refresh();
			bool sort_type = true;
			
			table.print_board();
			printw("\n");
			printw(" HAND ");
			refresh();
			if(sort_type == true){
				players[t].sort_by_color();
			}
			else{
				players[t].sort_by_number();
			}
			printw("\n");
			refresh();
			printw(" Select Action & Press 'x' \n 1. Draw_card ");
			getyx(win, curr_x[1], curr_y[1]);
			printw(" \n 2. Move_card ");
			getyx(win, curr_x[2], curr_y[2]);
			printw(" \n 3. Checkpoint ");
			getyx(win, curr_x[3], curr_y[3]);
			if(sort_type){
				printw(" \n 4. Sort_by_number ");
			}
			else{
				printw(" \n 4. Sort_by_color ");
			}
			getyx(win, curr_x[4], curr_y[4]);
			printw(" \n 5. Finish ");
			getyx(win, curr_x[5], curr_y[5]);
			printw(" \n");
			refresh();
			getyx(win, curr_x[0], curr_y[0]);
			mvinsch(curr_x[1], curr_y[1], '<');

// Select with Keyboard ============================================================//			

			int ch;
			int select = 1;
			keypad(win, TRUE);
			while( (ch = getch()) != 'x' ){
				switch(ch) {
	            case KEY_UP:
					if(select == 1) break;
					mvdelch(curr_x[select], curr_y[select]);
	                select--;
					mvinsch(curr_x[select], curr_y[select], '<');
					break;
	            case KEY_DOWN:
					if(select == 5) break;
					mvdelch(curr_x[select], curr_y[select]);
					select++;
					mvinsch(curr_x[select], curr_y[select], '<');
	                break;
		        }
			}

// Menu Case =====================================================================//
			
			mvinsch(curr_x[0], curr_y[0], '\n');
			printw("\n Your Select : ");
			if(select == 1) printw("Draw_card!");
			if(select == 2) printw("Move_card!");
			if(select == 3) printw("Checkpoint!");
			if(select == 4 && sort_type == true) printw("Sort_by_Number!");
			if(select == 4 && sort_type == false) printw("Sort_by_Color!");
			if(select == 5) printw("Draw_card!");
			printw("\n\n");
			refresh();
			
			if(select == 1){
			// 1: Draw Card
				int new_card_id = pop_card_from_deck();
				card* new_card = &all_cards[new_card_id];
				printw(" Drawed "); 
				refresh();
				all_cards[new_card_id].print_card();
				printw("\n HAND ");
				refresh();
				players[t].add_card(new_card);
				if(sort_type){
					players[t].sort_by_color();
				}
				else{
					players[t].sort_by_number();
				}
				printw("\n Press Any Key...\n");
				getch();
				break;
			}
			else if(select == 2){
			// 2: Move Card
				printw(" From : "); refresh();
				int fgid, foff;
				fgid = getch()-48;
				getch();
				foff = getch()-48;
				printw("\n To : "); refresh();
				int tgid, toff;
				tgid = getch()-48;
				getch();
				toff = getch()-48;
				printw("\n");
				refresh();
				bool valid_input = true;
				if(fgid > table.get_num_rows() || tgid > table.get_num_rows()+1){
					printw(" Invalid group ID\n");
					refresh();
					valid_input = false;
				}
				if(fgid < 0 || tgid < 0){
					printw(" Group ID must be positive or 0\n");
					refresh();
					valid_input = false;
				}
				if(toff < 0 || foff < 0){
					printw(" Card offset must be positive or 0\n");
					refresh();
					valid_input = false;
				}
				if(fgid == 0 && foff > players[t].get_card_num()){
					printw(" There is no card %d at player's hand", foff);
					refresh();
					valid_input = false;
				}
				if(tgid == 0){
					printw(" You can't move card to your hand!\n");
					refresh();
					valid_input = false;
				}
                vector <vector <card*>> temp = table.get_group();
                if( (unsigned) toff >  temp[tgid].size() ){
                    printw(" You can't put the card there\n");
					refresh();
                    valid_input = false;
                }
				if(!valid_input){
					continue;
				}
				if(fgid == tgid && foff == toff){
					printw(" Interesting...\n");
					refresh();
					//actually do nothing
				}
				//actual code start
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

				printw("\n Press Any Key...\n");
				getch();
			}
            else if(select == 3){
                if(table.check_valid_fin()){
                    printw(" Load previous table...\n");
					refresh();
                    players[t].copy(saved_checkpoint.get_player_data(t)); 
                    table.copy(saved_checkpoint.get_board_data());
                }
                else{
                    saved_checkpoint.copy(t, players[t], table);
                    printw(" Checkpoint saved!\n");
					refresh();
                }	

				printw("\n Press Any Key...\n");
				getch();

            }
			else if(select == 5){
				//todo : check if board is valid, if invalid, return to checkpoint
				if(table.check_valid_fin()){
                    printw(" Load previous table...\n");
					refresh();
                    players[t].copy(saved_checkpoint.get_player_data(t)); 
                    table.copy(saved_checkpoint.get_board_data());
                }
                else{
                    saved_checkpoint.copy(t, players[t], table);
                    printw(" Checkpoint saved!\n");
					refresh();
                    break;
                }       
			}
			else if(select == 4){
				if( sort_type == false) sort_type = true;
				else sort_type = false;

				printw("\n Press Any Key...\n");
				getch();

			}	
			else{
				printw(" Invalid input! Try again.\n");
				refresh();
			}
		}
        if(players[t].get_card_num() == 0){
			printw(" Winner is Player %d!\n",t);
			refresh();
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
