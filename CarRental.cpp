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



using namespace std;


int qstate;
MYSQL* conn;
MYSQL_STMT stmt;
MYSQL_ROW row;
MYSQL_RES* res;
string items[20000000];
string adminId, Employee_ID, CustID, CarID, CatID , RentalID, IC, StaffID, StaffName;

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
//void UpdateCat();//Car Category
//void DeleteCat();


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

void CarSearch();
void AddRental();
void DeleteRental();

//just empty table
void table();



string getInputOfLength(int, string, string);
string getPassword();
string getValidEmail();


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
    cout << setw(79) << "              Welcome To Car Rental Management System\n\n\n";
    cout << setw(69) << "1. Admin\n\n\n";
    cout << setw(69) << "2. Staff\n\n\n";
    cout << setw(72) << "3. Customer\n\n\n";
    cout << setw(69) << "4. Exit\n\n\n\n";
    cout << setw(87) << "----------------------------------------------\n" << endl;
    cout << setw(75) << "Please Enter Your Choice : ";
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
        //CustLogin();
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

    cout << setw(85) << "\n\n\t\t\t\t\t\tUsername: ";
    cin >> adminusername;

    // Validate username input
    if (adminusername.empty())
    {
        cout << "\n\n\n" << setw(80) << "Invalid username! Please try again." << endl;
        adminlogin(); // Repeat the function to allow re-entering the username
        return;
    }

    cout << setw(63) << "\n\n\t\t\t\t\t\tPassword:";
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
    int menu;
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);
    cout << "\n\n\n";
    cout << setw(58) << "1. Staff\n\n";
    cout << setw(56) << "2. Category\n\n";
    cout << setw(56) << "3. Car\n\n";
    cout << setw(61) << "4. Customer\n\n";
    cout << setw(61) << "5. Rent Car\n\n";
    cout << setw(59) << "6. Report\n\n";
    cout << setw(59) << "7. Logout\n\n";
    cout << setw(79) << "-------------------------------------------\n" << endl;
    cout << setw(70) << "Please Enter Your Choice : ";
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

        //Rentmenu();
        break;

    case 6:
        // ReportMenu();
        break;

    case 7:

        system("cls");
        main();
        break;
    }

}//end admin menu

//Manage Staff In Admin
void StaffMenu() {
    int choose;
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);
    cout << setw(200) << " " << endl;
    cout << setw(74) << "1. Register Staff\n\n";
    cout << setw(72) << "2. Update Staff\n\n";
    cout << setw(72) << "3. Delete Staff\n\n";
    cout << setw(73) << "4. Display Staff\n\n";
    cout << setw(72) << "5. Search Staff\n\n";
    cout << setw(65) << "6. Back\n\n\n";
    cout << setw(86) << "-------------------------------------------\n" << endl;
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
    cout << setw(97) << "|--------------------------------------------------------------------|" << endl;
    cout << setw(97) << "|         PASSWORD                                                   |" << endl;
    cout << setw(97) << "|       - A password must have at least eight characters.            |" << endl;
    cout << setw(97) << "|       - A password must consist of only letters and digits.        |" << endl;
    cout << setw(97) << "|       - A password must contain at least two digits.               |" << endl;
    cout << setw(97) << "|                                                                    |" << endl;
    cout << setw(97) << "|--------------------------------------------------------------------|" << "\n\n" << endl;
    //cout << setw(150) << "||" << endl;
    //cout << setw(150) << "||" << endl;
    //cout << setw(150) << "||" << endl;

    cout << setw(64) << "Enter Username:";
    cin.ignore(1, '\n');
    getline(cin, Username);
    Password = getPassword();

    string update_query = "update staff set status password ='" + Password + "'";
    const char* q = update_query.c_str();
    qstate = mysql_query(conn, q);

    cout << "\n\n" << setw(61) << "Enter Employee Id: ";
    cin.ignore();
    getline(cin, EmployeeID);

    cout << "\n\n" << setw(61) << "Enter Name: ";
    getline(cin, Name);

    Email = getValidEmail();

    Phone = getInputOfLength(10, "Phone number must be 10 digits!", "Enter Phone: ");

    string checkUser_query = "select * from staff where username = '" + Username + "'";
    const char* cu = checkUser_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count == 1)
        {
            cout << "Username already exists. Press Enter to Try Again...";
            _getch();
            RegStaff();
        }
        else
        {
            string insertStaff_query = "insert into staff (username, password,employee_id,name,email,phone) values ('" + Username + "', '" + Password + "','" + EmployeeID + "','" + Name + "','" + Email + "','" + Phone + "')";
            const char* q = insertStaff_query.c_str();
            qstate = mysql_query(conn, q);

            if (!qstate)
            {
                cout << "\n\n\n" << setw(75) << "You have been registered.\n\n";
                cout << setw(80) << "Do you want to add other staff? (y|n): ";
                cin >> option;

                if (option == 'y' || option == 'Y')
                    RegStaff();
                else
                    ListStaff();
                _getch();
            }
            else
            {
                cout << "Query Execution Problem!" << mysql_error(conn) << endl;
            }
        }

    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
}

