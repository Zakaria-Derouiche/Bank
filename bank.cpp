#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
#include <fstream>
#include <vector>
#include <ctime>
#include <cmath>
using namespace std;

const string File = "Record.txt";
const string UserFile = "Users.txt";


enum enProgramFunctions {enShowClients, enAddClient, enDeleteClient, enUpdateClient, enFindClient, enTransactions, enManageUsers, enLougout};

enum enTransactionFunction{enDeposit, enWithDraw, enTotalBalances, enMainMenu};

enum enManageUserFunction {enShowUsers, enAddUser, enDeleteUser, enUpdateUser, enFindUser, MainMenu};

enum enSystemAccess {enAll = -1, enAccessClient = 1, enAccessAddClient = 2, enAccessDeleteClient = 4, enAccessUpdateClient = 8, enAccessFindClient = 16, enAccessTransactions = 32, enAccessManageUsers = 64};

struct stClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string PhoneNumber;
	double AccountBalance;
	bool MarkToDelete = false;
};

struct stUser
{
    string UserName;
    string Password;
    short Permission = 0;
    bool MarkToDelete = false;
    bool MarkToUpdate = false;
};

stUser CurrentUser;

void ShowHomeScreen();
void ShowTransactionMenu();
void ShowManageUsersMenu();
vector<string> SplitString(string Line, string Separator = "#//#")
{
	vector<string> vData;
	int Pos = 0;
	while ((Pos = Line.find(Separator)) != std::string::npos)
	{
		vData.push_back(Line.substr(0, Pos));
		Line.erase(0, Pos + Separator.length());
	}
	if (Line != "")
		vData.push_back(Line);
	return vData;
}

stClient ConvertLineToRecord(string Line, string Separator = "#//#")
{
	vector<string> vData = SplitString(Line, Separator);
	stClient Client;
	Client.AccountNumber = vData.at(0);
	Client.PinCode = vData.at(1);
	Client.Name = vData.at(2);
	Client.PhoneNumber = vData.at(3);
	Client.AccountBalance = stod(vData.at(4));
	return Client;
}

stUser ConvertLineToUser(string Line, string Separator = "#//#")
{
    vector<string> vData = SplitString(Line, Separator);
	stUser User;
	User.UserName = vData.at(0);
	User.Password = vData.at(1);
	User.Permission = stoi(vData.at(2));
	return User;
}

void PrintLine()
{
	cout << "----------------------------------------------------------------------------------------";
}

void ShowLoginScreen()
{
    cout << "----------------------------------";
    cout << "\n\tLogin Screen\n";
    cout << "----------------------------------";
}

stUser ReadUserData()
{
    stUser User;
    cout << "\nenter UserName? ";
    cin >> User.UserName;
    cout << "\nenter Password? ";
    cin >> User.Password;
    return User;
}


bool FindUserByUserNameAndPassword(stUser &CurrentUser, vector<stUser> &Users )
{
    for (stUser U : Users)
    {
        if (U.UserName == CurrentUser.UserName && U.Password == CurrentUser.Password)
        {
            CurrentUser = U;
            return true;
        }
    }
    return false;
}

void PrintClientsList(vector<stClient> &Clients)
{
	cout << "\n\t\t\t\tClients List (" << Clients.size() << ") Client(s)\n";
	PrintLine();
	cout << endl;
	cout << left << setw(20) << "|Account Number";
	cout << left << setw(15) << "|Pin Code";
	cout << left << setw(30) << "|Client Name";
	cout << left << setw(15) << "|Phone";
	cout << left << setw(20) << "|Balance";
	cout << '\n';
	PrintLine();
	cout << '\n';
	for (stClient &C : Clients)
	{
		cout << '\n';
        cout << '|' << setw(19) << C.AccountNumber;
		cout << '|' << setw(14)<< left << C.PinCode;
        cout << '|' << setw(29)<< left << C.Name;
        cout << '|' << setw(14)<< left << C.PhoneNumber;
		cout << '|' << setw(19) << left << C.AccountBalance ;
	}
	cout << "\n\n";
	PrintLine();
}

