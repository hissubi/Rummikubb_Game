#include "class.h"
#include "extern.h"
#include <ncurses.h>

board::board(){
	group.clear();
	vector <card *> temp;
	group.push_back(temp);
    num_rows = 0;
}
board::board(int state_): checkpoint(state_){
    group.clear();
	vector <card *> temp;
	group.push_back(temp);
    num_rows = 0;
}

int board::get_num_rows(){
    return num_rows;
}
void board::set_num_rows(int in){
    num_rows = in;
}
vector <vector <card*>> board::get_group(){
    return group;
}

void board::set_group(vector <vector<card *>> in){
	group = in;
}

void board::print_board(){ // Update
	for(int i=1;i<=num_rows;i++){
		attron(A_BOLD);
		printw("\n GID %-2d ", i);
		attroff(A_BOLD);
		refresh();
		for(size_t j=0; j<group[i].size(); j++){
            // Print Card //
			group[i][j]->print_card();
			printw(" ");
			refresh();
		}
		printw("\n");
		refresh();
	}
}

int board::group_type_check(int idx){
    // Same-color OR Same-value 
    int color = -1;
    int value = -1;
    int color_ck = 1;
    int value_ck = 1;
    for(size_t i=0; i<group[idx].size(); i++){
        if(group[idx][i]->value == joker_value) 
            continue;
        if(color == -1) color = group[idx][i]->color;
        if(value == -1) value = group[idx][i]->value;
        if(color != group[idx][i]->color){
            color_ck = 0; // Not same color group
        }
        if(value != group[idx][i]->value){
            value_ck = 0; // Not same value group
        }
    }
    if(color_ck == 1 && value_ck == 0) return 1; // Same-color
    else if(color_ck == 0 && value_ck == 1) return 2; // Same-value
    else if(color_ck == 1 && value_ck == 1 
                    && group[idx].size() == 3) return 2; // 2-Joker, 1-card
    else return 3; // Error ( Unknown Grouping )
}

/* Integrity Check ============================== */

int board::check_valid_fin(){
    // Normal Termination -> return 0
    
    int N_Group = group.size();
    
	attron(A_BOLD | COLOR_PAIR(1));
    // 1 : Group Member Violation
    for(int i=1; i<N_Group; i++){
        if(group[i].size() < 3){
            printw(" Invalid type 01 : Too-Small Group");
            printw(" ( GID : %d )\n",i);
			attroff(A_BOLD | COLOR_PAIR(1));
			refresh();
            return 1;
        }
    }
    // 2 : Invalid Grouping
    for(int i=1; i<N_Group; i++){
        int gtc = group_type_check(i);
        int buf;
        int cbuf[5];
        if(gtc == 3){
            printw(" Invalid type 02 : Unknown Grouping");
            printw(" ( GID : %d )\n",i);
			attroff(A_BOLD | COLOR_PAIR(1));
			refresh();
            return 2;
        }
        else if(gtc == 1){
            // Sequence check
            buf = -1;
            for(size_t k=0; k<group[i].size(); k++){
                if(buf == -1){
                    buf = group[i][k]->value;
                    continue;
                }
                if(buf == joker_value){
                    buf = group[i][k]->value;
                    continue;
                }
                if(group[i][k]->value == joker_value){
                    buf = buf + 1;
                    continue;
                }
                if(group[i][k]->value == buf + 1){
                    buf = buf + 1;
                    continue;
                }
                printw(" invalid type 03 : Sequence Violation");
            	printw(" ( GID : %d )\n",i);
				attroff(A_BOLD | COLOR_PAIR(1));
				refresh();
                return 3;
            }
        }
        else if(gtc == 2){
            // Color duplicate check
            buf = -1;
            for(int j=0; j<5; j++) cbuf[j]=0;
            for(size_t k=0; k<group[i].size(); k++){
                if(group[i][k]->value != joker_value){
                    if(buf == -1)
                        buf = group[i][k]->value;
                    else{
                        if(buf != group[i][k]->value){
                            printw(" Invalid type 04 : Not Same-Value");
            				printw(" ( GID : %d )\n",i);
							attroff(A_BOLD | COLOR_PAIR(1));
							refresh();
                            return 4;
                        }
                    }
                }
                if(group[i][k]->value == joker_value) 
                    cbuf[0]++;
                else
                    cbuf[ group[i][k]->color ]++;
            }
            int cnt=0;
            for(int k=1; k<5; k++){
                if(cbuf[k] > 1){
                    printw(" Invalid type 05 : Color Duplication");
            		printw(" ( GID : %d )\n",i);
					attroff(A_BOLD | COLOR_PAIR(1));
					refresh();
                    return 5;
                }
                if(cbuf[k] == 1) cnt++;
            }
            if(cnt + cbuf[0] > 4){
                printw(" Invalid type 06 : Cannot Match Joker");
           		printw(" ( GID : %d )\n",i);
				attroff(A_BOLD | COLOR_PAIR(1));
				refresh();
                return 6;
            }
        }
    }
	attron(A_BOLD | COLOR_PAIR(2));
    printw(" Board Check Complete\n");
	refresh();
	attroff(COLOR_PAIR(2));
	return 0;
}

