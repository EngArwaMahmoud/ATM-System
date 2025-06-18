#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;
const string ClientsFile = "Clients.txt";

void ATMMainMenuScreen();
void LoginScreen();
short ReadWithdrawminue();

enum enATMMainMenue
{
	enQuickWithdraw = 1,
	enNormalWithdraw = 2,
	enDeposit = 3,
	enCheckBalance = 4,
	enLogOut = 5
};

struct stData {
    string AccountNumber;
    string PinCode;
    double AccountBalance;
    bool MarkDelete = false;
};
stData CurrentClient;

string ConvertRecordToLine(stData Client, string Seperator = "#//#")
{
    string S = "";

    S += (Client.AccountNumber) + Seperator;
    S += (Client.PinCode) + Seperator;
    S += to_string(Client.AccountBalance);

    return S;
}

vector <stData> SaveAccounts(string File, vector<stData> vClient)
{

    fstream MyFile;

    string DataLine;
    MyFile.open(File, ios::out);

    if (MyFile.is_open())
    {
        for (stData& C : vClient)
        {
            if (C.MarkDelete == false)
            {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }

        MyFile.close();
    }
    return vClient;
}

vector <string> SplitString(string Name, string delim)
{
    vector<string> vString;

    string Word;
    short pos = 0;

    while ((pos = Name.find(delim)) != std::string::npos)
    {
        Word = Name.substr(0, pos);
        if (Word != "")
        {
            vString.push_back(Word);
        }
        Name.erase(0, pos + delim.length());
    }

    if (Name != " ")
    {
        vString.push_back(Name);
    }
    return vString;
}

stData ConvertLineDataToRecord(string Line, string Seperator = "#//#")
{
    stData Data;
    vector<string> vClientData = SplitString(Line, Seperator);


    Data.AccountNumber = vClientData[0];
    Data.PinCode = vClientData[1];
    Data.AccountBalance = stod(vClientData[2]);

    return Data;
}

vector <stData> LoadRecordFromFile(string File)
{
    vector<stData> vClient;
    fstream MyFile;

    MyFile.open(File, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stData Data;

        while (getline(MyFile, Line))
        {
            Data = ConvertLineDataToRecord(Line);
            vClient.push_back(Data);
        }
        MyFile.close();
    }
    return vClient;
}

bool FindClientByAccountNumber(string AccountNumber, string PinCode , stData& Client)
{
    vector <stData> vdata = LoadRecordFromFile(ClientsFile);

    for (stData C : vdata)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

bool DepositBalanceToClientByAccountNumber(double amount, vector <stData>& vClient, string AccountNumber)
{
    int NewBalance = 0;
    char Answer = 'y';

    cout << "\n\nAre you sure you want Perform this Transactino ? Y/n?";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        for (stData& C : vClient)
        {
            if (C.AccountNumber == AccountNumber)
            {
                NewBalance = C.AccountBalance += amount;
                SaveAccounts(ClientsFile, vClient);
                CurrentClient.AccountBalance = NewBalance;
                cout << "Done successfully :), New Balance is " << NewBalance;

                return true;
            }
        }
        return false;
    }
}


void GoBackToATMMainMenue()
{
    cout << "\n\nPress any key to go back to ATM main menue...";
    cin.ignore();
    cin.get();
    system("cls");
    //system("pause>0");
    ATMMainMenuScreen();
}

int ReadWithdrawAmount()
{
    int Amount = 0;

    do {
            cout << "Enter an amount of 5's? ";
            cin >> Amount;

    } while (Amount % 5 != 0);

    return Amount;
}

int ReadDepositAmount()
{
    int Deposit = 0;

    do {
        cout << "\nEnter a positive Deposit Amount? ";
        cin >> Deposit;
    } while (Deposit < 0);

    return Deposit;
}

int GetQuickWithDrawOption(short Choose)
{
    switch (Choose)
    {
    case 1:
        return 20;
    case 2:
        return 50;
    case 3:
        return 100;
    case 4:
        return 200;
    case 5:
        return 400;
    case 6:
        return 600;
    case 7:
        return 800;
    case 8:
        return 1000;
    default:
        return 0;
    }
}

void PerformNormalWithDrawOption()
{
    int withdraw = ReadWithdrawAmount();

    if (CurrentClient.AccountBalance < withdraw)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        GoBackToATMMainMenue();
    }
    vector<stData>vClient = LoadRecordFromFile(ClientsFile);
    DepositBalanceToClientByAccountNumber(withdraw * -1, vClient, CurrentClient.AccountNumber);
}

