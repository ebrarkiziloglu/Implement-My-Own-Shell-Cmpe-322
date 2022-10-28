#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std; 
#define MAX_COMMAND_LEN 300
char command[MAX_COMMAND_LEN];
bool isterminated = false;
string delimiter = " ";
string dividor = ">";
string username = getenv("USER"); 

void readLineByLine(string pathToFile);
void redirectFile(string command);

int main() {
    username.append(" >>> ");
    
    while(! isterminated){
        cout << username; 
        cin.getline(command, MAX_COMMAND_LEN);
        string command_str = command;
        string command_prefix = command_str.substr(0, command_str.find(delimiter));
        if(command_str.compare("listdir") == 0) {
            system("ls");
        }
        else if(command_str.compare("mycomputername") == 0) {
            system("hostname");     // system("hostnamectl | grep 'hostname'");
        }
        else if(command_str.compare("whatsmyip") == 0) {
            system("curl ifconfig.me");
            cout << endl;
        }
        else if(command_prefix.compare("printfile") == 0) {       

            if(command_str.find(dividor) == string::npos) { // printfile (fileName)
                readLineByLine(command_str);
            }
            else{                                           // printfile (fileName) > (newFileName)
                redirectFile(command_str);
            }
            
        }
        else if(command_prefix.compare("dididothat") == 0) {


            // After the command "myuser >>> printfile myfile.txt", which of the following commands will return "Yes"?
            // a) myuser >>> dididothat "printfile" : YES
            // b) myuser >>> dididothat "printfile myfile.txt" : NO

            // Add dididothat command to history too!
        }
        else if(command_str.compare("hellotext") == 0) {
            int c = system("gedit");
            if(c == 0) {
                continue;
            }
            //// When the text editor is closed, user should be able to continue using this shell.
        }
        else if(command_str.compare("exit") == 0) {
            isterminated = true;
            break;
        }
    }
    return 0;
}

void readLineByLine(string command) {
    string filename = command.substr(10, command.length() - 9);
    char path[300];
    getcwd(path, 300);
    fstream file;
    bool endofFile = false;
    file.open(filename);
    string line;
    string input = "";
    if(file.is_open()) {
        do{
            if(file.eof()) {
                endofFile = true;
                break;
            }
            getline(file, line);
            cout << line;
            getline(cin, input);
        } while(input.length() == 0 && ! endofFile);
    }
    file.close();
}

void redirectFile(string command) {
    int dividorLocation = command.find(dividor);
    string firstFilename = command.substr(10, dividorLocation - 9 - 2);
    string secondFilename = command.substr(dividorLocation + 2, command.length() - dividorLocation);
    fstream firstFile;
    firstFile.open(firstFilename);
    ofstream secondFile(secondFilename, ios::trunc);
    string line;
    if(( firstFile.is_open() ) && ( secondFile.is_open() )) {
        while(!firstFile.eof()) {
            getline(firstFile, line);
            secondFile << line << endl;
        }
    }
    firstFile.close();
    secondFile.close();
}