#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include "../MyLibrary.h"

using namespace std;
using namespace MyLib;

const string FileName = "Clients49";
const string Users = "Users";
short LoginCounter = 3;

struct stClient {
  string AccountNumber;
  string PinCode;
  string ClientName;
  string Phone;
  double AccountBalance;
  bool MarkForDelete = false;
  bool MarkForUpdate = false;
};

struct stUser {
  string UserName;
  string Password;
  int Permissions;
  bool MarkForDelete = false;
  bool MarkForUpdate = false;
};

stUser LoggedInUser;

enum enChoice
{
  enShowClientList = 1,
  enAddNewClient = 2,
  enDeleteClient = 3,
  enUpdateClientInfo = 4,
  enFindClient = 5,
  enTransactions = 6,
  enManageUsers = 7,
  enLogout = 8
};

enum enTransactionType
{
  enDeposit = 1,
  enWithdraw = 2,
  enShowAllBalances = 3,
  enBackToMainMenu = 4
};

enum enManageUserOptions
{
  enListUsers = 1,
  enAddNewUser = 2,
  enDeleteUser = 3,
  enUpdateUser = 4,
  enFindUser = 5,
  enMainMenu = 6
};

enum enPermissions
{
  enListClientsP = 1,
  enAddNewClientP = 2,
  enDeleteClientP = 4,
  enUpdateClientInfoP = 8,
  enFindClientP = 16,
  enTransactionsP = 32,
  enManageUsersP = 64
};

void ShowMainMenu();
void Transactions();
void ShowManageUsersMenu();
void Login();

void ReturnToMainScreen(){
  cout << "Press any button to return to main menu...";
  system("pause>0");
  system("cls");
  ShowMainMenu();
};

bool IsUserPermittedToUseOption(enPermissions Operation, stUser User){
  if (User.Permissions == -1)
    return true;

  if ((Operation & User.Permissions) == Operation)
    return true;
  else
    return false;
};

void ShowAccessDeniedScreen(){
  cout << "==================================\n";
  cout << "Looks like you don't have access this operation, please contact your administrator.\n";
  cout << "==================================\n";
  ReturnToMainScreen();
};

void ReturnToTransactionScreen(){
  cout << "Press any button to return to transaction menu...";
  system("pause>0");
  system("cls");
  Transactions();
};

void ReturnToManageUsersMenu(){
  cout << "Press any button to return to manage users menu...";
  system("pause>0");
  system("cls");
  ShowManageUsersMenu();
};

stClient ReadClientData() {

  stClient Client;
  cout << "=========================" << endl;
  cout << "Enter Account Number: " << endl;
  getline(cin, Client.AccountNumber);
  cout << "Enter Pin Code: " << endl;
  getline(cin, Client.PinCode);
  cout << "Enter Your Name: " << endl;
  getline(cin, Client.ClientName);
  cout << "Enter Your Phone Number: " << endl;
  getline(cin, Client.Phone);
  cout << "Enter Your Balance: " << endl;
  cin >> Client.AccountBalance;
  cout << "=========================" << endl;

  return Client;
};

int ReadUserPermissions(){
  int permissions = 0;
  char decision = 'n';

  cout << "Do you want to give user full permission?" << endl;
  cin >> decision;

  if (decision == 'y') {
    permissions = -1;
  } else {
    cout << "Permission to clients list?" << endl;
    cin >> decision;
    if (decision == 'y' || decision == 'Y')
      permissions += 1;


    cout << "Permission to add new client?" << endl;
    cin >> decision;
    if (decision == 'y' || decision == 'Y')
      permissions += 2;


    cout << "Permission delete clients?" << endl;
    cin >> decision;
    if (decision == 'y' || decision == 'Y')
      permissions += 4;


    cout << "Permission update clients?" << endl;
    cin >> decision;
    if (decision == 'y' || decision == 'Y')
      permissions += 8;


    cout << "Permission to find clients?" << endl;
    cin >> decision;
    if (decision == 'y' || decision == 'Y')
      permissions += 16;


    cout << "Permission to transactions?" << endl;
    cin >> decision;
    if (decision == 'y' || decision == 'Y')
      permissions += 32;


    cout << "Permission to manage users?" << endl;
    cin >> decision;
    if (decision == 'y' || decision == 'Y')
      permissions += 64;
  }

  return permissions;
};

