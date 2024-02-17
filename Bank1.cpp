#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include "../MyLibrary.h"

using namespace std;
using namespace MyLib;

const string FileName = "Clients49";

struct stClient {
  string AccountNumber;
  string PinCode;
  string ClientName;
  string Phone;
  double AccountBalance;
  bool MarkForDelete = false;
  bool MarkForUpdate = false;
};

enum enChoice
{
  enShowClientList = 1,
  enAddNewClient = 2,
  enDeleteClient = 3,
  enUpdateClientInfo = 4,
  enFindClient = 5,
  enExit = 6
};

void ShowMainMenu();

void ReturnToMainScreen(){
  cout << "Press any button to return to main menu...";
  system("pause>0");
  system("cls");
  ShowMainMenu();
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

  vector<stClient> vClients = RetrieveClientsData(FileName);

string TurnClientDataToRecordLine(stClient Client, string delim = "#//#") {

  string temp;

  temp += Client.AccountNumber + delim;
  temp += Client.PinCode + delim;
  temp += Client.ClientName + delim;
  temp += Client.Phone + delim;
  temp += to_string(Client.AccountBalance);

  return temp;
};

void PrintClientData(string fileName) {
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

bool IsClientInClients(stClient &client,string AccountNumber) {
  for (stClient& c : vClients) {
    if (c.AccountNumber == AccountNumber){
      client = c;
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

void PrintClientInfo(stClient client){
  cout << "==============================\n";
  cout << "Name          : " << client.ClientName << endl;
  cout << "Account Number: " << client.AccountNumber << endl;
  cout << "Pin Code      : " << client.PinCode << endl;
  cout << "Phone         : " << client.Phone << endl;
  cout << "Balance       : " << client.AccountBalance << endl;
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
      cout << "Client deleted successfully.";
      return true;
    }
  }else {
    cout << "\nClient was not found!\n";
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

    case enExit:
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
  cout << "\t[6] Exit.\n";
  cout << "\n========================================\n";
  HandleUserChoice();
};

int main()
{
  ShowMainMenu();
  return 0;
}
