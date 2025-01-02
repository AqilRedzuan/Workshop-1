#include <iostream>
#include <vector>
#include <cmath>
#include <cctype>
#include <ctime>
#include <string>
#include <cstring>
#include <conio.h>
#include <mysql.h>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <limits>
#include <sstream>
#include <algorithm>
#include <windows.h> // For console functions

using namespace std;

int qstate;
MYSQL* conn;
MYSQL_STMT stmt;
MYSQL_ROW row;
MYSQL_RES* res;
string items[20000000];
string adminId, Employee_ID, CustID, CarID, CatID, RentalID, IC, StaffID, StaffName,CustName;

void adminlogin();
void AdminMenu();
void StaffMenu();
void RegStaff();

void UpdateStaff();
void DeleteStaff();
void ListStaff();
void SearchStaff();

void CarMenu();
void AddCar();
void UpdateCar();
void DeleteCar();

void ListCar();
void SearchCar();
void CustomerMenu();//In Admin
void RegCust();

void UpdateCust();
void DeleteCust();
void ListCust();
void SearchCust();

void CarCat();//Car Category
void AddCat();//Car Category
void RentalMenu();
void RentalInAdmin();

void DeleteRental();//In Admin
void UpdateRentalA();

void UpdateCat();//Car Category
void DeleteCat();
void ListCat();

void StaffLogin();
void StaffPage();
void CustMenu();//In Staff
void CustReg();//In Staff

void CustUpdate();
void CustDelete();
void CustSearch();
void CustList();

void MenuCar();
void CarUpdate();
void CarDelete();
void CarList();
void UpdateRentalS();
void DisplayRent();

void CarSearch();
void ReturnCar();
void RentList();
void SearchRent();

void CustLogin();
void CustPage();
void RentalInCustomer();
void DisplayRental();

//just empty table
void table();

string getInputOfLength(int, string, string);
string getPassword();
string getValidEmail();
string getEmail();

//DB Connection
class db_response
{
public:
    static void ConnectionFunction()
    {
        conn = mysql_init(0);
        if (conn)
            cout << "Database Connected" << endl;
        else
            cout << "Failed To Connect!" << endl;

        conn = mysql_real_connect(conn, "localhost", "root", "", "car_rental", 3306, NULL, 0);
        if (conn)
            cout << "successfull connect!" << endl;
        else
            cout << "Failed To Connect!" << endl;
    }
};

int getTerminalWidth() {
    // Get terminal width dynamically
    int cols = 80; // Default to 80 columns if the width cannot be fetched
#ifdef _WIN32
    // For Windows, you can implement platform-specific terminal width logic if needed.
#else
    std::string command = "tput cols";
    FILE* stream = popen(command.c_str(), "r");
    if (stream) {
        char buffer[16];
        if (fgets(buffer, sizeof(buffer), stream)) {
            cols = std::stoi(buffer);
        }
        pclose(stream);
    }
#endif
    return cols;
}

//message
void printMessageCenter(const char* message)
{
    int len = 0;
    int pos = 0;

    //calculate how many space need to print
    len = (78 - strlen(message)) / 2;

    //print leading spaces
    for (pos = 0; pos < len; pos++)
    {
        std::cout << " ";
    }

    //print message
    std::cout << message;

    //print trailing spaces
    for (pos = 0; pos < len; pos++)
    {
        std::cout << " ";
    }
}

void welcomeHeader(const std::string& filePath) {
    // Get terminal width
    int terminalWidth = getTerminalWidth();

    // Open the file
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return;
    }

    // Read the file contents into a vector of strings
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();

    // Display the content centered
    for (const auto& line : lines) {
        int padding = (terminalWidth - line.length()) / 2; // Calculate left padding
        if (padding > 0) {
            std::cout << std::string(padding, ' ') << line << std::endl;
        }
        else {
            std::cout << line << std::endl; // If line is too long, print as is
        }
    }
}

// Function to print a horizontal line
void printHorizontalLine() {
    cout << setw(97) << "|--------------------------------------------------------------------|" << endl;
}

// Function to print an empty line within the bordered section
void printEmptyLine() {
    cout << setw(97) << "|                                                                    |" << endl;
}

//header for start
void header() {

    system("cls");
    ifstream inputFile("welcome.txt"); //displaying welcome ASCII image text on output screen fn1353
    if (!inputFile) {
        cout << "Cannot open input file.\n";
    }
    char str[1000];
    while (inputFile) {
        inputFile.getline(str, 1000);  // delim defaults to '\n' cp
        if (inputFile)
            cout << str << endl;
    }
    inputFile.close();
    Sleep(500);
    cout << "\nStarting the program please wait....." << endl;
    Sleep(1500);
    cout << "\nloading up the system....." << endl;
    Sleep(2500); //function which waits for (n) seconds
    system("CLS"); //cleares screen
}

int main() {

    int mainmenu;
    db_response::ConnectionFunction();
    header();
    std::string filePath = "header.txt"; // Path to the welcome file
    welcomeHeader(filePath);
    cout << setw(81) << "Welcome To Car Rental Management System\n\n\n";
    cout << setw(64) << "1. Admin\n\n\n";
    cout << setw(64) << "2. Staff\n\n\n";
    cout << setw(67) << "3. Customer\n\n\n";
    cout << setw(63) << "4. Exit\n\n\n";
    cout << setw(81) << "----------------------------------------------\n" << endl;
    cout << setw(73) << "Please Enter Your Choice : ";
    cin >> mainmenu;

    std::string answer;
    switch (mainmenu) {
    case 1:
        adminlogin();
        break;

    case 2:
        StaffLogin();
        break;

    case 3:
        CustLogin();
        break;

    case 4:
        exit(0);

        std::cout << "\n                                         Do you want to exit the program? (yes/no): ";
        std::cin >> answer;

        for (char& c : answer) {
            c = std::tolower(c);
        }

        system("cls");

        if (answer == "yes") {
            std::cout << "\n Exiting the program..." << std::endl;
            std::cin.get();
            exit(0); // Terminate the program with exit code 0 (indicating a successful exit)
        }
        else {
            main();
        }
    default:
        system("cls");
        system("pause");
        //other IDs' will be an error
        cout << "Please Enter a Valid Input!\n\n";
        break;
    }
    return 0;
}

//Login Page Admin//
void adminlogin()
{
    system("cls");
    //welcome header//
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    string adminpass;
    string adminusername;

    cout << "\n\n";
    cout << setw(64) << "Username: ";
    cin >> adminusername;

    // Validate username input
    if (adminusername.empty())
    {
        cout << "\n\n\n" << setw(83) << "Invalid username! Please try again." << endl;
        adminlogin(); // Repeat the function to allow re-entering the username
        return;
    }

    cout << "\n\n";
    cout << setw(64) << "Password: ";
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == 8) {
            if (!adminpass.empty()) {
                adminpass.pop_back();
                cout << "\b \b";
            }
        }
        else {
            adminpass.push_back(ch);
            cout << '*';
        }
    }


    // Validate password input
    if (adminpass.empty())
    {
        cout << endl << "Invalid password! Please try again." << endl;
        adminlogin(); // Repeat the function to allow re-entering the password
        return;
    }

    string checkUser_query = "SELECT AdminID FROM admin WHERE Username = '" + adminusername + "' AND Password = '" + adminpass + "'";
    const char* cu = checkUser_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count == 1)
        {
            while (row = mysql_fetch_row(res))
                adminId = row[0];
            AdminMenu();
        }
        else
        {
            char c;
            cout << "\n\n\n" << setw(85) << "Invalid username or password. Want to try again? (y/n): ";
            cin >> c;
            if (c == 'y' || c == 'Y')
                adminlogin();
            else
                main();
        }
    }
    else
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
}//end

void AdminMenu()
{
    std::string answer;
    int menu;
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);
    cout << "\n\n\n";
    cout << setw(63) << "1. Staff\n\n";
    cout << setw(66) << "2. Category\n\n";
    cout << setw(61) << "3. Car\n\n";
    cout << setw(66) << "4. Customer\n\n";
    cout << setw(66) << "5. Rent Car\n\n";
    cout << setw(64) << "6. Report\n\n";
    cout << setw(64) << "7. Logout\n\n";
    cout << setw(83) << "-------------------------------------------\n" << endl;
    cout << setw(75) << "Please Enter Your Choice : ";
    cin >> menu;

    switch (menu)
    {
    case 1:

        StaffMenu();
        break;

    case 2:

        CarCat();
        break;

    case 3:

        CarMenu();
        break;

    case 4:

        CustomerMenu();
        break;

    case 5:

        RentalMenu();
        break;

    case 6:
        // ReportMenu();
        break;

    case 7:

        system("cls");
        std::cout << "\n                                   Do you want to logout the system? (yes/no): ";
        std::cin >> answer;

        for (char& c : answer) {
            c = std::tolower(c);
        }

        system("cls");

        if (answer == "yes" || answer == "y") {
            ifstream inputFile("thank.txt"); //displaying thank you ASCII image text on output screen fn1353
            if (!inputFile) {
                cout << "Cannot open input file.\n";
            }
            char str[1000];
            while (inputFile) {
                inputFile.getline(str, 1000);
                if (inputFile)
                    cout << str << endl;
            }
            inputFile.close();
            std::cout << "\n Logged out successfully\n" << std::endl;
            std::cin.get();
            exit(0); // Terminate the program with exit code 0 (indicating a successful exit)
        }
    }

}//end admin menu

//Manage Staff In Admin
void StaffMenu() {
    int choose;
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);
    cout << "\n\n";
    cout << setw(200) << " " << endl;
    cout << setw(72) << "1. Register Staff\n\n"; //67
    cout << setw(70) << "2. Update Staff\n\n";
    cout << setw(70) << "3. Delete Staff\n\n";
    cout << setw(71) << "4. Display Staff\n\n";
    cout << setw(70) << "5. Search Staff\n\n";
    cout << setw(63) << "6. Back\n\n\n";
    cout << setw(84) << "-------------------------------------------\n" << endl;
    cout << setw(76) << "Please Enter Your Choice : ";
    cin >> choose;

    if (choose == 1)
    {
        RegStaff();
    }
    else if (choose == 2)
    {
        UpdateStaff();
    }
    else if (choose == 3)
    {
        DeleteStaff();
    }
    else if (choose == 4)
    {
        ListStaff();
    }
    else if (choose == 5)
    {
        SearchStaff();
    }
    else if (choose == 6)
    {
        AdminMenu();
    }
}

// Function to validate Name
bool isValidName(const string& name) {
    for (char c : name) {
        if (isdigit(c)) { // Check if any character in the name is a digit
            return false;
        }
    }
    return true;
}

void RegStaff() {
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    char option;
    string Username = "";
    string Password = "";
    string Name = "";
    string Email = "";
    string EmployeeID = "";
    string Phone = "";

    int count = 0;
    int specials = 0;
    bool valid_password = true;

    cout << "\n";
    printHorizontalLine();
    printEmptyLine();
    cout << setw(97) << "|                              PASSWORD                              |" << endl;
    printEmptyLine();
    cout << setw(97) << "|       - A password must have at least eight characters.            |" << endl;
    printEmptyLine();
    cout << setw(97) << "|       - A password must consist of only letters and digits.        |" << endl;
    printEmptyLine();
    cout << setw(97) << "|       - A password must contain at least two digits.               |" << endl;
    printEmptyLine();
    printHorizontalLine();
    cout << "\n";

    cout << setw(64) << "Enter Username: ";
    cin.ignore(1, '\n');
    getline(cin, Username);
    Password = getPassword();

    string update_query = "update staff set status password ='" + Password + "'";
    const char* q = update_query.c_str();
    qstate = mysql_query(conn, q);

    cout << "\n" << setw(67) << "Enter Employee Id: ";
    cin.ignore();
    getline(cin, EmployeeID);

    // Validate Name
    do {
        cout << "\n" << setw(60) << "Enter Name: ";
        getline(cin, Name);
        if (!isValidName(Name)) {
            cout << "\033[31m"; // Set text color to red
            cout << "\n" << setw(88) << "Invalid Name. Name cannot contain numbers. Please try again.\n";
            cout << "\033[0m"; // Reset text color to default
        }
    } while (!isValidName(Name)); // Keep prompting until a valid name is entered

    Email = getValidEmail();

    Phone = getInputOfLength(10, "\n\t\t\t\t\tPhone number must be 10 digits long.", "\n\t\t\t\t\t\tEnter Phone : ");
    string checkUser_query = "select * from staff where username = '" + Username + "'";
    const char* cu = checkUser_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate) {
        res = mysql_store_result(conn);
        if (res->row_count == 1) {
            cout << "Username already exists. Press Enter to Try Again...";
            _getch();
            RegStaff();
        }
        else {
            string insertStaff_query = "insert into staff (username, password,employee_id,name,email,phone) values ('" + Username + "', '" + Password + "','" + EmployeeID + "','" + Name + "','" + Email + "','" + Phone + "')";
            const char* q = insertStaff_query.c_str();
            qstate = mysql_query(conn, q);

            if (!qstate) {
                cout << "\n\n\n" << setw(75) << "You have been registered.\n\n";
                cout << setw(80) << "Do you want to add other staff? (y/n): ";
                cin >> option;

                if (option == 'y' || option == 'Y')
                    RegStaff();
                else
                    ListStaff();
                _getch();
            }
            else {
                cout << "Query Execution Problem!" << mysql_error(conn) << endl;
            }
        }
    }
    else {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
}

