#include "class.h"
#include "extern.h"
#include <ncurses.h>

board::board(){
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
	for(int i=1;i<=num_rows; i++){
		printw("\n\n GROUP %d ", i);
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
    
    // 1 : Group Member Violation
    for(int i=1; i<N_Group; i++){
        if(group[i].size() < 3){
            cout << "Invalid type 01 : Too-Small Group\n";
            cout << "Group Num : " << i << endl;
            return 1;
        }
    }
    // 2 : Invalid Grouping
    for(int i=1; i<N_Group; i++){
        int gtc = group_type_check(i);
        int buf;
        int cbuf[5];
        if(gtc == 3){
            cout << "Invalid type 02 : Unknown Grouping\n";
            cout << "Group Num : " << i << endl;
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
                cout << "Invalid type 03 : Sequence Violation\n";
                cout << "Group Num : " << i << endl;
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
                            cout << "Invalid type 04 : Not Same-Value\n";
                            cout << "Group Num : " << i << endl;
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
                    cout << "Invalid type 05 : Color Duplication\n";
                    cout << "Group Num : " << i << endl;
                    return 5;
                }
                if(cbuf[k] == 1) cnt++;
            }
            if(cnt + cbuf[0] > 4){
                cout << "Invalid type 06 : Cannot Match Joker\n";
                cout << "Group Num : " << i << endl;
                return 6;
            }
        }
    }

    cout << "Board Check Complete\n";
	return 0;
}

void board::copy(board a) {
    
    group = a.get_group(); 
    /*
    num_rows = a.get_num_rows();
    group.resize(num_rows);
    for(int i = 0; i < num_rows; i++) {
        group[i].resize(a.get_group().at(i).size());
        for(size_t j = 0; j < group[i].size(); j++){
            group[i][j] = a.get_group().at(i).at(j);
        }
    } */       
}