stUser ReadUserData() {

  stUser User;
  cout << "=========================" << endl;
  cout << "Enter Username: " << endl;
  getline(cin, User.UserName);
  cout << "Enter Password: " << endl;
  getline(cin, User.Password);
  cout << "Enter Permissions: " << endl;
  User.Permissions = ReadUserPermissions();
  cout << "=========================" << endl;
  cout << "User Added Successfully\n";

  return User;
};

stClient UpdateClientData() {

  stClient Client;
  cout << "=========================" << endl;
  cout << "Enter Account Number: " << endl;
  cin.ignore(INT_MAX, '\n');
  getline(cin, Client.AccountNumber);
  cout << "Enter Pin Code: " << endl;
  getline(cin, Client.PinCode);
  cout << "Enter Your Name: " << endl;
  getline(cin, Client.ClientName);
  cout << "Enter Your Phone Number: " << endl;
  getline(cin, Client.Phone);
  cout << "Enter Your Balance: " << endl;
  cin >> Client.AccountBalance;
  cout << "=========================" << endl;

  return Client;
};

stUser UpdateUsersData() {

  stUser User;
  cout << "=========================" << endl;
  cout << "Enter Username: " << endl;
  cin.ignore(INT_MAX, '\n');
  getline(cin, User.UserName);
  cout << "Enter Password: " << endl;
  getline(cin, User.Password);
  cout << "Enter Permissions: " << endl;
  User.Permissions = ReadUserPermissions();
  cout << "=========================" << endl;

  return User;
};

stClient BreakingClientRecord(string ClientRecord) {
  stClient Client;
  vector < string > tempVector = SplitStringToVector(ClientRecord, "#//#");

  Client.AccountNumber = tempVector[0];
  Client.PinCode = tempVector[1];
  Client.ClientName = tempVector[2];
  Client.Phone = tempVector[3];
  Client.AccountBalance = stod(tempVector[4]);

  return Client;
};

stUser BreakingUserRecord(string UserRecord) {
  stUser User;
  vector < string > tempVector = SplitStringToVector(UserRecord, "#//#");

  User.UserName = tempVector[0];
  User.Password = tempVector[1];
  User.Permissions = stoi(tempVector[2]);

  return User;
};

vector <stClient> RetrieveClientsData(string FileName) {
  vector<stClient> vClients;
  fstream MyFile;
  string Line;
  MyFile.open(FileName, ios::in);

  while (getline(MyFile, Line)) {
    vClients.push_back(BreakingClientRecord(Line));
  }
  return vClients;
};

vector <stUser> RetrieveUsersData(string FileName) {
  vector<stUser> vUsers;
  fstream MyFile;
  string Line;
  MyFile.open(FileName, ios::in);

  while (getline(MyFile, Line)) {
    vUsers.push_back(BreakingUserRecord(Line));
  }
  return vUsers;
};

vector<stClient> vClients = RetrieveClientsData(FileName);
vector<stUser> vUsers = RetrieveUsersData(Users);

string TurnClientDataToRecordLine(stClient Client, string delim = "#//#") {

  string temp;

  temp += Client.AccountNumber + delim;
  temp += Client.PinCode + delim;
  temp += Client.ClientName + delim;
  temp += Client.Phone + delim;
  temp += to_string(Client.AccountBalance);

  return temp;
};

string TurnUserDataToRecordLine(stUser User, string delim = "#//#") {

  string temp;

  temp += User.UserName + delim;
  temp += User.Password + delim;
  temp += to_string(User.Permissions);

  return temp;
};