void UpdateStaff() {

    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    string Employee_ID;
    string dataStaff;
    int choose;
    char option;

    qstate = mysql_query(conn, "SELECT * FROM staff");

    if (!qstate)
    {
        res = mysql_store_result(conn);

        // Display staff data in a table
        cout << "\n";
        cout << setw(112) << "---------------------------------------------------------------------------------------------------\n";
        cout << setw(12) << "";
        printf("| %-13s | %-20s | %-30s | %-23s |\n", "Employee ID", "Name", "Email", "Phone");
        cout << setw(112) << "---------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {
            cout << setw(12) << "";
            printf("| %-13s | %-20s | %-30s | %-23s |\n", row[3], row[4], row[5], row[6]);
            cout << setw(112) << "---------------------------------------------------------------------------------------------------\n";
        }

        while (true) // Retry loop for invalid Employee_ID
        {
            cout << endl << endl << setw(70) << " Choose Employee ID : ";
            cin >> Employee_ID;

            // Query to check if Employee_ID exists
            string findbyid_query = "SELECT * FROM staff WHERE employee_id='" + Employee_ID + "'";
            const char* qi = findbyid_query.c_str();
            qstate = mysql_query(conn, qi);

            if (!qstate)
            {
                res = mysql_store_result(conn);
                if (mysql_num_rows(res) > 0) // If ID is valid
                {
                    // Display selected employee details
                    while ((row = mysql_fetch_row(res)))
                    {
                        system("cls");
                        welcomeHeader(filePath);
                        cout << "\n";
                        cout << setw(82) << "---------------------------------------" << endl;
                        cout << setw(57) << "\n\t\t\t\t\t\tEmployee ID: " << row[3] << endl;
                        cout << setw(57) << "\n\t\t\t\t\t\tName: " << row[4] << endl;
                        cout << setw(57) << "\n\t\t\t\t\t\tStaff Email: " << row[5] << endl;
                        cout << setw(57) << "\n\t\t\t\t\t\tStaff Phone: " << row[6] << endl;
                        cout << setw(82) << "---------------------------------------" << endl << endl;
                    }
                    break; // Exit retry loop
                }
                else
                {
                    cout << endl << setw(79) << "No Employee found with the given ID!" << endl;
                    cout << endl << setw(79) << "Do you want to try again? (y/n): ";
                    cin >> option;
                    if (option == 'y' || option == 'Y')
                        UpdateStaff();
                    else
                        StaffMenu();
                }
            }
            else
            {
                cout << "Query Execution Problem! " << mysql_error(conn) << endl;
                return;
            }
        }

        // Update options
        cout << setw(71) << "1. Staff Email\n\n";
        cout << setw(71) << "2. Staff Phone\n\n";
        cout << setw(64) << "3. Back\n\n";
        cout << setw(86) << "-------------------------------------------\n" << endl;
        cout << setw(78) << "Please Enter Your Choice : ";
        cin >> choose;
        cin.ignore(1, '\n'); // Clear newline from input buffer

        if (choose == 1)
        {
            cout << endl << endl << setw(70) << "1. New Staff Email: ";
            getline(cin, dataStaff);
            string update_query = "UPDATE staff SET email='" + dataStaff + "' WHERE employee_id='" + Employee_ID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (choose == 2)
        {
            cout << endl << endl << setw(70) << "2. New Staff Phone: ";
            getline(cin, dataStaff);
            string update_query = "UPDATE staff SET phone='" + dataStaff + "' WHERE employee_id='" + Employee_ID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (choose == 3)
        {
            StaffMenu();
        }

        cout << endl << setw(86) << "Do you want to update other Employee? (y/n): ";
        cin >> option;

        if (option == 'y' || option == 'Y')
            UpdateStaff();
        else
            ListStaff();
    }
    else
    {
        cout << "Query Execution Problem! " << mysql_error(conn) << endl;
        return;
    }

}

void DeleteStaff() {
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    char choose;
    string id;
    if (conn == nullptr) {
        cout << "Database connection failed!" << endl;
        return;
    }

    qstate = mysql_query(conn, "SELECT * FROM staff");
    if (!qstate) {
        res = mysql_store_result(conn);
        if (res == nullptr) {
            cout << "Failed to fetch staff data: " << mysql_error(conn) << endl;
            return;
        }

        cout << "\n";
        cout << setw(12) << "" << "-----------------------------------------------------------------------------------------\n";
        cout << setw(12) << "";
        printf("| %-13s | %-20s | %-30s | %-13s |\n", "Employee ID", "Name", "Email", "Phone");
        cout << setw(12) << "" << "-----------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res))) {
            cout << setw(12) << "";
            printf("| %-13s | %-20s | %-30s | %-13s |\n", row[3], row[4], row[5], row[6]);
            cout << setw(12) << "" << "-----------------------------------------------------------------------------------------\n";
        }

        mysql_free_result(res); // Free results after use

        cout << endl << setw(72) << "Enter Employee ID to delete: ";
        cin >> id;

        qstate = mysql_query(conn, ("SELECT employee_id FROM staff WHERE employee_id='" + id + "'").c_str());
        res = mysql_store_result(conn);
        if (!qstate && (row = mysql_fetch_row(res))) {
            mysql_free_result(res);
            string query = "DELETE FROM staff WHERE employee_id='" + id + "'";
            qstate = mysql_query(conn, query.c_str());
            if (!qstate) {
                cout << endl << setw(77) << "Successfully deleted from staff record" << endl;
            }
            else {
                cout << "Failed to Delete: " << mysql_error(conn) << endl;
            }
        }
        else {
            mysql_free_result(res);
            cout << endl << setw(92) << "Please Enter a valid Number. Do You Want To Try Again? (y/n): ";
            cin >> choose;
            if (choose == 'y' || choose == 'Y') {
                DeleteStaff();
            }
            else {
                StaffMenu();
            }
            return;
        }

        cout << endl << setw(83) << "Do you want to Delete another Staff? (y/n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y') {
            DeleteStaff();
        }
        else {
            ListStaff();
        }
    }
}

void ListStaff() {
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);
    qstate = mysql_query(conn, "SELECT * FROM staff");
    if (!qstate)
    {
        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(12) << "" << "-----------------------------------------------------------------------------------------\n";
        // Data headers
        cout << setw(12) << "";
        printf("| %-13s | %-20s | %-30s | %-13s |\n", "Employee ID", "Name", "Email", "Phone");
        cout << setw(12) << "" << "-----------------------------------------------------------------------------------------\n";

        // Fetching rows from MySQL
        while ((row = mysql_fetch_row(res))) {
            cout << setw(12) << "";
            printf("| %-13s | %-20s | %-30s | %-13s |\n", row[3], row[4], row[5], row[6]);
            cout << setw(12) << "" << "-----------------------------------------------------------------------------------------\n";
        }


    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
    cout << "\n" << setw(70) << "Press Enter To Back....";
    _getch();
    StaffMenu();
}

void SearchStaff()
{
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    char choose;
    string name;

    cin.ignore(1, '\n');
    cout << endl << setw(70) << "Search Staff by name: ";
    getline(cin, name);

    string search_query = "select * from staff where name like '%" + name + "%'";
    const char* q = search_query.c_str();
    qstate = mysql_query(conn, q);

    if (!qstate)
    {
        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(12) << "" << "-----------------------------------------------------------------------------------------\n";
        // Data headers
        cout << setw(12) << "";
        printf("| %-13s | %-20s | %-30s | %-13s |\n", "Employee ID", "Name", "Email", "Phone");
        cout << setw(12) << "" << "-----------------------------------------------------------------------------------------\n";

        // Fetching rows from MySQL
        while ((row = mysql_fetch_row(res))) {
            cout << setw(12) << "";
            printf("| %-13s | %-20s | %-30s | %-13s |\n", row[3], row[4], row[5], row[6]);
            cout << setw(12) << "" << "-----------------------------------------------------------------------------------------\n";
        }
        cout << endl << setw(83) << "Do you want to search other staff? (y/n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
            SearchStaff();
        else if (choose == 'n' || choose == 'N')
            StaffMenu();
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
}

string getInputOfLength(int requiredLength, string warning, string prompt) {
    string userInput;

    do {
        cout << setw(70) << prompt;
        cin >> userInput;

        if (userInput.length() == requiredLength) {
            break;
        }
        else {
            cout << setw(70) << warning << endl;
        }
    } while (true);

    return userInput;
}

bool checkForAlphabets(char c) {
    return isalpha(c);
}

bool checkForDigits(char c) {
    return isdigit(c);
}

string getPassword() {
    string userInput;

    do {
        cout << "\n" << setw(64) << "Enter password: ";
        cin >> userInput;

        int sz = userInput.length();

        if (sz >= 8 && count_if(userInput.begin(), userInput.end(), checkForDigits) >= 2) {
            return userInput;
        }
        cout << "\n";
        cout << setw(32) << "" << "Invalid password. Must be 8 in length with 2 digits" << endl;
    } while (true);
}

// Function to check if a string contains any digits
bool isNameValid(const string& name) {
    for (char c : name) {
        if (isdigit(c)) {
            return false; // Invalid if a digit is found
        }
    }
    return true;
}

bool isValidEmail(const string& email) {
    size_t atPosition = email.find('@');
    return (atPosition != string::npos);
}

bool isDateValid(const tm& rentalDate, const tm& returnDate) {
    time_t rentalTimeT = mktime(const_cast<tm*>(&rentalDate));
    time_t returnTimeT = mktime(const_cast<tm*>(&returnDate));
    return rentalTimeT < returnTimeT; // Return true if rentalDate is earlier than returnDate
}

bool stringToTm(const std::string& dateStr, std::tm& tmDate) {
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tmDate, "%Y-%m-%d");
    return !ss.fail();
}

// Function to calculate the number of days between two dates
int calculateDays(const std::string& startDateStr, const std::string& endDateStr) {
    std::tm startDate = {}, endDate = {};

    // Convert strings to std::tm
    if (!stringToTm(startDateStr, startDate) || !stringToTm(endDateStr, endDate)) {
        std::cerr << "Error: Invalid date format. Please use YYYY-MM-DD.\n";
        return -1; // Return -1 for invalid input
    }

    // Convert std::tm to time_t for calculation
    std::time_t startTime = std::mktime(&startDate);
    std::time_t endTime = std::mktime(&endDate);

    // Ensure conversion was successful
    if (startTime == -1 || endTime == -1) {
        std::cerr << "Error: Unable to process the dates.\n";
        return -1;
    }

    // Calculate the difference in seconds and convert to days
    int totalDays = static_cast<int>(std::difftime(endTime, startTime) / (60 * 60 * 24));
    return totalDays;
}

//ForUpdateCust
string getEmail() {
    string Email;

    do {
        cout << "\n";
        cout << setw(63) << "2. Email : ";
        cin >> Email;

        if (Email.find('@') != string::npos) {
            break;  // Valid email, exit the loop
        }
        else {
            cout << "\n";
            cout << setw(39) << "" << "Invalid email. It must contain the '@' symbol." << endl;

        }
    } while (true);

    return Email;
}

string getValidEmail() {
    string userEmail;

    do {
        cout << setw(64) << "\n\t\t\t\t\t\tEnter your email : ";
        cin >> userEmail;

        if (userEmail.find('@') != string::npos) {
            break;  // Valid email, exit the loop
        }
        else {
            cout << "\n";
            cout << setw(32) << "" << "Invalid email. It must contain the '@' symbol." << endl;

        }
    } while (true);

    return userEmail;
}

void CarCat() {

    int menu;
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);
    cout << "\n\n\n";
    cout << setw(70) << "1. Add Category\n\n"; 
    cout << setw(73) << "2. Update Category\n\n";
    cout << setw(73) << "3. Delete Category\n\n";
    cout << setw(62) << "4. Back\n\n";
    cout << setw(84) << "-------------------------------------------\n" << endl;
    cout << setw(77) << "Please Enter Your Choice : ";
    cin >> menu;

    switch (menu)
    {
    case 1:

        AddCat();
        break;

    case 2:

        UpdateCat();
        break;

    case 3:

        DeleteCat();
        break;


    case 4:

        system("cls");
        AdminMenu();
        break;
    }

}

void AddCat() {

    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    char option;
    string CarBrand = "";
    string Model = "";

    cout << "\n\n" << setw(68) << "Enter Car Brand: ";
    cin.ignore(1, '\n');
    getline(cin, CarBrand);
    cout << "\n\n" << setw(67) << "Enter Car Name: ";
    getline(cin, Model);

    if (!qstate)
    {
        string insertCar_query = "INSERT into carcat (CarBrand,Model) values ('" + CarBrand + "', '" + Model + "') ";
        const char* ca = insertCar_query.c_str();
        qstate = mysql_query(conn, ca);

        if (!qstate)
        {
            cout << "\n\n" << setw(75) << "Succesfully Insert!!!!\n\n";
            cout << "\n\n";
            cout << setw(85) << "Do you want to add other new Car Category? (y/n): ";
            cin >> option;

            if (option == 'y' || option == 'Y')
            {
                AddCat();
            }
            else
            {

                CarCat();
                _getch();
            }

        }
        else
        {
            cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
        }

    }
    else
    {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }

}

