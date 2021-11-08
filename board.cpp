#include "class.h"
#include "extern.h"

board::board(){
	num_rows = 0;
    return;
}

int board::get_num_rows(){
    return num_rows;
}

vector <vector <card*>> board::get_group(){
    return group;
}

int board::check_valid_fin(){
	return 1;
}

void board::print_board(){
	for(int i=0;i<num_rows;i++){
		for(int j=0;j<13;j++){
			if(group[i][j] > 0){
				//all_cards[group[i][j]].print_card();
			}
		}
		cout << endl;
	}
}

