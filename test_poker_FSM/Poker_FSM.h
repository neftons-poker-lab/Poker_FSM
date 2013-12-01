/*
Покерный автомат для 10 игроков (Надо чтоб сели больше 2х??)
Арифметическое устройство (на целых числах) у которого есть инициализация
и которое приводится в действие внешними воздействиями (ходами).
В нём нет смысла если нет возможности извлекать информацию о его состоянии.
Переключение состояний зависит только от ходов и не зависит от времени.
Тоесть логично получить инфу о состоянии сразу после хода.
Пробовал передавать инфу вовзращаемым значением функции Добавить_ход(ход). Плохо.
слишком наляписто, плохорасширяемо и элементарно некрасиво. Меняем тактику.
При ответе на функциюб Добавить_ход(ход) - минимум инфы. только ок или не ок.
Тоесть должны быть функции вовзрата состояния (или частей)
Отладка автомата. вбивать пол сотни ходов до выявления ошибки - гемор ещё тот.
Быстро я понял что надо функции сохранения и считывания с диска текущего состояния.

Внутреннее устройство. 
1. Допустмые хода. новейшая концепция. Раньше я думал что выявление всех допустимых ходов - факультативно.
и желательно только для программы покер чтоб предложить юзеру удобно ввести свой ход. Не заморачивался с этим понятием
и просто каждый раз по многим параметрам проверял допустим ли ход. Сложность системы возросла на порядок и 
больше нельзя было её развивать. тупик.
Сейчас хочу использовать эту концепцию как одну из основ. Допустимые хода - они не для каждого игрока
они только для того, кто сейчас ходит. Для остальных это понятие не имеет смысла, пока до них не дойдёт ход.
Возможные хода: (fold, call N, check, bet(min - max), raise(min-max)).
неполный райз может быть записан как raise(steck-steck).

2. очень важный вопрос: Когда прерывать передачу ходов по кругу?
	- самое очевидное - когда остался в торгах 1 или меньше игроков. нет.
	  А может одному игроку надо заколить или спасовать.
	- право заколить?
	- менее очевидное: Когда все ставки равны(кроме оленей) И ни у кого не осталось права повысить.
О-ПА!!!!! Вторая важнейшая концепция. Право повысить. В отличии от первой важнейшей концепции 
право повысить присуще каждому игроку, пока он его не утратит вследствии правил игры.

3. Менее важный вопрос. Формат ходов. Как видно из п1 есть 5 вариантов ходов. В первой версии автоматика были так же ходы:
сесть_за_стол, поставить бб, поставить_сб, поставить_анте, неполный бет, неполный райз.
Чисто по человечески это вроде как действия, и потому подлежать записи и обработке. Но пользоватся автоматиком становится геморно до жути.
Вот нафига гдето (вне автомата) хранить правила об АВТОМАТИЧЕСКИХ действиях и следовать им?
Поэтому внутренние переключения автомата предлагаю не считать ходом игрока. 
Нужно ли каждый раз в ходе передавать номер игрока? Это пока не обременяет, потому оставим.

4. Карты. Скоро доберёмся и до них.

Ну что, пошли переписывать с 0 наш автоматик *О*
*/


#include <string>
#include <vector>
#include <fstream>
#include <cstdint>   
#include "Cards.h"




#pragma once

#define numb_of_players 9

using namespace std;


enum class move_types{fold, call, check, bet, raise};
enum class table_states {start, preflop, flop, turn, river, end_of_game};
enum class player_state {all_in, folded, empty_seat, in_play}; 