void UpdateCat() {

    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    string CarCat_ID;
    string DataBrand;
    int choose;
    char option;

    qstate = mysql_query(conn, "SELECT * FROM carcat");

    if (!qstate)
    {
        res = mysql_store_result(conn);

        // Display staff data in a table
        cout << "\n\n";
        cout << setw(40) << "" << "---------------------------------------------\n";
        cout << setw(40) << "";
        printf("| %-5s | %-15s | %-15s |\n", "ID", "Car Brand", "Model");
        cout << setw(86) << "---------------------------------------------\n";

        while ((row = mysql_fetch_row(res))) {
            cout << setw(40) << "";
            printf("| %-5s | %-15s | %-15s |\n", row[0], row[1], row[2]);
            cout << setw(86) << "---------------------------------------------\n";
        }


        while (true) // Retry loop for invalid Employee_ID
        {
            cout << endl << endl << setw(68) << " Choose ID : ";
            cin >> CarCat_ID;

            // Query to check if Employee_ID exists
            string findbyid_query = "SELECT * FROM carcat WHERE CatID='" + CarCat_ID + "'";
            const char* qi = findbyid_query.c_str();
            qstate = mysql_query(conn, qi);

            if (!qstate)
            {
                res = mysql_store_result(conn);
                if (mysql_num_rows(res) > 0) // If ID is valid
                {
                    // Display selected employee details
                    while ((row = mysql_fetch_row(res)))
                    {
                        system("cls");
                        welcomeHeader(filePath);
                        cout << "\n\n";
                        cout << setw(80) << "---------------------------------------" << endl;
                        cout << "\n";
                        cout << setw(69) << "Car Category ID: " << row[0] << endl;
                        cout << "\n";
                        cout << setw(63) << "Car Brand: " << row[1] << endl;
                        cout << "\n";
                        cout << setw(59) << "Model: " << row[2] << endl;
                        cout << "\n";
                        cout << setw(80) << "---------------------------------------" << endl << endl;
                    }
                    break; // Exit retry loop
                }
                else
                {
                    cout << endl << setw(80) << "No Category found with the given ID!" << endl;
                    cout << endl << setw(80) << "Do you want to try again? (y/n): ";
                    cin >> option;
                    if (option == 'y' || option == 'Y')
                        UpdateCat();
                    else
                        CarCat();
                }
            }
            else
            {
                cout << "Query Execution Problem! " << mysql_error(conn) << endl;
                return;
            }
        }

        // Update options
        cout << setw(66) << "1. Model\n\n";
        cout << setw(65) << "2. Back\n\n";
        cout << setw(81) << "---------------------------------------\n" << endl;
        cout << setw(75) << "Please Enter Your Choice : ";
        cin >> choose;
        cin.ignore(1, '\n'); // Clear newline from input buffer

        if (choose == 1)
        {
            cout << endl << endl << setw(66) << "1. Model: ";
            getline(cin, DataBrand);
            string update_query = "UPDATE carcat SET Model='" + DataBrand + "' WHERE CatID='" + CarCat_ID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        
        else if (choose == 2)
        {
            CarCat();
        }

        cout << endl << setw(86) << "Do you want to update other Category? (y/n): ";
        cin >> option;

        if (option == 'y' || option == 'Y')
            UpdateCat();
        else
            ListCat();
            _getch();
    }
    else
    {
        cout << "Query Execution Problem! " << mysql_error(conn) << endl;
        return;
    }
}

void DeleteCat() {

    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    char choose;
    string id;
    qstate = mysql_query(conn, "select * from carcat");
    if (!qstate)
    {
        res = mysql_store_result(conn);

        cout << "\n\n";
        cout << setw(40) << "" << "---------------------------------------------\n";
        cout << setw(40) << "";
        printf("| %-5s | %-15s | %-15s |\n", "ID", "Car Brand", "Model");
        cout << setw(86) << "---------------------------------------------\n";

        while ((row = mysql_fetch_row(res))) {
            cout << setw(40) << "";
            printf("| %-5s | %-15s | %-15s |\n", row[0], row[1], row[2]);
            cout << setw(86) << "---------------------------------------------\n";
        }

        cout << endl << endl << setw(70) << "Enter ID to delete: ";
        cin >> id;
        bool found = false;

        // Check if the ID exists in carcat
        string query = "select CatID from carcat";
        const char* qd = query.c_str();
        qstate = mysql_query(conn, qd);
        res = mysql_store_result(conn);

        if (!qstate)
        {
            while ((row = mysql_fetch_row(res)))
            {
                if (row[0] == id)
                {
                    found = true;
                    break;
                }
            }
        }

        if (found)
        {
            // Check if the CatID is referenced in the car table
            string checkQuery = "select count(*) from car where CatID='" + id + "'";
            const char* checkQd = checkQuery.c_str();
            qstate = mysql_query(conn, checkQd);
            res = mysql_store_result(conn);

            if (!qstate && (row = mysql_fetch_row(res)) && atoi(row[0]) > 0)
            {
                cout << endl << setw(95) << "Cannot delete. This category is linked to existing cars." << endl;
                cout << setw(92) << "Do you want to try again? (y/n): ";
                cin >> choose;
                if (choose == 'y' || choose == 'Y')
                {
                    DeleteCat();
                }
                else
                {
                    CarCat();
                }
            }
            else
            {
                // Proceed to delete the CatID
                string deleteQuery = "delete from carcat where CatID='" + id + "'";
                const char* deleteQd = deleteQuery.c_str();
                qstate = mysql_query(conn, deleteQd);

                if (!qstate)
                {
                    cout << endl << setw(80) << "Successfully deleted from category record" << endl;
                }
                else
                {
                    cout << "Failed to Delete: " << mysql_error(conn) << endl;
                }
            }
        }
        else
        {
            cout << endl << setw(92) << "Please Enter a valid Number. Do You Want To Try Again? (y/n): ";
            cin >> choose;
            if (choose == 'y' || choose == 'Y')
            {
                DeleteCat();
            }
            else if (choose == 'n' || choose == 'N')
            {
                CarCat();
            }
        }

        cout << endl << setw(84) << "         Do you want to Delete another Category? (y/n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
        {
            DeleteCat();
        }
        else
        {
            ListCat();
        }
    }
}

void ListCat() {

    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);
    qstate = mysql_query(conn, "SELECT * FROM carcat");
    if (!qstate)
    {
        res = mysql_store_result(conn);

        cout << "\n\n";
        cout << setw(37) << "" << "---------------------------------------------\n";
        cout << setw(37) << "";
        printf("| %-5s | %-15s | %-15s |\n", "ID", "Car Brand", "Model");
        cout << setw(83) << "---------------------------------------------\n";

        while ((row = mysql_fetch_row(res))) {
            cout << setw(37) << "";
            printf("| %-5s | %-15s | %-15s |\n", row[0], row[1], row[2]);
            cout << setw(83) << "---------------------------------------------\n";
        }


    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
    cout << "\n" << setw(70) << "Press Enter To Back....";
    _getch();
    CarCat();
}


void CarMenu() {
    int choose;
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    cout << setw(300) << " " << endl;
    cout << setw(65) << "1. Add Car\n\n"; //7
    cout << setw(68) << "2. Update Car\n\n";
    cout << setw(68) << "3. Delete Car\n\n";
    cout << setw(69) << "4. Display Car\n\n";
    cout << setw(68) << "5. Search Car\n\n";
    cout << setw(63) << "6. Back\n\n\n";
    cout << setw(86) << "-------------------------------------------\n" << endl;
    cout << setw(77) << "Please Enter Your Choice : ";
    cin >> choose;

    if (choose == 1)
    {
        AddCar();
    }
    else if (choose == 2)
    {
        UpdateCar();
    }
    else if (choose == 3)
    {
        DeleteCar();
    }
    else if (choose == 4)
    {
        ListCar();
    }
    else if (choose == 5)
    {
        SearchCar();
    }
    else if (choose == 6)
    {
        AdminMenu();
    }
}

//Add Car
void AddCar() {
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    char option;
    string Plate = "";
    string Price = "";
    string Availability_status = "Available";

    qstate = mysql_query(conn, "SELECT * FROM carcat");

    std::cout << "\n\n\n";
    table();

    bool validCatID = false;
    char choice;

    do {
        cout << "\n\n" << setw(68) << "Car Category (CatID): ";
        cin.ignore(1, '\n');  // Clear buffer
        getline(cin, CatID);

        // Check if CatID exists in carcat
        string validateQuery = "SELECT 1 FROM carcat WHERE CatID = '" + CatID + "' LIMIT 1";
        const char* validateQ = validateQuery.c_str();
        qstate = mysql_query(conn, validateQ);

        if (!qstate) {
            res = mysql_store_result(conn);
            if (res && (row = mysql_fetch_row(res))) {
                validCatID = true; // CatID is valid
            }
            else {
                cout << "\n" << setw(82) << "Invalid CatID! Please enter a valid category ID.\n";

                // Prompt the user for yes/no to retry
                cout << "\n" << setw(74) << "Do you want to try again? (y/n): ";
                cin >> choice;

                if (choice == 'y' || choice == 'Y') {
                    cout << "\n" << setw(85) << "Exiting category selection.\n";
                    AddCar();  // Go to AddCar if yes
                    return;    // Exit the function after AddCar
                }
                else {
                    cout << "\n" << setw(85) << "Returning to Car Menu...\n";
                    CarMenu();  // Go to CarMenu if no
                    return;    // Exit the function after returning to CarMenu
                }
            }
        }
        else {
            cout << "Error validating CatID: " << mysql_error(conn) << endl;
        }

    } while (!validCatID);

    // Get remaining inputs
    cout << "\n" << setw(68) << "Car Plate Number: ";
    getline(cin, Plate);
    cout << "\n" << setw(68) << "Car Price: ";
    getline(cin, Price);

    if (!qstate) {
        string insertCar_query = "INSERT INTO car (Plate, Price, Status, CatID) VALUES ('" + Plate + "', '" + Price + "', '" + Availability_status + "', '" + CatID + "')";
        const char* ca = insertCar_query.c_str();
        qstate = mysql_query(conn, ca);

        if (!qstate) {
            cout << "\n\n" << setw(75) << "Successfully Inserted!\n\n";
            cout << setw(84) << "Do you want to add another new car? (y/n): ";
            cin >> option;

            if (option == 'y' || option == 'Y') {
                AddCar();
            }
            else {
                ListCar();
                _getch();
            }
        }
        else {
            cout << "Query Execution Problem! " << mysql_errno(conn) << endl;
        }
    }
    else {
        cout << "Query Execution Problem! " << mysql_errno(conn) << endl;
    }
}

//Update Car
void UpdateCar() {
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    string c_data;
    int choose;
    char option;

    qstate = mysql_query(conn, "SELECT * FROM car");

    if (!qstate)
    {

        res = mysql_store_result(conn);

            cout << "\n";
            cout << setw(30) << ""; cout << "-------------------------------------------------------\n";
            //data show in box
            cout << setw(30) << "";
            printf("| %-3s | %-13s | %-15s | %-11s | \n", "ID", "Plate", "Price", "Status");
            cout << setw(30) << ""; cout << "-------------------------------------------------------\n";

            while ((row = mysql_fetch_row(res)))
            {


                cout << setw(30) << "";
                printf("| %-3s | %-13s | %-15s | %-11s | \n", row[0], row[1], row[2], row[3]);
                cout << setw(30) << ""; cout << "-------------------------------------------------------\n";
        }
        cout << endl << endl << setw(66) << "  Choose ID : ";
        cin >> CarID;

        system("cls");
        std::string filePath = "admin.txt"; // Path to the admin file
        welcomeHeader(filePath);

        string findbyid_query = "SELECT * FROM car where carID='" + CarID + "'";
        const char* qi = findbyid_query.c_str();
        qstate = mysql_query(conn, qi);

        if (!qstate)
        {
            res = mysql_store_result(conn);
            cout << endl;
            while ((row = mysql_fetch_row(res)))
            {
                system("cls");
                std::string filePath = "admin.txt"; // Path to the admin file
                welcomeHeader(filePath);

                cout << "\n\n";
                cout << setw(82) << "---------------------------------------" << endl;
                cout << "\n";
                cout << setw(62) << "Plate: " << row[1] << endl;
                cout << "\n";
                cout << setw(62) << "Price: " << row[2] << endl;
                cout << "\n";
                cout << setw(63) << "Status: " << row[3] << endl;
                cout << "\n";
                cout << setw(82) << "---------------------------------------" << endl << endl;
            }
        }
        else
        {
            cout << "Query Execution Problem!" << mysql_error(conn) << endl;
            system("pause");
        }

        cout << setw(65) << "1. Plate\n\n";
        cout << setw(65) << "2. Price\n\n";
        cout << setw(66) << "3. Status\n\n";
        cout << setw(64) << "4. Back\n\n";
        cout << setw(86) << "---------------------------------------   \n" << endl;
        cout << setw(77) << "Please Enter Your Choice : ";
        cin >> choose;

        cin.ignore(1, '\n');
        if (choose == 1)
        {
            cout << endl << endl << setw(66) << "1. Plate: ";
            getline(cin, c_data);
            string update_query = "update car set Plate='" + c_data + "' where CarID='" + CarID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (choose == 2)
        {
            cout << endl << endl << setw(65) << "2. Price: ";
            getline(cin, c_data);
            string update_query = "update car set Price='" + c_data + "' where CarID='" + CarID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else  if (choose == 3)
        {
            int choice = 0;

            while (choice != 1 && choice != 2) {
                cout << endl << endl << setw(72) << "Update Car Status" << endl;
                cout << endl << endl << setw(67) << "1. Available" << endl;
                cout << endl << endl << setw(64) << "2. Rented" << endl;
                cout << endl << endl << setw(78) << "Enter your choice (1 or 2): ";
                cin >> choice;


                if (choice != 1 && choice != 2) {
                    cout << "Invalid choice. Please enter 1 or 2." << endl;
                }
            }

            string newStatus = (choice == 1) ? "Available" : "Rented";
            string updateCar_query = "UPDATE car SET Status = '" + newStatus + "' WHERE CarID='" + CarID + "'";
            const char* ca = updateCar_query.c_str();
            qstate = mysql_query(conn, ca);
        }
        else  if (choose == 4)
        {
            CarMenu();
        }

        cout << endl << setw(85) << "Do you want to update other Car? (y/n): ";
        cin >> option;

        if (option == 'y' || option == 'Y')
            UpdateCar();
        else
            ListCar();
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
}

//Delete Car
void DeleteCar() {
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    char choose;
    string id;

    qstate = mysql_query(conn, "SELECT car.*, carcat.CarBrand, carcat.Model FROM car JOIN carcat ON car.CatID = carcat.CatID");
    if (!qstate)
    {

        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(106) << "===========================================================================================\n";
        // Display headers with better alignment and spacing
        cout << setw(14) << "";
        printf("| %-5s | %-12s | %-12s | %-13s | %-8s | %-10s | %-9s |\n", "ID", "Car Brand", "Model", "Plate", "Price", "Status", "Cat ID");
        cout << setw(106) << "-------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res))) {
            cout << setw(14) << "";
            printf("| %-5s | %-12s | %-12s | %-13s | %-8s | %-10s | %-9s |\n", row[0], row[5], row[6], row[1], row[2], row[3], row[4]);
            cout << setw(106) << "-------------------------------------------------------------------------------------------\n";
        }


        cout << endl << endl << setw(70) << "Enter ID to delete: ";
        cin >> id;

        bool found = false;

        string query = "select CarID from car";
        const char* qd = query.c_str();
        qstate = mysql_query(conn, qd);
        res = mysql_store_result(conn);

        if (!qstate)
        {
            while ((row = mysql_fetch_row(res)))
            {
                if (row[0] == id)
                {
                    found = true;
                    break;
                }
            }
        }

        if (found)
        {
            string query = "delete from car where CarID='" + id + "'";
            const char* qd = query.c_str();
            qstate = mysql_query(conn, qd);
            if (!qstate)
            {
                cout << endl << setw(79) << "Successfully deleted from car record" << endl;
            }
            else
            {
                cout << "Failed to Delete" << mysql_error(conn) << endl;
            }
        }

        else
        {
            cout << endl << setw(90) << "Please Enter a valid ID.Do You Want To Try Again? (y/n): ";
            cin >> choose;
            if (choose == 'y' || choose == 'Y')
            {
                DeleteCar();
            }
            else if (choose == 'n' || choose == 'N')
            {
                CarMenu();
            }
        }
        cout << endl << setw(85) << "Do you want to Delete another Car? (y/n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
        {
            DeleteCar();
        }
        else
        {
            ListCar();
        }
    }
}

//List Car
void ListCar() {
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    qstate = mysql_query(conn, "SELECT car.*, carcat.CarBrand, carcat.Model FROM car JOIN carcat ON car.CatID = carcat.CatID");
    if (!qstate)
    {
        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(106) << "===========================================================================================\n";
        // Display headers with better alignment and spacing
        cout << setw(14) << "";
        printf("| %-5s | %-12s | %-12s | %-13s | %-8s | %-10s | %-9s |\n", "ID", "Car Brand", "Model", "Plate", "Price", "Status", "Cat ID");
        cout << setw(106) << "-------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res))) {
            cout << setw(14) << "";
            printf("| %-5s | %-12s | %-12s | %-13s | %-8s | %-10s | %-9s |\n", row[0], row[5], row[6], row[1], row[2], row[3], row[4]);
            cout << setw(106) << "-------------------------------------------------------------------------------------------\n";
        }
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
    cout << endl << setw(70) << "Press Enter To Back....";
    _getch();
    CarMenu();

}

//Search Car
void SearchCar() {
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    char choose;
    string Plate;

    cout << "\n\n";
    cin.ignore(1, '\n\n');
    cout << setw(74) << "Search Car by Plate: ";
    getline(cin, Plate);

    string searchCust_query = "SELECT car.*, carcat.CarBrand, carcat.Model FROM car JOIN carcat ON car.CatID = carcat.CatID WHERE Plate like '" + Plate + "%'";
    const char* sc = searchCust_query.c_str();
    qstate = mysql_query(conn, sc);

    if (!qstate)
    {
        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(106) << "===========================================================================================\n";
        // Display headers with better alignment and spacing
        cout << setw(14) << "";
        printf("| %-5s | %-12s | %-12s | %-13s | %-8s | %-10s | %-9s |\n", "ID", "Car Brand", "Model", "Plate", "Price", "Status", "Cat ID");
        cout << setw(106) << "-------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res))) {
            cout << setw(14) << "";
            printf("| %-5s | %-12s | %-12s | %-13s | %-8s | %-10s | %-9s |\n", row[0], row[5], row[6], row[1], row[2], row[3], row[4]);
            cout << setw(106) << "-------------------------------------------------------------------------------------------\n";
        }
        cout << endl << setw(85) << "Do you want to search other Car? (y/n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
            SearchCar();
        else if (choose == 'n' || choose == 'N')
            CarMenu();
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }

}

//Customer Menu
void CustomerMenu() {
    int choose;
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    cout << setw(300) << " " << endl;
    cout << setw(75) << "1. Register Customer\n\n"; 
    cout << setw(73) << "2. Update Customer\n\n";
    cout << setw(73) << "3. Delete Customer\n\n";
    cout << setw(74) << "4. Display Customer\n\n";
    cout << setw(73) << "5. Search Customer\n\n";
    cout << setw(63) << "6. Back\n\n\n";
    cout << setw(86) << "-------------------------------------------\n" << endl;
    cout << setw(77) << "Please Enter Your Choice : ";
    cin >> choose;

    if (choose == 1)
    {
        RegCust();
    }
    else if (choose == 2)
    {
        UpdateCust();
    }
    else if (choose == 3)
    {
        DeleteCust();
    }
    else if (choose == 4)
    {
        ListCust();
    }
    else if (choose == 5)
    {
        SearchCust();
    }
    else if (choose == 6)
    {
        AdminMenu();
    }
}

void RegCust() {
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    char option;
    string Name = "";
    string IC = "";
    string Password = "";
    string License = "";
    string Phone = "";
    string Email = "";

    cout << "\n\n" << setw(62) << "Enter Name  : ";
    cin.ignore(1, '\n');
    getline(cin, Name);
    Phone = getInputOfLength(10, "\n\t\t\t\t\tPhone number must be 10 digits long.", "\n\t\t\t\t\t\tEnter Phone : ");
    IC = getInputOfLength(12, "\n\t\t\t\t\tIC No must be 12 digits long", "\n\t\t\t\t\t\tEnter IC Number : ");
    Password = getPassword();
    cout << setw(62) << "\n\t\t\t\t\t\tEnter Lisence : ";
    cin.ignore();
    getline(cin, License);
    Email = getValidEmail();

    if (!qstate)
    {
        string insertMember_query = "insert into customer (Name,IC,Password,License,Phone,Email) values ('" + Name + "', '" + IC + "', '" + Password + "','" + License + "','" + Phone + "','" + Email + "')";
        const char* q = insertMember_query.c_str();
        qstate = mysql_query(conn, q);

        if (!qstate)
        {
            cout << "\n\n\n" << setw(76) << "You have been registered.\n\n";
            cout << setw(81) << "Do you want to add other Customer? (y/n): ";
            cin >> option;

            if (option == 'y' || option == 'Y')
                RegCust();
            else
                ListCust();
            _getch();
        }
        else
        {
            cout << "Query Execution Problem!" << mysql_error(conn) << endl;
        }
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
}

void UpdateCust() {
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    string data;
    int choose;
    char option;
    string Email = "";

    qstate = mysql_query(conn, "select * from customer");

    if (!qstate)
    {

        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(109) << "-----------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("| %-16s | %-15s | %-8s | %-15s | %-25s | \n", "IC", "Name", "License", "No. Phone", "Email");
        cout << setw(109) << "-----------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(13) << "";
            printf("| %-16s | %-15s | %-8s | %-15s | %-25s | \n", row[2], row[1], row[4], row[5], row[6]);
            cout << setw(109) << "-----------------------------------------------------------------------------------------------\n";
        }
        while (true) // Retry loop for invalid IC
        {
            cout << endl << endl << setw(67) << " Choose Customer IC : ";
            cin >> IC;

            // Query to check if IC exists
            string findbyid_query = "SELECT * FROM customer WHERE IC='" + IC + "'";
            const char* qi = findbyid_query.c_str();
            qstate = mysql_query(conn, qi);

            if (!qstate)
            {
                res = mysql_store_result(conn);
                if (mysql_num_rows(res) > 0) // If IC is valid
                {
                    // Display selected customer details
                    while ((row = mysql_fetch_row(res)))
                    {
                        system("cls");
                        welcomeHeader(filePath);
                        cout << "\n";
                        cout << setw(82) << "---------------------------------------" << endl;
                        cout << "\n";
                        cout << setw(56) << "IC: " << row[2] << endl;
                        cout << "\n";
                        cout << setw(58) << "Name: " << row[1] << endl;
                        cout << "\n";
                        cout << setw(61) << "License: " << row[4] << endl;
                        cout << "\n";
                        cout << setw(59) << "Email: " << row[6] << endl;
                        cout << "\n";
                        cout << setw(63) << "No. Phone: " << row[5] << endl;
                        cout << "\n";
                        cout << setw(82) << "---------------------------------------" << endl << endl;

                    }
                    break; // Exit retry loop
                }
                else
                {
                    cout << endl << setw(79) << "No Customer found with the given IC!" << endl;
                    cout << endl << setw(79) << "Do you want to try again? (y/n): ";
                    cin >> option;
                    if (option == 'y' || option == 'Y')
                        UpdateCust();
                    else
                        CustomerMenu();
                }
            }
            else
            {
                cout << "Query Execution Problem! " << mysql_error(conn) << endl;
                return;
            }
        }

        cout << setw(66) << "1. No. Phone\n\n";
        cout << setw(62) << "2. Email\n\n";
        cout << setw(61) << "3. Back\n\n";
        cout << setw(84) << "----------------------------------------\n" << endl;
        cout << setw(74) << "Please Enter Your Choice : ";
        cin >> choose;

        cin.ignore(1, '\n');
        if (choose == 1)
        {
            cout << endl << endl << setw(66) << "1. No. Phone: ";
            getline(cin, data);
            string update_query = "update customer set Phone='" + data + "' where IC='" + IC + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }

        else if (choose == 2)
        {
            Email = getEmail();
            string update_query = "update customer set Email='" + Email + "' where IC='" + IC + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else  if (choose == 3)
        {
            CustomerMenu();
        }

        cout << endl << setw(88) << "Do you want to update other Customer? (y/n): ";
        cin >> option;

        if (option == 'y' || option == 'Y')
            UpdateCust();
        else
            ListCust();
        _getch();
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }

}

void DeleteCust() {
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    char choose;
    string id;

    qstate = mysql_query(conn, "select * from customer");
    if (!qstate)
    {

        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(109) << "-----------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("| %-16s | %-15s | %-8s | %-15s | %-25s | \n", "IC", "Name", "License", "No. Phone", "Email");
        cout << setw(109) << "-----------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {
            cout << setw(13) << "";
            printf("| %-16s | %-15s | %-8s | %-15s | %-25s | \n", row[2], row[1], row[4], row[5], row[6]);
            cout << setw(109) << "-----------------------------------------------------------------------------------------------\n";
        }
        cout << endl << endl << setw(65) << "  Choose Customer IC : ";
        cin >> IC;

        bool found = false;

        string query = "select IC from customer";
        const char* qd = query.c_str();
        qstate = mysql_query(conn, qd);
        res = mysql_store_result(conn);

        if (!qstate)
        {
            while ((row = mysql_fetch_row(res)))
            {
                if (row[0] == IC)
                {
                    found = true;
                    break;
                }
            }
        }

        if (found)
        {
            string query = "delete from customer where IC='" + IC + "'";
            const char* qd = query.c_str();
            qstate = mysql_query(conn, qd);
            if (!qstate)
            {
                cout << endl << setw(80) << "  Successfully deleted from customer record" << endl;
            }
            else
            {
                cout << "Failed to Delete" << mysql_error(conn) << endl;
            }
        }

        else
        {
            cout << endl << setw(87) << "  Please Enter a valid ID.Do You Want To Try Again? (y/n): ";
            cin >> choose;
            if (choose == 'y' || choose == 'Y')
            {
                DeleteCust();
            }
            else if (choose == 'n' || choose == 'N')
            {
                CustomerMenu();
            }
        }
        cout << endl << setw(84) << "  Do you want to delete another student? (y/n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
        {
            DeleteCust();
        }
        else
        {
            ListCust();
        }
    }

}

void ListCust() {
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    qstate = mysql_query(conn, "SELECT * FROM customer");
    if (!qstate)
    {
        res = mysql_store_result(conn);


        cout << "\n";
        cout << setw(109) << "-----------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("| %-16s | %-15s | %-8s | %-15s | %-25s | \n", "IC", "Name", "License", "No. Phone", "Email");
        cout << setw(109) << "-----------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {
            cout << setw(13) << "";
            printf("| %-16s | %-15s | %-8s | %-15s | %-25s | \n", row[2], row[1], row[4], row[5], row[6]);
            cout << setw(109) << "-----------------------------------------------------------------------------------------------\n";
        }


    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
    cout << "\n" << setw(75) << "Press Enter To Back....";
    _getch();
    CustomerMenu();
}

void SearchCust() {
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    char choose;
    string name;

    cout << "\n\n";
    cin.ignore(1, '\n');
    cout << setw(72) << "Search Customer by name: ";
    getline(cin, name);

    string search_query = "select * from customer where Name like '%" + name + "%'";
    const char* q = search_query.c_str();
    qstate = mysql_query(conn, q);

    if (!qstate)
    {
        res = mysql_store_result(conn);

        if (mysql_num_rows(res) > 0) // Check if the result has rows
        {
            cout << "\n";
            cout << setw(109) << "-----------------------------------------------------------------------------------------------\n";
            // Data show in box
            cout << setw(13) << "";
            printf("| %-16s | %-15s | %-8s | %-15s | %-25s | \n", "IC", "Name", "License", "No. Phone", "Email");
            cout << setw(109) << "-----------------------------------------------------------------------------------------------\n";

            while ((row = mysql_fetch_row(res)))
            {
                cout << setw(13) << "";
                printf("| %-16s | %-15s | %-8s | %-15s | %-25s | \n", row[2], row[1], row[4], row[5], row[6]);
                cout << setw(109) << "-----------------------------------------------------------------------------------------------\n";
            }
        }
        else
        {
            // No results found
            cout << "\n\n";
            cout << "\033[31m"; // Set text color to red
            cout << setw(74) << "No customer found with the name \"" << name << "\".\n";
            cout << "\033[0m"; // Reset text color to default
        }

        cout << endl << setw(38) << "";
        cout << setw(10) << "Do you want to search another customer? (y/n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
            SearchCust();
        else if (choose == 'n' || choose == 'N')
            CustomerMenu();
    }
    else
    {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }


}

//Rental Menu In Admin
void RentalMenu() {

    int choose;

    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    cout << "\n\n\n";
    cout << setw(66) << "1. Rent Car\n\n";
    cout << setw(69) << "2. Update Rent\n\n";
    cout << setw(69) << "3. Delete Rent\n\n";
    cout << setw(68) << "4. Return Car\n\n";
    cout << setw(69) << "5. Rented List\n\n";
    cout << setw(64) << "6. Search\n\n";
    cout << setw(62) << "7. Back\n\n";
    cout << setw(83) << "-------------------------------------------\n" << endl;
    cout << setw(75) << "Please Enter Your Choice : ";
    cin >> choose;

    if (choose == 1)
    {
        RentalInAdmin();

    }
    else if (choose == 2)
    {
        UpdateRentalA();

    }
    else if (choose == 3)
    {
        DeleteRental();
    }
    else if (choose == 4) 
    {
        ReturnCar();

    }
    else if (choose == 5)
    {
        RentList();
    }
    else if (choose == 6)
    {
        SearchRent();

    }
    else if (choose == 7)
    {

        AdminMenu();
    }

}

//Rental In Admin
void RentalInAdmin() {

    db_response::ConnectionFunction();
    system("cls");
    std::string filePath = "rent.txt"; // Path to the admin file
    welcomeHeader(filePath);

    string custID, carID;
    std::string rentalDateStr, returnDateStr;
    string pickUpTimeStr = "10:00";
    string dropOffTimeStr = "10:00";


    // Prompt user for customer ID
    if (mysql_query(conn, "SELECT * FROM customer") == 0) {
        res = mysql_store_result(conn);
        std::cout << "\n";
        std::cout << std::setw(109) << "-----------------------------------------------------------------------------------------------\n";
        std::cout << std::setw(13) << "";
        printf("| %-5s | %-15s | %-12s | %-10s |  %-12s | %-21s | \n", "ID", "Name", "IC", "License", "No. Phone", "Email");
        std::cout << std::setw(109) << "-----------------------------------------------------------------------------------------------\n";

        // Loop through and print all the rows
        while ((row = mysql_fetch_row(res))) {
            std::cout << std::setw(13) << "";
            printf("| %-5s | %-15s | %-12s | %-10s |  %-12s | %-21s | \n", row[0], row[1], row[2], row[4], row[5], row[6]);
            std::cout << std::setw(109) << "-----------------------------------------------------------------------------------------------\n";
        }
        mysql_free_result(res);
    }
    else {
        std::cerr << "Error executing query: " << mysql_error(conn) << std::endl;
    }


    bool validCustomer = false;
    

    // Loop to retry if the customer ID is invalid
    do {
        std::cout << "\n\n" << std::setw(68) << "Enter Customer ID: ";
        std::cin >> custID;

        // Validate customer ID exists
        std::string query = "SELECT 1 FROM customer WHERE CustID = '" + custID + "'";
        if (mysql_query(conn, query.c_str()) != 0) {
            std::cerr << "Error executing query: " << mysql_error(conn) << std::endl;
            RentalInAdmin();
            return;
        }

        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);
        if (row) {
            validCustomer = true;
        }
        mysql_free_result(res);


        if (!validCustomer) {
            char retryChoice;
            std::cout << "\n\n" << std::setw(82) << "Customer ID not found. Do you want to retry? (y/n): ";
            std::cin >> retryChoice;

            // Handle the retry choice
            if (retryChoice == 'y' || retryChoice == 'Y') {
                std::cout << "Retrying customer ID input...\n";
                RentalInAdmin();
                return;  // Exit current function to restart
            }
            else if (retryChoice == 'n' || retryChoice == 'N') {
                std::cout << "Returning to RentalInAdmin...\n";
                RentalMenu();  // Return to admin menu
                return;  // Exit current function
            }

        }
    } while (!validCustomer);
        // If a valid customer ID is found, fetch the customer name
        std::string custName;
        std::string cust_query = "SELECT Name FROM customer WHERE CustID = '" + custID + "'";
        if (validCustomer) {
            
            if (mysql_query(conn, cust_query.c_str()) != 0) {
                std::cerr << "Error fetching customer name: " << mysql_error(conn) << std::endl;
            }
            else {
                res = mysql_store_result(conn);
                row = mysql_fetch_row(res);
                if (row) {
                    custName = row[0];  // Store the customer name
                    cout << "\n";
                    cout << setw(64) << "Customer found: " << custName << std::endl;
                }
                mysql_free_result(res);
            }
        }
        else {
            std::cout << "No valid customer found. Returning to the previous menu." << std::endl;
            RentalInAdmin();
        }
        printf("\n\n");
        // Prompt user for car ID
        qstate = mysql_query(conn, "SELECT car.*, carcat.CarBrand, carcat.Model FROM car JOIN carcat ON car.CatID = carcat.CatID");
        if (!qstate)
        {
            res = mysql_store_result(conn);

            cout << "\n";
            cout << setw(107) << "-----------------------------------------------------------------------------------------------\n";
            //data show in box
            cout << setw(11) << "";
            printf("| %-4s | %-10s | %-13s | %-15s | %-15s | %-19s | \n", "ID", "Car Brand", "Model", "Plate", "Price", "Status");
            cout << setw(107) << "-----------------------------------------------------------------------------------------------\n";

            while ((row = mysql_fetch_row(res)))
            {

                cout << setw(11) << "";
                printf("| %-4s | %-10s | %-13s | %-15s | %-15s | %-19s | \n", row[0], row[5], row[6], row[1], row[2], row[3]);
                cout << setw(107) << "-----------------------------------------------------------------------------------------------\n";
            }

            cout << "\n\n" << setw(64) << "Choose Car ID: ";
            cin.ignore(1, '\n');
            getline(cin, CarID);

            // Validate car ID exists and fetch price,status,model and car plate
            string query = "SELECT car.Price,car.Status,car.Plate, carcat.CarBrand, carcat.Model FROM car JOIN carcat ON car.CatID = carcat.CatID WHERE CarID ='" + CarID + "'";
            if (mysql_query(conn, query.c_str()) != 0) {
                std::cerr << "Error executing query: " << mysql_error(conn) << std::endl;
                mysql_close(conn);
                return;
            }
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);
            float carPrice = 0.0;
            int totalDays = 0;
            float totalPrice = 0.0, discountedPrice = 0.0;
            string carStatus = "", Plate = "", CarBrand = "", Model = "";
            if (row) {
                carPrice = atof(row[0]);
                carStatus = row[1];
                Plate = row[2];  // Store car plate number
                CarBrand = row[3];
                Model = row[4];
            }
            else {
                std::cerr << "Car with ID " << CarID << " not found!" << std::endl;
                RentalInAdmin();

            }

            mysql_free_result(res);

            bool isAvailable = (carStatus == "Available");
            printf("\n\n");

            if (!isAvailable) {
                std::cout << std::setw(77) << "The selected car is currently rented. Please pick another car." << std::endl;
                std::cout << std::setw(77) << "Press Enter to continue..." << std::endl;

                // Clear input buffer and wait for Enter key
                //std::cin.ignore();  // Clear up to 1000 characters or until a newline is encountered
                std::cin.get();               // Wait for Enter

                RentalInAdmin();  // Call the function to select a different car
                return;           // Exit the current function
            }


            //rest of the code for rental calculations and display ...
            if (isAvailable) {
                // Prompt for rental and return dates
                std::cout << setw(77) << "Enter Rental Date (YYYY-MM-DD): ";
                std::cin >> rentalDateStr;
                std::cout << "\n" << std::setw(77) << "Enter Return Date (YYYY-MM-DD): ";
                std::cin >> returnDateStr;

                // Convert dates and times to tm structs
                tm rentalDate = {}, returnDate = {}, pickUpTime = {}, dropOffTime = {};
                istringstream(rentalDateStr) >> get_time(&rentalDate, "%Y-%m-%d");
                istringstream(returnDateStr) >> get_time(&returnDate, "%Y-%m-%d");
                istringstream(pickUpTimeStr) >> get_time(&pickUpTime, "%H:%M");
                istringstream(dropOffTimeStr) >> get_time(&dropOffTime, "%H:%M");

                // Combine date and time for rental and return
                rentalDate.tm_hour = pickUpTime.tm_hour;
                rentalDate.tm_min = pickUpTime.tm_min;
                returnDate.tm_hour = dropOffTime.tm_hour;
                returnDate.tm_min = dropOffTime.tm_min;

                // Validate the dates
                if (!isDateValid(rentalDate, returnDate)) {
                    char choose;
                    cerr << setw(77) << "Error: Invalid dates entered. Ensure the rental date is today or later, and the return date is after the rental date." << endl;
                    cout << endl << setw(78) << "Do you want to try again ? (y/n): ";
                    cin >> choose;

                    if (choose == 'y' || choose == 'Y')
                    {
                        RentalInAdmin();
                    }
                    else
                    {
                        RentalMenu();
                    }
                }

                time_t rentalTimeT = mktime(&rentalDate);
                time_t returnTimeT = mktime(&returnDate);
                int totalSeconds = difftime(returnTimeT, rentalTimeT);

                // Convert seconds to total days (rounding up for partial days)
                totalDays = (totalSeconds + 86399) / 86400; // Add 86399 to round up partial days

                // Calculate total price
                totalPrice = totalDays * carPrice;

                // Apply discount for rentals of 3 or more days
                if (totalDays >= 3) {
                    float discountRate = 0.10; // 10% discount
                    discountedPrice = totalPrice * (1 - discountRate);
                }
                else {
                    discountedPrice = totalPrice; // No discount
                }

                // Display details and prices
                system("cls");
                std::string filePath = "rent.txt"; // Path to the admin file
                welcomeHeader(filePath);
                int nameWidth = custName.length() > 18 ? custName.length() + 1 : 18;

                std::cout << std::setw(82) << "     |-------------------------------------------------------|" << std::endl;
                std::cout << std::setw(82) << "     |                  Car Rental Details                   |" << std::endl;
                std::cout << std::setw(83) << "     |-------------------------------------------------------|\n" << std::endl;
                std::cout << std::setw(49) << " Customer Name:    " << std::setw(nameWidth) << custName << "\n" << std::endl;
                std::cout << std::setw(42) << " Car Brand: " << std::setw(27) << CarBrand << "\n" << std::endl;
                std::cout << std::setw(49) << " Car Plate Number: " << std::setw(23) << Plate << "\n" << std::endl;
                std::cout << std::setw(49) << " Rental Date:      " << std::setw(25) << rentalDateStr << "\n" << std::endl;
                std::cout << std::setw(48) << " Return Date:     " << std::setw(26) << returnDateStr << "\n" << std::endl;
                std::cout << std::setw(51) << " Total Price: RM     " << std::setw(16) << totalPrice << "\n" << std::endl;

                if (totalDays >= 3) {
                    std::cout << std::setw(63) << " Total price (with 10% discount):  RM"
                        << std::setw(static_cast<std::streamsize>(nameWidth) - 2) << discountedPrice << "\n" << std::endl;
                }

                std::cout << std::setw(82) << "   |-------------------------------------------------------|" << std::endl;
                // Prompt for rental confirmation
                std::cout << std::setw(76) << "\n                                            Confirm Rental (y/n): ";
                char confirm;
                std::cin >> confirm;

                // If confirmed, insert rental into database
                if (confirm == 'y' || confirm == 'Y') {
                    string Rent_Status = "Approved";
                    query = "INSERT INTO rent (CustID, CarID, Rent_Date, Return_Date,TotalPrice,Rent_Status) VALUES ('" + custID + "', '" + CarID + "', '" + rentalDateStr + "','" + returnDateStr + "'," + std::to_string(totalPrice) + ",'" + Rent_Status + "')";
                    if (mysql_query(conn, query.c_str()) != 0) {
                        std::cerr << "Error adding rental: " << mysql_error(conn) << std::endl;

                    }

                    else {
                        std::cout << setw(84) << "\n\t\tRental successfully! Please make sure customer pay at the counter!" << std::endl;
                        // Fetch and display transaction details
                        std::string fetchQuery = "SELECT rent.RentID, customer.Name, carcat.CarBrand, car.Plate, rent.Rent_Date, rent.Return_Date, rent.TotalPrice FROM rent JOIN customer ON rent.CustID = customer.CustID JOIN car ON rent.CarID = car.CarID JOIN carcat ON car.CatID = carcat.CatID ORDER BY RentID DESC LIMIT 1";
                        if (mysql_query(conn, fetchQuery.c_str()) != 0) {
                            std::cerr << "Error executing query: " << mysql_error(conn) << std::endl;
                            mysql_close(conn);
                            return;

                        }
                        res = mysql_store_result(conn);
                        row = mysql_fetch_row(res);
                        float total_price = 0.0;
                        string rentalID = "", custName = "", carStatus = "", carPlate = "", brand = "", rental_date = "", return_date = "";
                        if (row) {
                            rentalID = row[0];
                            custName = row[1];
                            brand = row[2];  // Store car plate number
                            carPlate = row[3];
                            rental_date = row[4];
                            return_date = row[5];
                            total_price = atof(row[6]);

                            system("cls");
                            std::string filePath = "rent.txt"; // Path to the admin file
                            welcomeHeader(filePath);

                            std::cout << std::setw(76) << "\n                                            Transaction Details:";
                            cout << "\n\n";
                            cout << setw(25) << "" << "----------------------------------------------------------\n";
                            cout << "\n";
                            std::cout << std::setw(42) << " Rental ID: " << std::setw(24) << rentalID << "\n" << std::endl;
                            std::cout << std::setw(46) << " Customer Name: " << std::setw(21) << custName << "\n" << std::endl;
                            std::cout << std::setw(42) << " Car Brand: " << std::setw(27) << brand << "\n" << std::endl;
                            std::cout << std::setw(42) << " Car Plate: " << std::setw(30) << carPlate << "\n" << std::endl;
                            std::cout << std::setw(44) << " Rental Date: " << std::setw(30) << rental_date << "\n" << std::endl;
                            std::cout << std::setw(44) << " Return Date: " << std::setw(30) << return_date << "\n" << std::endl;
                            std::cout << std::setw(47) << " Total Price: RM " << std::setw(20) << totalPrice << "\n" << std::endl;
                            cout << setw(25) << "" << "----------------------------------------------------------\n";
                        }
                        else {
                            std::cerr << "Error fetching transaction" << std::endl;
                            mysql_close(conn);

                        }
                        mysql_free_result(res);
                        cout << "\n";
                        cout << setw(71) << "Update the car status ? (y/n): ";
                        char option;
                        cin >> option;

                        if (option == 'y' || option == 'Y')
                        {
                            UpdateCar(); //in admin
                        }
                        else
                        {

                            AdminMenu();
                        }
                    }
                }
                else if (confirm == 'n' || confirm == 'N') {
                    std::cout << std::setw(79) << "Rental was not confirmed. Returning to the main menu." << std::endl;
                    RentalInAdmin(); // Return to the main menu or exit the function
                }
                else {
                    std::cerr << "Invalid input. Please enter 'y' or 'n'." << std::endl;
                    // You can repeat the confirmation prompt if needed
                }
            }
        }
    
}