void ReturnToMainMenu()
{
	ShowHomeScreen();
}

void ShowClientsList(string FileName)
{
	system("cls");
	vector<stClient> Clients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		while(getline(MyFile, Line))
		{
			Clients.push_back(ConvertLineToRecord(Line));
		}
		MyFile.close();
	}
	PrintClientsList(Clients);
	cout << "\n\nPress Any key to go back to the Main Menu...";
	system("pause>0");
}

bool FindClientByAccountNumber(string AccountNumber)
{
    vector<stClient> Clients;
    fstream MyFile;
    MyFile.open(File, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        while(getline(MyFile, Line))
        {
            Clients.push_back(ConvertLineToRecord(Line));
        }
        MyFile.close();
    }
    for (stClient &C : Clients)
    {
        if (C.AccountNumber == AccountNumber)
            return true;
    }
    return false;
}

stClient ReadClientData(string AccountNumber)
{
    stClient Client;
    Client.AccountNumber = AccountNumber;
    cout << "Please enter Pin Code? ";
    cin >> ws >> Client.PinCode;
    cout << "Please enter Name? ";
    getline(cin >> ws, Client.Name);
    cout << "Please enter Phone Number? ";
    cin >> ws >> Client.PhoneNumber;
    cout << "Please enter Account balance? ";
    cin >> ws >> Client.AccountBalance;
    return Client;
}

string ConvertRecordToLine(stClient Client, string Separator ="#//#")
{
    string Line = "";
    Line += Client.AccountNumber + Separator;
    Line += Client.PinCode + Separator;
    Line += Client.Name + Separator;
    Line += Client.PhoneNumber + Separator;
    Line += to_string(Client.AccountBalance);
    return Line;
}

void AddRecordToFile(string FileName, string Line)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << Line << endl;
        MyFile.close();
    }
}

void AddClient()
{
    system("cls");
	cout << "\n----------------------------------\n";
	cout << "\tAdd Client Screen";
	cout << "\n----------------------------------";
    string AccountNumber = "";
	cout << "\n\t\tAdding a new Client\n";
	cout << "Please enter an Account Number? ";
	cin >> AccountNumber;
    while(FindClientByAccountNumber(AccountNumber))
	{
		cout << "\nClient with Account Number(" << AccountNumber << ") is Already exists";
		cout << "\nenter another Account Number? ";
		cin >> AccountNumber;
	}
	AddRecordToFile(File,ConvertRecordToLine(ReadClientData(AccountNumber)));
	cout << "\nClient Added successfully ";
}

void AddClients()
{
	char AddMore = 'y';
	do {
        AddClient();
        cout << "\ndo you want to Add a new client? y/n? ";
        cin >> AddMore;
	}while (tolower(AddMore) == 'y');
	cout << "\n\nPress Any key to go back to the Main Menu...";
	system("pause>0");
}

bool FindClientByAccountNumber(string AccountNumber, vector<stClient> &Clients, stClient &Client)
{
    fstream MyFile;
    MyFile.open(File, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        while(getline(MyFile, Line))
        {
            Clients.push_back(ConvertLineToRecord(Line));
        }
        MyFile.close();
    }
    for (stClient &C : Clients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

void PrintClientInformation(stClient &Client)
{
    cout << "\nClient Information\n";
    cout << "\nAccount Number: " << Client.AccountNumber;
    cout << "\nPin Code: " << Client.PinCode;
    cout << "\nName: " << Client.Name;
    cout << "\nPhone Number: " << Client.PhoneNumber;
    cout << "\nAccount Balance: " << Client.AccountBalance << endl;
}

vector<stClient> LoadFileToVector(string FileName)
{
    vector<stClient> Clients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line = "";
        while(getline(MyFile, Line))
        {
            Clients.push_back(ConvertLineToRecord(Line));
        }
        MyFile.close();
    }
    return Clients;
}

vector<stUser> LoadFileToVectorUser(string FileName)
{
    vector<stUser> Users;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line = "";
        while(getline(MyFile, Line))
        {
            Users.push_back(ConvertLineToUser(Line));
        }
        MyFile.close();
    }
    return Users;
}

