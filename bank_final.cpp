///group members : Muhammad Ikrash(23L-0684), Ali Imran(23L-0632) 

#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<ctime>
#include<sstream>
#include<Windows.h>

///COLORS
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define ORANGE  "\033[38;2;255;165;0m"
#define PURPLE  "\033[38;2;128;0;128m"
#define PINK    "\033[38;2;255;20;147m"
#define BOLD    "\033[1m"
#define UNDERLINE "\033[4m"
#define BLINK   "\033[5m"
#define REVERSE "\033[7m"
#define HIDDEN  "\033[8m"
#define ITALIC "\033[3m"
#define UNUNDERLINE "\033[24m"


using namespace std;

class User;
class UserClient;
class CompanyClient;
class BankEmployee;
class Card;
class Transaction;
class AccountType;
class Loan;
class Fraudulent;

time_t now = time(0);
tm* ltm = localtime(&now);

int counter = 0;

void saveUserLogID(string id)
{
	fstream file;
	file.open("userlogid.txt", ios::app);
	file << id << endl;
	file.close();
}

void saveCompanyLogID(string id)
{
	fstream file;
	file.open("companylogid.txt", ios::app);
	file << id << endl;
	file.close();
}

class Fraudulent {
public:
	string nameid;
	Fraudulent(string nameid) : nameid(nameid) {}
};

class User {
private:
	string name;
	string address;
	long long int cnic;
	string loginID;
	string password;
	string phone;
	int clientID;

public:
	int balance = 0;
	int wrongAttempts = 0;
	bool isAccountApproved = false;
	bool isBlocked = false;
	bool isFraudulent = false;
	static int clientCount;
	int cardCount = 0;
	string type;

	User(string name, string address, string loginID, string password, string phone, long long int cnic, int clientID) : name(name), address(address), loginID(loginID), password(password), phone(phone), cnic(cnic), clientID(clientID) { clientID++; }

	string getName() { return name; }
	string getAddress() { return address; }
	string getLoginID() { return loginID; }
	string getPassword() { return password; }
	string getPhone() { return phone; }
	long long int getCNIC() { return cnic; }
	int getClientID() { return clientID; }
	int getBalance() { return balance; }

	void setName(string name) { this->name = name; }
	void setAddress(string address) { this->address = address; }
	void setLoginID(string loginID) { this->loginID = loginID; }
	void setPassword(string password) { this->password = password; }
	void setPhone(string phone) { this->phone = phone; }
	void setCNIC(long long int cnic) { this->cnic = cnic; }
	void setClientID(int clientID) { this->clientID = clientID; }
	void setBalance(int balance) { this->balance = balance; }

	void incrementCardCount() { cardCount++; }

	void displayAllTransactions()
	{
		fstream file;
		file.open(getLoginID() + "transactions.txt", ios::in);
		if (!file)
		{
			cout << "File not found" << endl;
			return;
		}
		cout << MAGENTA << "Transactions: " << endl;
		cout << "-------------" << endl;
		cout << YELLOW;
		cout << file.rdbuf() << endl;
		cout << GREEN;
	}


	void worngAttempts()
	{
		wrongAttempts++;
	}

	int getWrongAttempts()
	{
		return wrongAttempts;
	}

	virtual void displayAllInfo() = 0;

};

int User::clientCount = 0;

class Transaction
{
public:
	int day;
	int month;
	int year;
	int userID;
	int amount;
	string type;
	int receiverID = -1;

	//constructor for deposit and withdraw
	Transaction(int day, int month, int year, int userID, int amount, string type, string loginID) : day(day), month(month), year(year), userID(userID), amount(amount), type(type)
	{
		ofstream file;
		file.open(loginID + "transactions.txt", ios::app);
		file << day << "-" << month << "-" << year << ", " << userID << ", " << amount << ", " << type << endl;
		file.close();
	}
	//constructor for transfer
	Transaction(int day, int month, int year, int userID, int amount, string type, int receiverID, string loginID) : day(day), month(month), year(year), userID(userID), amount(amount), type(type), receiverID(receiverID)
	{
		ofstream file;
		file.open(loginID + "Transactions.txt", ios::app);
		file << day << "-" << month << "-" << year << ", " << userID << ", " << amount << ", " << type << ", " << receiverID << endl;
		file.close();
	}

	void displayDepWith()
	{
		cout << "Date: " << day << "/" << month << "/" << year << endl;
		cout << "User ID: " << userID << endl;
		cout << "Amount: " << amount << endl;
		cout << "Type: " << type << endl;
	}
};


class AccountType {
public:
	string type;
	int dailyWithdrawLimit;
	string getType() { return type; }
	int getLimit() { return dailyWithdrawLimit; }
};


class Card
{
	long long int cardNumber;
	int cardPin;

public:
	Card(long long int cardNumber, int cardPin) : cardNumber(cardNumber), cardPin(cardPin) {}

	long long int getCardNumber() { return cardNumber; }
	int getCardPin() { return cardPin; }

	void setCardNumber(long long int cardNumber) { this->cardNumber = cardNumber; }
	void setCardPin(int cardPin) { this->cardPin = cardPin; }
};

class UserClient : public User {
public:
	vector<Transaction> transactions;
	vector<Card> cards;
	AccountType accountType;

	long long int generateCardNumber()
	{
		long long int cardNum = 0;
		for (int i = 0; i < 16; i++)
			cardNum = cardNum * 10 + rand() % 10;
		return cardNum;
	}

	UserClient(string name, string address, string loginID, string password, string phone, long long int cnic, int clientID, string type, long int limit, int pin) : User(name, address, loginID, password, phone, cnic, clientID)
	{
		accountType.type = type;
		accountType.dailyWithdrawLimit = limit;
		this->setClientID(clientID);
		incrementCardCount();
		cards.push_back(Card(generateCardNumber(), pin));
		++counter;
	}

	//making another constructor for reading from file	withuot pin
	UserClient(string name, string address, string loginID, string password, string phone, long long int cnic, int clientID, string type, long int limit,int balance, bool isBlocked,bool isAccountApproved, bool isFraud, int wrongAttempts) : User(name, address, loginID, password, phone, cnic, clientID)
	{
		accountType.type = type;
		accountType.dailyWithdrawLimit = limit;
		this->setClientID(clientID);
		this->setBalance(balance);
		this->isBlocked = isBlocked;
		this->isAccountApproved = isAccountApproved;
		this->isFraudulent = isFraud;
		this->wrongAttempts = wrongAttempts;

		long long int c;
		int p;

		fstream file;
		file.open(getLoginID() + "cards.txt", ios::in);
		if (!file)
		{
			cout << "File not found" << endl;
			return;
		}

		while (file >> c >> p)
		{
			cards.push_back(Card(c, p));
		}
	}


	void seperateDataFile() 
	{
		ofstream file;
		file.open(getLoginID() + ".txt", ios::trunc); // open the file in write mode
		file << getLoginID() << endl;
		file << getName() << endl;
		file << getAddress() << endl;
		file << getCNIC() << endl;
		file << getPhone() << endl;
		file << getClientID() << endl;
		file << getPassword() << endl;
		file << getBalance() << endl;
		file << isBlocked << endl; //this will write 0 or 1
		file << isAccountApproved << endl;
		file << isFraudulent << endl;
		file << wrongAttempts << endl;
		file << accountType.type << endl;
		file << accountType.dailyWithdrawLimit << endl;
		file.close();
	}