void PrintClientData(string fileName) {

  if (!IsUserPermittedToUseOption(enPermissions::enListClientsP, LoggedInUser))
    ShowAccessDeniedScreen();

  fstream MyFile;
  string RecordLine;
  vector < string > tVector;
  vector<stClient> Clients;

  MyFile.open(fileName, ios::in);

  while (getline(MyFile, RecordLine)) {
    Clients.push_back(BreakingClientRecord(RecordLine));
  }

  cout << "\n\t\t\t\tClient(s) List " << Clients.size() << " Client(s)\n";
  cout << "------------------------------------------------------------------------------------------------" << endl;
  cout << "| " << left << setw(20) << "Account Number "
  << " | " << left << setw(20) << "Name "
  << " | " << left << setw(10) << "Pin Code "
  << " | " << left << setw(10) << "Phone "
  << " | " << left << setw(20) << "Balance " << " |" << endl;
  cout << "------------------------------------------------------------------------------------------------" << endl;

  if (MyFile.is_open()) {
    for (stClient &client : Clients) {
      cout << "| " << left << setw(20) << client.AccountNumber 
      << " | " <<left << setw(20) << client.ClientName
      << " | " << left << setw(10) << client.PinCode
      << " | " << left << setw(10) << client.Phone
      << " | " << left << setw(20) << client.AccountBalance << " |" << endl;
    }
    cout << "------------------------------------------------------------------------------------------------" << endl;
    MyFile.close();
  }
};

void PrintAllBalances(string fileName) {
  fstream MyFile;
  string RecordLine;
  vector < string > tVector;
  vector<stClient> Clients;

  MyFile.open(fileName, ios::in);

  while (getline(MyFile, RecordLine)) {
    Clients.push_back(BreakingClientRecord(RecordLine));
  }

  cout << "\n\t\t\t\tClient(s) List " << Clients.size() << " Client(s)\n";
  cout << "----------------------------------------------------------------------" << endl;
  cout << "| " << left << setw(20) << "Account Number "
  << " | " << left << setw(20) << "Name "
  << " | " << left << setw(20) << "Balance " << " |" << endl;
  cout << "----------------------------------------------------------------------" << endl;

  if (MyFile.is_open()) {
    for (stClient &client : Clients) {
      cout << "| " << left << setw(20) << client.AccountNumber 
      << " | " <<left << setw(20) << client.ClientName
      << " | " << left << setw(20) << client.AccountBalance << " |" << endl;
    }
    cout << "----------------------------------------------------------------------" << endl;
    MyFile.close();
  }
};

bool IsClientInClients(stClient &client,string AccountNumber) {
  for (stClient& c : vClients) {
    if (c.AccountNumber == AccountNumber){
      client = c;
      return true;
    }
  }
  return false;
};

bool IsUserInUsers(stUser &user,string username) {
  for (stUser& u : vUsers) {
    if (u.UserName == username){
      user = u;
      return true;
    }
  }
  return false;
};

void AddNewClient(){

  stClient Client;
  char decision;
  fstream MyFile;
  MyFile.open(FileName, ios::out | ios::in | ios::app);
  string NewClient = "";

  do {
    cout << "Do You Want To Add Client ?" << endl;
    cin >> decision;
    cin.ignore(INT_MAX, '\n');
    if (decision == 'y' or decision == 'Y') {
      if (MyFile.is_open()) {
        Client = ReadClientData();
        if (!(IsClientInClients(Client, Client.AccountNumber))) {
          NewClient = TurnClientDataToRecordLine(Client);
          MyFile << NewClient << endl;
        } else {
        cout << "Client Record Is Already Saved\n\n";
      }
      }
    }
  } while (decision == 'y' or decision == 'Y');
  MyFile.close();
};