void PerformQuickWithDrawOption(short Choose )
{
    if (Choose == 9)
    {
        return;
    }

    short withdraw = GetQuickWithDrawOption(Choose);

    if (CurrentClient.AccountBalance < withdraw)
    {
        cout << "\nThe amount exceeds your balance, make another choice.\n";
        GoBackToATMMainMenue();
    }
    vector<stData>vClient = LoadRecordFromFile(ClientsFile);
    DepositBalanceToClientByAccountNumber(withdraw * -1, vClient, CurrentClient.AccountNumber);
}

void PerformDeposit()
{
    int Deposit = ReadDepositAmount();
    vector<stData>vClient = LoadRecordFromFile(ClientsFile);
    DepositBalanceToClientByAccountNumber(Deposit, vClient, CurrentClient.AccountNumber);
}
// Show Screen

void ShowDepositScreen()
{
    int Deposit;
    system("cls");
    cout << "====================================\n";
    cout << "\tDeposit Screen\n";
    cout << "====================================\n";
    
    PerformDeposit();
}

void ShowNormalWithdrawScreen()
{
    system("cls");
    cout << "====================================\n";
    cout << "\tNormal Withdraw Screen\n";
    cout << "====================================\n";

    PerformNormalWithDrawOption();
}

void ShowQuickWithdrawscreen()
{
	system("cls");
	cout << "====================================\n";
	cout << "\tQuick Withdraw Screen\n";
	cout << "====================================\n";
	cout << "\t[1] 20 EGP";
	cout << "\t[2] 50 EGP\n";
	cout << "\t[3] 100 EGP";
	cout << "\t[4] 200 EGP\n";
	cout << "\t[5] 400 EGP";
	cout << "\t[6] 600 EGP\n";
	cout << "\t[7] 800 EGP";
	cout << "\t[8] 1000 EGP\n";
	cout << "\t[9] Exit\n";
    cout << "====================================\n";
	cout << "Your Balance is " << (CurrentClient.AccountBalance) << " EGP\n";

    PerformQuickWithDrawOption(ReadWithdrawminue());
}

void ShowCheckBalanceScreen()
{
	system("cls");
	cout << "====================================\n";
	cout << "\tCheck Balance Screen\n";
	cout << "====================================\n";

	cout << "Your Balance is " << (CurrentClient.AccountBalance) << " EGP\n";
}

short ReadATMMainMenue()
{
    int Choose;
    cout << "Choose What do you want to do? [1 to 5] ? ";
    cin >> Choose;

    return Choose;
}

short ReadWithdrawminue()
{
    short Choose = 0;

    while (Choose < 1 || Choose > 9)
    {
    cout << "Choose What to do from? [1 to 9] ? ";
    cin >> Choose;
    }
    return Choose;
}

void ShowATMOption(enATMMainMenue Choose)
{

    switch ((Choose))
    {
    case enATMMainMenue::enQuickWithdraw:
        ShowQuickWithdrawscreen();
        GoBackToATMMainMenue();
        break;

    case enATMMainMenue::enNormalWithdraw:
        ShowNormalWithdrawScreen();
        GoBackToATMMainMenue();
        break;

    case enATMMainMenue::enDeposit:
        ShowDepositScreen();
        GoBackToATMMainMenue();
        break;

    case enATMMainMenue::enCheckBalance:
        ShowCheckBalanceScreen();
        GoBackToATMMainMenue();
        break;

    case enATMMainMenue::enLogOut:
        system("cls");
        LoginScreen();
        break;
    }
}

void ATMMainMenuScreen()
{
    system("cls");

    cout << "====================================\n";
    cout << "\tATM Main Menu Screen\n";
    cout << "====================================\n";

    cout << "\t[1] Quick Withdraw\n";
    cout << "\t[2] Normal Withdraw\n";
    cout << "\t[3] Deposit\n";
    cout << "\t[4] Check Balance\n";
    cout << "\t[5] Log Out\n";

    cout << "====================================\n";

    ShowATMOption((enATMMainMenue)ReadATMMainMenue());
}

bool LoadUserInfo(string AccountNumber, string PinCode)
{
    if (FindClientByAccountNumber(AccountNumber, PinCode, CurrentClient))
        return true;
    else
        return false;
}

void LoginScreen()
{

    bool LoginFailed = false;
    string accountNumber;
	string PinCode;

    do
    {
        system("cls");

        cout << "====================================\n";
        cout << "\tLogin Screen\n";
        cout << "====================================\n";

        if (LoginFailed)
        {
            cout << "\nInvalid Account Number/password. Please try again.\n";
        }

        cout << "\nEnter Account Number? ";
        cin >> accountNumber;

        cout << "Enter Pin Code? ";
        cin >> PinCode;

        LoginFailed = !LoadUserInfo(accountNumber, PinCode);

    } while (LoginFailed);

	ATMMainMenuScreen();
}

int main()
{
	LoginScreen();
	system("cls");
	return 0;
}