	///function to override base class function the above file with new balanace after transaction
	static void writeInFile(vector<UserClient> users)
	{
		ofstream file;
		file.open("users.txt", ios::out); 
		for (int i = 0; i < users.size(); i++)
			file << users[i].getLoginID() << ", " << users[i].getName() << ", " << users[i].getAddress() << ", " << users[i].getCNIC() << ", " << users[i].getClientID() << ", " << users[i].getPassword() << ", " << users[i].getBalance() << endl; //", "<< users[i].accountType.type<< ", " << users[i].accountType.dailyWithdrawLimit << ", "<<endl; 
		file.close();
	}

	void separateCardsFile()
	{
		ofstream file;
		file.open(getLoginID() + "cards.txt", ios::trunc);
		for (int i = 0; i < cards.size(); i++)
			file << cards[i].getCardNumber() << endl << cards[i].getCardPin() << endl;
	}

	void displayCards()
	{
		cout << UNDERLINE << MAGENTA << "Available Cards: " << UNUNDERLINE << YELLOW << endl;
		for (int i = 0; i < cards.size(); i++)
			cout << i + 1 << ". " << cards[i].getCardNumber() << endl;
		cout << GREEN;
	}


	void transferMoney(vector<UserClient>& users) {
		displayCards();
		int cardIndex;
		int pin;

		while (true) 
		{
			cout << "Select card: ";
			cin >> cardIndex;
			if (cardIndex == 0) return;
			if (cardIndex > cards.size()) {
				system("cls");
				cout <<RED<< "Invalid Card Number. Press 0 to Go Back."<<GREEN << endl;
			}
			else  break;
		}

		long long int cardNumber = cards[cardIndex - 1].getCardNumber();

		while (true) 
		{
			cout << "Enter pin: ";
			cin >> pin;
			if (pin == 0) return;
			if (pin != cards[cardIndex - 1].getCardPin()) {
				if (getWrongAttempts() == 3) {
					cout << "Fraudulent Report Sent." << endl;
					return;
				}
				cout <<RED<< "Incorrect Pin. Try Again or Press 0 to Go Back."<<GREEN << endl;
			} 
			else	break;
		}

		int amount;
		cout << "Enter amount to transfer: ", cin >> amount;
		if (amount > getBalance()) {
			cout << "Insufficient balance." << endl;
			return;
		}

		string receiverId;
		cout << "Enter receiver UserID: ", cin >> receiverId;
		for (UserClient& user : users) 
		{
			if (user.getLoginID() == receiverId) {
				user.setBalance(user.getBalance() + amount);
				transactions.push_back(Transaction(ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year, getClientID(), amount, "transfer", user.getClientID(), getLoginID()));
				setBalance(getBalance() - amount);
				cout << "Amount of " << amount << " transferred to " << user.getLoginID() << " successfully." << endl;
				separateCardsFile();
				seperateDataFile();
				user.seperateDataFile();
				writeInFile(users);
				return;  
			}
		}
		system("cls");
		cout<<RED<< "Receiver"<<receiverId<<" Not Found!"<<GREEN << endl;

	}

	void newCard()
	{
		long long int x = generateCardNumber();
		cout << "New card number: " << x << endl;
		int pin;
		cout << "Set pin for new card: ";
		cin >> pin;
		cards.push_back(Card(x, pin));
		//write card data in cards.txt
		ofstream file;
		file.open("cards.txt", ios::app);
		file << getClientID() << ", " << x << ", " << pin << endl;
		file.close();
		separateCardsFile();
	}

	void withDraw()
	{
		displayCards();
		long long int card;
		int pin, x;

	CARD:
		cout << "Select Card:", cin >> x;
		if (x == 0) return;
		if (x > cards.size())
		{
			system("cls");
			cout << RED << "Invalid Card Number, Press 0 to Go Back OR" << GREEN << endl;
			goto CARD;
		}
		card = cards[x - 1].getCardNumber();
		
		cout<<"Enter pin: ", cin >> pin;
		if (pin != cards[x - 1].getCardPin())
		{
			this->worngAttempts();
			cout << RED << "Incorrect Pin Try Again, or Press 0 to Go Back" << GREEN << endl;
			if (getWrongAttempts() == 3)
			{
				cout << RED << "Fraudulent Report Sent" << GREEN << endl;
				return;
			}
			goto CARD;
		}

	USERWITHDRAWAMT:
		int amount;
		cout << "Enter amount to withdraw: ", cin >> amount;
		if(amount == 0) return;

		if (amount <= accountType.dailyWithdrawLimit)
		{
			if (amount <= getBalance())
			{
				this->setBalance(this->getBalance() - amount);
				transactions.push_back(Transaction(ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year, getClientID(), amount, "withdraw", this->getLoginID()));
				cout << "Amount withdrawn successfully" << endl;
				seperateDataFile();
			}
			else
			{
				system("cls");
				cout << RED << "Insufficient balance, Try Again or \n Press 0 to Go Back" << GREEN << endl;
				goto USERWITHDRAWAMT;
			}
		}
		else
		{
			system("cls");
			cout << RED << "Amount exceeds daily withdraw limit" << GREEN << endl;
			goto USERWITHDRAWAMT;
		}
	}

	void deposit()
	{
		long long int card;
		int pin, x;

	CARD:
		displayCards();
		cout << GREEN;
		cout << "Select Card:", cin >> x;
		if (x == 0) return;
		if (x > cards.size())
		{
			system("cls");
			cout << RED << "Invalid Card Number, Press 0 to Go Back OR" << GREEN << endl;
			goto CARD;
		}
		card = cards[x - 1].getCardNumber();

		cout << "Enter pin: ";
		cin >> pin;

		bool cardFound = false;

		for (int i = 0; i < cards.size(); i++)
		{
			if (cards[i].getCardNumber() == card && cards[i].getCardPin() == pin)
			{
				cardFound = true;
				break;
			}
		}

		if (!cardFound)
		{
			system("cls");
			cout << RED << "Incorrect Card or Pin, Try Again or\n press 0 to exit" << endl;
			displayCards();
			goto CARD;
		}

		int amount;
		cout << "Enter amount to deposit: ", cin >> amount;
		this->setBalance(this->getBalance() + amount);
		transactions.push_back(Transaction(ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year, getClientID(), amount, "deposit", this->getLoginID()));
		system("cls");
		cout << "Amount deposited successfully" << endl;
		seperateDataFile();
	}