void UpdateRentalA() {
    db_response::ConnectionFunction();
    system("cls");
    std::string filePath = "rent.txt"; // Path to the admin file
    welcomeHeader(filePath);

    string rentID, rentStatus;

    // Display the table with RentID, Customer IC, Car Plate, and Rent Status
    qstate = mysql_query(conn, "SELECT rent.RentID, customer.IC, car.Plate, rent.Rent_Status FROM rent "
        "JOIN customer ON rent.CustID = customer.CustID "
        "JOIN car ON rent.CarID = car.CarID");

    std::cout << "\n\n";
    table();  // Assuming this is the function that initializes the table display
    std::cout << "\n\n" << std::setw(75) << "Enter Rental ID to update: ";
    std::cin >> rentID;

    // Validate that the rental ID exists
    bool validRental = false;
    string query = "SELECT 1 FROM rent WHERE RentID = '" + rentID + "'";

    if (mysql_query(conn, query.c_str()) != 0) {
        std::cerr << "Error executing query: " << mysql_error(conn) << std::endl;
        RentalInAdmin();  // Return to admin menu on error
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    if (row) {
        validRental = true;
    }
    mysql_free_result(res);

    if (validRental) {
        // Display options for rent status
        std::cout << "\n\n" << std::setw(60) << "Choose New Rent Status:\n";
        std::cout << "1. Approved\n";
        std::cout << "2. Returned\n";
        std::cout << "3. Pending\n";
        std::cout << std::setw(60) << "Enter your choice (1-3): ";

        int choice;
        std::cin >> choice;

        // Map choice to status
        switch (choice) {
        case 1:
            rentStatus = "Approved";
            break;
        case 2:
            rentStatus = "Returned";
            break;
        case 3:
            rentStatus = "Pending";
            break;
        default:
            std::cerr << "Invalid choice, please enter a number between 1 and 3." << std::endl;
            UpdateRentalA();
            return;
        }

        // Update the rent status in the database
        query = "UPDATE rent SET Rent_Status = '" + rentStatus + "' WHERE RentID = '" + rentID + "'";

        if (mysql_query(conn, query.c_str()) != 0) {
            std::cerr << "Error updating rent status: " << mysql_error(conn) << std::endl;
        }
        else {
            std::cout << std::setw(70) << "Rental status updated successfully to: " << rentStatus << std::endl;
        }

        // Ask if the admin wants to return to the admin menu
        char backChoice;
        std::cout << "\nDo you want to go back to the Rental Menu? (y/n): ";
        std::cin >> backChoice;

        if (backChoice == 'y' || backChoice == 'Y') {
            RentalMenu();  // Function that shows the main admin menu
        }
        else {
            std::cout << "Thank you for using the admin system. Exiting now..." << std::endl;
            exit(0);  // Or use return to exit gracefully
        }
    }
    else {
        std::cerr << "Rental ID not found!" << std::endl;
        UpdateRentalA();  // Return to the admin menu on error
    }

    mysql_free_result(res);
}

//Cancel Rent
void  DeleteRental() {

    system("cls");
    std::string filePath = "rent.txt"; // Path to the admin file
    welcomeHeader(filePath);

    char choose;
    string ID;

    qstate = mysql_query(conn, "select * from rent");

    if (!qstate)
    {

        res = mysql_store_result(conn);

        cout << "\n\n";
        cout << setw(107) << "---------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("|%-9s | %-5s | %-5s | %-10s | %-10s | %-10s | %-10s |\n", "Rental ID", "Car ID",  "Customer ID" , "Rental Date", "Return Date", "Total Price", "Rental Status");
        cout << setw(107) << "---------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {
            cout << setw(13) << "";
            printf("|%-9s | %-5s | %-5s | %-10s | %-10s | %-10s | %-10s |\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
            cout << setw(107) << "--------------------------------------------------------------------------------------\n";
        }
        cout << endl << endl << setw(72) << "  Choose Customer ID : ";
        cin >> ID;

        bool found = false;

        string query = "select RentID from rent";
        const char* qd = query.c_str();
        qstate = mysql_query(conn, qd);
        res = mysql_store_result(conn);

        if (!qstate)
        {
            while ((row = mysql_fetch_row(res)))
            {
                if (row[0] == ID)
                {
                    found = true;
                    break;
                }
            }
        }

        if (found)
        {
            string query = "delete from rent where RentID='" + ID + "'";
            const char* qd = query.c_str();
            qstate = mysql_query(conn, qd);
            if (!qstate)
            {
                cout << endl << setw(35) << "         Successfully deleted from Rent record" << endl;
            }
            else
            {
                cout << "Failed to Delete" << mysql_error(conn) << endl;
            }
        }

        else
        {
            cout << endl << setw(35) << "             Please Enter a valid ID.Do You Want To Try Again? (y/n): ";
            cin >> choose;
            if (choose == 'y' || choose == 'Y')
            {
                DeleteRental();
            }
            else if (choose == 'n' || choose == 'N')
            {
                RentalMenu();
            }
        }
        cout << endl << setw(35) << "         Do you want to delete another student?(y|n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
        {
            DeleteRental();
        }
        else
        {
            //RentedList();
        }
    }
}

void ReturnCar() {

    system("cls");
    string ID;
    std::string filePath = "rent.txt"; // Path to the admin file
    welcomeHeader(filePath);

    qstate = mysql_query(conn, "SELECT rent.RentID,  car.CarID, carcat.CarBrand, car.Plate, rent.Rent_Date, rent.Return_Date, rent.Rent_Status FROM carcat JOIN car ON carcat.CatID = car.CatID JOIN rent ON car.CarID = rent.CarID");

    if (!qstate)
    {

        res = mysql_store_result(conn);

        cout << "\n\n";
        cout << setw(107) << "---------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("|%-9s | %-5s | %-5s | %-10s | %-10s | %-10s | %-10s |\n", "Rental ID", "Car ID", "Customer ID", "Rental Date", "Return Date", "Total Price", "Rental Status");
        cout << setw(107) << "---------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {
            cout << setw(13) << "";
            printf("|%-9s | %-5s | %-5s | %-10s | %-10s | %-10s | %-10s |\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
            cout << setw(107) << "--------------------------------------------------------------------------------------\n";
        }
        cout << "\n\n" << setw(68) << "Choose Rental ID: ";
        cin.ignore(1, '\n'); //Discard any leftover newline from previous input
        getline(cin, ID);

        //update rental status to 'returned'
        std::string update_query = "UPDATE rent SET Rent_Status = 'returned' WHERE RentID = '" + ID + "' ";
        if (mysql_query(conn, update_query.c_str())) {
            std::cerr << "Error updating rental status: " << mysql_error(conn) << std::endl;

        }
        // Log the return date and time
        std::string log_query = "INSERT into car_return (RentID, ReturnDate) VALUES ('" + ID + "', NOW())";
        if (mysql_query(conn, log_query.c_str())) {
            std::cerr << "Error logging return date: " << mysql_error(conn) << std::endl;
            mysql_close(conn);

        }
        std::cout << "\n";
        cout << setw(83) << "Rental car return successfully! Update the car status? (y/n): ";
        char option;
        cin >> option;

        if (option == 'y' || option == 'Y')
        {
            UpdateCar();
        }
        else
        {
            RentalMenu();
        }
    }
}

void RentList() {

    system("cls");
    std::string filePath = "rent.txt"; // Path to the admin file
    welcomeHeader(filePath);

    qstate = mysql_query(conn, "select * from rent");

    if (!qstate)
    {

        res = mysql_store_result(conn);

        cout << "\n\n";
        cout << setw(107) << "---------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("|%-9s | %-5s | %-5s | %-10s | %-10s | %-10s | %-10s |\n", "Rental ID", "Car ID", "Customer ID", "Rental Date", "Return Date", "Total Price", "Rental Status");
        cout << setw(107) << "---------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {
            cout << setw(13) << "";
            printf("|%-9s | %-5s | %-5s | %-10s | %-10s | %-10s | %-10s |\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
            cout << setw(107) << "--------------------------------------------------------------------------------------\n";
        }
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
    cout << "\n" << setw(75) << "Press Enter To Back....";
    _getch();
    RentalMenu();
}

void SearchRent() {

    system("cls");
    std::string filePath = "rent.txt"; // Path to the admin file
    welcomeHeader(filePath);
    
    char choose;
    string rental_id;

    cin.ignore(1, '\n');
    std::cout << "\n\n";
    cout << setw(75) << "SEARCH rent by RentID: ";
    getline(cin, rental_id);

    string searchRent_query = "SELECT * FROM rent WHERE RentID like '" + rental_id + "'";
    const char* sr = searchRent_query.c_str();
    qstate = mysql_query(conn, sr);

    if (!qstate)
    {

        res = mysql_store_result(conn);

        cout << "\n\n";
        cout << setw(107) << "---------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("|%-9s | %-5s | %-5s | %-10s | %-10s | %-10s | %-10s |\n", "Rental ID", "Car ID", "Customer ID", "Rental Date", "Return Date", "Total Price", "Rental Status");
        cout << setw(107) << "---------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {
            cout << setw(13) << "";
            printf("|%-9s | %-5s | %-5s | %-10s | %-10s | %-10s | %-10s |\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
            cout << setw(107) << "--------------------------------------------------------------------------------------\n";
        }
    }

    cout << endl << setw(40) << "";
    cout << setw(10) << "Do yo want to search other Rent? (y/n): ";
    cin >> choose;

    if (choose == 'y' || choose == 'Y') { 
        SearchRent(); 
    }
        
    else if (choose == 'n' || choose == 'N') {

        RentalMenu();
    }
    else {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
}

//Staff Login
void StaffLogin() {

    int menu;
    system("cls");
    std::string filePath = "staff.txt"; // Path to the admin file
    welcomeHeader(filePath);

    string staffpassword;
    string staffusername;
    cout << setw(63) << "Username: ";
    cin >> staffusername;

    // Validate username input
    if (staffusername.empty())
    {
        cout << "Invalid username! Please try again." << endl;
        StaffLogin(); // Repeat the function to allow re-entering the username
        return;
    }

    cout << "\n\n" << setw(63) << "Password: ";
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == 8) {
            if (!staffpassword.empty()) {
                staffpassword.pop_back();
                cout << "\b \b";
            }
        }
        else {
            staffpassword.push_back(ch);
            cout << '*';
        }
    }

    // Validate password input
    if (staffpassword.empty())
    {
        cout << endl << "Invalid password! Please try again." << endl;
        StaffLogin(); // Repeat the function to allow re-entering the password
        return;
    }

    string checkUser_query = "SELECT * , name FROM staff WHERE username = '" + staffusername + "' AND password = '" + staffpassword + "'";
    const char* cu = checkUser_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count == 1)
        {
            string user, pw;
            while (row = mysql_fetch_row(res))
            {
                StaffID = row[0];
                user = row[1];
                pw = row[2];
                StaffName = row[3];
            }

            if (user == staffusername && pw == staffpassword)
                StaffPage();
        }

        char c;
        cout << "\n\n\n" << setw(86) << "Invalid username or password. Want to try again? (y/n): ";
        cin >> c;
        if (c == 'y' || c == 'Y')
            StaffLogin();
        else
            main();

    }
    else
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
}

