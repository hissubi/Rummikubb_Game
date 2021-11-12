#ifndef CLASS_H
#define CLASS_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define joker_value 99

class card{
        public:
            card();
            card(int id_, int color_, int value_);
            
            int get_id();
            int get_loc();
		    void set_loc(int loc_);
            
            void print_card();

        friend void load_log_data(int& nplayers, int& turn);
        friend class board;

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
		int group_type_check(int idx);

        void print_board();
        void copy(board a);
        
        friend void load_log_data(int& nplayers, int& turn);
		
    private:
		int num_rows;
		vector <vector <card*>> group;
};

class player{
	public:
		player();
		player(int id);
        
        bool get_is_register();
		int get_player_id();
        int get_card_num();
        vector<card*> get_card();
		
        void sort_by_color();
		void sort_by_number();
		
        void add_card(card* hand_card);
        void copy(player p);
        friend void load_log_data(int& nplayers, int& turn);


    private:
        bool is_register;
		int player_id;
		int card_num;
		vector<card*> hand_card; 
};

class checkpoint{
    public:
        checkpoint();
        checkpoint(int nplayer_);

        void set_nplayer(int nplayer_);
        player get_player_data(int player_num);
        board get_board_data();

        void copy(int player_num, player p, board b);
        void copy_all(player* p, board b);
    private:
        player* player_data;
        board board_data;
        int nplayer;
};
#endif