	void displayAllInfo()
	{
		system("cls");
		cout<<UNDERLINE<<MAGENTA<< "Name: " <<UNUNDERLINE<<ORANGE<< getName() << endl;
		cout<<UNDERLINE<<MAGENTA<< "Address: " <<UNUNDERLINE<<ORANGE<< getAddress() << endl;
		cout<<UNDERLINE<<MAGENTA<< "Login ID: " <<UNUNDERLINE<<ORANGE<< getLoginID() << endl;
		cout<<UNDERLINE<<MAGENTA<< "Password: " <<UNUNDERLINE<<ORANGE<< getPassword() << endl;
		cout<<UNDERLINE<<MAGENTA<< "Phone: " <<UNUNDERLINE<<ORANGE<< getPhone() << endl;
		cout<<UNDERLINE<<MAGENTA<< "CNIC: " <<UNUNDERLINE<<ORANGE<< getCNIC() << endl;
		cout<<UNDERLINE<<MAGENTA<< "User ID: " <<UNUNDERLINE<<ORANGE<< getClientID() << endl;
		cout<<UNDERLINE<<MAGENTA<< "Balance: " <<UNUNDERLINE<<ORANGE<< getBalance() << endl;
		cout<<UNDERLINE<<MAGENTA<< "Account Type: " <<UNUNDERLINE<<ORANGE<< accountType.type << endl;
		cout<<UNDERLINE<<MAGENTA<< "Daily Withdraw Limit: " <<UNUNDERLINE<<ORANGE<< accountType.dailyWithdrawLimit << endl;
		cout<<RESET<< endl;
	}

};

class CompanyClient : public UserClient
{
public:
	vector<UserClient> employees;
	vector<Transaction> transactions;
	vector<Card> cards;
	AccountType accountType;

	int companyID;
	int taxNumber;
	int loan = 0;
	bool isBlocked = false;
	bool isLoanAccepted = false;
	bool isAccountApproved = false;
	static int companyCount;

	CompanyClient(string name, string address, string loginID, string password, string phone, long long int cnic, int clientID, string type, long int limit, int pin, int companyCount, int taxNumber) : UserClient(name, address, loginID, password, phone, cnic, clientID, type, limit, pin), companyID(companyCount++), taxNumber(taxNumber)
	{
		this->companyID = companyCount;
		accountType.type = type;
		accountType.dailyWithdrawLimit = limit;
		ofstream file;
		file.open("companies.txt", ios::app);
		file << loginID << ", " << name << ", " << address << ", " << cnic << ", " << clientID << ", " << password << ", " << 0 << endl;
		file.close();
		long long int x = generateCardNumber();
		cards.push_back(Card(x, pin));
		file.open("cards.txt", ios::app);
		file << clientID << ", " << x << ", " << pin << endl;
		file.close();
		separateCardsFile();
		seperateDataFile();
		saveCompanyLogID(loginID);
		++counter;
	}


	CompanyClient(string name, string address, string loginID, string password, string phone, long long int cnic, int clientID, string type, long int limit, int balance, bool isBlocked, bool isAccountApproved, bool isFraud, int wrongAttempts, int companyID, int taxNumber,int loan, bool isLoanAccepted ,vector<UserClient>u) : UserClient(name, address, loginID, password, phone, cnic, clientID, type, limit, balance, isBlocked, isAccountApproved, isFraud, wrongAttempts)
	{
		this->companyID = companyID;
		this->taxNumber = taxNumber;
		this->accountType.type = type;
		this->accountType.dailyWithdrawLimit = limit;
		this->isBlocked = isBlocked;
		this->isAccountApproved = isAccountApproved;
		this->isFraudulent = isFraud;
		this->wrongAttempts = wrongAttempts;
		this->setBalance(balance);
		this->loan = loan;
		this->isLoanAccepted = isLoanAccepted;

		long long int c;
		int p;

		fstream file;
		file.open(getLoginID() + "cards.txt", ios::in);
		if (!file)
		{
			cout << "File not found" << endl;
			return;
		}

		while (file >> c >> p)
		{
			cards.push_back(Card(c, p));
		}

		int l;
		fstream f;
		f.open(loginID + "loans.txt", ios::in);
		f >> l;
		if (l > 0)
		{
			this->isLoanAccepted = false;
			this->loan = l;
		}
		f.close();
		
		fstream e;
		e.open(loginID + "employees.txt", ios::in);
		
		string s;
		while (e >> s)
		{
			for (int i = 0; i < u.size(); i++)
			{
				if (s == u[i].getLoginID())
				{
					employees.push_back(u[i]);
				}
			}
		}
		e.close();
	}

	static void writeInFile(vector<CompanyClient> companies)
	{
		ofstream file;
		file.open("companies.txt", ios::out); 

		for (int i = 0; i < companies.size(); i++)
		{
			file << companies[i].getLoginID() << ", " << companies[i].getName() << ", " << companies[i].getAddress() << ", " << companies[i].getCNIC() << ", " << companies[i].getClientID() << ", " << companies[i].getPassword() << ", " << companies[i].getBalance() << endl;
		}
	}

	void separateEmployeesFile() 
	{
		ofstream file;
		file.open(this->getLoginID() + "employees.txt", ios::app);
		for (int i = 0; i < employees.size(); i++)
		{
			file << employees[i].getLoginID() << endl;
		}
		file.close();
	}


	void addEmployee(vector<UserClient>& users)
	{
		string id;
		cout << "Enter UserID: ", cin >> id;
		for (int i = 0; i < users.size(); i++)
		{
			if (id == users[i].getLoginID())
			{
				employees.push_back(users[i]);
				cout << "Employee Added Successfully!" << endl;
				fstream file("companies_employees.txt", ios::app);
				file << users[i].getClientID() << ", " << this->companyID << endl;
				file.close();
				separateEmployeesFile();

				return;
			}
		}
	}


	void seperateDataFile()
	{
		ofstream file;
		file.open(getLoginID() + ".txt", ios::trunc); // open the file in truncate mode so that it will be empty
		file << getLoginID() << endl;
		file << getName() << endl;
		file << getAddress() << endl;
		file << getCNIC() << endl;
		file << getPhone() << endl;
		file << getClientID() << endl;
		file << getPassword() << endl;
		file << getBalance() << endl;
		file << isBlocked << endl;
		file << isAccountApproved << endl;
		file << isFraudulent << endl;
		file << wrongAttempts << endl;
		file << accountType.type << endl;
		file << accountType.dailyWithdrawLimit << endl;
		file << companyID << endl;
		file << taxNumber << endl;
		file << loan << endl;
		file<< isLoanAccepted << endl;
		file.close();
	}

	void separateCardsFile()
	{
		ofstream file;
		file.open(getLoginID() + "cards.txt", ios::trunc);
		for (int i = 0; i < cards.size(); i++)
		{
			file << cards[i].getCardNumber() << endl << cards[i].getCardPin() << endl;
		}
	}

	void newCard()
	{
		long long int x = generateCardNumber();
		cout << "New card number: " << x << endl;
		int pin;
		cout << "Set pin for new card: ";
		cin >> pin;
		cards.push_back(Card(x, pin));
		///write card data in cards.txt
		ofstream file;
		file.open("cards.txt", ios::app);
		file << getClientID() << ", " << x << ", " << pin << endl;
	}

	void displayCards()
	{
		cout << UNDERLINE << MAGENTA << "Available Cards: " << UNUNDERLINE << YELLOW << endl;
		for (int i = 0; i < cards.size(); i++)
		{
			cout << i+1<<". "<<cards[i].getCardNumber() << endl;
		}
		cout << GREEN;
	}