string Tablestate_to_string(table_states table_state){
	switch (table_state){
		case table_states::start: return "start";
		case table_states::preflop: return "preflop";
		case table_states::flop: return "flop";
		case table_states::turn: return "turn";
		case table_states::river: return "river";
		case table_states::end_of_game: return "end_of_game";
		default: return "error";
	}
}
string Movetype_to_string(move_types move_type){
	switch (move_type){
		case move_types::fold: return "fold";
		case move_types::check: return "check";
		case move_types::bet: return "bet";
		case move_types::call: return "call";
		case move_types::raise: return "raise";
		default: return "error";
	}
}
move_types MoveTypefromString(string str){
	if (str == "fold") return move_types::fold;
	if (str == "check") return move_types::check;
	if (str == "bet") return move_types::bet;
	if (str == "call") return move_types::call;
	if (str == "raise") return move_types::raise;
}

string PlayerState_to_string(player_state state){
	switch (state){
	case player_state::all_in: return "all_in";
	case player_state::empty_seat: return "empty_seat";
	case player_state::folded: return "folded";
	case player_state::in_play: return "in_play";
	default: return "error";
	}
}



struct Move{
	Move(int player, int value, move_types type){
		Move::player = player;
		Move::value = value;
		Move::move_type = type;
	}
	int player;
	int value;
	move_types move_type;
};

Move StringToMove(string str){

	Move move(0, 0, move_types::raise);

	if (str.substr(0, 4) == "fold"){
		move.move_type = move_types::fold;
	}
	if (str.substr(0, 5) == "check"){
		move.move_type = move_types::check;
	}
	if (str.substr(0, 4) == "call"){
		move.move_type = move_types::call;
		move.value = atoi(str.substr(5,str.size()-5).c_str());
		}
	if (str.substr(0, 3) == "bet"){
		move.move_type = move_types::bet;
		move.value = atoi(str.substr(4,str.size()-4).c_str());
		}
	if (str.substr(0, 5) == "raise"){
		move.move_type = move_types::raise;
		move.value = atoi(str.substr(6,str.size()-6).c_str());
		}
	return move;
}



class Poker_FSM{
public:
	
	Poker_FSM(int button, int big_blind, int small_blind, int ante, int* Stacks);
	Poker_FSM(string filename);
	void Init(int button, int big_blind, int small_blind, int ante, int* Stacks);
	int Calculate();
	int WhoseMove();
	string CurrentState();
	int SaveFSM(string filename);
	int LoadFSM(string filename);
	int AddMove(Move move);
	string last_error;

	uint8_t PlayerCards[numb_of_players][2];
	uint8_t TableCards[5];


private:
	//Available mowes
	int whose_move; 
	bool fold_available;
	bool check_available;
	bool raise_available;
	bool call_available;
	bool bet_available;
	int value_to_call;
	int min_bet;
	int max_bet;
	int min_raise;
	int max_raise;
	bool IsAvalMove(Move move);
	void ClearAvailableMoves();
	void SetRestrictios();
	
	//Свойство каждого игрока
	int Stacks[numb_of_players];
	int Bets[numb_of_players];
	bool Rights_on_raise[numb_of_players];
	player_state player_states[numb_of_players]; 
	uint32_t Hands[numb_of_players];
	
	//Свойства стола
	table_states table_state;
	vector <Move> Moves; //Ходы!!!
	int big_blind;
	int small_blind;
	int ante;
	int button;
	int sb_position;
	int bb_position;
	int last_bet_reraise;
	
	//Служебные функции
	int NextPlayerSimple();
	int NextActivePlayer();
	bool IsRoundCompleet();
	bool IsActivePlayer(int player);
	void IncrTableState();
	bool IsAllBetsEqual();
	void EndOfGame();

};

bool Poker_FSM::IsAllBetsEqual(){
	//А что если игрок поставилл миллион, а 100 его заколил алином и больше никого не осталось?
	//Тогда в игре 1 игрок и его ставка равна сама себе!
	int bet = 0;
	for (int seat=0; seat<numb_of_players; seat++){
		if ((IsActivePlayer(seat))&&(bet == 0)) bet = Bets[seat];
		if ((Bets[seat] != bet)&&(IsActivePlayer(seat))) return 0;
	}
	return 1;
}

