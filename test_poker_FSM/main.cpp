#include <iostream>

#include "Poker_FSM.h"

#include <string>



using namespace std;

	

int main(){

	cout<<"Start program"<<endl;

	int Stacks[9] = {1000, 2000, 400, 0, 0, 4000, 0, 5000, 400};

	Poker_FSM my_fsm(3, 50, 25, 5, Stacks);

	int player;
	int value;
	string move_str;
	Move move(0, 0, move_types::raise);
	int error;
	Cards_processor::Deck my_deck;

	//Заполнили случайно картами
	for (int i=0; i<9; i++){
		my_fsm.PlayerCards[i][0] = my_deck.GetCard();
		my_fsm.PlayerCards[i][1] = my_deck.GetCard();
	}
	for (int i=0; i<5; i++)	my_fsm.TableCards[i] = my_deck.GetCard();
	

	//my_fsm.LoadFSM("d:/123.txt");


	while (1){
		
		system("cls");
		cout<<my_fsm.CurrentState();
		if (my_fsm.last_error[0]){
			cout<<endl<<"Last error: "<<my_fsm.last_error<<endl;
			my_fsm.last_error = "";
		}
		//cout<<" or 'save d:/123.txt' or 'load d:/123.txt' \n ";

		getline(std::cin, move_str);

		if (move_str.substr(0, 4) == "load"){
			my_fsm.LoadFSM(move_str.substr(5,move_str.size()-5));
			continue;
		}
		if (move_str.substr(0, 4) == "save"){
			my_fsm.SaveFSM(move_str.substr(5,move_str.size()-5));
			continue;
		}
		if (move_str.substr(0, 4) == "calc"){
			my_fsm.Calculate();
			continue;
		}



	
		move = StringToMove(move_str);
		move.player = my_fsm.WhoseMove();

		my_fsm.AddMove(move);
	
		
	}

	






	int g;
	cin>>g;
	return 0;
}