void LoadVectorToFile(vector<stClient> &Clients, string FileName)
{
    fstream MyFile;
    MyFile.open(File, ios::out);
    if (MyFile.is_open())
    {
        for(stClient &C : Clients)
        {
            if (!C.MarkToDelete)
                MyFile << ConvertRecordToLine(C) << endl;
        }
        MyFile.close();
    }
}

void DeleteClient()
{
    system("cls");
    cout << "\n----------------------------------\n";
	cout << "\tDelete Client Screen";
	cout << "\n----------------------------------";
    string AccountNumber;
    vector<stClient> Clients;
    stClient Client;
    cout << "\n\t\tDeleting a client\n";
    cout << "\nPlease enter Account Number? ";
    cin >> AccountNumber;
    if (FindClientByAccountNumber(AccountNumber, Clients, Client))
    {
        char Delete;
        PrintClientInformation(Client);
        cout << "\nDo you ant to delete this client? y/n? ";
        cin >> Delete;
        if (tolower(Delete) == 'y')
        {
            for (stClient &C : Clients)
            {
                if(C.AccountNumber == AccountNumber)
                    C.MarkToDelete = true;
            }
            LoadVectorToFile(Clients, File);
            Clients = LoadFileToVector(File);
            cout << "\nClient deleted successfully";
        }else
        {
            cout << "\nDeletion canceled...";
        }
    }else
    {
        cout << "\nClient with Account Number(" << AccountNumber << ") is not Found.";
    }
    cout << "\n\nPress Any key to go back to the Main Menu...";
	system("pause>0");
}

void UpdateClientInformaton()
{
    system("cls");
    cout << "\n----------------------------------\n";
	cout << "\tUpdate Client Info Screen";
	cout << "\n----------------------------------";
    vector<stClient> Clients;
    stClient Client;
    string AccountNumber = "";
    cout << "\n\t\tUpdating Client Information\n";
    cout << "\nPlease enter the Account Number of Client That to want to update his information? ";
    cin >> AccountNumber;
    if (FindClientByAccountNumber(AccountNumber, Clients, Client))
    {
        char Update;
        PrintClientInformation(Client);
        cout << "\nAre you want to update Information? y/n? ";
        cin >> Update;
        if (tolower(Update) == 'y')
        {
            Client = ReadClientData(AccountNumber);
            for (stClient &C : Clients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = Client;
                }
            }
            LoadVectorToFile(Clients, File);
        }else
        {
            cout << "\nUpdate canceled...";
        }
    }else
    {
        cout << "\nClient with this (" << AccountNumber << ") is not found";
    }
    cout << "\n\nPress Any key to go back to the Main Menu...";
	system("pause>0");
}

void FindClient()
{
    system("cls");
    cout << "\n----------------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n----------------------------------";
    stClient Client;
    vector<stClient> Clients;
    string AccountNumber = "";
    cout << "\n\t\tSearching for a client\n";
    cout << "\nEnter an Account Number? ";
    cin >> AccountNumber;
    if (FindClientByAccountNumber(AccountNumber, Clients, Client))
    {
        PrintClientInformation(Client);
    }else
    {
        cout << "\nClient with this Account Number(" << AccountNumber << ") is not found.";
    }
    cout << "\n\nPress Any key to go back to the Main Menu...";
	system("pause>0");
}

void Deposit(vector<stClient> &Clients, string &AccountNumber, double Amount)
{
    for (stClient &C : Clients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.AccountBalance += Amount;
        }
    }
}

void DepositOperation()
{
    system("cls");
    cout << "\n----------------------------------\n";
	cout << "\tDeposit Screen";
	cout << "\n----------------------------------";
    vector<stClient> Clients;
    stClient Client;
    string AccountNumber = "";
    cout << "\nPlease enter the Account Number of Client? ";
    cin >> AccountNumber;
    if (FindClientByAccountNumber(AccountNumber, Clients, Client))
    {
        double Amount = 0;
        char Confirm = 'y';
        PrintClientInformation(Client);
        cout << "\nPlease enter the deposit amount? ";
        cin >> Amount;
        cout << "Are you sure that you want to perform this transaction? y/n? ";
        cin >> Confirm;
        if (tolower(Confirm) == 'y')
        {
            Deposit(Clients, AccountNumber, Amount);
            LoadVectorToFile(Clients, File);
            cout << "\nTransaction done";
        }else
        {
            cout << "\nTransaction canceled...";
        }
    }else
    {
        cout << "\nClient with this (" << AccountNumber << ") is not found";
    }
    cout << "\nPress any key to return to Transaction Menu.";
    system("pause>0");
}