	void viewEmployees()
	{
		cout <<UNDERLINE<<MAGENTA<< this->getName() << "'s Employees: " <<UNUNDERLINE<< endl;
		for (int i = 0; i < employees.size(); i++)
		{
			cout <<ORANGE<< i + 1 << ". " << employees[i].getName() << ", " << employees[i].getLoginID() << endl;
		}
		cout<<GREEN<< endl;
	}

	void giveSalary(vector<UserClient>&users)
	{
		viewEmployees();

		int opt;
		cout << "Select Employee: ", cin >> opt;
		string loginID = employees[opt - 1].getLoginID();

		for (int i = 0; i < employees.size(); i++)
		{
			if (employees[i].getLoginID() == loginID)
			{
				if (employees[i].isBlocked)
				{
					cout << RED << "Employee is blocked" << GREEN << endl;
					return;
				}

				if (employees[i].isAccountApproved == false)
				{
					cout << RED << "Employee account is not approved" << GREEN << endl;
					return;
				}

				displayCards();
				long long int card;
				int pin, x;
				CARD:
				cout<<"Select Card: ", cin >> x;
				if (x == 0) return;
				if (x > cards.size())
				{
					system("cls");
					cout << RED << "Invalid Card Number, Press 0 to Go Back OR" << GREEN << endl;
					goto CARD;
				}
				card = cards[x - 1].getCardNumber();

				cout<<"Enter pin: ", cin >> pin;
				if (pin == 0) return;

				if (pin != cards[x - 1].getCardPin())
				{
					system("cls");
					cout << RED << "Incorrect Pin Try Again, or Press 0 to Go Back" << GREEN << endl;
					this->worngAttempts();
					if (getWrongAttempts() == 3)
					{
						cout << RED << "Fraudulent Report Sent" << GREEN << endl;
						return;
					}
					goto CARD;
				}
				

				int amount;
				cout << "Enter amount to give: ", cin >> amount; 
				if (amount <= getBalance())
				{
					this->setBalance(this->getBalance() - amount);
					employees[i].setBalance(employees[i].getBalance() + amount);
					for (int j = 0; j < users.size(); j++)
					{
						if (users[j].getLoginID() == loginID)
						{
							users[j].setBalance(users[j].getBalance() + amount);
							users[j].seperateDataFile();
						}
					}
					transactions.push_back(Transaction(ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year, companyID, amount, "salary", this->getLoginID()));
					cout << "Salary given successfully" << endl;
					seperateDataFile();
					UserClient::writeInFile(users);
					return;
				}
			}
		}
		cout << RED << "Employee not found" << GREEN << endl;
	}

	void deposit()
	{
		int amount;
		cout << "Enter amount to deposit: ", cin >> amount;
		displayCards();
		long long int card;
		int pin, x;
		CARD:
		cout<<"Select Card: ", cin >> x;
		if (x == 0) return;
		if (x > cards.size())
		{
			system("cls");
			cout << RED << "Invalid Card Number, Press 0 to Go Back OR" << GREEN << endl;
			goto CARD;
		}
		card = cards[x - 1].getCardNumber();
		
		do 
		{
			cout<<"Enter pin: ", cin >> pin;
			if (pin == 0) return;

			if (pin != cards[x - 1].getCardPin())
			{
				cout << RED << "Incorrect Pin Try Again, or Press 0 to Go Back" << GREEN << endl;
				this->worngAttempts();
			}

		} while (pin != cards[x - 1].getCardPin());

		this->setBalance(this->getBalance() + amount);
		transactions.push_back(Transaction(ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year, companyID, amount, "deposit", this->getLoginID()));
		seperateDataFile();
		system("cls");
		cout << "Amount deposited successfully" << endl;
	}

	void withdraw()
	{
		cout << "Enter amount to withdraw: ";
		int amount;
		cin >> amount;
		if (amount <= accountType.dailyWithdrawLimit)
		{
			if (amount <= getBalance())
			{
				long long int card;
				int pin;
				int x;
				//cout << "Select Card: ", cin >> card;

				//cout << "Enter pin: ", cin >> pin;
			CARD:
				displayCards();
				cout << "Select Card: ", cin >> x;
				if (x == 0) return;
				if (x > cards.size())
				{
					system("cls");
					cout << RED << "Invalid Card Number, Press 0 to Go Back OR" << GREEN << endl;
					goto CARD;
				}
				card = cards[x - 1].getCardNumber();

				cout << "Enter pinn: ", cin >> pin;
				if (pin == 0) return;

				if (pin != cards[x - 1].getCardPin())
				{
					system("cls");
					cout << RED << "Incorrect Pin Try Again, or Press 0 to Go Back" << GREEN << endl;
					this->worngAttempts();
					if (getWrongAttempts() == 3)
					{
						cout << RED << "Fraudulent Report Sent" << GREEN << endl;
						return;
					}
					goto CARD;
				}


				//bool cardFound = false;

				//for (int i = 0; i < cards.size(); i++)
				//{
				//	if (cards[i].getCardNumber() == card && cards[i].getCardPin() == pin)
				//	{
				//		cardFound = true;
				//		break;
				//	}
				//}

				//if (!cardFound)
				//{
				//	cout << RED << "Card not found or wrong pin" << GREEN << endl;
				//	return;
				//}

				this->setBalance(this->getBalance() - amount);
				transactions.push_back(Transaction(ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year, companyID, amount, "withdraw", this->getLoginID()));
				seperateDataFile();
				cout << "Amount withdrawn successfully" << endl;
			}
			else
			{
				cout << RED << "Insufficient balance" << GREEN << endl;
			}
		}
		else
		{
			cout << RED << "Amount exceeds daily withdraw limit of  "<<BLUE<<accountType.dailyWithdrawLimit << GREEN << endl;
		}
	}

	void viewTransactions()
	{
		cout << MAGENTA << UNDERLINE << "Transactions: " << UNUNDERLINE << YELLOW << endl;
		fstream file(getLoginID() + "transactions.txt", ios::in);
		cout<< file.rdbuf();
		file.close();
		cout << GREEN;
	}

	void viewBalance()
	{
		cout << UNDERLINE<<MAGENTA<<"Balance: " <<UNUNDERLINE<<ORANGE<< getBalance() << endl;
	}

	bool doesEmployeeExist(string loginID)
	{
		for (int i = 0; i < employees.size(); i++)
		{
			if (employees[i].getLoginID() == loginID)
			{
				return true;
			}
		}
		return false;
	}

	void displayAllInfo()
	{
		cout << UNDERLINE << MAGENTA << "Name: " << UNUNDERLINE << ORANGE << getName() << endl;
		cout << UNDERLINE << MAGENTA << "Address: " << UNUNDERLINE << ORANGE << getAddress() << endl;
		cout << UNDERLINE << MAGENTA << "Login ID: " << UNUNDERLINE << ORANGE << getLoginID() << endl;
		cout << UNDERLINE << MAGENTA << "Password: " << UNUNDERLINE << ORANGE << getPassword() << endl;
		cout << UNDERLINE << MAGENTA << "Phone: " << UNUNDERLINE << ORANGE << getPhone() << endl;
		cout << UNDERLINE << MAGENTA << "CNIC: " << UNUNDERLINE << ORANGE << getCNIC() << endl;
		cout << UNDERLINE << MAGENTA << "Company ID: " << UNUNDERLINE << ORANGE << getClientID() << endl;
		cout << UNDERLINE << MAGENTA << "Balance: " << UNUNDERLINE << ORANGE << getBalance() << endl;
		cout << UNDERLINE << MAGENTA << "No. of Employees: " << UNUNDERLINE << ORANGE << employees.size() << endl;
		cout << RESET << endl;
	}

};