bool MarkClientForDeleteByAccountNumber( vector<stClient>& vClients ,string AccountNumber) {
  for (stClient &c : vClients) {
    if (c.AccountNumber == AccountNumber) {
      c.MarkForDelete = true;
      return true;
    }
  }
  return false;
}

bool MarkUserForDeleteByUsername( vector<stUser>& vUsers ,string Username) {
  for (stUser &u : vUsers) {
    if (u.UserName == Username) {
      u.MarkForDelete = true;
      return true;
    }
  }
  return false;
}

void PrintClientInfo(stClient client){
  cout << "==============================\n";
  cout << "Name          : " << client.ClientName << endl;
  cout << "Account Number: " << client.AccountNumber << endl;
  cout << "Pin Code      : " << client.PinCode << endl;
  cout << "Phone         : " << client.Phone << endl;
  cout << "Balance       : " << client.AccountBalance << endl;
  cout << "==============================\n";
};

void PrintUsersData(stUser User){
  cout << "==============================\n";
  cout << "Username     : " << User.UserName << endl;
  cout << "Password     : " << User.Password << endl;
  cout << "Permissions  : " << User.Permissions << endl;
  cout << "==============================\n";
};

vector<stClient> AddNewDataToFile(){
  fstream MyFile;
  MyFile.open("Clients49", ios::out);
  string DataLine;
  if (MyFile.is_open()) {
    for (stClient &c : vClients) {
      if (!(c.MarkForDelete)){
        DataLine = TurnClientDataToRecordLine(c);
        MyFile << DataLine << endl;
      }
    }
  MyFile.close();
  }
  vClients = RetrieveClientsData(FileName);
  return vClients;
};

vector<stUser> AddNewUsersToFile(){
  fstream MyFile;
  MyFile.open(Users, ios::out);
  string DataLine;
  if (MyFile.is_open()) {
    for (stUser &u : vUsers) {
      if (!(u.MarkForDelete)){
        DataLine = TurnUserDataToRecordLine(u);
        MyFile << DataLine << endl;
      }
    }
  MyFile.close();
  }
  vUsers = RetrieveUsersData(Users);
  return vUsers;
};

bool DeleteClientByAccountNumber(){
  stClient Client;
  char Decision = 'n';
  string AccountNumber = ReadString("Enter Account Number");
  vClients = RetrieveClientsData(FileName);
  if (IsClientInClients(Client, AccountNumber)) {
    PrintClientInfo(Client);

    cout << "Do you confirm this deletion?\n";
    cin >> Decision;
    if (Decision == 'y' || Decision == 'Y') {
      MarkClientForDeleteByAccountNumber(vClients, AccountNumber);
      AddNewDataToFile();
      vClients = RetrieveClientsData(FileName);
      cout << "User deleted successfully.";
      return true;
    }
  }else {
    cout << "\nUser was not found!\n";
  }
    return false;

};

bool MarkClientForUpdateByAccountNumber(string AccountNumber) {
  for (stClient &c : vClients) {
    if (c.AccountNumber == AccountNumber) {
      c.MarkForUpdate = true;
      return true;
    }
  }
  return false;
}

bool MarkUserForUpdateByUsername(string Username) {
  for (stUser &u : vUsers) {
    if (u.UserName == Username) {
      u.MarkForUpdate = true;
      return true;
    }
  }
  return false;
}

void AddNewDataToFileAfterUpdating(string AccountNumber){
  fstream MyFile;
  MyFile.open(FileName, ios::out);
  if (MyFile.is_open()) {
    for (stClient &c : vClients) {
      if (c.MarkForUpdate) {
        c = UpdateClientData();
      } 
      MyFile << TurnClientDataToRecordLine(c) << endl;
    }
  }
  MyFile.close();
};