void StaffPage() {

    std::string answer;
    int menu;
    system("cls");
    std::string filePath = "staff.txt"; // Path to the admin file
    welcomeHeader(filePath);

    cout << setw(63) << " Login as: " << StaffName << "\n\n\n";
    cout << setw(73) << "1. Manage Customer\n\n" << endl;
    cout << setw(69) << "2. Monitor Car\n\n" << endl;
    cout << setw(76) << "3. Update Rent Status\n\n" << endl;
    cout << setw(70) << "4. Display Rent\n\n" << endl;
    cout << setw(64) << "5. Logout\n\n" << endl;
    cout << setw(87) << "-----------------------------------------\n" << endl;
    cout << setw(78) << "Please Enter Your Choice : ";
    cin >> menu;

    switch (menu)
    {
    case 1:
        CustMenu();
        break;

    case 2:
        MenuCar();
        break;
    case 3:
        UpdateRentalS();
        break;
    case 4:
        DisplayRent();
        break;
    case 5:
        system("cls");
        std::cout << "\n                                   Do you want to logout the system? (yes/no): ";
        std::cin >> answer;

        for (char& c : answer) {
            c = std::tolower(c);
        }

        system("cls");

        if (answer == "yes" || answer == "y") {
            ifstream inputFile("thank.txt"); //displaying thank you ASCII image text on output screen fn1353
            if (!inputFile) {
                cout << "Cannot open input file.\n";
            }
            char str[1000];
            while (inputFile) {
                inputFile.getline(str, 1000);
                if (inputFile)
                    cout << str << endl;
            }
            inputFile.close();
            std::cout << "\n Logged out successfully\n" << std::endl;
            std::cin.get();
            exit(0); // Terminate the program with exit code 0 (indicating a successful exit)
        }
        else {
            StaffPage();
        }
    }
}