void board::copy_to_cp(int num) {
    group = table.get_group(); 
    num_rows = table.get_num_rows();
}

void board::copy_from_cp(int num) {
    table.set_group(group); 
    table.set_num_rows(num_rows);
}


int board::get_sum_group(int gid){

	int ret = 0;
	// joker checkpoint
	int joker_1st = -1;
	int joker_2nd = -1;
	int cnt_joker = 0;

    for(size_t i=0; i<group[gid].size(); i++){
		if(group[gid][i]->get_value() == joker_value){
			cnt_joker++;
			if(joker_1st == -1) joker_1st = i;
			else joker_2nd = i;
		}
		ret += group[gid][i]->get_value();
    }
	ret -= 99 * cnt_joker;

	if(cnt_joker == 0){
		return ret;
	}
	if(cnt_joker == 1){
// joker 1
		if(joker_1st == 0){
			if(group[gid][1]->get_value() == group[gid][2]->get_value())
				ret += group[gid][1]->get_value();
			else
				ret += group[gid][1]->get_value() - 1;
		}
		else if(joker_1st == (signed) group[gid].size() - 1){
			if(group[gid][0]->get_value() == group[gid][1]->get_value())
				ret += group[gid][joker_1st-1]->get_value();
			else
				ret += group[gid][joker_1st-1]->get_value() + 1;
		}
		else{
			if(group[gid][joker_1st-1]->get_value() == group[gid][joker_1st+1]->get_value())
				ret += group[gid][joker_1st-1]->get_value();
			else
				ret += group[gid][joker_1st-1]->get_value() + 1;
		}
	}
	else{
// joker 2
		int tmp_val = -1;
		int seq = 0;
		int size = group[gid].size();
		if( size == 3 ){
			if(group[gid][0]->get_value() != joker_value) 
				ret += group[gid][0]->get_value()*2 + 3;
			else if(group[gid][1]->get_value() != joker_value) 
				ret += group[gid][1]->get_value()*2;
			else ret += group[gid][2]->get_value() * 2 - 3;
		}
		else{
			for(int i=0; i<size; i++){
				if(group[gid][i]->get_value() == joker_value) continue;
				if(tmp_val == -1) tmp_val = group[gid][i]->get_value();
				if(tmp_val != group[gid][i]->get_value()) seq = 1;
			}
			if( seq == 0 ){
				ret += tmp_val * 2;
			}
			else{
				if(joker_1st == 0 && joker_2nd == 1){
					ret += group[gid][2]->get_value() * 2 - 3;
				}
				else if(joker_1st == size-2 && joker_2nd == size-1){
					ret += group[gid][size-3]->get_value() * 2 + 3;
				}
				else if(joker_1st + 1 == joker_2nd){
					ret += group[gid][joker_1st-1]->get_value() + 1;
					ret += group[gid][joker_2nd+1]->get_value() - 1;
				}
				else{
					ret += group[gid][joker_1st+1]->get_value() - 1;
					ret += group[gid][joker_2nd-1]->get_value() + 1;
				}
			}
		}
	}
		
	return ret;
}

