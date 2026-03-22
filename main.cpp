#include <iostream>
#include "json.hpp"
#include <string>
#include <random>
#include <fstream>
#include <map>
# include<ctype.h>
using json = nlohmann::json;
using namespace std;
string pass_gen(int length=12)
{
    string pass = "";
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> distr4(65, 90);

    for (int i = 0; i < length / 3; i++)
        pass.push_back(char(distr4(gen)));

    if (length < 8)
    {
        uniform_int_distribution<> distr5(97, 122);
        for (int i = 0; i < length - (length / 3) - 1; i++)
            pass.push_back(char(distr5(gen)));
    }
    else
    {
        uniform_int_distribution<> distr6(97, 122);
        for (int i = 0; i < length - (length / 3) - 2; i++)
            pass.push_back(char(distr6(gen)));
    }

    if (length <= 8)
    {
        uniform_int_distribution<> distr(33, 47);
        uniform_int_distribution<> distr2(58, 64);
        uniform_int_distribution<> distr3(1, 2);

        int selector = distr3(gen);
        if (selector == 1)
            pass.push_back(char(distr(gen)));
        else
            pass.push_back(char(distr2(gen)));
    }
    else
    {
        uniform_int_distribution<> distr7(33, 47);
        uniform_int_distribution<> distr8(58, 64);
        uniform_int_distribution<> distr9(1, 2);

        for (int i = 0; i < 2; i++)
        {
            int selector_2 = distr9(gen);
            if (selector_2 == 1)
                pass.push_back(char(distr7(gen)));
            else
                pass.push_back(char(distr8(gen)));
        }
    }

    uniform_int_distribution<> distr10(48, 57);
    for (int j = 0; j < 3; j++)
        pass.push_back(char(distr10(gen)));

    return pass;
}

bool search(string p)
{
    ifstream inFile("data.json");
    if (!inFile.is_open())
    {
        cout << "Error IN DATABASE KINDLY CHECK data.json\n";
        return true;
    }

    if (inFile.peek() == ifstream::traits_type::eof())
    {
        inFile.close();
        return true;
    }

    json data;
    inFile >> data;
    inFile.close();

    for (auto &entry : data)
    {
        for (auto &[user, pass] : entry.items())
        {
            if (pass == p)
                return false; 
        }
    }

    return true; 
}
string search_pass(string p)
{
    ifstream inFile("data.json");
    if (!inFile.is_open())
    {
        cout << "Error IN DATABASE KINDLY CHECK data.json\n";
        return "";
    }

    if (inFile.peek() == ifstream::traits_type::eof())
    {
        inFile.close();
        return "";
    }

    json data;
    inFile >> data;
    inFile.close();
    for (auto &entry : data)
    {
        for (auto &[user, pass] : entry.items())
        {
            if (pass == p)
                cout<<user;
                return ""; // found
        }
    }

    cout<<"Password Not Found"; 
    return "";
}
bool appendToDatabase(const map<string, string>& entries)
{
    // Read existing data
    ifstream inFile("data.json");
    json data;
    
    if (!inFile.is_open())
    {
        data = json::array();
    }
    else if (inFile.peek() == ifstream::traits_type::eof())
    {
        data = json::array();
    }
    else
    {
        try {
            inFile >> data;
        }
        catch (json::parse_error& e) {
            cout << "Error parsing JSON: " << e.what() << endl;
            inFile.close();
            return false;
        }
    }
    inFile.close();
    for (const auto& [username, password] : entries)
    {
        json newEntry;
        newEntry[username] = password;
        data.push_back(newEntry);
    }
    ofstream outFile("data.json");
    if (!outFile.is_open())
    {
        cout << "Error: Cannot write to data.json" << endl;
        return false;
    }
    
    outFile << data.dump(4); 
    outFile.close();
    cout << "Successfully added " << entries.size() << " entries to database!" << endl;
    return true;
}
string getPassword(const std::string& username) {
    std::ifstream file("data.json");
    if (!file.is_open()) {
        return "Error: Could not open data.json";
    }
    json data;
    file >> data;
    for (const auto& entry : data) {
        for (const auto& [key, value] : entry.items()) {
            if (key == username) {
                return value.get<std::string>();
            }
        }
    }
    return "Error: Username not found";
}
bool updatePassword(const std::string& username, const std::string& newPassword) {
    std::ifstream inFile("data.json");
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open data.json" << std::endl;
        return false;
    }
    json data;
    inFile >> data;
    inFile.close();
    bool found = false;
    for (auto& entry : data) {
        if (entry.contains(username)) {
            entry[username] = newPassword;
            found = true;
            break;
        }
    }
    if (!found) {
        std::cerr << "Error: Username not found" << std::endl;
        return false;
    }
    std::ofstream outFile("data.json");
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not write to data.json" << std::endl;
        return false;
    }
    outFile << data.dump(4);
    outFile.close();
    return true;
}
int main()
{
    cout << "1) Do you want to generate a new password:" << endl;
    cout << "2) Do you want to search password in data logs by username: " << endl;
    cout << "3) Do you want to add your own password in database " << endl;
    cout << "4) Do you want to update any password in database" << endl;

    int n;
    cin >> n;

    switch (n)
    {
    case 1:
    {
        cout << "what length of password you want? ";
        int length;
        cin >> length;
        string pass = pass_gen(length);
        if(search(pass)==false){
            cout<<pass<<"already found in database"<<endl;
            cout<<"Do you want to continue with this password..."<<endl;
            string choice;
            cin>>choice;
            if(choice=="yes"){
                cout<<"entre the username for password: "<<endl;
                string username;
                cin>>username;
                map <string,string> temp;
                temp[username]=pass;
                appendToDatabase(temp);
            }
            else{
                cout<<"regenerating again..."<<endl;

                while(search(pass)==false){
                   string pass=pass_gen();
                }
                map <string,string> temp;
                string username;
                cout<<"Entre the username: "<<endl;
                cin>>username;
                temp[username]=pass;
                appendToDatabase(temp);
            }
        }
        else if(search(pass)==true){
            cout<<"The password generated by system is: "<<pass<<endl;
            cout<<"entre the username: "<<endl;
            string username;
            cin>>username;
            map <string,string> temp;
            temp[username]=pass;
            appendToDatabase(temp);
        }
        break;
    }
    case 2:
    {
        cout<<"Entre the username to serach the password: ";
        string user;
        cin>>user;
        cout<<getPassword(user)<<endl;
        break;
    }
    case 3:
    {
        string username;
        string password;
        map <string,string> temp;
        string cho;
        do{
            cout<<"entre the username: ";
            cin>>username;
            cout<<endl;
            cout<<"entre the password: ";
            cin>>password;
            cout<<endl;
            temp[username]=password;
            cout<<"Do you want to add more than one...";
            cin>>cho;

        }while(cho!="no");
        appendToDatabase(temp);
        break;
    }
    case 4:
    {
        string username;
        cout<<"entre the username: "<<endl;
        cin>>username;
        cout<<"entre the previous password: "<<endl;
        string prev;
        cin>>prev;
        string pas=getPassword(username);
        if(prev!=pas){
            cout<<"password did not match try again later..."<<endl;
            return 0;
        }
        else{
            cout<<"Entre the new_password: "<<endl;
            string new_pass;
            cin>>new_pass;
            updatePassword(username,new_pass);
            cout<<"password updated succesfully..."<<endl;
        }
        break;
    }
    default:
        break;
    }
    return 0;
}