bool Poker_FSM::IsAvalMove(Move move){
	if (move.player != whose_move) return 0;
	if (move.move_type == move_types::fold){
		if (!fold_available) return 0;
	}
	if (move.move_type == move_types::check){
		if (!check_available) return 0;
	}
	if (move.move_type == move_types::call){
		if (!call_available) return 0;
		if (move.value != value_to_call) return 0;
	}
	if (move.move_type == move_types::bet){
		if (!bet_available) return 0;
		if (move.value < min_bet) return 0;
		if (move.value > max_bet) return 0;
	}
	if (move.move_type == move_types::raise){
		if (!raise_available) return 0;
		if (move.value < min_raise) return 0;
		if (move.value > max_raise) return 0;
	}
	return 1;
}

Poker_FSM::Poker_FSM(int button, int big_blind, int small_blind, int ante, int* Stacks){
	Init(button, big_blind, small_blind, ante, Stacks);
}

void Poker_FSM::Init(int _button, int _big_blind, int _small_blind, int _ante, int* _Stacks){

	//Стартуем машину
	//Карты важны только в самом конце машины при дележе денег
	Poker_FSM::button = _button;
	Poker_FSM::big_blind = _big_blind;
	Poker_FSM::small_blind = _small_blind;
	Poker_FSM::ante = _ante;
	
	for (int seat = 0; seat<numb_of_players; seat++){
		Poker_FSM::Stacks[seat] = _Stacks[seat];
		Bets[seat] = 0;
		if (Poker_FSM::Stacks[seat] > 0){
			player_states[seat] = player_state::in_play;
			Rights_on_raise[seat] = 0;
		}
		else{
			player_states[seat] = player_state::empty_seat;
			Rights_on_raise[seat] = 0;
		}
		Hands[seat] = 0;
	}
	//Делаем автоматические ставки
	//sb
	whose_move = button;
	whose_move = NextActivePlayer();
	sb_position = whose_move;
	if (Stacks[whose_move] > small_blind + ante){
		Stacks[whose_move] -= small_blind + ante;
		Bets[whose_move] += small_blind + ante;
	}
	else{
		Bets[whose_move] = Stacks[whose_move];
		Stacks[whose_move] = 0;
		player_states[whose_move] = player_state::all_in;
	}
	whose_move = NextActivePlayer();
	bb_position = whose_move;
	//bb
	if (Stacks[whose_move] > big_blind + ante){
		Stacks[whose_move] -= big_blind + ante;
		Bets[whose_move] += big_blind + ante;
	}
	else{
		Bets[whose_move] = Stacks[whose_move];
		Stacks[whose_move] = 0;
		player_states[whose_move] = player_state::all_in;
	}
	
	whose_move = NextActivePlayer();
	//Ante
	for (int seat=0; seat<numb_of_players; seat++){
		if ((seat != sb_position)&&(seat != bb_position)&&(IsActivePlayer(seat))){
			if (Stacks[seat] > ante){
				Stacks[seat] -= ante;
				Bets[seat] += ante;
			}
			else{
				Bets[seat] = Stacks[whose_move];
				Stacks[seat] = 0;
				player_states[seat] = player_state::all_in;
			}
		}
	}

	//Начинаем префлоп
	table_state = table_states::start;
	IncrTableState();

	
}