void WithDrawOperation()
{
    system("cls");
    cout << "\n----------------------------------\n";
	cout << "\tWithDraw Screen";
	cout << "\n----------------------------------";
    vector<stClient> Clients;
    stClient Client;
    string AccountNumber = "";
    cout << "\nPlease enter the Account Number of Client? ";
    cin >> AccountNumber;
    if (FindClientByAccountNumber(AccountNumber, Clients, Client))
    {
        double Amount = 0;
        char Confirm = 'y';
        PrintClientInformation(Client);
        cout << "\nPlease enter the withDraw amount? ";
        cin >> Amount;
        while (Client.AccountBalance < Amount)
        {
            cout << "\nAmount exceeds the Balance, you can WithDraw up to " << Client.AccountBalance;
            cout << "\nPlease enter another amount ";
            cin >> Amount;
        }
        cout << "Are you sure that you want to perform this transaction? y/n? ";
        cin >> Confirm;
        if (tolower(Confirm) == 'y')
        {
            Deposit(Clients, AccountNumber, Amount * -1);
            LoadVectorToFile(Clients, File);
            cout << "\nTransaction done";
        }else
        {
            cout << "\nTransaction canceled...";
        }
    }else
    {
        cout << "\nClient with this (" << AccountNumber << ") is not found";
    }
    cout << "\nPress any key to return to Transaction Menu.";
    system("pause>0");
}

void PrintBalances(vector<stClient> &Clients)
{
    cout << "\n\t\t\t\tBalances List (" << Clients.size() << ") Client(s)\n";
	PrintLine();
	cout << endl;
	cout << left << setw(20) << "|Account Number";
	cout << left << setw(30) << "|Client Name";
	cout << left << setw(20) << "|Balance";
	cout << '\n';
	PrintLine();
	cout << '\n';
	for (stClient &C : Clients)
	{
		cout << '\n';
        cout << '|' << setw(19) << C.AccountNumber;
        cout << '|' << setw(29)<< left << C.Name;
		cout << '|' << setw(19) << left << C.AccountBalance ;
	}
	cout << "\n\n";
	PrintLine();
}

double CalculateTotalBalance(vector<stClient> &Clients)
{
    double TotalBalances = 0;
    for (stClient &C : Clients)
        TotalBalances += C.AccountBalance;
    return TotalBalances;
}

void TotalBalance()
{
    system("cls");
	vector<stClient> Clients;
	fstream MyFile;
	MyFile.open(File, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		while(getline(MyFile, Line))
		{
			Clients.push_back(ConvertLineToRecord(Line));
		}
		MyFile.close();
	}
	PrintBalances(Clients);
	cout << "\n\t\t\tTotale Balances = " <<  CalculateTotalBalance(Clients);
	cout << "\nPress any key to return to Transaction Menu.";
    system("pause>0");
}

void TransactionsFunctions(int Number)
{
   switch ((enTransactionFunction) Number)
	{
	    case enTransactionFunction::enDeposit:
	        DepositOperation();
	        ShowTransactionMenu();
	        break;
        case enTransactionFunction::enWithDraw:
	        WithDrawOperation();
	        ShowTransactionMenu();
	        break;
        case enTransactionFunction::enTotalBalances:
            TotalBalance();
            ShowTransactionMenu();
	        break;
        case enTransactionFunction::enMainMenu:
            ShowHomeScreen();
	        break;
	}
}

