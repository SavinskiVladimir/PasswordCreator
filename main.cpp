#include <iomanip>
#include<iostream>
#include<vector>
#include<string>
#include<ctime>
#include <limits>
#include<fstream>

using namespace std;

class InvalidInputException : public std::runtime_error {
public:
    InvalidInputException(const std::string& message)
        : std::runtime_error(message) {}
};

class PasswordFormer
{
    ifstream Fin;
    ofstream Fout;
    string neccesarry_sumbols = "^:'_@!*%$#=+-";
    string letters = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
    bool isNew(string pass)
    {
        Fin.open("used.txt");
        if (Fin.tellg() == 0) return true;
        int sum = 0;
        int score= 0;
        int count = 0;
        string temp;
        while (!Fin.eof())
        {
            Fin>>temp;
            temp = decrypt(temp);
            for (int j = 0; j < min(temp.size(), pass.size()); j++)
            {
                score += temp[j] == pass[j] ? 1 : 0;
            }
            sum += score;
            count++;
            score = 0;
        }
        Fin.close();
        if ((double)sum / (double)count >= 7.0) return false;
        return true;
    }
    string encrypt(string result)
    {
        char code = rand() % 265;
        for (int i = 0; i < result.size(); i++) result[i] = result[i] ^ code;
        result = code + result;
        return result;
    }
    string decrypt(string result)
    {
        char code = result[0];
        result = result.substr(1);
        for (int i = 0; i < result.size(); i++) result[i] = result[i] ^ code;
        return result;
    }
    void add(string& result)
    {
        Fout.open("used.txt", ios::app);
        Fout<<encrypt(result)<<"\n";
        Fout.close();
    }
    string formPassword()
    {
        int len = rand() % 10 + 10;
        string result(len, '\0');
        int count_nec = rand() % 5 + 1;
        for (int i = 0; i < count_nec; i++)
        {
            bool flag = true;
            while (flag)
            {
                int temp = rand() % len;
                if (result[temp] == '\0')
                {
                    result[temp] = neccesarry_sumbols[rand() % neccesarry_sumbols.size()];
                    flag = false;
                }
            }
        }
        for (int i = 0; i < len; i++)
        {
            if (result[i] == '\0') result[i] = letters[rand() % letters.size()];
        }
        if (!isNew(result)) return formPassword();
        add(result);
        return result;
    }
public:
    string makePassword(ifstream& Fin, ofstream& Fout)
    {
        this->Fin = move(Fin);
        this->Fout = move(Fout);
        return formPassword();
    }
};

int main()
{
    srand((int)time(0));
    ifstream Fin;
    ofstream Fout;
    PasswordFormer pf;
    int key;
    cout<<"1 - Create password\n2 - End session\n";
    bool flag = true;
    while (flag)
    {
        while (true)
        {
            cout<<"Enter command: ";
            try
            {
                cin>>key;
                if (cin.fail()) {
                    throw InvalidInputException("Incorrect input. Try again.");
                }
                break;
            } catch (const InvalidInputException& e)
            {
                cout<<e.what()<<"\n";
                cin.clear();
                cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        switch (key)
        {
        case 1: cout<<pf.makePassword(Fin, Fout)<<"\n"; break;
        case 2: flag = false; break;
        default: cout<<"Unsupported command\n"; break;
        }
    }
    return 0;
}