void Poker_FSM::IncrTableState(){
	//Определяем кол-во активных игроков
	int active_players = 0;
	for (int seat=0; seat<numb_of_players; seat++) if (IsActivePlayer(seat)) active_players++;
	//И если играть больше некому - конец игры
	if (active_players <= 1){table_state = table_states::end_of_game; EndOfGame(); return;}
	
	if (table_state == table_states::start){
		//Раздаём всем активным игрокам право повысить
		for (int seat=0; seat<numb_of_players; seat++) if (IsActivePlayer(seat)) Rights_on_raise[seat] = 1;
		//Даём право хода следующему за бб
		whose_move = bb_position;
		whose_move = NextActivePlayer();
		last_bet_reraise = big_blind;
		//Накладываем ограничения на ход
		SetRestrictios();
		//Ставим новый раунд и выходим слушать ходы игроков
		table_state = table_states::preflop;
		if (IsRoundCompleet()) IncrTableState();
		return;
	}

	if (table_state == table_states::preflop){
		//Раздаём всем активным игрокам право повысить
		for (int seat=0; seat<numb_of_players; seat++) if (IsActivePlayer(seat)) Rights_on_raise[seat] = 1;
		//Даём право хода следующему за батоном
		whose_move = button;
		whose_move = NextActivePlayer();
		last_bet_reraise = big_blind;
		//Накладываем ограничения на ход
		SetRestrictios();
		//Ставим новый раунд и выходим слушать ходы игроков
		table_state = table_states::flop;
		if (IsRoundCompleet()) IncrTableState();
		return;
	}

	if (table_state == table_states::flop){
		//Раздаём всем активным игрокам право повысить
		for (int seat=0; seat<numb_of_players; seat++) if (IsActivePlayer(seat)) Rights_on_raise[seat] = 1;
		//Даём право хода следующему за батоном
		whose_move = button;
		whose_move = NextActivePlayer();
		last_bet_reraise = big_blind;
		//Накладываем ограничения на ход
		SetRestrictios();
		//Ставим новый раунд и выходим слушать ходы игроков
		table_state = table_states::turn;
		if (IsRoundCompleet()) IncrTableState();
		return;
	}

	if (table_state == table_states::turn){
		//Раздаём всем активным игрокам право повысить
		for (int seat=0; seat<numb_of_players; seat++) if (IsActivePlayer(seat)) Rights_on_raise[seat] = 1;
		//Даём право хода следующему за батоном
		whose_move = button;
		whose_move = NextActivePlayer();
		last_bet_reraise = big_blind;
		//Накладываем ограничения на ход
		SetRestrictios();
		//Ставим новый раунд и выходим слушать ходы игроков
		table_state = table_states::river;
		if (IsRoundCompleet()) IncrTableState();
		return;
	}

	if (table_state == table_states::river){
		//Раздаём всем активным игрокам право повысить
		table_state = table_states::end_of_game;
		return;
	}

	return;
}


bool Poker_FSM::IsRoundCompleet(){
	/* 2. очень важный вопрос: Когда прерывать передачу ходов по кругу?
	- самое очевидное - когда остался в торгах 1 или меньше игроков. нет. А может одному игроку надо заколить или спасовать.
	- право заколить?
	- менее очевидное: Когда все ставки равны(кроме оленей) И ни у кого не осталось права повысить. */
	int active_players = 0;
	for (int seat=0; seat<numb_of_players; seat++) if (IsActivePlayer(seat)) active_players++;


	//Максимальный прошлый бет
	int MaxOfBets = 0;
	int max_player = 0;
	for (int seat=0; seat<numb_of_players; seat++){
		if (Bets[seat] > MaxOfBets){
			MaxOfBets = Bets[seat];
			max_player = seat;
		}
	}
	

	if ((active_players == 1)&&(IsActivePlayer(max_player))) {EndOfGame(); return 1;}
	
	if (active_players == 0) {EndOfGame(); return 1;}

	for (int seat=0; seat<numb_of_players; seat++) if (Rights_on_raise[seat]) return 0;
	
	if (!IsAllBetsEqual()) return 0;
	
	return 1;
}
bool Poker_FSM::IsActivePlayer(int player){
	if (!((player_states[player] == player_state::empty_seat)||
			(player_states[player] == player_state::all_in)||
			(player_states[player] == player_state::folded))) return 1;
	else return 0;
}