void UpdateStaff()
{
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
            cout << endl << endl << setw(35) << " Choose Employee ID : ";
            cin >> Employee_ID;

            // Query to check if Employee_ID exists
            string findbyid_query = "SELECT * FROM staff WHERE employee_id='" + Employee_ID + "'";
            const char* qi = findbyid_query.c_str();
            qstate = mysql_query(conn, qi);

            if (!qstate)
            {
                res = mysql_store_result(conn);
                if (mysql_num_rows(res) > 0) // If Employee_ID is valid
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
                    cout << endl << setw(92) << "No employee found with the given ID!" << endl;
                    cout << endl << setw(92) << "Do you want to try again? (y/n): ";
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

        cout << endl << setw(86) << "Do you want to update other staff? (y/n): ";
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
    qstate = mysql_query(conn, "select * from staff");
    if (!qstate)
    {
        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(112) << "---------------------------------------------------------------------------------------------------\n";
        //data show in box 
        cout << setw(12) << "";
        printf("| %-3s | %-13s | %-25s | %-10s | %-20s |\n", "No","Employee ID", "Name", "Email", "Phone");
        cout << setw(112) << "---------------------------------------------------------------------------------------------------\n";
    
        while ((row = mysql_fetch_row(res)))
        {
            cout << setw(12) << "";
            printf("| %-3s | %-13s | %-25s | %-10s | %-20s |\n", row[0], row[3], row[4], row[5], row[6]);
            cout << setw(112) << "---------------------------------------------------------------------------------------------------\n";
        }

        cout << endl << endl << setw(70) << "Enter Number to delete: ";
        cin >> id;
        bool found = false;

        string query = "select staff_id from staff";
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
            string query = "delete from staff where staff_id='" + id + "'";
            const char* qd = query.c_str();
            qstate = mysql_query(conn, qd);
            if (!qstate)
            {
                cout << endl << setw(78) << "Successfully deleted from staff record" << endl;
            }
            else
            {
                cout << "Failed to Delete" << mysql_error(conn) << endl;
            }
        }

        else
        {
            cout << endl << setw(92) << "Please Enter a valid Number. Do You Want To Try Again? (y|n): ";
            cin >> choose;
            if (choose == 'y' || choose == 'Y')
            {
                DeleteStaff();
            }
            else if (choose == 'n' || choose == 'N')
            {
                StaffMenu();
            }
        }
        cout << endl << setw(78) << "         Do you want to Delete another Staff? (y|n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
        {
            DeleteStaff();
        }
        else
        {
            ListStaff();
        }
    }

}//end

