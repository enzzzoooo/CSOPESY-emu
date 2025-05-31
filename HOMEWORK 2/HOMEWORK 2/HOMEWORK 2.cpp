#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <map>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <vector>

enum class ScreenType {
    MAIN_CONSOLE,
    PROCESS_CONSOLE
};

class Process {
public:
    std::string name;
    std::string timestamp;

    Process(const std::string& processName) : name(processName) {
        timestamp = getCurrentTimestamp();
    }

private:
    std::string getCurrentTimestamp() {
        std::time_t now = std::time(0);
        std::tm localTime;
#ifdef _WIN32
        localtime_s(&localTime, &now);
#else
        localtime_r(&now, &localTime);
#endif

        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << (localTime.tm_mon + 1) << "/"
            << std::setfill('0') << std::setw(2) << localTime.tm_mday << "/"
            << (localTime.tm_year + 1900) << ", ";

        int hour = localTime.tm_hour;
        std::string ampm = (hour >= 12) ? "PM" : "AM";
        if (hour > 12) hour -= 12;
        if (hour == 0) hour = 12;

        oss << std::setfill('0') << std::setw(2) << hour << ":"
            << std::setfill('0') << std::setw(2) << localTime.tm_min << ":"
            << std::setfill('0') << std::setw(2) << localTime.tm_sec << " " << ampm;

        return oss.str();
    }
};

ScreenType currentScreen = ScreenType::MAIN_CONSOLE;
std::map<std::string, Process> processes;
std::string currentProcessName = "";

void printLogo() {
    std::cout << "  ___  ____   __   ____  ____  ____  _  _ " << std::endl;
    std::cout << " / __)/ ___) /  \\ (  _ \\(  __)/ ___)( \\/ )" << std::endl;
    std::cout << "( (__ \\___ \\(  O ) ) __/ ) _) \\___ \\ )  / " << std::endl;
    std::cout << " \\___)(____/ \\__/ (__)  (____)(____/(__/  " << std::endl;
}

void clearScreen() {
    std::system("cls");
}

std::string parseScreenCommand(const std::string& command, std::string& flag, std::string& name) {
    std::istringstream iss(command);
    std::string word;
    std::vector<std::string> tokens;

    while (iss >> word) {
        tokens.push_back(word);
    }

    if (tokens.size() == 2 && tokens[0] == "screen" && tokens[1] == "-ls") {
        flag = "-ls";
        name = "";
        return "valid";
    }

    if (tokens.size() != 3) {
        return "invalid";
    }

    if (tokens[0] != "screen") {
        return "invalid";
    }

    flag = tokens[1];
    name = tokens[2];

    if (flag == "-s" || flag == "-r") {
        return "valid";
    }

    return "invalid";
}

std::string toLower(const std::string& str) {
    std::string result = str;
    for (char& c : result) {
        c = std::tolower(c);
    }
    return result;
}

int main() {
    bool running = true;
    currentScreen = ScreenType::MAIN_CONSOLE;
    
    MainConsole mainConsole;
    ProcessConsole processConsole;
    
    while (running) {
        if (currentScreen == ScreenType::MAIN_CONSOLE) {
            mainConsole.run();
            if (currentScreen == ScreenType::MAIN_CONSOLE) {
                running = false; // exit loop
            }
        }
        else if (currentScreen == ScreenType::PROCESS_CONSOLE) {
            processConsole.run();
        }
    }
    
    return 0;
}

class Console {
public:
    virtual ~Console() = default;
    virtual void run() = 0;
    virtual void display() = 0;
};

class MainConsole : public Console {
public:
    void display() override {
        clearScreen();
        printLogo();
        std::cout << "\033[32mHello. Welcome to CSOPESY commandLine!\033[0m" << std::endl;
        std::cout << "Type \033[33m'exit'\033[0m to quit, \033[33m'clear'\033[0m to clear the screen" << std::endl;
        std::cout << "Enter a command: " << std::endl;
    }
    
    void run() override {
        display();
        
        std::string command;
        while (true) {
            std::cout << "> ";
            std::getline(std::cin, command);
            
            if (handleCommand(command)) {
                return; // Screen switch or exit requested
            }
        }
    }
    
private:
    bool handleCommand(const std::string& command) {
        std::string lowerCommand = toLower(command);
        std::string flag, name;
        
        if (lowerCommand == "screen") {
            std::cout << "Please specify a screen option: '-s <name>', '-r <name>', or '-ls'." << std::endl;
        }
        else if (parseScreenCommand(command, flag, name) == "valid") {
            return handleScreenCommand(flag, name);
        }
        else if (lowerCommand.rfind("screen ", 0) == 0) {
            std::cout << "Invalid screen command. Use '-s <name>', '-r <name>', or '-ls'." << std::endl;
        }
        else if (lowerCommand == "initialize") {
            std::cout << "initialize command recognized. Doing something." << std::endl;
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
            display();
        }
        else if (lowerCommand == "exit") {
            std::cout << "exit command recognized. Exiting program." << std::endl;
            currentScreen = ScreenType::MAIN_CONSOLE;
            return true;
        }
        else {
            std::cout << "Unknown command: " << command << std::endl;
        }
        
        return false;
    }
    
    bool handleScreenCommand(const std::string& flag, const std::string& name) {
        if (flag == "-ls") {
            listProcesses();
            return false;
        }
        else if (flag == "-s") {
            createProcess(name);
            return true;
        }
        else if (flag == "-r") {
            return reattachProcess(name);
        }
        return false;
    }
    
    void listProcesses() {
        if (processes.empty()) {
            std::cout << "No active processes found." << std::endl;
        }
        else {
            std::cout << "Active processes:" << std::endl;
            for (const auto& [procName, process] : processes) {
                std::cout << " - " << procName << " (created at " << process.timestamp << ")" << std::endl;
            }
        }
    }
    
    void createProcess(const std::string& name) {
        Process newProcess(name);
        processes[name] = newProcess;
        currentProcessName = name;
        currentScreen = ScreenType::PROCESS_CONSOLE;
    }
    
    bool reattachProcess(const std::string& name) {
        auto it = processes.find(name);
        if (it != processes.end()) {
            currentProcessName = name;
            currentScreen = ScreenType::PROCESS_CONSOLE;
            return true;
        }
        else {
            std::cout << "Process '" << name << "' not found." << std::endl;
            return false;
        }
    }
};

class ProcessConsole : public Console {
public:
    void display() override {
        Process& currentProcess = processes[currentProcessName];
        clearScreen();
        std::cout << "Process name: " << currentProcess.name << std::endl;
        std::cout << "Current line of instruction / Total line of instruction: 0/50" << std::endl;
        std::cout << "Timestamp: " << currentProcess.timestamp << std::endl;
        std::cout << std::endl;
        std::cout << "Type \033[33m'exit'\033[0m to return to main console" << std::endl;
    }
    
    void run() override {
        display();
        
        std::string command;
        while (true) {
            std::cout << "> ";
            std::getline(std::cin, command);
            
            if (handleCommand(command)) {
                return; // Exit requested
            }
        }
    }
    
private:
    bool handleCommand(const std::string& command) {
        std::string lowerCommand = toLower(command);
        
        if (lowerCommand == "exit") {
            currentProcessName = "";
            currentScreen = ScreenType::MAIN_CONSOLE;
            return true;
        }
        else {
            std::cout << "Unknown command in process console: " << command << std::endl;
            std::cout << "Type 'exit' to return to main console." << std::endl;
        }
        
        return false;
    }
};
