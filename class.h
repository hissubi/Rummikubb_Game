#ifndef CLASS_H
#define CLASS_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

#define joker_value 99

class card{
        public:
            card();
            card(int id_, int color_, int value_);
            
            int get_id();
            int get_loc();
			int get_value();
		    void set_loc(int loc_);
            
            void print_card();

        friend void load_log_data(int& nplayers, int& turn);
        friend class board;
        friend class player;

        private:
                int id;
                int color;
                int value;
		int loc;
};

class checkpoint{
    public:
        checkpoint();
        checkpoint(int state_);
        virtual ~checkpoint(){}
        
        virtual void copy_from_cp(int num)=0;
        virtual void copy_to_cp(int num)=0;
    private:
        int state;
};

class board: public checkpoint{
	public:
		board();
        board(int state_);
        ~board(){}

        int get_num_rows();
        void set_num_rows(int);
        vector <vector <card*>> get_group();
		void set_group(vector <vector <card*>>);
		
        int check_valid_fin();
		int group_type_check(int idx);

        void print_board();
        void copy_from_cp(int num);
        void copy_to_cp(int num);
        
        friend void load_log_data(int& nplayers, int& turn);

		int get_sum_group(int gid);
		
    private:
		int num_rows;
		vector <vector <card*>> group;
};

class player: public checkpoint{
	public:
		player();
		player(int id);
        ~player(){}
        
        bool get_is_register();
		void set_is_register(bool in);
		int get_player_id();
        void set_player_id(int id_);
		int get_card_num();
        void set_card_num(int in);
        vector<card*> get_card();
        void set_card(vector<card*>in);
		
        bool is_bigger_by_color(card* a, card* b);
        bool is_bigger_by_number(card* a, card* b);
        void sort_by_color();
		void sort_by_number();
		
        void add_card(card* hand_card);
        void copy_from_cp(int num);
        void copy_to_cp(int num);
        friend void load_log_data(int& nplayers, int& turn);


    private:
        bool is_register;
		int player_id;
		int card_num;
		vector<card*> hand_card; 
};

#endif