void CustMenu() {

    int choose;
    system("cls");
    std::string filePath = "staff.txt"; // Path to the admin file
    welcomeHeader(filePath);

    cout << setw(75) << "1. Register Customer\n\n";
    cout << setw(73) << "2. Update Customer\n\n";
    cout << setw(73) << "3. Delete Customer\n\n";
    cout << setw(74) << "4. Display Customer\n\n";
    cout << setw(73) << "5. Search Customer\n\n";
    cout << setw(63) << "6. Back\n\n\n";
    cout << setw(85) << "-------------------------------------------\n" << endl;
    cout << setw(78) << "Please Enter Your Choice : ";
    cin >> choose;

    if (choose == 1)
    {
        CustReg();
    }
    else if (choose == 2)
    {
        CustUpdate();
    }
    else if (choose == 3)
    {
        CustDelete();
    }
    else if (choose == 4)
    {
        CustList();
    }
    else if (choose == 5)
    {
        CustSearch();
    }
    else if (choose == 6)
    {
        StaffPage();
    }
}

void CustReg() {

    system("cls");
    std::string filePath = "staff.txt"; // Path to the admin file
    welcomeHeader(filePath);

    char option;
    string Name = "";
    string IC = "";
    string Password = "";
    string License = "";
    string Phone = "";
    string Email = "";


    cout << "\n" << setw(62) << "Enter Name  : ";
    cin.ignore(1, '\n');
    getline(cin, Name);
    Phone = getInputOfLength(10, "\n\t\t\t\t\tPhone number must be 10 digits long.", "\n\t\t\t\t\t\tEnter Phone : ");
    IC = getInputOfLength(12, "\n\t\t\t\t\tIC No must be 12 digits long", "\n\t\t\t\t\t\tEnter IC Number.: ");
    Password = getPassword();
    Email = getValidEmail();
    cout << setw(64) << "\n\t\t\t\t\t\tEnter License Number : ";
    cin.ignore();
    getline(cin, License);



    if (!qstate)
    {
        string insertMember_query = "insert into customer (Name,IC,Password,Phone,Email,License) values ('" + Name + "', '" + IC + "','" + Password + "','" + Phone + "','" + Email + "','" + License + "')";
        const char* q = insertMember_query.c_str();
        qstate = mysql_query(conn, q);

        if (!qstate)
        {
            cout << "\n\n" << setw(75) << "You have been registered.\n\n";
            cout << setw(81) << "Do you want to add other Customer? (y/n): ";
            cin >> option;

            if (option == 'y' || option == 'Y')
                CustReg();
            else
                CustList();
            _getch();
        }
        else
        {
            cout << "Query Execution Problem!" << mysql_error(conn) << endl;
        }
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
}

void CustUpdate() {

    system("cls");
    std::string filePath = "staff.txt"; // Path to the admin file
    welcomeHeader(filePath);

    string ID;
    string DataCust;
    int choose;
    char option;

    qstate = mysql_query(conn, "SELECT * FROM customer");

    if (!qstate)
    {
        res = mysql_store_result(conn);

        // Display staff data in a table
        cout << "\n";
        cout << setw(105) << "-------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("|%-3s | %-12s | %-15s | %-10s | %-13s | %-20s |\n", "ID", "Name", "IC", "License", "No.Phone", "Email");
        cout << setw(105) << "-------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {
            cout << setw(13) << "";
            printf("|%-3s | %-12s | %-15s | %-10s | %-13s | %-20s |\n", row[0], row[1], row[2], row[4], row[5], row[6]);
            cout << setw(105) << "-------------------------------------------------------------------------------------------\n";
        }

        while (true) // Retry loop for invalid Employee_ID
        {
            cout << endl << endl << setw(70) << " Choose ID to update: ";
            cin >> ID;

            // Query to check if Employee_ID exists
            string findbyid_query = "SELECT * FROM customer WHERE CustID='" + ID + "'";
            const char* qi = findbyid_query.c_str();
            qstate = mysql_query(conn, qi);

            if (!qstate)
            {
                res = mysql_store_result(conn);
                if (mysql_num_rows(res) > 0) // If ID is valid
                {
                    // Display selected employee details
                    while ((row = mysql_fetch_row(res)))
                    {
                        system("cls");
                        welcomeHeader(filePath);
                        cout << "\n";
                        cout << setw(82) << "---------------------------------------" << endl;
                        cout << "\n";
                        cout << setw(56) << "IC: " << row[2] << endl;
                        cout << "\n";
                        cout << setw(58) << "Name: " << row[1] << endl;
                        cout << "\n";
                        cout << setw(61) << "License: " << row[4] << endl;
                        cout << "\n";
                        cout << setw(59) << "Customer Email: " << row[6] << endl;
                        cout << "\n";
                        cout << setw(63) << "Customer No. Phone: " << row[5] << endl;
                        cout << "\n";
                        cout << setw(82) << "---------------------------------------" << endl << endl;

                    }
                    break; // Exit retry loop
                }
                else
                {
                    cout << endl << setw(77) << "No Customer found with the given ID!" << endl;
                    cout << endl << setw(77) << "Do you want to try again? (y/n): ";
                    cin >> option;
                    if (option == 'y' || option == 'Y')
                        CustUpdate();
                    else
                        CustMenu();
                }
            }
            else
            {
                cout << "Query Execution Problem! " << mysql_error(conn) << endl;
                return;
            }
        }

        // Update options
        cout << setw(71) << "1. Customer Email\n\n";
        cout << setw(75) << "2. Customer No. Phone\n\n";
        cout << setw(61) << "3. Back\n\n";
        cout << setw(83) << "---------------------------------------\n" << endl;
        cout << setw(75) << "Please Enter Your Choice : ";
        cin >> choose;
        cin.ignore(1, '\n'); // Clear newline from input buffer

        if (choose == 1)
        {
            cout << endl << endl << setw(75) << "1. New Customer Email: ";
            getline(cin, DataCust);
            string update_query = "UPDATE customer SET Email='" + DataCust + "' WHERE CustID='" + ID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (choose == 2)
        {
            cout << endl << endl << setw(75) << "2. New Customer Phone: ";
            getline(cin, DataCust);
            string update_query = "UPDATE customer SET Phone='" + DataCust + "' WHERE CustID='" + ID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (choose == 3)
        {
            CustMenu();
        }

        cout << endl << setw(86) << "Do you want to update other Customer? (y/n): ";
        cin >> option;

        if (option == 'y' || option == 'Y')
            CustUpdate();
        else
            CustList();
    }
    else
    {
        cout << "Query Execution Problem! " << mysql_error(conn) << endl;
        return;
    }
}

void CustDelete() {

    system("cls");
    std::string filePath = "staff.txt"; // Path to the admin file
    welcomeHeader(filePath);

    char choose;
    string id;

    qstate = mysql_query(conn, "select * from customer");
    if (!qstate)
    {

        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("|%-15s | %-25s | %-15s | %-20s | %-10s | %-20s |\n", "ID", "Name", "IC", "License", "No.Phone", "Email");
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(13) << "";
            printf("|%-15s | %-25s | %-15s | %-20s | %-10s |%-20s |\n", row[0], row[1], row[2], row[3], row[4], row[5]);
            cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        }
        cout << endl << endl << setw(43) << "  Choose Customer IC : ";
        cin >> IC;

        bool found = false;

        string query = "select IC from customer";
        const char* qd = query.c_str();
        qstate = mysql_query(conn, qd);
        res = mysql_store_result(conn);

        if (!qstate)
        {
            while ((row = mysql_fetch_row(res)))
            {
                if (row[0] == IC)
                {
                    found = true;
                    break;
                }
            }
        }

        if (found)
        {
            string query = "delete from customer where IC='" + IC + "'";
            const char* qd = query.c_str();
            qstate = mysql_query(conn, qd);
            if (!qstate)
            {
                cout << endl << setw(35) << "         Successfully deleted from customer record" << endl;
            }
            else
            {
                cout << "Failed to Delete" << mysql_error(conn) << endl;
            }
        }

        else
        {
            cout << endl << setw(35) << "             Please Enter a valid ID.Do You Want To Try Again? (y|n): ";
            cin >> choose;
            if (choose == 'y' || choose == 'Y')
            {
                CustDelete();
            }
            else if (choose == 'n' || choose == 'N')
            {
                StaffPage();
            }
        }
        cout << endl << setw(35) << "         Do you want to delete another student?(y|n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
        {
            CustDelete();
        }
        else
        {
            CustList();
        }
    }
}

void CustSearch() {

    system("cls");
    std::string filePath = "staff.txt"; // Path to the admin file
    welcomeHeader(filePath);

    char choose;
    string name;

    cin.ignore(1, '\n');
    cout << setw(24) << "";
    cout << setw(24) << "Search Customer by name: ";
    getline(cin, name);

    string search_query = "select * from customer where Name like '%" + name + "%'";
    const char* q = search_query.c_str();
    qstate = mysql_query(conn, q);

    if (!qstate)
    {
        res = mysql_store_result(conn);


        cout << "\n";
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("|%-15s | %-25s | %-15s | %-20s | %-10s | %-20s |\n", "ID", "Name", "IC", "License", "No.Phone", "Email");
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {

            cout << setw(13) << "";
            printf("|%-15s | %-25s | %-15s | %-20s | %-10s |%-20s |\n", row[0], row[1], row[2], row[3], row[4], row[5]);
            cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        }


        cout << endl << setw(50) << "";
        cout << setw(10) << "Do you want to search other Customer? (y|n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
            CustSearch();
        else if (choose == 'n' || choose == 'N')
            CustMenu();
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }

}

void CustList() {

    system("cls");
    std::string filePath = "staff.txt"; // Path to the admin file
    welcomeHeader(filePath);

    qstate = mysql_query(conn, "SELECT * FROM customer");
    if (!qstate)
    {
        res = mysql_store_result(conn);


        cout << "\n";
        cout << setw(105) << "-------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("|%-3s | %-12s | %-15s | %-10s | %-13s | %-20s |\n", "ID", "Name", "IC", "License", "No.Phone", "Email");
        cout << setw(105) << "-------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {
            cout << setw(13) << "";
            printf("|%-3s | %-12s | %-15s | %-10s | %-13s | %-20s |\n", row[0], row[1], row[2], row[4], row[5], row[6]);
            cout << setw(105) << "-------------------------------------------------------------------------------------------\n";
        }
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
    std::cout << "\n";
    cout << setw(73) << "Press Enter To Back....";
    _getch();
    CustMenu();
}

//Car In Staff
void MenuCar() {
    int choose;
    system("cls");
    std::string filePath = "staff.txt"; // Path to the admin file
    welcomeHeader(filePath);

    cout << setw(61) << "1. Update Car\n\n";
    cout << setw(61) << "2. Delete Car\n\n";
    cout << setw(62) << "3. Display Car\n\n";
    cout << setw(61) << "4. Search Car\n\n";
    cout << setw(56) << "5. Back\n\n\n";
    cout << setw(79) << "-------------------------------------------\n" << endl;
    cout << setw(70) << "Please Enter Your Choice : ";
    cin >> choose;

    if (choose == 1)
    {
        CarUpdate();
    }
    else if (choose == 2)
    {
        CarDelete();
    }
    else if (choose == 3)
    {
        CarList();
    }
    else if (choose == 4)
    {
        CarSearch();
    }

    else if (choose == 5)
    {
        StaffPage();
    }
}

void CarUpdate() {
    system("cls");
    std::string filePath = "staff.txt"; // Path to the admin file
    welcomeHeader(filePath);

    string c_data;
    int choose;
    char option;

    qstate = mysql_query(conn, "SELECT * FROM car");

    if (!qstate)
    {

        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(18) << ""; cout << "------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(18) << "";
        printf("| %-3s | %-13s | %-15s | %-11s | %-15s | %-15s | \n", "ID", "Car Brand", "Car Name", "Plate", "Price", "Status");
        cout << setw(18) << ""; cout << "------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(18) << "";
            printf("| %-3s | %-13s | %-15s | %-11s | %-15s | %-15s |\n", row[0], row[1], row[2], row[3], row[4], row[5]);
            cout << setw(18) << ""; cout << "------------------------------------------------------------------------------------------\n";
        }
        cout << endl << endl << setw(73) << "  Choose ID : ";
        cin >> CarID;

        system("cls");
        std::string filePath = "admin.txt"; // Path to the admin file
        welcomeHeader(filePath);

        string findbyid_query = "SELECT * FROM car where carID='" + CarID + "'";
        const char* qi = findbyid_query.c_str();
        qstate = mysql_query(conn, qi);

        if (!qstate)
        {
            res = mysql_store_result(conn);
            cout << endl;
            while ((row = mysql_fetch_row(res)))
            {
                system("cls");
                std::string filePath = "admin.txt"; // Path to the admin file
                welcomeHeader(filePath);

                cout << "\n";
                cout << setw(82) << "---------------------------------------" << endl;
                cout << setw(60) << "Car Brand: " << row[1] << endl;
                cout << setw(59) << "Car Name: " << row[2] << endl;
                cout << setw(56) << "Plate: " << row[3] << endl;
                cout << setw(65) << "Price: " << row[4] << endl;
                cout << setw(57) << "Status: " << row[5] << endl;
                cout << setw(82) << "---------------------------------------" << endl << endl;
            }
        }
        else
        {
            cout << "Query Execution Problem!" << mysql_error(conn) << endl;
            system("pause");
        }

        cout << setw(71) << "1. Price\n\n";
        cout << setw(63) << "2. Status\n\n";
        cout << setw(61) << "3. Back\n\n";
        cout << setw(86) << "-------------------------------------------\n" << endl;
        cout << setw(70) << "Please Enter Your Choice : ";
        cin >> choose;

        cin.ignore(1, '\n');
        if (choose == 1)
        {
            cout << endl << endl << setw(71) << "1. Price: ";
            getline(cin, c_data);
            string update_query = "update car set Price='" + c_data + "' where CarID='" + CarID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (choose == 2)
        {
            int choice = 0;

            while (choice != 1 && choice != 2) {
                cout << endl << endl << setw(75) << "Update Car Status" << endl;
                cout << endl << endl << setw(75) << "1. Available" << endl;
                cout << endl << endl << setw(75) << "2. Rented" << endl;
                cout << endl << endl << setw(75) << "Enter your choice (1 or 2): ";
                cin >> choice;


                if (choice != 1 && choice != 2) {
                    cout << "Invalid choice. Please enter 1 or 2." << endl;
                }
            }

            string newStatus = (choice == 1) ? "Available" : "Rented";
            string updateCar_query = "UPDATE car SET Status = '" + newStatus + "' WHERE CarID='" + CarID + "'";
            const char* ca = updateCar_query.c_str();
            qstate = mysql_query(conn, ca);
        }

        else  if (choose == 3)


        {
            MenuCar();
        }

        cout << endl << setw(85) << "Do you want to update other Car? (y|n): ";
        cin >> option;

        if (option == 'y' || option == 'Y')
            CarUpdate();
        else
            CarList();
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
}

void CarDelete() {
    system("cls");
    std::string filePath = "staff.txt"; // Path to the admin file
    welcomeHeader(filePath);

    char choose;
    string id;

    qstate = mysql_query(conn, "select * from car");
    if (!qstate)
    {

        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(18) << ""; cout << "------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(25) << "";
        printf("| %-3s | %-13s | %-15s | %-11s | %-15s | %-15s | \n", "ID", "Car Brand", "Car Name", "Plate", "Price", "Status");
        cout << setw(18) << ""; cout << "------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {

            cout << setw(25) << "";
            printf("| %-3s | %-13s | %-15s | %-11s | %-15s | %-15s |\n", row[0], row[1], row[2], row[3], row[4], row[5]);
            cout << setw(18) << ""; cout << "------------------------------------------------------------------------------------------\n";
        }

        cout << endl << endl << setw(70) << "Enter ID to delete: ";
        cin >> id;

        bool found = false;

        string query = "select CarID from car";
        const char* qd = query.c_str();
        qstate = mysql_query(conn, qd);
        res = mysql_store_result(conn);

        if (!qstate)
        {
            while ((row = mysql_fetch_row(res)))
            {
                if (row[0] == id)
                {
                    found = true;
                    break;
                }
            }
        }

        if (found)
        {
            string query = "delete from car where CarID='" + id + "'";
            const char* qd = query.c_str();
            qstate = mysql_query(conn, qd);
            if (!qstate)
            {
                cout << endl << setw(75) << "Successfully deleted from car record" << endl;
            }
            else
            {
                cout << "Failed to Delete" << mysql_error(conn) << endl;
            }
        }

        else
        {
            cout << endl << setw(78) << "Please Enter a valid ID.Do You Want To Try Again? (y|n): ";
            cin >> choose;
            if (choose == 'y' || choose == 'Y')
            {
                DeleteCar();
            }
            else if (choose == 'n' || choose == 'N')
            {
                MenuCar();
            }
        }
        cout << endl << setw(78) << "Do you want to Delete another Car?(y|n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
        {
            DeleteCar();
        }
        else
        {
            ListCar();
        }
    }
}

void CarList() {
    system("cls");
    std::string filePath = "staff.txt"; // Path to the admin file
    welcomeHeader(filePath);

    qstate = mysql_query(conn, "SELECT * FROM car");
    if (!qstate)
    {
        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(109) << "---------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(15) << "";
        printf("| %-3s | %-13s | %-15s | %-11s | %-15s | %-15s | \n", "ID", "Car Brand", "Car Name", "Plate", "Price", "Status");
        cout << setw(109) << "---------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {

            cout << setw(15) << "";
            printf("| %-3s | %-13s | %-15s | %-11s | %-15s | %-15s |\n", row[0], row[1], row[2], row[3], row[4], row[5]);
            cout << setw(109) << "---------------------------------------------------------------------------------------------\n";
        }

    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
    cout << endl << setw(70) << "Press Enter To Back....";
    _getch();
    MenuCar();

}

void CarSearch() {
    system("cls");
    std::string filePath = "staff.txt"; // Path to the admin file
    welcomeHeader(filePath);

    char choose;
    string Brand;

    cout << "\n";
    cin.ignore(1, '\n');
    cout << setw(71) << "Search Car by Brand: ";
    getline(cin, Brand);

    string searchCust_query = "SELECT car.*, carcat.CarBrand, carcat.Model FROM car JOIN carcat ON car.CatID = carcat.CatID WHERE CarBrand like '" + Brand + "%'";
    const char* sc = searchCust_query.c_str();
    qstate = mysql_query(conn, sc);
    if (!qstate)
    {
        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(99) << "-------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(25) << "";
        printf("| %-3s | %-13s | %-15s | %-11s | %-15s | %-15s | \n", "ID", "Car Brand", "Car Name", "Plate", "Price", "Status");
        cout << setw(99) << "-------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(25) << "";
            printf("| %-3s | %-13s | %-15s | %-11s | %-15s | %-15s |\n", row[0], row[1], row[2], row[3], row[4], row[5]);
            cout << setw(99) << "-------------------------------------------------------------------------\n";
        }

        cout << endl << setw(75) << "Do you want to search other Car? (y|n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
            CarSearch();
        else if (choose == 'n' || choose == 'N')
            MenuCar();
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }

}

