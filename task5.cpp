#include <iostream>
#include <openssl/evp.h>
#include <fstream>



using namespace std;

char SALT[3] = "xx";
char passwd_file[] = "passwd";
string delim = ":";

bool password_requirments_passed(string password) {
    if (password.length() > 12) {
        cout << "Illegal password inputted, please input another password!";
        return false;
    }
    if (password.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890") != std::string::npos) {
        cout << "Illegal password inputted, please input another password!" << endl;
        return false;
    }
    return true;
}
//Encrypt supplied password and return hash
char* encrypt_password(char* password) {

}

// Check if user exists in passwd file
bool check_user_exists(string inputted_user) {
    ifstream user_lookupFile(passwd_file);
    string user_credential_details, user_name;
    while (getline(user_lookupFile, user_credential_details)) {
        //cout << "Line is " << user_credential_details << endl;
        user_name = user_credential_details.substr(0, user_credential_details.find(delim));
        //cout << "username = " << user_name << endl;
        if (user_name == inputted_user) {
            return true;
        }
    }
    return false;
}

int main(int argc, char *argv[]) {
    srand(time(nullptr) );
    string user_account, password;
    bool does_user_exists = false;
    int bad_password_counter = 0, group_id = 1005, maximum_password_attempts, user_id;
    user_id = rand() % 100 + 1000;

    if (argc != 2) {
        cout << "Incorrect number of arguments sent the program, please supply a maximum amount of password attempts argument";
        return 1;
    }
    maximum_password_attempts = atoi(argv[1]);
    if (maximum_password_attempts == 0) {
        cout << "Invalid max password attempts # inputted, please re-run the program with the correct argument supplied";
        return 1;
    }

    // Open passwd file, or create if does not exist
    fstream appendFileToWorkWith;
    appendFileToWorkWith.open(passwd_file, std::fstream::in | std::fstream::out | std::fstream::app);
    // If file does not exist, Create new file
    if (!appendFileToWorkWith) {
        cout << "passwd file doesnot exist yet, creating one now.";
        appendFileToWorkWith.open(passwd_file,  fstream::in | fstream::out | fstream::trunc);
    }
    appendFileToWorkWith.close();

    cout << "Please input your user id" << endl;
    cin >> user_account;
    while (user_account.length() < 3 || user_account.length() > 32) {
        cout << "Illegal user name inputted, try again" << endl;
        cout << "Please input your user id" << endl;
        cin >> user_account;
    }
    does_user_exists = check_user_exists(user_account);


    if (does_user_exists) {
        cout << "Input your password " << endl;
        cin >> password;
        while (!password_requirments_passed(password)) {
            bad_password_counter++;
            if (bad_password_counter >= maximum_password_attempts) {
                cout << "Too many unsuccessful attempts, exiting program";
                return 1;
            }
            cout << "Input your password " << endl;
            cin >> password;
            // Do password lookup here
        }
        // Good password continue
    } else {
        //Create new user in the passwd file.
        appendFileToWorkWith.open(passwd_file, std::fstream::in | std::fstream::out | std::fstream::app);
        cout << "No match found. Creating new profile" << endl;
        cout << "Please enter a password: " << endl;
        cin >> password;
        while (!password_requirments_passed(password)) {
            cout << "Please enter a password: " << endl;
            cin >> password;
        }
        appendFileToWorkWith << user_account << ":" << password << ":" << user_id << ":" << group_id << ":" << "CrptX user" << ":" << "/cryptx" << ":" << "/bin/bash" << "\n";
        cout << "A new profile has been created for you." << endl;
        appendFileToWorkWith.close();
    }

    cout << "Login Succesful" << endl;
    return 0;
}