int Poker_FSM::NextPlayerSimple(){
	if ((whose_move >= 0)&&(whose_move < numb_of_players-1)){
		return whose_move+1;
	}
	else return 0;
}

int Poker_FSM::NextActivePlayer(){
	int old_whose_move = whose_move;
	while ((whose_move = NextPlayerSimple()) != old_whose_move){
		if (IsActivePlayer(whose_move)) {
			int temp = whose_move;
			whose_move = old_whose_move;
			return temp;
			break;
		}
	}
	//Если пришли сюда - то нет больше игроков не алин, не фолд и не емпти сит
	return -1;
}

int Poker_FSM::WhoseMove(){
	return whose_move;
}

int Poker_FSM::AddMove(Move move){
	//Отсеиваем неправильные ходы
	if (!IsAvalMove(move)) {last_error = "Not available move"; return 0;} //ошибка
	
	//Максимальный прошлый бет
	int MaxOfBets = 0;
	for (int seat=0; seat<numb_of_players; seat++) if (Bets[seat] > MaxOfBets) MaxOfBets = Bets[seat];


	switch (move.move_type){
	case move_types::fold:
		player_states[whose_move] = player_state::folded;
		Rights_on_raise[whose_move] = 0;

		whose_move = NextActivePlayer();
		if (IsRoundCompleet()) IncrTableState();
		SetRestrictios();
		Moves.push_back(move);
		return 1; //ок
	
	case move_types::call:
		Rights_on_raise[whose_move] = 0;
		Stacks[whose_move] -= move.value;
		Bets[whose_move] += move.value;
		if (Stacks[whose_move] == 0) player_states[whose_move] = player_state::all_in; 

		whose_move = NextActivePlayer();
		if (IsRoundCompleet()) IncrTableState();
		SetRestrictios();
		Moves.push_back(move);
		return 1; //ок

	case move_types::check:
		Rights_on_raise[whose_move] = 0;
		whose_move = NextActivePlayer();
		
		if (IsRoundCompleet()) IncrTableState();
		SetRestrictios();
		Moves.push_back(move);
		return 1; //ок

	case move_types::bet:
		Rights_on_raise[whose_move] = 0;
		if (move.value >= last_bet_reraise){
			//Даём возможность всем остальным сделать райз
			for (int seat=0; seat<numb_of_players; seat++){
				if ((IsActivePlayer(seat))&&(seat != whose_move)) Rights_on_raise[seat] = 1;
			}
			//Поднимаем last_bet_reraise до уровня bet
			last_bet_reraise = move.value;
		}
		Stacks[whose_move] -= move.value;
		Bets[whose_move] += move.value;
		if (Stacks[whose_move] == 0) player_states[whose_move] = player_state::all_in; 

		whose_move = NextActivePlayer();
		if (IsRoundCompleet()) IncrTableState();
		SetRestrictios();
		Moves.push_back(move);
		return 1; //ок

	case move_types::raise:
		Rights_on_raise[whose_move] = 0;

		if (move.value + Bets[whose_move] - MaxOfBets  >= last_bet_reraise){
			//Даём возможность всем остальным сделать райз
			for (int seat=0; seat<numb_of_players; seat++){
				if ((IsActivePlayer(seat))&&(seat != whose_move)) Rights_on_raise[seat] = 1;
			}
			//Поднимаем last_bet_reraise до уровня превышения над максимумом 
			last_bet_reraise = move.value + Bets[whose_move] - MaxOfBets;
		}

		Stacks[whose_move] -= move.value;
		Bets[whose_move] += move.value;
		if (Stacks[whose_move] == 0) player_states[whose_move] = player_state::all_in; 

		whose_move = NextActivePlayer();
		if (IsRoundCompleet()) IncrTableState();
		SetRestrictios();
		Moves.push_back(move);
		return 1; //ок

		default: {last_error = "unknown_error AddMove"; return 0;} //error
	}
	last_error = "unknown_error AddMove";
	return 0;
}