int CompanyClient::companyCount = 0;

class BankEmployee : public User
{
public:
	//vector<Loan> loans;
	vector<Fraudulent>fraud;

	BankEmployee(string name, string address, string loginID, string password, string phone, long long int cnic, int clientID) : User(name, address, loginID, password, phone, cnic, clientID)
	{
		ofstream file;
		file.open("admin.txt", ios::app);
		file << loginID << ", " << name << ", " << address << ", " << cnic << ", " << clientID << ", " << password << endl;
		file.close();
	}

	void displayAllInfo()
	{
		system("cls");
		cout << "Name: " << getName() << endl;
		cout << "Address: " << getAddress() << endl;
		cout << "Login ID: " << getLoginID() << endl;
		cout << "Password: " << getPassword() << endl;
		cout << "Phone: " << getPhone() << endl;
		cout << "CNIC: " << getCNIC() << endl;
		cout << "Bank Employee ID: " << getClientID() << endl;
	}

	void blockUser(vector<UserClient>&users, vector<CompanyClient>&companies)
	{
		cout << "User ID to block: " << endl;
		for (int i = 0; i < users.size(); i++)
			cout << users[i].getLoginID() << endl;


		cout << "Company ID to block:" << endl;
		for (int i = 0; i < companies.size(); i++)
			cout << companies[i].getLoginID() << endl;


		string userID;
		cout << "Enter ID to block: ", cin >> userID;

		for (int i = 0; i < users.size(); i++)
		{
			if (users[i].getLoginID() == userID)
			{
				users[i].isBlocked = true;
				users[i].seperateDataFile();
				UserClient::writeInFile(users);
				system("cls");
				cout << "User blocked successfully" << endl;
				return;
			}
		}

		for (int i = 0; i < companies.size(); i++)
		{
			if (companies[i].getLoginID() == userID)
			{
				companies[i].CompanyClient::isBlocked = true;
				companies[i].isBlocked = true;
				companies[i].seperateDataFile();
				CompanyClient::writeInFile(companies);
				system("cls");
				cout << "Company blocked successfully" << endl;
				return;
			}
		}
		system("cls");

		cout << "User not found" << endl;
	}

	void unblockUser(vector<UserClient>&users, vector<CompanyClient>& companies)
	{
		cout << "Enter user ID to unblock: ";
		string userID;
		cin >> userID;

		for (int i = 0; i < users.size(); i++)
		{
			if (users[i].getLoginID() == userID)
			{
				users[i].isBlocked = false;
				system("cls");
				cout << "User unblocked successfully" << endl;
				return;
			}
		}

		for (int i = 0; i < companies.size(); i++)
		{
			if (companies[i].getLoginID() == userID)
			{
				companies[i].CompanyClient::isBlocked = false; 
				system("cls");

				cout << "Company unblocked successfully" << endl;
				return;
			}
		}
		cout << RED << "User not found" << GREEN << endl;
	}

	void receiveLoanRequest(int amount, string name, vector<CompanyClient>company)
	{
		
		fstream file(name + "loans.txt",ios::out | ios::trunc);

		for (int i = 0; i < company.size(); i++)
		{
			if (company[i].getLoginID() == name)
			{
				company[i].loan =+ amount;
				file<<company[i].loan;
				company[i].isLoanAccepted = false;
				cout << "Loan Request Received" << endl;
				return;
			}
		}
		file.close();
	}

	void displayLoanRequests(vector<CompanyClient>company)
	{
		cout<<MAGENTA<<UNDERLINE<< "Loan Requests: " << UNUNDERLINE<< endl;
		for (int i = 0; i < company.size(); i++)
		{
			if (!company[i].isLoanAccepted)
			{
				cout<<GREEN<<i+1<<". Company: "<<CYAN<<company[i].getLoginID()<<GREEN<<"		Amount:"<<CYAN<<company[i].loan<<endl;
			}
		}
	}

	void approveLoanRequet(vector<CompanyClient>& companies)
	{
		displayLoanRequests(companies);

		int opt;
		string name;
		cout<<"Select Company to Approve Loan Request: ", cin >> opt;
		name = companies[opt - 1].getLoginID();
		
		int amount;
		fstream file(name + "loans.txt", ios::in);
		file >> amount;
		file.close();

		for (int i = 0; i < companies.size(); i++)
		{
			if (name == companies[i].getLoginID())
			{
				companies[i].isLoanAccepted = true;
				companies[i].setBalance(companies[i].getBalance() + amount);
				companies[i].loan = 0;
				CompanyClient::writeInFile(companies);
				companies[i].seperateDataFile();
				cout << "Loan Approved" << endl;
				return;
			}
		}
		cout << RED << "Company not found" << GREEN << endl;
	}



	void disapproveLoanRequests(vector<CompanyClient>& companies)
	{
		displayLoanRequests(companies);

		int opt;
		string name;
		cout << "Select Company to Disapprove Loan Request: ", cin >> opt;
		name = companies[opt - 1].getLoginID();

		for (int i = 0; i < companies.size(); i++)
		{
			if (name == companies[i].getLoginID())
			{
				companies[i].isLoanAccepted = false;
				companies[i].loan = 0;
				CompanyClient::writeInFile(companies);
				companies[i].seperateDataFile();
				fstream file;
				file.open(name + "loans.txt", ios::out | ios::trunc);
				file.close();

				cout << "Loan Disapproved" << endl;
				return;
			}
		}
		cout << RED << "Company not found" << GREEN << endl;
	}

	void viewTransactionHistoryOfSpecificAccount(string nameid)
	{
		fstream file(nameid + "transactions.txt", ios::in);
		cout << file.rdbuf();
	}

	void setFraudulent(vector<UserClient>&users, string nameid)
	{
		int index;

		bool isfound = false;
		for (int i = 0; i < users.size(); i++)
		{
			if (users[i].getLoginID() == nameid)
			{
				isfound = true;
				index = i;
				break;
			}
		}
		users[index].isFraudulent = true;
		fraud.push_back(Fraudulent(nameid));
	}

	void displayFraudulent()
	{
		system("cls");

		cout << "Fraudulent Users: " << endl;
		cout << "-----------------" << endl;
		for (int i = 0; i < fraud.size(); i++)
		{
			cout << i + 1 << fraud[i].nameid << endl;
		}
	}

	void freezeFraudulent(vector<UserClient>& users)
	{
		displayFraudulent();
		int opt;
		string name;
		cout << "Select Account to Block: ", cin >> opt;
		name = fraud[opt - 1].nameid;


		for (int i = 0; i < users.size(); i++)
		{
			if (name == users[i].getLoginID())
			{
				users[i].isBlocked = true;
				cout << "Account Blocked" << endl;
				break;
			}
		}
	}