void UpdateRentalS() {
    db_response::ConnectionFunction();
    system("cls");
    std::string filePath = "rent.txt"; // Path to the admin file
    welcomeHeader(filePath);

    string rentID, rentStatus;

    // Display the table with RentID, Customer IC, Car Plate, and Rent Status
    qstate = mysql_query(conn, "SELECT rent.RentID, customer.IC, car.Plate, rent.Rent_Status FROM rent "
        "JOIN customer ON rent.CustID = customer.CustID "
        "JOIN car ON rent.CarID = car.CarID");

    table();  // Assuming this is the function that initializes the table display
    std::cout << "\n\n" << std::setw(65) << "Enter Rental ID to update: ";
    std::cin >> rentID;

    // Validate that the rental ID exists
    bool validRental = false;
    string query = "SELECT 1 FROM rent WHERE RentID = '" + rentID + "'";

    if (mysql_query(conn, query.c_str()) != 0) {
        std::cerr << "Error executing query: " << mysql_error(conn) << std::endl;
        RentalInAdmin();  // Return to admin menu on error
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    if (row) {
        validRental = true;
    }
    mysql_free_result(res);

    if (validRental) {
        // Display options for rent status
        std::cout << "\n\n" << std::setw(60) << "Choose New Rent Status:\n";
        std::cout << "1. Approved\n";
        std::cout << "2. Returned\n";
        std::cout << "3. Pending\n";
        std::cout << std::setw(60) << "Enter your choice (1-3): ";

        int choice;
        std::cin >> choice;

        // Map choice to status
        switch (choice) {
        case 1:
            rentStatus = "Approved";
            break;
        case 2:
            rentStatus = "Returned";
            break;
        case 3:
            rentStatus = "Pending";
            break;
        default:
            std::cerr << "Invalid choice, please enter a number between 1 and 3." << std::endl;
            UpdateRentalS();
            return;
        }

        // Update the rent status in the database
        query = "UPDATE rent SET Rent_Status = '" + rentStatus + "' WHERE RentID = '" + rentID + "'";

        if (mysql_query(conn, query.c_str()) != 0) {
            std::cerr << "Error updating rent status: " << mysql_error(conn) << std::endl;
        }
        else {
            std::cout << std::setw(70) << "Rental status updated successfully to: " << rentStatus << std::endl;
        }

        // Ask if the admin wants to return to the admin menu
        char backChoice;
        std::cout << "\nDo you want to go back to the Staff Menu? (y/n): ";
        std::cin >> backChoice;

        if (backChoice == 'y' || backChoice == 'Y') {
            DisplayRent();  // Function that shows the main admin menu
        }
        else {
            std::cout << "Thank you for using the admin system. Exiting now..." << std::endl;
            StaffPage();  // Or use return to exit gracefully
        }
    }
    else {
        std::cerr << "Rental ID not found!" << std::endl;
        UpdateRentalA();  // Return to the admin menu on error
    }

    mysql_free_result(res);
}

void DisplayRent() {
    db_response::ConnectionFunction();
    system("cls");
    std::string filePath = "rent.txt"; // Path to the admin file
    welcomeHeader(filePath);

    // Display the table with RentID, Customer IC, Car Plate, and Rent Status
    qstate = mysql_query(conn, "SELECT rent.RentID, customer.IC, car.Plate, rent.Rent_Status FROM rent "
        "JOIN customer ON rent.CustID = customer.CustID "
        "JOIN car ON rent.CarID = car.CarID");

    table();  // Assuming this is the function that initializes the table display
    cout << "\n" << setw(75) << "Press Enter To Back....";
    _getch();
    StaffPage();

}


int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

//Empty Table For Category
void table() {
    res = mysql_store_result(conn);

    int col_count = mysql_field_count(conn);

    std::vector<std::vector<std::string>> data;
    std::vector<int> max_widths(col_count, 0);

    // Fetch data and calculate maximum widths
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        std::vector<std::string> row_data;
        for (int i = 0; i < col_count; ++i) {
            const char* data_str = row[i];
            row_data.push_back(data_str);
            int width = strlen(data_str);
            if (width > max_widths[i]) {
                max_widths[i] = width;
            }
        }
        data.push_back(row_data);
    }

    // Calculate total table width
    int table_width = 0;
    for (int width : max_widths) {
        table_width += width + 10; // Add spacing and padding
    }
    table_width += col_count + 1; // Account for border characters

    // Get console width and calculate left padding
    int console_width = getConsoleWidth();
    int padding = (console_width - table_width) / 2;
    if (padding < 0) padding = 0;

    // Print table header
    printf("%*s", padding, ""); // Add padding
    for (int i = 0; i < col_count; ++i) {
        MYSQL_FIELD* field = mysql_fetch_field(res);
        printf("        %-*s ", max_widths[i], field->name);
    }
    printf("\n\n");

    // Print table data
    for (const auto& row : data) {
        printf("%*s", padding, ""); // Add padding
        for (int i = 0; i < col_count; ++i) {
            printf("      | %-*s ", max_widths[i] + 2, row[i].c_str());
        }
        printf("|\n");
    }
}

