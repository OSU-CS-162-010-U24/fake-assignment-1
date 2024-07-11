#include <iostream>
#include <cstdlib>
#include <ctime>

bool player_wants_to_keep_playing() {
	char user_input;
	do {
		std::cout << "Do you want to play again? (Y/N): ";
		std::cin >> user_input;

		if (user_input != 'Y' && user_input != 'N') {
			std::cout << "Error: Invalid input!" << std::endl;
		}
	} while(user_input != 'Y' && user_input != 'N');
	
	return user_input == 'Y';
}

void initialize_game_board(char game_board[42]) {
	for (int i = 0; i < 42; i++) {
		game_board[i] = '_';
	}
}

int compute_1d_index(int row, int column) {
	return row * 7 + column;
}

int compute_row_index(int slot_index) {
	return slot_index / 7;
}

int compute_column_index(int slot_index) {
	return slot_index % 7;
}

void print_game_board(char game_board[42]) {
	std::cout << "Game board:" << std::endl;
	for (int i = 0; i < 7; i++) {
		std::cout << (i + 1) << ' ';
	}
	std::cout << std::endl;

	for (int i = 0; i < 6; i++) {
		// Print the ith row
		for (int j = 0; j < 7; j++) {
			// 2D index: (i, j)
			int slot_index = compute_1d_index(i, j);
			std::cout << game_board[slot_index] << ' ';
		}

		std::cout << std::endl;
	}
}

bool valid_placement(int column, char game_board[]) {
	if (column < 0 || column > 6) {
		return false;
	}

	int slot_index = compute_1d_index(0, column);
	if (game_board[slot_index] != '_') {
		return false;
	}

	return true;
}

int prompt_player_for_column_selection(char game_board[]) {
	int user_input;
	do {
		std::cout << "What column would you like to select? (1-7): ";
		std::cin >> user_input;
		user_input--;

		if (!valid_placement(user_input, game_board)) {
			std::cout << "Error: Invalid placement!" << std::endl;
		}
	} while(!valid_placement(user_input, game_board));
	
	return user_input;
}

void place_tile(char game_board[], int column_index, char tile) {
	for (int i = 5; i >= 0; i--) {
		// If the slot at row i, column column_index is empty ('_'),
		// place the tile there and return
		int slot_index = compute_1d_index(i, column_index);
		if (game_board[slot_index] == '_') {
			game_board[slot_index] = tile;
			return;
		}
	}
}

void place_player_tile(char game_board[], int column_index) {
	place_tile(game_board, column_index, 'X');
}

void place_computer_tile(char game_board[], int column_index) {
	place_tile(game_board, column_index, 'O');
}

void do_player_turn(char game_board[]) {
	// Print the game board
	print_game_board(game_board);
	
	// Prompt for column selection
	int column_selection = prompt_player_for_column_selection(game_board);
	
	// Place tile in location
	place_player_tile(game_board, column_selection);
}

int random_valid_column(char game_board[]) {
	int selected_column;
	do {
		// Select a random column index in 0-6
		selected_column = rand() % 7;
	} while(!valid_placement(selected_column, game_board));
	return selected_column;
}

void do_computer_turn(char game_board[]) {
	// Randomly select non-full column to place tile in
	int column_selection = random_valid_column(game_board);

	// Place the tile in the location
	place_computer_tile(game_board, column_selection);
}

bool check_four_consecutive(char game_board[], int row_dif, int column_dif, char tile) {
	for (int i = 0; i < 42; i++) {
		int i_row = compute_row_index(i);
		int i_column = compute_column_index(i);
		
		bool four_consecutive = true;
		for (int j = 0; j < 4; j++) {
			int j_row = i_row + j * row_dif;
			int j_column = i_column + j * column_dif;
			int slot_index = compute_1d_index(j_row, j_column);

			if (j_row < 0 || j_row > 5 || j_column < 0 ||
					j_column > 6 ||
					(game_board[slot_index] != tile)) {
				four_consecutive = false;
				break;
			}
		}
		if (four_consecutive) {
			return true;
		}
	}

	return false;
}

bool check_player_has_won(char game_board[]) {
	if (check_four_consecutive(game_board, 1, 0, 'X') ||
			check_four_consecutive(game_board, 0, 1, 'X') ||
			check_four_consecutive(game_board, 1, 1, 'X') ||
			check_four_consecutive(game_board, 1, -1, 'X')) {
		return true;
	}
	return false;
}

bool check_computer_has_won(char game_board[]) {
	if (check_four_consecutive(game_board, 1, 0, 'O') ||
			check_four_consecutive(game_board, 0, 1, 'O') ||
			check_four_consecutive(game_board, 1, 1, 'O') ||
			check_four_consecutive(game_board, 1, -1, 'O')) {
		return true;
	}
	return false;
}

bool check_full_board(char game_board[42]) {
	for (int i = 0; i < 42; i++) {
		if (game_board[i] == '_') {
			return false;
		}
	}

	return true;
}

void print_game_result(bool player_won, bool computer_won, bool tie) {
	if (player_won) {
		std::cout << "The player won!" << std::endl;
	} else if (computer_won) {
		std::cout << "The computer won!" << std::endl;
	} else {
		std::cout << "Tie!" << std::endl;
	}
}

void play_the_game() {
	// Create the game board
	// 6 x 7 = 42 slots
	char game_board[42];
	initialize_game_board(game_board);

	// Flip a coin to decide who goes first
	// rand() gives you an int between 0 and RAND_MAX
	bool players_turn = (rand() % 2) == 1;

	// While the game is not over, do the current player's turn
	bool player_won = false;
	bool computer_won = false;
	bool tie = false;
	while (!player_won && !computer_won && !tie) {
		if (players_turn) {
			do_player_turn(game_board);
			player_won = check_player_has_won(game_board);
		} else {
			do_computer_turn(game_board);
			computer_won = check_computer_has_won(game_board);
		}
		
		if (!player_won && !computer_won) {
			tie = check_full_board(game_board);
		}

		// Alternate player turns
		players_turn = !players_turn;
	}
	
	print_game_board(game_board);
	print_game_result(player_won, computer_won, tie);
}

int main() {
	srand(time(nullptr)); // ONLY DO THIS ONCE

	do {
		play_the_game();
	} while(player_wants_to_keep_playing());
}