	void displayUnApprovedUsers(vector<UserClient>& user)
	{
		cout << MAGENTA << "Unapproved Users: " << "\n-----------------" << YELLOW << endl;
		for (int i = 0; i < user.size(); i++)
		{
			if (!user[i].isAccountApproved)
			{
				cout << user[i].getLoginID() << endl;
			}
		}

		string name;
		cout << GREEN << "Enter name to Approve: ", cin >> name;

		for (int i = 0; i < user.size(); i++)
		{
			if (user[i].getLoginID() == name)
			{
				user[i].isBlocked = false;
				user[i].isAccountApproved = true;
				system("cls");
				cout << user[i].getLoginID() << "  " << user[i].getName() << " Account Approved! " << endl;
				ofstream file; /// write in file
				file.open("users.txt", ios::app);
				file << user[i].getLoginID() << ", " << user[i].getName() << ", " << user[i].getAddress() << ", " << user[i].getCNIC() << ", " << user[i].getClientID() << ", " << user[i].getPassword() << ", " << user[i].getBalance() << endl;
				file.close();
				saveUserLogID(user[i].getLoginID()); /// write in userlogid.txt

				user[i].seperateDataFile();
				user[i].separateCardsFile();

				file.open("cards.txt", ios::app);
				file << user[i].getClientID() << ", " << user[i].cards[0].getCardNumber() << ", " << user[i].cards[0].getCardPin() << endl;
				file.close();
				return;
			}
		}
		cout << "User not found" << endl;
	}


	void displayUnApprovedCompanies(vector<CompanyClient>& company)
	{
		cout<<MAGENTA << "Unapproved Companies: " << "\n-----------------" << YELLOW << endl;
		for (int i = 0; i < company.size(); i++)
		{
			if (!company[i].isAccountApproved)
			{
				cout << company[i].getLoginID() << endl;
			}
		}

		string name;
		cout << GREEN << "Enter name to Approve: ", cin >> name;

		for (int i = 0; i < company.size(); i++)
		{
			if (name == company[i].getLoginID())
			{
				company[i].isAccountApproved = true; 
				system("cls");
				cout << company[i].getLoginID() << "  " << company[i].getName() << " Account Approved! " << endl;
				ofstream file; /// write in file
				file.open("companies.txt", ios::app);
				file << company[i].getLoginID() << ", " << company[i].getName() << ", " << company[i].getAddress() << ", " << company[i].getPassword() << ", " << company[i].getBalance() << endl;
				file.close();
				saveCompanyLogID(company[i].getLoginID()); /// write in companylogid.txt
				company[i].seperateDataFile();
				company[i].separateCardsFile();
				return;
			}
		}
		cout << "Company not found" << endl;
	}

	
};