void ShowTransactionMenu()
{
    system("cls");
	int Number = 0;
	cout << "\n=============================================================\n";
	cout << "\t\tTransactions Menu Screen";
	cout << "\n=============================================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] WithDraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menu\n";
	cout << "\n=============================================================\n";
	cout << "\nChoose what you want to do? [1 to 4]? ";
	cin >> Number;
	TransactionsFunctions(Number - 1);
}

void Login()
{
    system("cls");
    ShowLoginScreen();
    CurrentUser = ReadUserData();
    vector<stUser> Users = LoadFileToVectorUser(UserFile);
    while (!FindUserByUserNameAndPassword(CurrentUser, Users))
    {
        system("cls");
        ShowLoginScreen();
        cout << "\nInvalid Username/Password!";
        CurrentUser = ReadUserData();
    }
    ShowHomeScreen();
}

void ReturnLoginScreen()
{
    system("cls");
    Login();
}


void PrintUsersList(vector<stUser> &Users)
{
    cout << "\n\t\t\t\tUsers List (" << Users.size() << ") User(s)\n";
	PrintLine();
	cout << endl;
	cout << left << setw(35) << "|UserName";
	cout << left << setw(35) << "|Password";
	cout << left << setw(20) << "|Permission";
	cout << '\n';
	PrintLine();
	cout << '\n';
	for (stUser &U : Users)
	{
		cout << '\n';
        cout << '|' << setw(34) << U.UserName;
		cout << '|' << setw(34)<< left << U.Password;
        cout << '|' << setw(19)<< left << U.Permission;
	}
	cout << "\n\n";
	PrintLine();
}

void ShowListUser(string FileName)
{
    system("cls");
	vector<stUser> Users;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		while(getline(MyFile, Line))
		{
			Users.push_back(ConvertLineToUser(Line));
		}
		MyFile.close();
	}
	PrintUsersList(Users);
	cout << "\nPress any key to return to Manage User Menu.";
    system("pause>0");
}

void AddNewUserScreen()
{
    cout << "----------------------------------";
    cout << "\n\tAdd New User Screen\n";
    cout << "----------------------------------\n";
    cout << "Adding new user:\n";
}

void ReadUserPermmissions(stUser &User)
{
    char FullAccess = 'n';
    User.Permission = 0;
    cout << "\ndo you want to give him full access? y/n? ";
    cin >> FullAccess;
    if (FullAccess == 'y')
    {
        User.Permission = enAll;
        return;
    }
    char Permission = 'y';
    cout << "\nDo you want to give access to:";
    cout << "\nAccess client list? y/n? ";
    cin >> ws >> Permission;
    if (Permission == 'y')
        User.Permission += enAccessClient;
    cout << "\nAdd client? y/n? ";
    cin >> ws >> Permission;
    if (Permission == 'y')
        User.Permission += enAccessAddClient;
    cout << "\nDelete client? y/n? ";
    cin >> ws >> Permission;
    if (Permission == 'y')
        User.Permission += enAccessDeleteClient;
    cout << "\nUpdate client? y/n? ";
    cin >> ws >> Permission;
    if (Permission == 'y')
        User.Permission += enAccessUpdateClient;
    cout << "\nFind client? y/n? ";
    cin >> ws >> Permission;
    if (Permission == 'y')
        User.Permission += enAccessFindClient;
    cout << "\nTransactions? y/n? ";
    cin >> ws >> Permission;
    if (Permission == 'y')
        User.Permission += enAccessTransactions;
    cout << "\nManage users? y/n? ";
    cin >> ws >> Permission;
    if (Permission == 'y')
        User.Permission += enAccessManageUsers;
}


string ConvertUserToLine(stUser User, string Separator = "#//#")
{
    string Line = "";
    Line += User.UserName + Separator;
    Line += User.Password + Separator;
    Line += to_string(User.Permission);
    return Line;
}

bool FindUserByUserName(vector<stUser> &Users, string UserName, stUser &User, bool MarkToDelete = false, bool MarkToUpdate = false)
{
    for (stUser &U :  Users)
    {
        if (U.UserName == UserName)
        {
            if (MarkToDelete)
                U.MarkToDelete = true;
            if (MarkToUpdate)
                U.MarkToUpdate = true;
            User = U;
            return true;
        }
    }
    return false;
}