void AddNewUsersToFileAfterUpdating(string Username){
  fstream MyFile;
  MyFile.open(Users, ios::out);
  if (MyFile.is_open()) {
    for (stUser &u : vUsers) {
      if (u.MarkForUpdate) {
        u = UpdateUsersData();
      } 
      MyFile << TurnUserDataToRecordLine(u) << endl;
    }
  }
  MyFile.close();
};

bool UpdateClientByAccountNumber(){
  stClient Client;
  char Decision = 'n';
  string AccountNumber = ReadString("Enter Account Number: ");
  vClients = RetrieveClientsData(FileName);
  if (IsClientInClients(Client, AccountNumber)) {
    PrintClientInfo(Client);

    cout << "Do you confirm updating on this client?\n";
    cin >> Decision;
    if (Decision == 'y' || Decision == 'Y') {
      MarkClientForUpdateByAccountNumber(AccountNumber);
      AddNewDataToFileAfterUpdating(AccountNumber);
      vClients = RetrieveClientsData(FileName);
      cout << "Client updated successfully.\n";
      return true;
    }
  } else {
    cout << "\nClient was not found!\n";
  }
    return false;

};

void FindClient(){
  stClient Client;
  string AccountNumber = ReadString("Enter Account Number: ");
  vClients = RetrieveClientsData(FileName);
  if (IsClientInClients(Client, AccountNumber)) {
    PrintClientInfo(Client);
  } else {
    cout << "Client Is Not Found!" << endl;
  }
};

void FindUser(){
  stUser User;
  string Username = ReadString("Enter Username: ");
  vUsers = RetrieveUsersData(Users);
  if (IsUserInUsers(User, Username)) {
    PrintUsersData(User);
  } else {
    cout << "User Is Not Found!" << endl;
  }
};

void DepositAnAmount(){
  stClient Client;
  vClients = RetrieveClientsData(FileName);
  
  system("cls");

  cout << "\n==============================\n";
  cout << "\t   Deposit\n";
  cout << "==============================\n";

  string AccountNumber;
  AccountNumber = ReadString("Enter Account Number: ");

  IsClientInClients(Client, AccountNumber);
  PrintClientInfo(Client);

  double DepositAmount;
  DepositAmount = ReadPositiveDouble("Enter Deposit Amount: ");

  for (stClient &c : vClients) {
    if (c.AccountNumber == AccountNumber) {
      c.AccountBalance = c.AccountBalance + DepositAmount;
      AddNewDataToFileAfterUpdating(AccountNumber);
    }
  }
  vClients = RetrieveClientsData(FileName);
};

void WithdrawAnAmount(){
  stClient Client;
  vClients = RetrieveClientsData(FileName);
  
  system("cls");

  cout << "\n==============================\n";
  cout << "\t   Withdraw\n";
  cout << "==============================\n";

  string AccountNumber;
  AccountNumber = ReadString("Enter Account Number: ");

  IsClientInClients(Client, AccountNumber);
  PrintClientInfo(Client);

  double DepositAmount;
  DepositAmount = ReadPositiveDouble("Enter Withdraw Amount: ");

  for (stClient &c : vClients) {
    if (c.AccountNumber == AccountNumber) {
      c.AccountBalance = c.AccountBalance + DepositAmount * -1;
      AddNewDataToFileAfterUpdating(AccountNumber);
    }
  }
  vClients = RetrieveClientsData(FileName);
};

void ShowTransactionsMenu(){
  cout << "\n========================================\n";
  cout << "\tTransactions Menu\n";
  cout << "========================================\n\n";
  cout << "\t[1] Deposit.\n";
  cout << "\t[2] Withdraw.\n";
  cout << "\t[3] Total Balances.\n";
  cout << "\t[4] Main Menu.\n";
  cout << "\n========================================\n";
};

enTransactionType GetTransactionType(){
  short TransactionType;
  cout << "Enter Transaction Type: \n";
  cin >> TransactionType;
  return enTransactionType(TransactionType);
};

