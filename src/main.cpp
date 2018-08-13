#include "game.hpp"


int main(int argc, char **argv) {

	Game game;


	//init with verbose enabled
	if (argc != 3){
		std::cout << "bad args.. starting game with defaults" << std::endl;
		game.init();
	}
	else
		game.init(1, std::atoi(argv[1]), std::atoi(argv[2]), 0);

	game.log("he he xD");
	game.runLoop();
	
}