string Poker_FSM::CurrentState(){

	string number;
	string otvet = " =======  Poker FSM for 9 players    =======\n\n";

	otvet += " Blinds: " + to_string(small_blind) + "/" + to_string(big_blind);
	if (ante) otvet += "  Ante: " + to_string(ante);
	otvet += "\n";

	otvet += " Round: " + Tablestate_to_string(table_state) + "\n";
	
	otvet += " last raise: " + to_string(last_bet_reraise) + "\n";
	otvet += " Table cards: " + Cards_processor::Card_to_string(TableCards[0]) + " ";
	otvet += Cards_processor::Card_to_string(TableCards[1]) + " ";
	otvet += Cards_processor::Card_to_string(TableCards[2]) + " ";
	otvet += Cards_processor::Card_to_string(TableCards[3]) + " ";
	otvet += Cards_processor::Card_to_string(TableCards[4]) + "\n";

	
	int pot = 0;
	for (int seat = 0; seat < numb_of_players; seat++) pot += Bets[seat];
	otvet += " Pot: $" + to_string(pot) + "\n\n";


	otvet += " |---|------|-----|---------|--------|--------|---|\n";
	otvet += " |BTN|Player|Cards| Status  | Stack  |  Bet   |R/a|\n";
	otvet += " |---|------|-----|---------|--------|--------|---|\n";

	for (int seat = 0; seat < numb_of_players; seat++){
		otvet += " |";
		if (seat == button) otvet += "btn|";
		else if (seat == sb_position) otvet += "sb |";
		else if (seat == bb_position) otvet += "bb |";
		else otvet += "   |";

		otvet += "  #" + to_string(seat) + "  |";
		
		otvet += Cards_processor::Card_to_string(PlayerCards[seat][0]) + " " + Cards_processor::Card_to_string(PlayerCards[seat][1]) + "|";
		
		
		
		if (player_states[seat] == player_state::all_in) otvet += " all-in  |";
		if (player_states[seat] == player_state::empty_seat) otvet += "  empty  |";
		if (player_states[seat] == player_state::folded) otvet += " folded  |";
		if (player_states[seat] == player_state::in_play) otvet += " in play |";

		number = to_string(Stacks[seat]);
		otvet += " $" + number;
		for (int i=5; i >= number.size(); i--) otvet +=" ";
		otvet += "|";

		number = to_string(Bets[seat]);
		otvet += " $" + number;
		for (int i=5; i >= number.size(); i--) otvet +=" ";
		otvet += "|";

		if (Rights_on_raise[seat]) otvet += " + |";
		else otvet += "   |";

		//otvet += "bet: " + to_string(Bets[seat]) + "\n";
		otvet += "\n";
		otvet += " |---|------|-----|---------|--------|--------|---|\n";
	}
	otvet += "\n";
	
	if (table_state != table_states::end_of_game){
		otvet += " Player " + to_string(whose_move) + " choose your move: ";
		if (fold_available) otvet += "fold";
		if (check_available) otvet += ", check";
		if (call_available) otvet += ", call " + to_string(value_to_call);
		if (bet_available) otvet += ", bet " + to_string(min_bet) + "-" + to_string(max_bet);
		if (raise_available) otvet += ", raise " + to_string(min_raise) + "-" + to_string(max_raise);
	} else {
		otvet += "Game over. Type 'calc' to calculate the results\n";
	}
		
	otvet += "\n";



	return otvet;
}

Poker_FSM::Poker_FSM(string filename){
	LoadFSM(filename);
}


