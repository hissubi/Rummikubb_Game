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
	init_color(COLOR_MAGENTA, 488, 950, 800);
	init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
	init_color(COLOR_CYAN, 920, 100, 920);
	init_pair(8, COLOR_CYAN, COLOR_BLACK);

	for(int i=0;i<8;i++){
		for(int j=1;j<=13;j++){
			int id = i*13+j;
			all_cards[id] = card(id,i/2,j);
			deck.push_back(id);
		}
	}
	all_cards[105] = card(105, 7, joker_value);
	deck.push_back(105);
	all_cards[106] = card(106, 7, joker_value);
	deck.push_back(106);

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
		printw(" Do you want to load the game? (y/n) ");
		refresh();
		getstr(load);
		printw("\n"); refresh();
        if(load[0] == 'y') {        

            load_log_data(num_players, turn);
            saved_checkpoint.set_nplayer(num_players);
            printw(" Load game starts!\n");
			printw("\n"); refresh();
			//todo : rearrange deck
			break;
        }
        else if(load[0] == 'n') {
            while(1)
            {
	            printw(" Enter Players number : ");
				refresh();
				getstr(ncurse_tmp);
				printw("\n"); refresh();
				num_players = atoi(ncurse_tmp);
				if(num_players < 2){
	            	printw(" There must be 2 or more players!\n");
					refresh();
				}
	            else if(num_players > 4){
	            	printw(" Maximum players is 4!\n");
					refresh();
				}
                else break;
            }
            saved_checkpoint.set_nplayer(num_players);
	        for(int i=1;i<=num_players;i++){
		        players[i] = player(i);
		        distribute_initial_card(i);
	        }
            save_log_data(num_players, turn);
            break;
        }
        else {
			printw(" Invalid input! Try again.\n");
			refresh();
        }
    }