void Transactions(){
  vClients = RetrieveClientsData(FileName);

  enTransactionType Transaction;
  ShowTransactionsMenu();
  Transaction = GetTransactionType();

  switch(Transaction) {
    case enDeposit:
      DepositAnAmount();
      ReturnToTransactionScreen();
      break;

    case enWithdraw:
      WithdrawAnAmount();
      ReturnToTransactionScreen();
      break;
    
    case enShowAllBalances:
      PrintAllBalances(FileName);
      ReturnToTransactionScreen();
      break;

    case enBackToMainMenu:
      break;
  }

};

// User functions

void ShowUsersList(){
  fstream MyFile;
  string UserLine;
  vector < string > tVector;
  vector <stUser> vUsers;

  MyFile.open(Users, ios::in);

  while (getline(MyFile, UserLine)) {
    vUsers.push_back(BreakingUserRecord(UserLine));
  }

  cout << "\n\t\t\tUser(s) List " << vUsers.size() << " User(s)\n";
  cout << "-----------------------------------------------------------------" << endl;
  cout << "| " << left << setw(20) << "User Name "
  << " | " << left << setw(20) << "Password "
  << " | " << left << setw(15) << "Permissions " << " |" << endl;
  cout << "-----------------------------------------------------------------" << endl;

  if (MyFile.is_open()) {
    for (stUser &user : vUsers) {
      cout << "| " << left << setw(20) << user.UserName
      << " | " << left << setw(20) << user.Password
      << " | " << left << setw(15) << user.Permissions << " |" << endl;
    }
    cout << "-----------------------------------------------------------------" << endl;
    MyFile.close();
  }
};

void AddNewUser(){
  stUser User;
  char decision;
  fstream MyFile;
  MyFile.open(Users, ios::out | ios::in | ios::app);
  string NewUser = "";

  do {
    cout << "Do You Want To Add New User ?" << endl;
    cin >> decision;
    cin.ignore(INT_MAX, '\n');
    if (decision == 'y' or decision == 'Y') {
      if (MyFile.is_open()) {
        User = ReadUserData();
        if (!(IsUserInUsers(User, User.UserName))) {
          NewUser = TurnUserDataToRecordLine(User);
          MyFile << NewUser << endl;
        } else {
        cout << "User Is Already Saved\n\n";
      }
      }
    }
  } while (decision == 'y' or decision == 'Y');
  MyFile.close();
};

bool DeleteUserByUsername() {
  stUser User;
  char Decision = 'n';
  string Username = ReadString("Enter Username: ");
  vUsers = RetrieveUsersData(Users);
  if (IsUserInUsers(User, Username)) {
    PrintUsersData(User);

    cout << "Do you confirm this deletion?\n";
    cin >> Decision;
    if (Decision == 'y' || Decision == 'Y') {
      MarkUserForDeleteByUsername(vUsers, Username);
      AddNewUsersToFile();
      vUsers = RetrieveUsersData(Users);
      cout << "User deleted successfully.";
      return true;
    }
  }else {
    cout << "\nUser was not found!\n";
  }
    return false;

};

bool UpdateUserByUsername(){
  stUser User;
  char Decision = 'n';
  string Username = ReadString("Enter Username: ");
  vUsers = RetrieveUsersData(Users);
  if (IsUserInUsers(User, Username)) {
    PrintUsersData(User);

    cout << "Do you confirm updating on this User?\n";
    cin >> Decision;
    if (Decision == 'y' || Decision == 'Y') {
      MarkUserForUpdateByUsername(Username);
      AddNewUsersToFileAfterUpdating(Username);
      vUsers = RetrieveUsersData(Users);
      cout << "User updated successfully.\n";
      return true;
    }
  } else {
    cout << "\nUser was not found!\n";
  }
    return false;

};