/// ====================  M A I N  ====================
int main()
{
	srand(time(NULL));

	string loginID, password, name, address, phone, type;
	long long int cnic; int tax, pin, loan;
	long int limit;
	int clientID, balance;

	//more variables for reading from file
	bool isBlocked, isAccountApproved, isFraud;
	int wrongAttempts;

	BankEmployee admin("Ali Imran", "State Life", "admin1234", "admin@1234", "03008194763", 3520281221737, 1);
	//admin.displayAllInfo();
	vector<UserClient> users;
	vector<CompanyClient> companies;

	fstream file2;
	file2.open("userlogid.txt", ios::in);
	string n;

	while (file2>> n)
	{
		fstream file3;
		file3.open(n + ".txt", ios::in);
		getline(file3, loginID);
		getline(file3, name);
		getline(file3, address);
		file3>>cnic>>phone>>clientID>>password>>balance>>isBlocked>>isAccountApproved>>isFraud>>wrongAttempts>>type>>limit;
		file3.close();

		//cout<<loginID << " " << name << " " << address << " " << cnic << " " << phone << " " << clientID << " " << password << " " << balance << " " << type << " " << limit << endl;
		++counter;
		users.push_back(UserClient(name, address, loginID, password, phone, cnic, clientID, type, limit, balance, isBlocked, isAccountApproved, isFraud, wrongAttempts));
	}
	
	file2.close();

	int companyID;
	bool isLoanAccepted;

	fstream file4;
	file4.open("companylogid.txt", ios::in);
	string n2;

	while (file4>>n2)
	{

		fstream file5;
		file5.open(n2 + ".txt", ios::in);
		getline(file5, loginID);
		getline(file5, name);
		getline(file5, address);
		file5 >> cnic>>phone>>clientID>>password>>balance>>isBlocked>>isAccountApproved>>isFraud>>wrongAttempts>>type>>limit>>companyID>>tax>>loan>>isLoanAccepted;
		file5.close();
		//cout<<loginID << " " << name << " " << address << " " << cnic << " " << phone << " " << clientID << " " << password << " " << balance << " " << type << " " << limit << " " << companyID << " " << tax << " " << loan << " acc" << isLoanAccepted << endl;

		if(isLoanAccepted) loan = 0;

		companies.push_back(CompanyClient(name, address, loginID, password, phone, cnic, clientID, type, limit, balance, isBlocked, isAccountApproved, isFraud, wrongAttempts, companyID, tax,loan, isLoanAccepted, users));
		++counter;
	}

	file4.close();

	if (true)
	{
	Main_Menu:
		cout << MAGENTA"\t\t\tW E L C O M E   TO   B A N K   M A N A G E M E N T   S Y S T E M" << endl;
		cout << "\t\t\t=================================================================" << endl;

		cout << RED << "Choose an Option: " << endl
			<< "1. Create New Account" << endl
			<< "2. Login Account" << endl
			<< "3. Exit" << endl;

		int opt;
		cin >> opt;
		///=================  C R E A T E   A C C O U N T  ====================
		if (opt == 1)///Create Account
		{
			cout << "Choose Account Type: " << endl
				<< "1. User Account" << endl
				<< "2. Company Account" << endl;
			cin >> opt;

			if (opt == 1) ///create user account
			{
				cin.ignore();
				cout << GREEN;
				cout << "Enter Name: ", getline(cin, name);
				cout << "Enter Address: ", getline(cin, address);
				cout << "Enter CNIC: ", cin >> cnic;
				cout << "Enter Phone: ", cin >> phone;
				cout << "Enter Login ID: ", cin >> loginID;
				cout << "Enter Password: ", cin >> password;
				cout << "Enter Card Pin: ", cin >> pin;

				cout << "Select Account Type: " << endl
					<< "1. Bronze" << endl
					<< "2. Gold" << endl;
				int t;
				cin >> t;

				if (t == 1) {
					type = "Bronze";
					limit = 100000;
				}
				else if (t == 2) {
					type = "Gold";
					limit = 500000;
				}

				users.push_back(UserClient(name, address, loginID, password, phone, cnic, counter, type, limit, pin));
				system("cls");
				cout << "Account will be approved by admin" << endl;
				goto Main_Menu;
			}

			else if (opt == 2)  ///create company account
			{
				cin.ignore();
				cout << GREEN;
				cout << "Enter Name: ", getline(cin, name);
				cout << "Enter Address: ", cin >> address;
				cout << "Enter CNIC: ", cin >> cnic;
				cout << "Enter Phone: ", cin >> phone;
				cout << "Enter Login ID: ", cin >> loginID;
				cout << "Enter Password: ", cin >> password;
				cout << "Enter Tax Number: ", cin >> tax;
				cout << "Enter Card Pin: ", cin >> pin;

				cout << "Select Account Type: " << endl
					<< "1. Bronze" << endl
					<< "2. Gold" << endl
					<< "3. Business" << endl;
				int t;
				cin >> t;

				if (t == 1) {
					type = "Bronze";
					limit = 100000;
				}
				else if (t == 2) {
					type = "Gold";
					limit = 500000;
				}
				else if (t == 3) {
					type = "Business";
					limit = 2000000;
				}

				companies.push_back(CompanyClient(name, address, loginID, password, phone, cnic, counter, type, limit, pin, counter, tax));
				system("cls");
				cout<<"Account will be approved by admin"<<endl;
				goto Main_Menu;
			}
			else
			{
				system("cls");
				goto Main_Menu;
			}
		}
		///=================  L O G I N   A C C O U N T  ====================
		else if (opt == 2)
		{
			system("cls");
		LOGIN:

			cout << MAGENTA << "\t\t\t\t\t\t" << "L O G I N   P A G E" << endl;
			cout << "\t\t\t\t\t\t=====================" << endl;
			cout << RED;
			cout << "Select Account Type: " << endl
				<< "1. User Account" << endl
				<< "2. Company Account" << endl
				<< "3. Bank Employee" << endl
				<< "4. Back" << endl;
			cin >> opt;

			if (opt == 1)   ///logging in user account
			{
				system("cls");
				cout << MAGENTA << "\t\t\tU S E R   L O G I N   P A G E" << endl;
				cout << "\t\t\t===============================" << endl;
				cout << GREEN;
				cout << "Enter Login ID: ", cin >> loginID;

				int index; bool found = false;
				for (int i = 0; i < users.size(); i++)
				{
					if (users[i].getLoginID() == loginID)
					{
						index = i;
						found = true;
						break;
					}
				}
				if (!found)
				{
					system("cls");
					cout << "User not found" << endl;
					goto LOGIN;
				}

				cout << "Enter Password: ", cin >> password;

				if (users[index].getPassword() == password)
				{
					if (users[index].isBlocked)
					{
						system("cls");
						cout << RED << "Account is Blocked!" << endl;
						goto LOGIN;
					}
					if (users[index].isFraudulent)
					{
						system("cls");
						cout << RED << "Account is Fraudulent!" << endl;
						goto LOGIN;
					}
					///-----------------------
					if (users[index].isAccountApproved == false)
					{
						system("cls");
						cout << users[index].getLoginID() << "  " << users[index].getName();
						cout << RED << " Account is not Approved!" << endl;
						goto LOGIN;
					}
					cout << GREEN;
					cout << "Login Successful!" << endl;

				Options:

					cout << "Choose an Option: " << endl
						<< "1. Deposit" << endl
						<< "2. Withdraw" << endl
						<< "3. Transfer Money" << endl
						<< "4. View Balance" << endl
						<< "5. View Transactions" << endl
						<< "6. Request New Card" << endl
						<< "7. Log in to Company" << endl
						<< "8. Display All Information" << endl
						<< "9. Log Out" << endl;
					cin >> opt;

					if (opt == 1)
					{
						system("cls");
						users[index].deposit();
						UserClient::writeInFile(users);
						goto Options;
					}

					else if (opt == 2)
					{
						system("cls");
						users[index].withDraw();
						UserClient::writeInFile(users);
						goto Options;
					}

					else if (opt == 3)
					{
						system("cls");
						users[index].transferMoney(users);
						UserClient::writeInFile(users);
						goto Options;
					}

					else if (opt == 4)
					{
						system("cls");
						cout << "Your Balance is : " << users[index].getBalance() << endl;
						goto Options;
					}

					else if (opt == 5)
					{
						system("cls");
						users[index].displayAllTransactions();
						goto Options;
					}

					else if (opt == 6)
					{
						system("cls");
						users[index].newCard();
						goto Options;
					}

					else if (opt == 7)
					{
						cout << CYAN << "\t\t\t\t C O M P A N Y   L O G I N   P A G E" << endl;
						cout << GREEN << "Enter Company ID", cin >> loginID;
						int index;
						bool found = false;

						for (int i = 0; i < companies.size(); i++)
						{
							if (companies[i].getLoginID() == loginID)
							{
								index = i;
								found = true;
								break;
							}
						}

						if (!found)
						{
							system("cls");
							cout << "Company not found" << endl;
							goto Options;
						}

						if (companies[index].isAccountApproved == false)
						{
							system("cls");
							cout << "Company not Approved" << endl;
							goto Options;
						}

						if (companies[index].doesEmployeeExist(users[index].getLoginID()))
						{
							cout << "Employee Found!" << endl;
						}

						else
						{
							system("cls");
							cout << "Employee not found" << endl;
							goto Options;
						}

						cout << "Enter Password: ", cin >> password;
						if (companies[index].getPassword() == password)
						{
							system("cls");
						optionUsercom:
							cout << "Choose an Option: " << endl
								<< "1. Deposit" << endl
								<< "2. Withdraw" << endl
								<< "3. Give Salary" << endl
								<< "4. View Balance" << endl
								<< "5. View Transactions" << endl
								<< "6. Add Employee" << endl
								<< "7. View Employee" << endl
								<< "8. Request Loan" << endl
								<< "9. Display All Information" << endl
								<< "10. Log Out" << endl;
							cin >> opt;

							if (opt == 1)
							{
								system("cls");
								companies[index].deposit();
								CompanyClient::writeInFile(companies);
								goto optionUsercom;
							}

							else if (opt == 2)
							{
								system("cls");
								companies[index].withdraw();
								CompanyClient::writeInFile(companies);
								goto optionUsercom;
							}

							else if (opt == 3)
							{
								system("cls");
								companies[index].giveSalary(users);
								CompanyClient::writeInFile(companies);
								goto optionUsercom;
							}

							else if (opt == 4)
							{
								system("cls");
								companies[index].viewBalance();
								goto optionUsercom;
							}

							else if (opt == 5)
							{
								system("cls");
								companies[index].viewTransactions();
								goto optionUsercom;
							}

							else if (opt == 6)
							{
								system("cls");
								companies[index].addEmployee(users);
								goto optionUsercom;
							}

							else if (opt == 7)
							{
								system("cls");
								companies[index].viewEmployees();
								goto optionUsercom;
							}
							else if (opt == 8)///request loan from admin 
							{
								system("cls");
								int amount;
								cout << "Enter Loan Amount: ", cin >> amount;
								admin.receiveLoanRequest(amount, companies[index].getLoginID(),companies);
								goto optionUsercom;
							}

							else if (opt == 9)
							{
								system("cls");
								companies[index].displayAllInfo();
								goto optionUsercom;
							}

							else
							{
								system("cls");
								goto LOGIN;
							}
						}
					}

					else if (opt == 8)
					{
						system("cls");
						users[index].displayAllInfo();
						goto Options;
					}

					else
					{
						system("cls");
						cout << "Thanks For Using Bank Services" << endl;
						goto LOGIN;
					}
				}

				else
				{
					system("cls");
					cout << RED << "Incorrect Password" << GREEN << endl;
					goto LOGIN;
				}

			}

			else if (opt == 2)  ///logging in company account
			{
				cout << MAGENTA << "\t\t\tC O M P A N Y   L O G I N   P A G E" << endl;
				cout << "\t\t\t======================================" << endl;
				cout << GREEN << "Enter Login ID: ", cin >> loginID;

				int index; bool found = false;
				for (int i = 0; i < companies.size(); i++)
				{
					if (companies[i].getLoginID() == loginID)
					{
						index = i;
						found = true;
						break;
					}
				}
				if (!found)
				{
					system("cls");
					cout <<RED<< "Company not found" <<GREEN<< endl;
					goto Main_Menu;
				}

				if (companies[index].isAccountApproved == false)
				{
					system("cls");
					cout <<RED<< "Company not Approved"<<GREEN << endl;
					goto LOGIN;
				}


				cout << "Enter Password: ", cin >> password;

				if (companies[index].getPassword() == password)
				{
					cout << "Login Successful!" << endl;
					if (companies[index].isBlocked)
					{
						cout <<RED<< "Account is Blocked!" << GREEN<<endl;
						goto Main_Menu;
					}
					if (companies[index].isFraudulent)
					{
						cout <<RED "Account is Fraudulent!" <<GREEN<< endl;
						goto Main_Menu;
					}
					if (companies[index].isAccountApproved == false)
					{
						cout <<RED<< "Account is not Approved!" <<GREEN<< endl;
						goto Main_Menu;
					}
				CompanyOptions:
					cout <<GREEN<< "Choose an Option: " << endl
						<< "1. Deposit" << endl
						<< "2. Withdraw" << endl
						<< "3. Give Salary" << endl
						<< "4. View Balance" << endl
						<< "5. View Transactions" << endl
						<< "6. Add Employee" << endl
						<< "7. View Employee" << endl
						<< "8. Request Loan" << endl
						<< "9. Display All Information" << endl
						<< "10. Log Out" << endl;
					cin >> opt;

					if (opt == 1)
					{
						companies[index].deposit();
						CompanyClient::writeInFile(companies);
						goto CompanyOptions;
					}

					else if (opt == 2)
					{
						companies[index].withdraw();
						CompanyClient::writeInFile(companies);
						goto CompanyOptions;
					}

					else if (opt == 3)
					{
						companies[index].giveSalary(users);
						CompanyClient::writeInFile(companies);
						goto CompanyOptions;
					}

					else if (opt == 4)
					{
						companies[index].viewBalance();
						goto CompanyOptions;
					}

					else if (opt == 5)
					{
						companies[index].viewTransactions();
						goto CompanyOptions;
					}

					else if (opt == 6)
					{
						companies[index].addEmployee(users);
						goto CompanyOptions;
					}
					else if (opt == 7)
					{
						system("cls");
						companies[index].viewEmployees();
						goto CompanyOptions;
					}


					else if (opt == 8)
					{
						int amount;
						cout << "Enter Loan Amount: ", cin >> amount;
						admin.receiveLoanRequest(amount, companies[index].getLoginID(),companies);
						goto CompanyOptions;
					}
					else if (opt == 9)
					{
						system("cls");
						companies[index].displayAllInfo();
						goto CompanyOptions;
					}

					else
					{
						goto LOGIN;
					}
				}
				else
				{
					system("cls");
					cout << RED << "Incorrect Password" << GREEN << endl;
					goto Main_Menu;
				}
			}

			else if (opt == 3)  ///logging in bank employee account
			{
				system("cls");
				cout << MAGENTA << "\t\t\tB A N K   E M P L O Y E E   L O G I N   P A G E" << endl;
				cout << "\t\t\t==============================================" << endl;


				cout << GREEN << "Enter Login ID: ", cin >> loginID;

			Bank_Pass:

				cout << "Enter Password: ", cin >> password;
				if (password == "3") goto Main_Menu;


				if (admin.getPassword() == password)
				{
					cout << "Login Successful!" << endl;
				BankEmployeeOptions:
					cout << "Choose an Option: " << endl
						<< "1. Block User" << endl
						<< "2. Unblock User" << endl
						<< "3. Approve Loan Request" << endl
						<< "4. View Loan Requests" << endl
						<< "5. View Transaction History of Specific Account" << endl
						<< "6. Set Fraudulent" << endl
						<< "7. Freeze Fraudulent" << endl
						<< "8. Display Unapproved Users" << endl
						<< "9. Display Unapproved Companies" << endl
						<< "10. View User Count" << endl
						<< "11 Disapprove Loan Requests" << endl
						<< "12. Log Out" << endl;
					cin >> opt;

					if (opt == 1)
					{
						system("cls");
						admin.blockUser(users, companies);
						goto BankEmployeeOptions;
					}

					else if (opt == 2)
					{
						system("cls");
						admin.unblockUser(users, companies);
						goto BankEmployeeOptions;
					}

					else if (opt == 3)
					{
						system("cls");
						admin.approveLoanRequet(companies);
						goto BankEmployeeOptions;
					}

					else if (opt == 4)
					{
						system("cls");
						admin.displayLoanRequests(companies);
						goto BankEmployeeOptions;
					}

					else if (opt == 5)
					{
						string nameid;
						cout << "Enter Name ID: ", cin >> nameid;
						admin.viewTransactionHistoryOfSpecificAccount(nameid);
						goto BankEmployeeOptions;
					}

					else if (opt == 6)
					{
						system("cls");
						string nameid;
						cout << "Enter Name ID: ", cin >> nameid;
						admin.setFraudulent(users, nameid);
						goto BankEmployeeOptions;
					}

					else if (opt == 7)
					{
						system("cls");
						admin.freezeFraudulent(users);
						goto BankEmployeeOptions;
					} 

					else if (opt == 8)
					{
						system("cls");
						admin.displayUnApprovedUsers(users);
						goto BankEmployeeOptions;
					}

					else if (opt == 9)
					{
						system("cls");
						admin.displayUnApprovedCompanies(companies);
						goto BankEmployeeOptions;
					}
					else if (opt == 10)
					{
						cout << "User Count: "<<YELLOW << counter <<GREEN<< endl;
						goto BankEmployeeOptions;
					}

					else if(opt==11)
					{
						system("cls");
						admin.disapproveLoanRequests(companies);
						goto BankEmployeeOptions;
					}

					else
					{
						system("cls");
						goto Main_Menu;
					}
				}
				else
				{
					system("cls");
					cout <<RED<< "Wrong Password" << endl << "Try Again, or Press 3 to Main Menu"<<GREEN << endl;
					goto Bank_Pass;
				}
			}

			else
			{
				system("cls");
				goto Main_Menu;
			}
		}

		else if (opt == 3)
		{
			system("cls");
			cout <<CYAN<< "			Thanks For Using Bank Services" << RESET << endl;
		}
	}

	return 0;
}