// Start Game ===========================================================//
    
	saved_checkpoint.copy_all(players, table);
	
	noecho();

    for(;; turn++){	

		int t = turn%num_players+1;
			
		int state = 0;
        bool sort_type = true;

		int initial_GID = table.get_num_rows();
		
		while(1){
            clear();
			attron(A_BOLD | COLOR_PAIR(5));
			printw("\n     Welcome to Rummikub !!");
		   	attron(A_BLINK);
			printw("  (^_^)\n");
			attroff(A_BOLD | A_BLINK | COLOR_PAIR(5));
			refresh();
			printw("\n\n   *****************************\n");
			attron(A_BOLD | COLOR_PAIR(3));
			printw("   *       Player %d turn       *\n", t);
			attroff(A_BOLD | COLOR_PAIR(3));
			printw("   *****************************\n\n");
			refresh();
			
			table.print_board();
			
			/**** print blank group that player can use to make new group ****/
			printw("\n\n");
			attron(A_BOLD);
			printw(" GID% -2d \n", table.get_num_rows()+1);
			attroff(A_BOLD);
			printw("\n");
			/********/

			printw("\n");
			attron(A_BOLD);
			printw(" HAND   ");
			attroff(A_BOLD);
			refresh();
			if(sort_type == true){
				players[t].sort_by_color();
			}
			else{
				players[t].sort_by_number();
			}
			printw("\n");
			refresh();
			attron(A_BOLD);
			//printw("\n ----------------------------------- \n | ");
			printw("\n   | ");
			attron(COLOR_PAIR(7));
			printw(" Select Action & Press Space bar  ");
			attroff(COLOR_PAIR(7));
			printw("|\n   |  1. Draw_card ");
			getyx(win, curr_x[1], curr_y[1]);
			printw("\t\t       |");
			printw(" \n   |  2. Move_card ");
			getyx(win, curr_x[2], curr_y[2]);
			printw("\t\t       |");
			printw(" \n   |  3. Checkpoint ");
			getyx(win, curr_x[3], curr_y[3]);
			printw("\t\t       |");
			if(sort_type){
				printw(" \n   |  4. Sort_by_number ");
				getyx(win, curr_x[4], curr_y[4]);
				printw("\t       |");
			}
			else{
				printw(" \n   |  4. Sort_by_color ");
				getyx(win, curr_x[4], curr_y[4]);
				printw("\t\t       |");
			}
			printw(" \n   |  5. Finish ");
			getyx(win, curr_x[5], curr_y[5]);
			printw("\t\t       |");
			printw(" \n");
			//printw(" ----------------------------------- \n");
			attroff(A_BOLD);
			refresh();
			getyx(win, curr_x[0], curr_y[0]);
			mvaddch(curr_x[1], curr_y[1], '<');

// Select with Keyboard ============================================================//			

			int ch;
			int select = 1;
			keypad(win, TRUE);
			while( (ch = getch()) != ' ' ){
				switch(ch) {
	            case KEY_UP:
					if(select == 1) break;
					mvaddch(curr_x[select], curr_y[select], ' ');
	                select--;
					mvaddch(curr_x[select], curr_y[select], '<');
					break;
	            case KEY_DOWN:
					if(select == 5) break;
					mvaddch(curr_x[select], curr_y[select], ' ');
					select++;
					mvaddch(curr_x[select], curr_y[select], '<');
	                break;
				default:
					break;
		        }
			}

// Menu Case =====================================================================//
			
			mvinsch(curr_x[0], curr_y[0], '\n');
			printw("\n Your Selection : ");
			if(select == 1) printw("Draw_card!");
			if(select == 2) printw("Move_card!");
			if(select == 3) printw("Checkpoint!");
			if(select == 4 && sort_type == true) printw("Sort_by_Number!");
			if(select == 4 && sort_type == false) printw("Sort_by_Color!");
			if(select == 5) printw("Finish!");
			printw("\n\n");
			refresh();
			
			if(select == 1){
			// 1: Draw Card
				if(state != 0){
					printw(" You can't draw after moving card!\n");
					printw("\n Press Any Key...\n");
					getch();
					continue;
				}
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
                saved_checkpoint.copy(t, players[t], table);
				printw("\n Press Any Key...\n");
				getch();
				break;
			}
			else if(select == 2){
			// 2: Move Card
				printw(" Press 'q' to quit Move_card\n");
				

				int fgid , foff = 0;
				int tgid, toff = 0;
				int vbase_x = 10;
				int vbase_y = 7;
				int starting_x = 10;
				int starting_y = 13 + table.get_num_rows() * 3;
				int current_x = starting_x;
				int current_y = starting_y;
				mvinsch(current_y, current_x, '<');
				int phase = 0;	//input phase
				while( (ch = getch()) != 'q'){
					switch(ch) {
		            case KEY_UP:
						if(current_y -3 == vbase_y){
							break;
						}
						mvdelch(current_y, current_x);
	     		        current_y -= 3;
						mvinsch(current_y, current_x, '<');
						break;
		            case KEY_DOWN:
						if(current_y== starting_y){
							break;
						}
						mvdelch(current_y, current_x);
	     		        current_y += 3;
						mvinsch(current_y, current_x, '<');
		                break;
					case KEY_LEFT:
						if(current_x == vbase_x - 3*phase){
							break;
						}
						mvdelch(current_y, current_x);
	     		        current_x -= 3;
						mvinsch(current_y, current_x, '<');
						break;
					case KEY_RIGHT:
						if(current_y == starting_y &&
								current_x >= vbase_x + 3*players[t].get_card_num() - 3){
							break;
						}
						if(current_x >= vbase_x + 3*20){
							break;
						}
						mvdelch(current_y, current_x);
	     		        current_x += 3;
						mvinsch(current_y, current_x, '<');
						break;
					case ' ':
						if(current_y == starting_y){
							fgid = 0;
						}
						else{
							fgid = (((current_y - vbase_y) )/3);
						}
						foff = (current_x - vbase_x) / 3;
						phase = 1;
						break;
					case '\n':
						if(current_y == starting_y){
							tgid = 0;
						}
						else{
							tgid = (((current_y - vbase_y) )/3);
						}
							toff = (current_x - vbase_x +3 ) / 3;
						if(phase == 1){
							goto val_check;
						}
						else{
							break;
						}
					default:
						break;
					}
				}
				if(ch == 'q'){
					continue;
				}
				val_check:
				/*printw(" From : "); refresh();
				fgid = getch()-48;
				getch();
				foff = getch()-48;
				printw("\n To : "); refresh();
				tgid = getch()-48;
				getch();
				toff = getch()-48;
				printw("\n");
				refresh();

				getch();*/
				
				bool valid_input = true;
				if(fgid > table.get_num_rows() || tgid > table.get_num_rows()+1){	//can't
					printw(" Invalid group ID\n");
					refresh();
					valid_input = false;
				}
				if(fgid < 0 || tgid < 0){	//can't
					printw(" Group ID must be positive or 0\n");
					refresh();
					valid_input = false;
				}
				if(toff < 0 || foff < 0){	//can't
					printw(" Card offset must be positive or 0\n");
					refresh();
					valid_input = false;
				}
				if(fgid == 0 && foff > players[t].get_card_num()){	//can't
					printw(" There is no card %d at player's hand", foff);
					refresh();
					valid_input = false;
				}
				if(tgid == 0){
					printw(" You can't move card to your hand!\n");
					refresh();
					valid_input = false;
				}
				if(players[t].get_is_register() == false && tgid <= initial_GID && tgid != 0){
					printw(" You can't manipulate board's groups before register!\n");
					valid_input = false;
				}
                vector <vector <card*>> temp = table.get_group();
                if( (unsigned) toff >  temp[tgid].size() ){			//pull card
					toff = temp[tgid].size();
                }
				if(temp[fgid].size() == 0 && fgid != 0){
					printw(" You can't move blank!\n");
					valid_input = false;
				}
				if(!valid_input){
					refresh();
					getch();
					continue;
				}
				if(fgid == tgid && foff == toff){
					printw(" Interesting...\n");
					refresh();
					//actually do nothing
				}
				//actual code start
                if(state == 0) state = 1;
                else if(state == 2) state = 3;
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
            else if(select == 3){
				if(players[t].get_is_register() == false){
					printw(" Unregistered player\n");
					int sum = 0;
					vector <vector <card*>> temp = table.get_group();
					for(int i = initial_GID+1;i<=table.get_num_rows();i++){
						for(size_t j=0;j<temp[i].size();j++){
							sum += temp[i][j]->get_value();
						}
					}
					printw(" Sum of points of initial move : %d\n",sum);
					refresh();
					if(sum < 30){
						printw(" Your initial move must have a value of 30 points or more");
						printw("\n Press Any Key...\n");
						refresh();
                        players[t].copy(saved_checkpoint.get_player_data(t)); 
                        table.copy(saved_checkpoint.get_board_data());
                        if(state == 1) {
                            state = 0;
                        }
                        else if(state == 3) {
                            state = 2;
                        }
						getch();
						continue;
					}
				}

                if(state == 0) {
                    printw(" You can make checkpoint after making a new group!\n");
                    refresh();
                }
                else if(state == 2){
                    printw("Checkpoint is up to date.\n");
                }
                else{
                    if(table.check_valid_fin()){
                        printw(" Load previous table...\n");
					    refresh();
                        players[t].copy(saved_checkpoint.get_player_data(t)); 
                        table.copy(saved_checkpoint.get_board_data());
                        if(state == 1) {
                            state = 0;
                        }
                        else if(state == 3) {
                            state = 2;
                        }
                    }
                    else{
                        saved_checkpoint.copy(t, players[t], table);
                        printw(" Checkpoint saved!\n");
					    refresh();
                        state = 2;
						if(players[t].get_is_register() == false){
							players[t].set_is_register(true);
						}
                    }	
                }
				

				printw("\n Press Any Key...\n");
				getch();
            }
			else if(select == 5){
				//todo : check if board is valid, if invalid, return to checkpoint
				if(state == 0){
					printw(" Can't finish yet\n");
					refresh();
				}
                else{
				    if(table.check_valid_fin()){
                        printw(" Load previous table...\n");
					    refresh();
                        players[t].copy(saved_checkpoint.get_player_data(t)); 
                        table.copy(saved_checkpoint.get_board_data());
                        if(state == 1) {
                            state = 0;
                        }
                        else if(state == 3){
                            state = 2;
                        }       
                    }
                    else{
                        saved_checkpoint.copy(t, players[t], table);
                        break;
                    }    
                }   
				printw("\n Press Any Key...\n");
				getch();
			}
			else if(select == 4){
				if( sort_type == false) sort_type = true;
				else sort_type = false;
			}	
			else{
				printw(" Invalid input! Try again.\n");
				refresh();
			}
		}
        if(players[t].get_card_num() == 0){
			clear();
            printw(" Winner is Player %d!\n",t);
			refresh();
			return 0;
		}
        
        save_log_data(num_players, turn);
        
        clear();
        printw("\n Player %d turn finished! Change your seat with Player %d!\n", t, t%num_players+1);
		printw("\n Press Any Key...\n");
		refresh();
        getch();
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