int Poker_FSM::LoadFSM(string filename){
	
	ifstream my_file;
	my_file.open(filename, ofstream::in);
	if(!my_file) {last_error = "Could not open file"; return 0;}
	string line;
	string sub_str;
	int begin;
	Move move(0, 0, move_types::raise);

	int _button;
	int _small_blind;
	int _big_blind;
	int _ante;
	int _Stacks[numb_of_players];
	
	getline(my_file, line);
	begin = line.find('=');
	sub_str = line.substr(begin+1, line.size()-begin);
	_button = atoi(sub_str.c_str());
	
	getline(my_file, line);
	begin = line.find('=');
	sub_str = line.substr(begin+1, line.size()-begin);
	_small_blind = atoi(sub_str.c_str());
	
	getline(my_file, line);
	begin = line.find('=');
	sub_str = line.substr(begin+1, line.size()-begin);
	_big_blind = atoi(sub_str.c_str());

	getline(my_file, line);
	begin = line.find('=');
	sub_str = line.substr(begin+1, line.size()-begin);
	_ante = atoi(sub_str.c_str());

	for (int seat = 0; seat < numb_of_players; seat++){
		getline(my_file, line);
		begin = line.find('=');
		sub_str = line.substr(begin+1, line.size()-begin);
		_Stacks[seat] = atoi(sub_str.c_str());
	}

	Init(_button, _big_blind, _small_blind, _ante, _Stacks);

	while (getline(my_file, line)){
		begin = line.find('=');
		sub_str = line.substr(begin+1, line.size()-begin);
		move.player = atoi(sub_str.c_str());

		getline(my_file, line);
		begin = line.find('=');
		sub_str = line.substr(begin+1, line.size()-begin);
		move.value = atoi(sub_str.c_str());

		getline(my_file, line);
		begin = line.find('=');
		sub_str = line.substr(begin+1, line.size()-begin);
		move.move_type = MoveTypefromString(sub_str);

		AddMove(move);
	}
	return 1;
}


int Poker_FSM::SaveFSM(string filename){

	ofstream my_file;
	my_file.open(filename, ofstream::trunc|ofstream::out);
	my_file<<"button="<<to_string(button)<<"\n";
	my_file<<"small_blind="<<to_string(small_blind)<<"\n";
	my_file<<"big_blind="<<to_string(big_blind)<<"\n";
	my_file<<"ante="<<to_string(ante)<<"\n";


	for (int seat = 0; seat < numb_of_players; seat++){
		my_file<<"Start_stack #"<<seat<<" ="<<Stacks[seat] + Bets[seat]<<"\n";
	}

	for ( vector<Move>::iterator it = Moves.begin(); it != Moves.end(); ++it ){

		my_file<<"player="<<to_string(it->player)<<"\n";
		my_file<<"value="<<to_string(it->value)<<"\n";
		my_file<<"move_type="<<Movetype_to_string(it->move_type)<<"\n";
	}

	my_file.close();
	return 0;
}

void Poker_FSM::EndOfGame(){

	table_state = table_states::end_of_game;
	whose_move = -1;






}


void Poker_FSM::SetRestrictios(){
	//Чистим все предыдушие ограничения(возможности)
	fold_available = 0;
	check_available = 0;
	raise_available = 0;
	call_available = 0;
	bet_available = 0;
	value_to_call = 0;
	min_bet = 0;
	max_bet = 0;
	min_raise = 0;
	max_raise = 0;
	int MaxOfBets = 0;
	for (int seat=0; seat<numb_of_players; seat++) if (Bets[seat] > MaxOfBets) MaxOfBets = Bets[seat];

	//Если игрок активный - он полюбому может спасовать
	if (IsActivePlayer(whose_move)) fold_available = 1;
	//Если всё уравняно - он может сказать чек
	if (IsAllBetsEqual()) check_available = 1;
	//Если до него всё было уравняно - то он может сказать и бет
	if (IsAllBetsEqual()){
		bet_available = 1;
		if (Stacks[whose_move] > big_blind){
			min_bet = big_blind;
			max_bet = Stacks[whose_move];
		}
		else{
			min_bet = Stacks[whose_move];
			max_bet = Stacks[whose_move];
		}
	}
	//Если до него не всё уравняно - он может сделать call
	if (!IsAllBetsEqual()){
		call_available = 1;
		value_to_call = MaxOfBets - Bets[whose_move];
		if (value_to_call > Stacks[whose_move]) value_to_call = Stacks[whose_move];
	}
	//Если до него ставили, и у него денег больше чем на колл и у него есть право на райз - он может райз
	if ((!IsAllBetsEqual())&&(Stacks[whose_move] > MaxOfBets - Bets[whose_move])&&(Rights_on_raise[whose_move])){
		raise_available = 1;
		max_raise = Stacks[whose_move];
		min_raise = MaxOfBets - Bets[whose_move] + last_bet_reraise;
		if (min_raise > max_raise) min_raise = max_raise;
	}
}


