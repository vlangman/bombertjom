#include "game.hpp"

int main(int argc, char **argv) {

	Game game;
	GameState	State;
	int answer = 0;

	State = GAME_MENU;
	//init with verbose enabled
	if (argc != 3){
		std::cout << "bad args.. starting game with defaults" << std::endl;
		if (State == GAME_MENU) {
			std::cout << "Press 1 to start\n";
			std::cout << "Press 2 to resume\n";
			std::cout << "Press 4 to start\n";

			std::cin >> answer;
			if (answer == 1)
				game.init();
			else if (answer == 2)
				game.init(1,  500,  500, 1);
			else if (answer == 4) {
				std::cout << "exiting\n";
				return 0;
			}
			else {
				std::cout << "Unknown\n";
				return 0;
			}
			
		}
		
	}
	else
	{
		game.init(1, std::atoi(argv[1]), std::atoi(argv[2]), 0);
	}

	game.runLoop();
	
}