void AddUser()
{
    system("cls");
    AddNewUserScreen();
    char AddMore = 'y';
    while (AddMore == 'y')
    {
        stUser User;
        vector<stUser> Users = LoadFileToVectorUser(UserFile);
        cout << "\nEnter user name? ";
        cin >> ws >> User.UserName;
        while (FindUserByUserName(Users, User.UserName, User, false, false))
        {
            cout << "\nUser with [" << User.UserName << "] already exist\nEnter user name?";
            cin >> ws >> User.UserName;

        }
        cout << "\Enter Password? ";
        cin >> ws >> User.Password;
        ReadUserPermmissions(User);
        AddRecordToFile(UserFile, ConvertUserToLine(User, "#//#"));
        cout << "\nUser Added successfully, do you want to add more users? y/n? ";
        cin >> ws >> AddMore;
    }
}

void DeleteUserScreen()
{
    cout << "----------------------------------";
    cout << "\n\tDelete User Screen\n";
    cout << "----------------------------------\n";
}

void PrintUserData(stUser User)
{
    cout << "\nThe following are the user informations details\n";
    cout << "------------------------\n";
    cout << "User name: " << User.UserName << endl;
    cout << "Password: " << User.Password << endl;
    cout << "Permission: " << User.Permission;
    cout << "\n------------------------";

}

void LoadVectorUserToFile(vector<stUser> &Users)
{
    fstream MyFile;
    MyFile.open(UserFile, ios::out);
    if (MyFile.is_open())
    {
        for(stUser &U : Users)
        {
            if (!U.MarkToDelete)
                MyFile << ConvertUserToLine(U) << endl;
        }
        MyFile.close();
    }
}


void DeleteUser()
{
    system("cls");
    string UserName = "";
    char ConfirmDeletion = 'n';
    stUser User;
    DeleteUserScreen();
    cout << "Please enter user name? ";
    cin >> ws >> UserName;
    vector<stUser> Users = LoadFileToVectorUser(UserFile);
    while (!FindUserByUserName(Users, UserName, User, true, false))
    {
        cout << "\nUser does not exist..";
        cout << "Please enter user name? ";
        cin >> ws >> UserName;
    }
    PrintUserData(User);
    cout << "\nAre you want to delete this user? y/n? ";
    cin >> ConfirmDeletion;
    if (User.UserName == "Admin" && User.Password == "1234")
        cout << "Operation denied, you can't delete this user.";
    else
    {
        LoadVectorUserToFile(Users);
        cout << "\nUser Deleted successfully.";
    }
    cout << "\nPress any key to return to Manage user Menu.";
    system("pause>0");
}

void UpdateUserScreen()
{
    cout << "----------------------------------";
    cout << "\n\tUpdate User Screen\n";
    cout << "----------------------------------\n";
}


void UpdateUser()
{
    system("cls");
    string UserName = "";
    char ConfirmUpadte = 'n';
    stUser User;
    vector<stUser> Users = LoadFileToVectorUser(UserFile);
    UpdateUserScreen();
    cout << "Please enter Username? ";
    cin >> ws >>UserName;
    if (FindUserByUserName(Users, UserName, User, false, true))
    {
        PrintUserData(User);
        cout << "\nAre you want to update this user? y/n? ";
        cin >> ws >> ConfirmUpadte;
        if (User.UserName == "Admin" && User.Password == "1234")
            cout << "\nyou can't update this user:(";
        else
        {
            cout << "\nEnter Password? ";
            for (stUser &U : Users)
            {
                if (U.MarkToUpdate)
                {
                    cin >> U.Password;
                    ReadUserPermmissions(U);
                }
            }
            LoadVectorUserToFile(Users);
            cout << "\nUser Updated successfully.";
        }
    }else
    {
        cout << "\nSorry this user does not exist.";
    }
    cout << "\nPress any key to return to Manage user Menu.";
    system("pause>0");
}

void FindUserScreen()
{
    cout << "----------------------------------";
    cout << "\n\tFind User Screen\n";
    cout << "----------------------------------\n";
}