int Poker_FSM::Calculate(){
	//Создаём переменные
	int side_pot = 0;
	bool InPlay[numb_of_players];
	int MinBet = 0;
	bool IsClaimToRevard[numb_of_players];
	uint32_t best_hand;
	int winers;
	bool IsWin[numb_of_players];
	int seat;
	//Проверяем всё ли готово для калькуляции
	if (table_state != table_states::end_of_game) return 0;
		
	//Заполняем промежуточные данные
	//Выясняем кто претендует на пот, заодно считаем силу его руки
	for (seat = 0; seat < numb_of_players; seat++){
		if ((player_states[seat] == player_state::all_in)||(player_states[seat] == player_state::in_play)){
			IsClaimToRevard[seat] = 1;
			Hands[seat] = Cards_processor::GetHand(PlayerCards[seat][0], PlayerCards[seat][1], TableCards[0],  TableCards[1], TableCards[2], TableCards[3], TableCards[4]); 
		}
		else {
			IsClaimToRevard[seat] = 0;
			Hands[seat] = 0;
		}
	}

	do {
		//Из тех кто претендует находим у кого минимальная ставка и какая лутшая рука
		MinBet = 0;
		best_hand = 0;
		for (seat = 0; seat < numb_of_players; seat++){
			if (IsClaimToRevard[seat]){
				if ((MinBet == 0)&&(Bets[seat] != 0)) MinBet = Bets[seat];
				if ((Bets[seat] != 0)&&(Bets[seat] < MinBet)) MinBet = Bets[seat];
				if (Hands[seat] > best_hand) best_hand = Hands[seat];
			}
		}
		
		//Если больше делить нечего - выходим из цикла
		if (MinBet == 0) break;

		//Определяем кто выиграл/поделил боковой пот
		winers = 0;
		for (seat = 0; seat < numb_of_players; seat++){
			if (IsClaimToRevard[seat]){
				if (Hands[seat] == best_hand){
					IsWin[seat] = 1;
					winers ++;
				}
			}
		}
		
		//Формируем боковой пот с помощью вычета минимальной ставки претендента из всех ставок за столом
		side_pot = 0;
		for (seat = 0; seat < numb_of_players; seat++){
			if (Bets[seat] > MinBet) {
				side_pot += MinBet;
				Bets[seat] += MinBet;
			} else {
				side_pot += Bets[seat];
				Bets[seat] = 0;
				//Дальше этот игрок не претендует на другие поты
				IsClaimToRevard[seat] = 0;
			}
		}

		//Делим боковой пот между победителями
		//Делим нацело
		int rest = side_pot % winers;
		for (seat = 0; seat < numb_of_players; seat++){
			if (IsWin[seat]){
				Stacks[seat] += (side_pot-rest)/winers;
			}
		}
		//Расдаём сдачу всем победителям начиная с малого блайнда
		seat = sb_position;
		while (rest){
			Stacks[seat]++;
			rest--;
			seat++;
			if (seat >= numb_of_players) seat = 0;
		}
		//УРААААААААА! поделили один пот :)

	} while (1);


	return 1;
}