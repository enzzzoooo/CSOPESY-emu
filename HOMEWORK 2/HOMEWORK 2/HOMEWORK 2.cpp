#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>

void printLogo() {
	std::cout << "  ___  ____   __   ____  ____  ____  _  _ " << std::endl;
	std::cout << " / __)/ ___) /  \\ (  _ \\(  __)/ ___)( \\/ )" << std::endl;
	std::cout << "( (__ \\___ \\(  O ) ) __/ ) _) \\___ \\ )  / " << std::endl;
	std::cout << " \\___)(____/ \\__/ (__)  (____)(____/(__/  " << std::endl;
}

void clearScreen() {
	std::system("cls");

}

void printHeader() {
	clearScreen();
	printLogo();
	std::cout << "\033[32mHello. Welcome to CSOPESY commandLine!\033[0m" << std::endl;
	std::cout << "Type \033[33m'exit'\033[0m to quit, \033[33m'clear'\033[0m to clear the screen" << std::endl;
	std::cout << "Enter a command: " << std::endl;
}

std::string toLower(const std::string& str) {
	std::string result = str;
	for (char& c : result) {
		c = std::tolower(c);
	}
	return result;
}

int main() {
	printHeader();
	
	std::string command;
	bool running = true;
	
	while (running) {
		std::cout << "> ";
		std::getline(std::cin, command);
		
		//JIC SOMEONE DECIDED NA MAGING PILOSOPO AND TYPE INITIALIZE INSTEAD OF initialize VERY FUNNY
		std::string lowerCommand = toLower(command);
		
		if (lowerCommand == "initialize") {
			std::cout << "initialize command recognized. Doing something." << std::endl;
		}
		else if (lowerCommand == "screen") {
			std::cout << "screen command recognized. Doing something." << std::endl;
		}
		else if (lowerCommand == "scheduler-test") {
			std::cout << "scheduler-test command recognized. Doing something." << std::endl;
		}
		else if (lowerCommand == "scheduler-stop") {
			std::cout << "scheduler-stop command recognized. Doing something." << std::endl;
		}
		else if (lowerCommand == "report-util") {
			std::cout << "report-util command recognized. Doing something." << std::endl;
		}
		else if (lowerCommand == "clear") {
			printHeader();
		}
		else if (lowerCommand == "exit") {
			std::cout << "exit command recognized. Exiting program." << std::endl;
			running = false;
		}
		else {
			std::cout << "Unknown command: " << command << std::endl;
		}
	}
	
	return 0;
}