void FindUser()
{
    system("cls");
    string UserName = "";
    vector<stUser> Users = LoadFileToVectorUser(UserFile);
    stUser User;
    FindUserScreen();
    cout << "\nEnter User Name? ";
    cin >> ws >> UserName;
    if (FindUserByUserName(Users, UserName, User, false, false))
        PrintUserData(User);
    else
        cout << "\nSorry this user does not exist.";
    cout << "\nPress any key to return to Manage user Menu.";
    system("pause>0");
}

void ManageUserFunctions( short Number)
{
    switch ((enManageUserFunction) Number)
	{
	    case enManageUserFunction::enShowUsers:
	        ShowListUser(UserFile);
	        ShowManageUsersMenu();
	        break;
        case enManageUserFunction::enAddUser:
            AddUser();
            ShowManageUsersMenu();
	        break;
        case enManageUserFunction::enDeleteUser:
            DeleteUser();
            ShowManageUsersMenu();
	        break;
        case enManageUserFunction::enUpdateUser:
            UpdateUser();
            ShowManageUsersMenu();
	        break;
        case enManageUserFunction::enFindUser:
            FindUser();
            ShowManageUsersMenu();
	        break;
        case enManageUserFunction::MainMenu:
            ReturnToMainMenu();
	        break;
	}
}


void ShowManageUsersMenu()
{
    system("cls");
    int Number = 0;
	cout << "\n=============================================================\n";
	cout << "\t\t\tManage Users Menu Screen";
	cout << "\n=============================================================\n";
	cout << "\t[1] List Users.\n";
	cout << "\t[2] Add New User.\n";
	cout << "\t[3] Delete User.\n";
	cout << "\t[4] Update User.\n";
	cout << "\t[5] Find User.\n";
	cout << "\t[6] Main Menu.\n";
	cout << "\n=============================================================\n";
	cout << "\nChoose what you want to do? [1 to 6]? ";
	cin >> Number;
	ManageUserFunctions(Number - 1);
}

void ProgramFunctions(int Number)
{
	switch ((enProgramFunctions) Number)
	{
	    case enProgramFunctions::enShowClients:
	        ShowClientsList(File);
            ReturnToMainMenu();
	        break;
        case enProgramFunctions::enAddClient:
	        AddClients();
            ReturnToMainMenu();
	        break;
        case enProgramFunctions::enDeleteClient:
	        DeleteClient();
	        ReturnToMainMenu();
	        break;
        case enProgramFunctions::enUpdateClient:
	        UpdateClientInformaton();
            ReturnToMainMenu();
	        break;
        case enProgramFunctions::enFindClient:
	        FindClient();
            ReturnToMainMenu();
	        break;
        case enProgramFunctions::enTransactions:
            ShowTransactionMenu();
            break;
        case enProgramFunctions::enManageUsers:
            ShowManageUsersMenu();
            break;
        case enProgramFunctions::enLougout:
	        Login();
	        break;
	}
}

bool CheckUserPermission(short Number)
{
    short Per [7] = {1, 2, 4, 8, 16, 32, 64};
    if (CurrentUser.Permission == enSystemAccess::enAll)
        return true;
    else if (Number == 7)
        return true;
    else if ((Per[Number] & CurrentUser.Permission) == Per[Number])
        return true;
    else
        return false;
}

void ShowHomeScreen()
{
    system("cls");
	int Number = 0;
	cout << "\n=============================================================\n";
	cout << "\t\t\tMain Menu Screen";
	cout << "\n=============================================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client.\n";
	cout << "\t[5] Find Client Info.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Manage Users.\n";
	cout << "\t[8] Logout";
	cout << "\n=============================================================\n";
	cout << "\nChoose what you want to do? [1 to 8]? ";
	cin >> Number;
    if (CheckUserPermission(Number -1))
        ProgramFunctions(Number-1);
    else
    {
        system("cls");
        cout << "\nAccess denied, please contact the admin.";
        cout << "\nPress any key to return to Main Menu.";
        system("pause>0");
        ShowHomeScreen();
    }

}

void RunProgram()
{
	Login();
}


int main(){
	RunProgram();
	system("pause>0");
	return 0;
}