void HandleManageUsersChoice(){
  short option;
  cout << "Enter operation number: \n";
  cin >> option;

  switch(enManageUserOptions(option)) {
  
    case enManageUserOptions::enListUsers:
      system("cls"); 
      ShowUsersList();
      ReturnToManageUsersMenu();
      break;

    case enManageUserOptions::enAddNewUser:
      system("cls");
      AddNewUser();
      ReturnToManageUsersMenu();
      break;

    case enManageUserOptions::enDeleteUser:
      system("cls");
      DeleteUserByUsername();
      ReturnToManageUsersMenu();
      break;

    case enManageUserOptions::enUpdateUser:
      system("cls");
      UpdateUserByUsername();
      ReturnToManageUsersMenu();
      break;

    case enManageUserOptions::enFindUser:
      system("cls");
      FindUser();
      ReturnToManageUsersMenu();
      break;

    case enManageUserOptions::enMainMenu:
      ReturnToMainScreen();
      break;
  };
};

void ShowManageUsersMenu(){ 
  cout << "\n========================================\n";
  cout << "\tManage Users\n";
  cout << "========================================\n\n";
  cout << "\t[1] List Users.\n";
  cout << "\t[2] Add New User.\n";
  cout << "\t[3] Delete User.\n";
  cout << "\t[4] Update User.\n";
  cout << "\t[5] Find User.\n";
  cout << "\t[6] Main Menu.\n";
  cout << "\n========================================\n";
  HandleManageUsersChoice();
};

void HandleUserChoice(){
  short Choice;
  cout << "Enter operation number: ";
  cin >> Choice;

  switch(enChoice(Choice)) {
    case enShowClientList:
      system("cls");
        PrintClientData(FileName);
      ReturnToMainScreen();
      break;

    case enAddNewClient:
      system("cls");
        AddNewClient();
      ReturnToMainScreen();
      break;

    case enDeleteClient:
      system("cls");
        DeleteClientByAccountNumber();
      ReturnToMainScreen();
      break;

    case enUpdateClientInfo:
      system("cls");
        UpdateClientByAccountNumber();
      ReturnToMainScreen();
      break;

    case enFindClient:
      system("cls");
        FindClient();
      ReturnToMainScreen();
      break;

    case enTransactions:
      system("cls");
        Transactions();
      ReturnToMainScreen();
      break;

    case enManageUsers:
      system("cls");
        ShowManageUsersMenu();
      ReturnToMainScreen();
      break;

    case enLogout:
      system("pause<0");
      system("cls");
      Login();
      break;
  };
};

void ShowMainMenu(){
  system("cls");
  cout << "\n========================================\n";
  cout << "\t\tMain Menu\n";
  cout << "========================================\n\n";
  cout << "\t[1] Show Client List.\n";
  cout << "\t[2] Add New Client.\n";
  cout << "\t[3] Delete Client.\n";
  cout << "\t[4] Update Client Info.\n";
  cout << "\t[5] Find Client.\n";
  cout << "\t[6] Transactions.\n";
  cout << "\t[7] Manage Users.\n";
  cout << "\t[8] Logout.\n";
  cout << "\n========================================\n";
  HandleUserChoice();
};

void Login(){
  stUser User;
  cout << "\n========================================\n";
  cout << "\t Login Screen\n";
  cout << "========================================\n\n";
  string Username = ReadString("Enter your Username: ");
  string Password;
  short c = 3;

  if(IsUserInUsers(User, Username)) {
      vUsers = RetrieveUsersData(Users);
      LoggedInUser = User;

      while (c > 0)
      {
        Password = ReadString("Enter Password: ");
        if (Password == User.Password)
        {
      ShowMainMenu();
        } else {
        cout << "Wrong Password!" << ", you have " << c - 1 << " tries" <<endl;
        c--;

        if (c == 0)
        cout << "You have exceed the number of login tries, please contact the nearest branch\n";
      }
    };

    } else {
      system("cls");
      cout << "User not found please re-check entered username" << endl;
      Login();
    };
};

int main()
{
  
  Login();

  return 0;
};