void ListStaff() {
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);
    qstate = mysql_query(conn, "SELECT * FROM staff");
    if (!qstate)
    {
        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(112) << "---------------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(12) << "";
        printf("| %-13s | %-20s | %-18s | %-13s |\n", "Employee ID", "Name", "Email", "Phone");
        cout << setw(112) << "---------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {

            cout << setw(12) << "";
            printf("| %-13s | %-20s | %-18s | %-13s |\n", row[3], row[4], row[5], row[6]);
            cout << setw(112) << "---------------------------------------------------------------------------------------------------\n";
        }


    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
    cout << setw(70) << "Press Enter To Back....";
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
    cout << endl << setw(72) << "Search Staff by name: ";
    getline(cin, name);

    string search_query = "select * from staff where name like '%" + name + "%'";
    const char* q = search_query.c_str();
    qstate = mysql_query(conn, q);

    if (!qstate)
    {
        res = mysql_store_result(conn);

        cout << setw(109) << "---------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(15) << "";
        printf("| %-13s | %-10s | %-20s | %-25s |\n", "Employee ID", "Name", "Email", "Phone");
        cout << setw(109) << "---------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {

            cout << setw(15) << "";
            printf("| %-13s | %-10s | %-20s | %-25s |\n", row[3], row[4], row[5], row[6]);
            cout << setw(109) << "---------------------------------------------------------------------------------------------\n";
        }
        cout << endl << setw(78) << "Do you want to search other staff?(y|n): ";
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
        cout << setw(64) << prompt;
        cin >> userInput;

        if (userInput.length() == requiredLength) {
            break; 
        }
        else {
            cout << setw(64) << warning << endl;
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
        cout << "\n\n" << setw(64) << "Enter password: ";
        cin >> userInput;

        int sz = userInput.length();

        if (sz >= 8 && count_if(userInput.begin(), userInput.end(), checkForDigits) >= 2) {
            return userInput;
        }
        cout << setw(24) << "" << "Invalid password. Must be 8 in length with 2 digits" << endl;
    } while (true);
}

bool isValidEmail(const string& email) {
    size_t atPosition = email.find('@');
    return (atPosition != string::npos);
}