void CustLogin() {

    int menu;
    system("cls");
    std::string filePath = "customer.txt"; // Path to the admin file
    welcomeHeader(filePath);

    string custpassword;
    string custusername;
    cout << setw(63) << "Username: ";
    cin >> custusername;

    // Validate username input
    if (custusername.empty())
    {
        cout << "Invalid username! Please try again." << endl;
        CustLogin(); // Repeat the function to allow re-entering the username
        return;
    }

    cout << "\n\n" << setw(63) << "Password: ";
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == 8) {
            if (!custpassword.empty()) {
                custpassword.pop_back();
                cout << "\b \b";
            }
        }
        else {
            custpassword.push_back(ch);
            cout << '*';
        }
    }

    // Validate password input
    if (custpassword.empty())
    {
        cout << endl << "Invalid password! Please try again." << endl;
        CustLogin(); // Repeat the function to allow re-entering the password
        return;
    }

    string checkUser_query = "SELECT * , name FROM customer WHERE IC = '" + custusername + "' AND Password = '" + custpassword + "'";
    const char* cu = checkUser_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count == 1)
        {
            string user, pw;
            while (row = mysql_fetch_row(res))
            {
                CustID = row[0];
                user = row[2];
                pw = row[3];
                CustName = row[1];
            }

            if (user == custusername && pw == custpassword)
                CustPage();
                _getch();
        }

        char c;
        cout << "\n\n\n" << setw(86) << "Invalid username or password. Want to try again? (y/n): ";
        cin >> c;
        if (c == 'y' || c == 'Y')
            CustLogin();
        else
            main();

    }
    else
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
}

void CustPage() {

    std::string answer;
    int menu;
    system("cls");
    std::string filePath = "customer.txt"; // Path to the admin file
    welcomeHeader(filePath);

    cout << setw(55) << " Login as: " << CustName << "\n\n\n";
    cout << setw(65) << "1. Rent Car\n\n" << endl;
    cout << setw(61) << "2. Display Rental\n\n" << endl;
    cout << setw(56) << "3. Logout\n\n" << endl;
    cout << setw(79) << "-----------------------------------------\n" << endl;
    cout << setw(70) << "Please Enter Your Choice : ";
    cin >> menu;

    switch (menu)
    {
    case 1:
        RentalInCustomer();
        break;

    case 2:
        DisplayRental();
        break;
    case 3:
        system("cls");
        std::cout << "\n                                   Do you want to logout the system? (yes/no): ";
        std::cin >> answer;

        for (char& c : answer) {
            c = std::tolower(c);
        }

        system("cls");

        if (answer == "yes" || answer == "y") {
            ifstream inputFile("thank.txt"); //displaying thank you ASCII image text on output screen fn1353
            if (!inputFile) {
                cout << "Cannot open input file.\n";
            }
            char str[1000];
            while (inputFile) {
                inputFile.getline(str, 1000);
                if (inputFile)
                    cout << str << endl;
            }
            inputFile.close();
            std::cout << "\n Logged out successfully\n" << std::endl;
            std::cin.get();
            exit(0); // Terminate the program with exit code 0 (indicating a successful exit)
        }
        else {
            CustPage();
        }
    }
}

void RentalInCustomer() {

    db_response::ConnectionFunction();
    system("cls");
    std::string filePath = "rent.txt"; // Path to the admin file
    welcomeHeader(filePath);

    // Connection setup and initializations...
    std::string IC, carID, rentalDateStr, returnDateStr, pickUpTimeStr = "10:00", dropOffTimeStr = "10:00";

    // Prompt customer to enter their IC
    std::cout << "\n\n" << std::setw(65) << "Enter Your Customer ID : ";
    std::cin >> IC;

    // Validate customer ID exists
    std::string query = "SELECT CustID  FROM customer WHERE IC = '" + IC + "'";
    if (mysql_query(conn, query.c_str()) != 0) {
        std::cerr << "Error executing query: " << mysql_error(conn) << std::endl;
        RentalInCustomer();
        return;
    }
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        std::cerr << "\nInvalid Customer IC. Please try again.\n";
        mysql_free_result(res);
        RentalInCustomer();
        return;
    }

    // Retrieve the CustID from the result
    std::string custID = row[0];
    mysql_free_result(res);

    // Check if the customer has an active rental
    query = "SELECT RentID FROM rent WHERE CustID = '" + custID + "' AND Rent_Status != 'Returned'";
    if (mysql_query(conn, query.c_str()) != 0) {
        std::cerr << "Error checking active rentals: " << mysql_error(conn) << std::endl;
        RentalInCustomer();
        return;
    }
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    if (row) {
        std::cerr << "\nYou already have an active rental. Please return the car before renting another.\n";
        mysql_free_result(res);
        return;
    }
    mysql_free_result(res);

    // If no active rentals, proceed with car selection
    std::cout << "\nWelcome! Let's proceed with car selection...\n";
    query = "SELECT car.*, carcat.CarBrand, carcat.Model FROM car JOIN carcat ON car.CatID = carcat.CatID WHERE car.Status = 'Available'";
    if (mysql_query(conn, query.c_str()) != 0) {
        std::cerr << "\nError retrieving car list: " << mysql_error(conn) << std::endl;
        RentalInCustomer();
        return;
    }
    res = mysql_store_result(conn);
    std::cout << "\nAvailable Cars:\n";
    std::cout << std::setw(109) << "---------------------------------------------------------------------------------------------\n";
    std::cout << std::setw(15) << "";
    printf("| %-3s | %-10s | %-13s | %-15s | %-15s | %-18s | \n", "ID", "Car Brand", "Model", "Plate", "Price", "Status");
    std::cout << std::setw(109) << "---------------------------------------------------------------------------------------------\n";

    while ((row = mysql_fetch_row(res))) {
        std::cout << std::setw(15) << "";
        printf("| %-3s | %-10s | %-13s | %-15s | %-15s | %-18s | \n", row[0], row[5], row[6], row[1], row[2], row[3]);
        std::cout << std::setw(109) << "---------------------------------------------------------------------------------------------\n";
    }
    mysql_free_result(res);

    // Prompt customer to choose a car
    std::cout << "\n\n" << std::setw(60) << "Choose Car ID: ";
    std::cin.ignore(1, '\n');
    std::getline(std::cin, carID);

    // Validate car ID exists and proceed with rental
    query = "SELECT car.Price, car.Status, car.Plate, carcat.CarBrand, carcat.Model FROM car JOIN carcat ON car.CatID = carcat.CatID WHERE CarID ='" + carID + "' LIMIT 1";
    if (mysql_query(conn, query.c_str()) != 0) {
        std::cerr << "\nError executing query: " << mysql_error(conn) << std::endl;
        RentalInCustomer();
        return;
    }
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    if (!row) {
        std::cerr << "\nCar with ID " << carID << " not found or unavailable. Please try again.\n";
        mysql_free_result(res);
        RentalInCustomer();
        return;
    }

    float carPrice = atof(row[0]);
    std::string carStatus = row[1], carPlate = row[2], carBrand = row[3], carModel = row[4];
    mysql_free_result(res);

    if (carStatus != "Available") {
        std::cerr << "\nThe selected car is currently unavailable. Please choose a different car.\n";
        RentalInCustomer();
        return;
    }

    // Proceed with rental details
    std::cout << "\n\n" << std::setw(77) << "Enter Rental Date (YYYY-MM-DD): ";
    std::cin >> rentalDateStr;
    std::cout << "\n" << std::setw(77) << "Enter Return Date (YYYY-MM-DD): ";
    std::cin >> returnDateStr;

    std::tm rentalDate = {}, returnDate = {};
    if (!stringToTm(rentalDateStr, rentalDate) || !stringToTm(returnDateStr, returnDate)) {
        std::cerr << "\nInvalid date format. Please use YYYY-MM-DD.\n";
        RentalInCustomer();
        return;
    }

    // Validate rental dates
    if (!isDateValid(rentalDate, returnDate)) {
        std::cerr << "\nInvalid dates entered. Please ensure the rental date is today or later, and the return date is after the rental date.\n";
        RentalInCustomer();
        return;
    }

    // Calculate rental cost and confirm rental
    int totalDays = calculateDays(rentalDateStr, returnDateStr);
    float totalPrice = totalDays * carPrice;
    std::cout << "\nRental Details:\n";
    std::cout << "Car Brand: " << carBrand << ", Model: " << carModel << ", Plate: " << carPlate << "\n";
    std::cout << "Rental Period: " << totalDays << " days\n";
    std::cout << "Total Price: RM " << totalPrice << "\n";
    std::cout << "\nConfirm Rental (y/n): ";
    char confirm;
    std::cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        std::string rentStatus = "Pending";
        query = "INSERT INTO rent (CustID, CarID, Rent_Date, Return_Date, TotalPrice, Rent_Status) VALUES ('" + custID + "', '" + carID + "', '" + rentalDateStr + "','" + returnDateStr + "', " + std::to_string(totalPrice) + ", '" + rentStatus + "')";
        if (mysql_query(conn, query.c_str()) != 0) {
            std::cerr << "\nError processing rental: " << mysql_error(conn) << std::endl;
        }
        else {
            std::cout << "\nRental confirmed! Please proceed to payment at the counter.\n";
            DisplayRental();

        }
    }
    else {
        std::cout << "\nRental was not confirmed. Returning to the main menu.\n";
        CustPage();
    }
}

void DisplayRental() {
    system("cls");
    std::string filePath = "customer.txt"; // Path to the admin file
    welcomeHeader(filePath);

    // Modified query to fetch only the necessary columns
    std::string query = "SELECT rent.RentID, customer.IC, rent.Rent_Date, rent.Return_Date, carcat.Model, rent.TotalPrice, rent.Rent_Status "
        "FROM rent "
        "JOIN car ON rent.CarID = car.CarID "
        "JOIN carcat ON car.CatID = carcat.CatID "
        "JOIN customer ON rent.CustID = customer.CustID";  // Join with customer to fetch IC

    qstate = mysql_query(conn, query.c_str());

    if (!qstate)
    {
        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(114) << "--------------------------------------------------------------------------------------------------------------------\n";
        // Data show in box
        cout << setw(13) << "";
        printf("|%-15s | %-15s | %-20s | %-10s | %-15s | %-10s | %-10s |\n",
            "Rental ID", "Customer IC", "Rental Date", "Return Date", "Car Model", "Total Price", "Rental Status");
        cout << setw(114) << "--------------------------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {
            cout << setw(13) << "";
            printf("|%-15s | %-15s | %-20s | %-10s | %-15s | %-10s | %-10s |\n",
                row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
            cout << setw(114) << "--------------------------------------------------------------------------------------------------------------------\n";
        }
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }

    cout << "\n" << setw(75) << "Press Enter To Back....";
    _getch();
    CustPage();
}
