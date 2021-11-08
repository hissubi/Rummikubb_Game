#ifndef CLASS_H
#define CLASS_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class card{
        public:
                card();
                card(int id_, int color_, int value_);
                int get_id();
                void print_card();
		int get_loc();
		void set_loc(int loc_);
        friend void load_log_data(int& nplyaers, int& turn);

        private:
                int id;
                int color;
                int value;
		int loc;
};

class board{
	public:
		board();
        int get_num_rows();
        vector <vector <card*>> get_group();
		int check_valid_fin();
		void print_board();
        friend void load_log_data(int& nplyaers, int& turn);

		
private:
		int num_rows;
		vector <vector <card*>> group;
};

class player{
	public:
		player();
		player(int id);
        bool get_is_register();
		int get_card_num();
        vector<int> get_card();
		void print_hand_by_color();
		void print_hand_by_number();
		void add_card(int card_id);
        friend void load_log_data(int& nplyaers, int& turn);


    private:
        bool is_register;
		int player_id;
		int card_num;
		vector<int> card_id; 
};

#endif
