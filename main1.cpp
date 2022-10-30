#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <deque>

using namespace std; 
#define MAX_COMMAND_LEN 300
char command[MAX_COMMAND_LEN];      // command line to be read from the shell
bool isterminated = false;          // if the "exit" command runs, the shell terminates
std::string delimiter = " ";             // separates the command from its arguments
std::string dividor = ">";               // separates the file names in "printfile" command
std::string username = getenv("USER");   

void readLineByLine(std::string pathToFile);
void redirectFile(std::string command);

int main() {
    username.append(" >>> ");           // creates the shell welcome text
    deque<std::string> history;              // keeps history of the commands
    
    while(! isterminated){              // runs in loop untill "exit" command is executed
        while(history.size() > 15) {    // only keeps the last 15 commands in the history
            history.pop_front();
        }
        std::cout << username;               // starts the shell, ready to take command
        std::cin.getline(command, MAX_COMMAND_LEN);   // reads the command
        std::string command_str = command;            // converts command to the std::string
        
        // Remove the redundant whitespaces from the beginning and the end of the command:
        while(command_str.find_first_of(" ") == 0) {
            command_str = command_str.substr(1, command_str.size()-1);
        }
        while(command_str.find_last_of(" ") == 0) {
            command_str = command_str.substr(0, command_str.size()-1);
        }
        if(command_str.compare("") != 0) {      // if the command is not empty, adds it to the history
            history.push_back(command_str);
        }
        // if the command includes a whitespace: we need the first word to determine the type of the command:
        std::string command_prefix = command_str.substr(0, command_str.find(delimiter));
        
        if(command_str.compare("listdir") == 0) {
            system("ls");
        }
        else if(command_str.compare("mycomputername") == 0) {
            system("hostname");     // system("hostnamectl | grep 'hostname'");
        }
        else if(command_str.compare("whatsmyip") == 0) {
            system("curl ifconfig.me");
            std::cout << endl;
        }
        else if(command_prefix.compare("printfile") == 0) {       
            if(command_str.find(dividor) == std::string::npos) { 
                readLineByLine(command_str);        // printfile <fileName>
            }
            else{                                           
                redirectFile(command_str);          // printfile <fileName> > <newFileName>
            }
            
        }
        else if(command_prefix.compare("dididothat") == 0) {
            // remove "dididothat" command from the histroy:
            history.pop_back();
            // find the first whitespace in the command:
            int delimiterPosition = command_str.find(delimiter);
            // get the command to be checked:
            std::string commandToCheck = command_str.substr(delimiterPosition + 1, command_str.size() - delimiterPosition - 1);
            deque<std::string>::iterator iterator;
            bool commandFound = false;
            // iterate over the history to check if the command is there or not:
            for (iterator = history.begin(); iterator != history.end(); ++iterator) {
                if(commandToCheck.compare(*iterator) == 0) {    // command is in the history
                    std::cout <<"YES\n";         
                    commandFound = true;
                    iterator = history.end() - 1;
                }
            }
            if(! commandFound){                                 // command is not in the history
                std::cout << "NO\n";             
            }
        }
        else if(command_str.compare("hellotext") == 0) {
            int c = system("gedit");
            if(c == 0) { 
                // When the text editor is closed, system() returns 0, so the shell continues to process
                continue;
            }
        }
        else if(command_str.compare("exit") == 0) {
            isterminated = true;
            break;
        }
    }
    return 0;
}

// following method prints the given file line-by-line:
void readLineByLine(std::string command) {
    std::string filename = command.substr(10, command.length() - 9);
    char path[300];
    getcwd(path, 300);
    fstream file;
    bool endofFile = false;
    file.open(filename);
    std::string line;
    std::string input = "";
    if(file.is_open()) {
        do{
            if(file.eof()) {
                endofFile = true;
                break;
            }
            getline(file, line);
            std::cout << line;
            getline(std::cin, input);
        } while(input.length() == 0 && ! endofFile);
    }
    file.close();
}

// following method copies the given file's content into the provided output file, line-by-line
void redirectFile(std::string command) {
    int dividorLocation = command.find(dividor);
    std::string firstFilename = command.substr(10, dividorLocation - 11);
    std::string secondFilename = command.substr(dividorLocation + 2, command.length() - dividorLocation);
    fstream firstFile;
    firstFile.open(firstFilename);
    ofstream secondFile(secondFilename/*, ios::trunc */);
    std::string line;
    if(firstFile.is_open() && secondFile.is_open()) {
        while(!firstFile.eof()) {
            getline(firstFile, line);
            secondFile << line << endl;
        }
    }
    firstFile.close();
    secondFile.close();
}