string getValidEmail() {
    string userEmail;

    do {
        cout << setw(64) << "Enter your email : ";
        cin >> userEmail;

        if (userEmail.find('@') != string::npos) {
            break;  // Valid email, exit the loop
        }
        else {
            cout << "Invalid email. It must contain the '@' symbol." << endl;
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
    cout << setw(58) << "1. Add Category\n\n";
    cout << setw(56) << "2. Update Category\n\n";
    cout << setw(61) << "3. Delete Category\n\n";
    cout << setw(59) << "4. Back\n\n";
    cout << setw(79) << "-------------------------------------------\n" << endl;
    cout << setw(70) << "Please Enter Your Choice : ";
    cin >> menu;

    switch (menu)
    {
    case 1:

        AddCat();
        break;

    case 2:

        //UpdateCat();
        break;

    case 3:

        //DeleteCat();
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
    cout << "\n\n" << setw(69) << "Enter Car Name: ";
    getline(cin, Model);

    if (!qstate)
    {
        string insertCar_query = "INSERT into carcat (CarBrand,Model) values ('" + CarBrand + "', '" + Model + "') ";
        const char* ca = insertCar_query.c_str();
        qstate = mysql_query(conn, ca);

        if (!qstate)
        {
            cout << "\n\n\n" << setw(75) << "Succesfully Insert!!!!.\n\n";
            cout << setw(79) << "Do you want to add other new Car Category? (y/n): ";
            cin >> option;

            if (option == 'y' || option == 'Y')
            {
                AddCat();
            }
            else
            {

                AdminMenu();
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

void CarMenu() {
    int choose;
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    cout << setw(300) << " " << endl;
    cout << setw(58) << "1. Add Car\n\n";
    cout << setw(61) << "2. Update Car\n\n";
    cout << setw(61) << "3. Delete Car\n\n";
    cout << setw(62) << "4. Display Car\n\n";
    cout << setw(61) << "5. Search Car\n\n";
    cout << setw(56) << "6. Back\n\n\n";
    cout << setw(79) << "-------------------------------------------\n" << endl;
    cout << setw(70) << "Please Enter Your Choice : ";
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
    string PricePerHour = "";
    string Availability_status = "Available";

    qstate = mysql_query(conn, "SELECT * FROM carcat");

    table();

    cout << "\n\n" << setw(68) << "Car Category: ";
    cin.ignore(1, '\n');
    getline(cin, CatID);
    cout << "\n\n" << setw(71) << "Car Plate Number:";
    getline(cin, Plate);
    cout << "\n\n" << setw(68) << "Enter Car Price Per Hour : ";
    getline(cin, PricePerHour);


    if (!qstate)
    {
        string insertCar_query = "INSERT into car (Plate,PricePerHour,Status,CatID) values ('" + Plate + "', '" + PricePerHour + "', '" + Availability_status + "', '" + CatID + "' ) ";
        const char* ca = insertCar_query.c_str();
        qstate = mysql_query(conn, ca);

        if (!qstate)
        {
            cout << "\n\n\n" << setw(75) << "Succesfully Insert!!!!.\n\n";
            cout << setw(79) << "Do you want to add other new car? (y/n): ";
            cin >> option;

            if (option == 'y' || option == 'Y')
            {
                AddCar();
            }
            else
            {

                ListCar();
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
        cout << setw(18) << ""; cout << "------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(18) << "";
        printf("| %-3s | %-13s | %-15s | %-11s | %-15s | %-15s | \n", "ID", "Car Brand", "Car Name", "Plate", "Price Per Hour", "Status");
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
                cout << setw(65) << "Price Per Hour: " << row[4] << endl;
                cout << setw(57) << "Status: " << row[5] << endl;
                cout << setw(82) << "---------------------------------------" << endl << endl;
            }
        }
        else
        {
            cout << "Query Execution Problem!" << mysql_error(conn) << endl;
            system("pause");
        }

        cout << setw(66) << "1. Car Brand\n\n";
        cout << setw(65) << "2. Car Name\n\n";
        cout << setw(62) << "3. Plate\n\n";
        cout << setw(71) << "4. Price Per Hour\n\n";
        cout << setw(63) << "5. Status\n\n";
        cout << setw(61) << "6. Back\n\n";
        cout << setw(86) << "-------------------------------------------\n" << endl;
        cout << setw(70) << "Please Enter Your Choice : ";
        cin >> choose;

        cin.ignore(1, '\n');
        if (choose == 1)
        {
            cout << endl << endl << setw(66) << "1. Car Brand: ";
            getline(cin, c_data);
            string update_query = "update car set Brand ='" + c_data + "' where CarID='" + CarID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (choose == 2)
        {
            cout << endl << endl << setw(65) << "2. Car Name: ";
            getline(cin, c_data);
            string update_query = "update car set CarName='" + c_data + "' where CarID='" + CarID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (choose == 3)
        {
            cout << endl << endl << setw(62) << "3. Plate: ";
            getline(cin, c_data);
            string update_query = "update car set Plate='" + c_data + "' where CarID='" + CarID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (choose == 4)
        {
            cout << endl << endl << setw(71) << "4. Price Per Hour: ";
            getline(cin, c_data);
            string update_query = "update car set PricePerHour='" + c_data + "' where CarID='" + CarID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else  if (choose == 5)
        {
            cout << endl << endl << setw(63) << "5. Status: ";
            getline(cin, c_data);
            string update_query = "update car set Status='" + c_data + "' where CarID='" + CarID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else  if (choose == 6)
        {
            CarMenu();
        }

        cout << endl << setw(85) << "Do you want to update other Car? (y|n): ";
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

    qstate = mysql_query(conn, "select * from car");
    if (!qstate)
    {

        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(18) << ""; cout << "------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(25) << "";
        printf("| %-3s | %-13s | %-15s | %-11s | %-15s | %-15s | \n", "ID", "Car Brand", "Car Name", "Plate", "Price Per Hour", "Status");
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
                CarMenu();
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
        cout << setw(109) << "---------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(15) << "";
        printf("| %-3s | %-13s | %-15s | %-11s | %-15s | \n", "Car Brand", "Model", "Plate", "Price Per Hour", "Status");
        cout << setw(109) << "---------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {

            cout << setw(15) << "";
            printf("| %-3s | %-13s | %-15s | %-11s | %-15s | \n", row[5], row[6], row[1], row[2], row[3]);
            cout << setw(109) << "---------------------------------------------------------------------------------------------\n";
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

    cout << "\n";
    cin.ignore(1, '\n');
    cout << setw(71) << "Search Car by Plate: ";
    getline(cin, Plate);

    string searchCust_query = "SELECT car.*, carcat.CarBrand, carcat.Model FROM car JOIN carcat ON car.CatID = carcat.CatID WHERE Plate like '" + Plate + "%'";
    const char* sc = searchCust_query.c_str();
    qstate = mysql_query(conn, sc);

    if (!qstate)
    {
        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(99) << "-------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(25) << "";
        printf("| %-3s | %-13s | %-15s | %-11s | %-15s | \n", "Car Brand", "Model", "Plate", "Price Per Hour", "Status");
        cout << setw(99) << "-------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(25) << "";
            printf("| %-3s | %-13s | %-15s | %-11s | %-15s | \n", row[5], row[6], row[1], row[2], row[3]);
            cout << setw(99) << "-------------------------------------------------------------------------\n";
        }

        cout << endl << setw(75) << "Do you want to search other Car? (y|n): ";
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
    cout << setw(70) << "1. Register Customer\n\n";
    cout << setw(68) << "2. Update Customer\n\n";
    cout << setw(68) << "3. Delete Customer\n\n";
    cout << setw(69) << "4. Display Customer\n\n";
    cout << setw(68) << "5. Search Customer\n\n";
    cout << setw(58) << "6. Back\n\n\n";
    cout << setw(79) << "-------------------------------------------\n" << endl;
    cout << setw(70) << "Please Enter Your Choice : ";
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
    string License = "";
    string Phone = "";
    string Email = "";

    cout << "\n" << setw(64) << "Enter Name  : ";
    cin.ignore(1, '\n');
    getline(cin, Name);
    Phone = getInputOfLength(10, "Phone number must be 10 digits long.", "Enter Phone : ");
    IC = getInputOfLength(12, "IC No must be 12 digits long", "Enter IC Num.: ");
    cout << setw(64) << "Enter Lisence : ";
    cin.ignore();
    getline(cin, License);
    Email = getValidEmail();

    if (!qstate)
    {
        string insertMember_query = "insert into customer (Name,IC,License,Phone,Email) values ('" + Name + "', '" + IC + "','" + License + "','" + Phone + "','" + Email + "')";
        const char* q = insertMember_query.c_str();
        qstate = mysql_query(conn, q);

        if (!qstate)
        {
            cout << "\n\n\n" << setw(76) << "You have been registered.\n\n";
            cout << setw(81) << "Do you want to add other Customer? (y|n): ";
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

    qstate = mysql_query(conn, "select * from customer");

    if (!qstate)
    {

        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", "IC", "Name", "License", "Email", "No.Phone");
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(13) << "";
            printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", row[2], row[1], row[3], row[5], row[4]);
            cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        }
        cout << endl << endl << setw(42) << "  Choose Customer IC : ";
        cin >> IC;

        system("cls");

        string findbyid_query = "SELECT * FROM customer where IC='" + IC + "'";
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


                cout << setw(82) << "---------------------------------------" << endl;
                cout << setw(54) << "No. Phone:" << row[4] << endl;
                cout << setw(50) << "Email:" << row[5] << endl;
                cout << setw(82) << "---------------------------------------" << endl << endl;
            }
        }
        else
        {
            cout << "Query Execution Problem!" << mysql_error(conn) << endl;
            system("pause");
        }

        cout << setw(70) << "1. No. Phone\n\n";
        cout << setw(66) << "2. Email\n\n";
        cout << setw(86) << "-------------------------------------------\n" << endl;
        cout << setw(79) << "Please Enter Your Choice : ";
        cin >> choose;

        cin.ignore(1, '\n');
        if (choose == 1)
        {
            cout << endl << endl << setw(70) << "1. No. Phone: ";
            getline(cin, data);
            string update_query = "update customer set Phone='" + data + "' where IC='" + IC + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }

        else if (choose == 2)
        {
            cout << endl << endl << setw(66) << "2. Email: ";
            getline(cin, data);
            string update_query = "update customer set Email='" + data + "' where IC='" + IC + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }


        cout << endl << setw(83) << "Do you want to update other Customer? (y|n): ";
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
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", "IC", "Name", "License", "Email", "No.Phone");
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(13) << "";
            printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", row[2], row[1], row[3], row[5], row[4]);
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
                DeleteCust();
            }
            else if (choose == 'n' || choose == 'N')
            {
                AdminMenu();
            }
        }
        cout << endl << setw(35) << "         Do you want to delete another student?(y|n): ";
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
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", "IC", "Name", "License", "Email", "No.Phone");
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(13) << "";
            printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", row[2], row[1], row[3], row[5], row[4]);
            cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        }


    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
    cout << setw(75) << "Press Enter To Back....";
    _getch();
    CustomerMenu();
}

void SearchCust() {
    system("cls");
    std::string filePath = "admin.txt"; // Path to the admin file
    welcomeHeader(filePath);

    char choose;
    string name;

    cout << "\n";
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
        printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", "IC", "Name", "License", "Email", "No.Phone");
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(13) << "";
            printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", row[2], row[1], row[3], row[5], row[4]);
            cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        }


        cout << endl << setw(50) << "";
        cout << setw(10) << "Do you want to search other Customer? (y|n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
            SearchCust();
        else if (choose == 'n' || choose == 'N')
            CustomerMenu();
    }
    else
    {
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
    //cout << "                Login as:" << staff << "\n\n";
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
    while ((ch = _getch()) != 13)
    {
        staffpassword += ch;
        cout << '*';
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

    int menu;
    system("cls");
    std::string filePath = "staff.txt"; // Path to the admin file
    welcomeHeader(filePath);

    cout << "                Login as:" << StaffName << "\n\n";
    cout << setw(71) << "1. Manage Customer\n\n" << endl;
    cout << setw(68) << "2. Manage Car\n\n" << endl;
    cout << setw(61) << "3. Logout" << endl;
    cout << setw(83) << "      -----------------------------------------\n" << endl;
    cout << setw(75) << "Please Enter Your Choice : ";
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
        system("cls");
        main();
        break;
    }
}

void CustMenu() {
    
    int choose;
    system("cls");
    cout << setw(1000) << " " << endl;
    std::string filePath = "staff.txt"; // Path to the admin file
    welcomeHeader(filePath);

    cout << setw(74) << "1. Register Customer\n\n";
    cout << setw(72) << "2. Update Customer\n\n";
    cout << setw(72) << "3. Delete Customer\n\n";
    cout << setw(73) << "4. Display Customer\n\n";
    cout << setw(72) << "5. Search Customer\n\n";
    cout << setw(63) << "6. Back\n\n\n";
    cout << setw(86) << "-------------------------------------------\n" << endl;
    cout << setw(77) << "Please Enter Your Choice : ";
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
    string License = "";
    string Phone = "";
    string Email = "";


    cout << "\n" << setw(64) << "Enter Name  : ";
    cin.ignore(1, '\n');
    getline(cin, Name);
    Phone = getInputOfLength(10, "Phone number must be 10 digits long.", "Enter Phone : ");
    IC = getInputOfLength(12, "IC No must be 12 digits long", "Enter IC Number.: ");
    Email = getValidEmail();
    cout << setw(64) << "Enter License Number : ";
    cin.ignore();
    getline(cin, License);



    if (!qstate)
    {
        string insertMember_query = "insert into customer (Name,IC,Phone,Email,License) values ('" + Name + "', '" + IC + "','" + Phone + "','" + Email + "','" + License + "')";
        const char* q = insertMember_query.c_str();
        qstate = mysql_query(conn, q);

        if (!qstate)
        {
            cout << "\n\n\n" << setw(76) << "You have been registered.\n\n";
            cout << setw(81) << "Do you want to add other Customer? (y|n): ";
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
        cout << setw(112) << "---------------------------------------------------------------------------------------------------\n";
        cout << setw(12) << "";
        printf("|%-15s | %-25s | %-15s | %-20s | %-10s | %-20s |\n", "ID", "Name", "IC", "License", "No.Phone", "Email");
        cout << setw(112) << "---------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {
            cout << setw(12) << "";
            printf("|%-15s | %-25s | %-15s | %-20s | %-10s |%-20s |\n", row[0], row[1], row[2], row[3], row[4], row[5]);
            cout << setw(112) << "---------------------------------------------------------------------------------------------------\n";
        }

        while (true) // Retry loop for invalid Employee_ID
        {
            cout << endl << endl << setw(35) << " Choose ID : ";
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
                        cout << setw(57) << "\n\t\t\t\t\t\ Name: " << row[1] << endl;
                        cout << setw(57) << "\n\t\t\t\t\t\License: " << row[3] << endl;
                        cout << setw(57) << "\n\t\t\t\t\t\tIC: " << row[2] << endl;
                        cout << setw(57) << "\n\t\t\t\t\t\tCustomer Email: " << row[5] << endl;
                        cout << setw(57) << "\n\t\t\t\t\t\tCustomer Phone: " << row[4] << endl;
                        cout << setw(82) << "---------------------------------------" << endl << endl;
                    }
                    break; // Exit retry loop
                }
                else
                {
                    cout << endl << setw(92) << "No Customer found with the given ID!" << endl;
                    cout << endl << setw(92) << "Do you want to try again? (y/n): ";
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
        cout << setw(71) << "2. Customer Phone\n\n";
        cout << setw(64) << "3. Back\n\n";
        cout << setw(86) << "-------------------------------------------\n" << endl;
        cout << setw(78) << "Please Enter Your Choice : ";
        cin >> choose;
        cin.ignore(1, '\n'); // Clear newline from input buffer

        if (choose == 1)
        {
            cout << endl << endl << setw(70) << "1. New Customer Email: ";
            getline(cin, DataCust);
            string update_query = "UPDATE customer SET Email='" + DataCust + "' WHERE CustID='" + ID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (choose == 2)
        {
            cout << endl << endl << setw(70) << "2. New Customer Phone: ";
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


    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
    cout << setw(75) << "Press Enter To Back....";
    _getch();
    CustMenu();
}

//car

void MenuCar() {
    int choose;
    system("cls");
    std::string filePath = "staff.txt"; // Path to the admin file
    welcomeHeader(filePath);

    cout << setw(300) << " " << endl;
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
        printf("| %-3s | %-13s | %-15s | %-11s | %-15s | %-15s | \n", "ID", "Car Brand", "Car Name", "Plate", "Price Per Hour", "Status");
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
                cout << setw(65) << "Price Per Hour: " << row[4] << endl;
                cout << setw(57) << "Status: " << row[5] << endl;
                cout << setw(82) << "---------------------------------------" << endl << endl;
            }
        }
        else
        {
            cout << "Query Execution Problem!" << mysql_error(conn) << endl;
            system("pause");
        }

        cout << setw(71) << "1. Price Per Hour\n\n";
        cout << setw(63) << "2. Status\n\n";
        cout << setw(61) << "3. Back\n\n";
        cout << setw(86) << "-------------------------------------------\n" << endl;
        cout << setw(70) << "Please Enter Your Choice : ";
        cin >> choose;

        cin.ignore(1, '\n');
        if (choose == 1)
        {
            cout << endl << endl << setw(71) << "1. Price Per Hour: ";
            getline(cin, c_data);
            string update_query = "update car set PricePerHour='" + c_data + "' where CarID='" + CarID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (choose == 2)
        {
            cout << endl << endl << setw(63) << "2. Status: ";
            getline(cin, c_data);
            string update_query = "update car set Status='" + c_data + "' where CarID='" + CarID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
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
        printf("| %-3s | %-13s | %-15s | %-11s | %-15s | %-15s | \n", "ID", "Car Brand", "Car Name", "Plate", "Price Per Hour", "Status");
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
        printf("| %-3s | %-13s | %-15s | %-11s | %-15s | %-15s | \n", "ID", "Car Brand", "Car Name", "Plate", "Price Per Hour", "Status");
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
    string name;

    cout << "\n";
    cin.ignore(1, '\n');
    cout << setw(71) << "Search Car by Brand: ";
    getline(cin, name);

    string search_query = "select * from car where Brand like '" + name + "%'";
    const char* q = search_query.c_str();
    qstate = mysql_query(conn, q);

    if (!qstate)
    {
        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(99) << "-------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(25) << "";
        printf("| %-3s | %-13s | %-15s | %-11s | %-15s | %-15s | \n", "ID", "Car Brand", "Car Name", "Plate", "Price Per Hour", "Status");
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

void AddRental() {


}




//empty table
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

    // Print table header
    for (int i = 0; i < col_count; ++i) {
        MYSQL_FIELD* field = mysql_fetch_field(res);
        printf("        %-*s ", max_widths[i], field->name);
    }
    printf("\n\n");


    // Print table data
    for (const auto& row : data) {
        for (int i = 0; i < col_count; ++i) {
            printf("      | %-*s ", max_widths[i] + 2, row[i].c_str());
        }
        printf("|\n");
    } //car table end here!!

}