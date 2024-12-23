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
#include <openssl/evp.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "main.h"


using namespace std;

int qstate;
MYSQL* conn;
MYSQL_STMT stmt;
MYSQL_ROW row;
MYSQL_RES* res;
string items[20000000];
string adminId, CustID, CarID, cat_Id, RentalID;

//method declaration
void login();
void menu();
void table();
void cartable();

//Customer CRUD method
void AddCust();
void UpdateCust();
void DeleteCust();
void ListCust();
void SearchCust();

//Car CRUD method
void AddCar();
void UpdateCar();
void DeleteCar();
void ListCar();
void SearchCar();

//Rental
void RentalData();
void ReturnCar();
void DeleteRental();
void RentedList();
void SearchRentData();

//Report
void report();


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

        conn = mysql_real_connect(conn, "localhost", "root", "", "crms", 3306, NULL, 0);
        if (conn)
            cout << "successfull connect!" << endl;
        else
            cout << "Failed To Connect!" << endl;
    }
};

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


//welcome header
void welcomehHeader(const char* message) {
    system("cls");
    cout << "\n\n";
    std::cout << "\t\t\t|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n";
    std::cout << "\t\t\t|||||||||||                                                      ||||||||||\n";
    std::cout << "\t\t\t|||||||            CAR RENTAL MANAGEMENT SYSTEM BY SYUSYU...        |||||||\n";
    std::cout << "\t\t\t||||||||||                                                       ||||||||||\n";
    std::cout << "\t\t\t|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n";
    std::cout << "\t\t\t---------------------------------------------------------------------------\n";
    printMessageCenter(message);
    std::cout << "\n\t\t\t---------------------------------------------------------------------------\n\n";

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


//main page
int main() {

    int mainmenu;
    db_response::ConnectionFunction();
    header();
    welcomehHeader(" \t\t\t                       Car Rental Management System                  ");
    cout << setw(84) << "              Welcome To Car Rental Management System\n\n\n";
    cout << setw(69) << "1. Login\n\n\n";
    cout << setw(69) << "2. Exit\n\n\n\n";
    cout << setw(87) << "----------------------------------------------\n" << endl;
    cout << setw(75) << "Please Enter Your Choice : ";
    cin >> mainmenu;

    std::string answer;
    switch (mainmenu) {
    case 1:
        login();
        break;
    case 2:

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



//login
void login()
{
    welcomehHeader(" \t\t\t                  Welcome to the CRMS                    ");
    string username;
    string password;

    cout << setw(80) << "\n\n\t\t\t\t\t\tUsername: ";
    cin >> username;

    //username validation
    if (username.empty())
    {
        cout << "\n\n\n" << setw(80) << "Invalid username! Please try again." << endl;
        login(); // Repeat the function to allow re-entering the username
        return;
    }

    cout << setw(63) << "\n\n\t\t\t\t\t\tPassword:";
    char ch;
    while ((ch = _getch()) != 13)
    {
        password += ch;
        cout << '*';
    }

    //password validation

    if (password.empty())
    {
        cout << endl << "Invalid password! Please try again." << endl;
        login(); // Repeat the function to allow re-entering the password
        return;
    }

    string check_query = "SELECT adminId FROM admin WHERE username = '" + username + "' AND password ='" + password + "'";
    const char* cu = check_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count == 1)
        {

            while (row = mysql_fetch_row(res))
                adminId = row[0];
            menu();
        }
        else
        {
            char c;
            cout << "\n\n\n" << setw(85) << "Invalid username or password. Want to try again? (y/n): ";
            cin >> c;
            if (c == 'y' || c == 'Y')
                login();
            else
                main();
        }
    }
    else
        cout << "Query Execution Problem: " << mysql_errno(conn) << endl;
}

//mainmenu
void menu()
{
    int mainmenu;
    system("cls");
    welcomehHeader(" \t\t\t                       This is MainMenu                   ");

    cout << setw(70) << "1. Customer\n\n";
    cout << setw(65) << "2. Car\n\n";
    cout << setw(68) << "3. Rental\n\n";
    cout << setw(68) << "4. Report\n\n";
    cout << setw(68) << "5. Logout\n\n";
    cout << setw(86) << "-------------------------------------------\n" << endl;
    cout << setw(77) << "Please Enter Your Choice : ";
    cin >> mainmenu;

    std::string answer;
    switch (mainmenu)

    {
    case 1:
        int choose;
        system("cls");

        cout << setw(1000) << " " << endl;
        welcomehHeader(" \t\t\t                        Car Rental Management System                  ");
        cout << setw(69) << "1. Add Customer\n\n";
        cout << setw(72) << "2. Update Customer\n\n";
        cout << setw(72) << "3. Delete Customer\n\n";
        cout << setw(73) << "4. Display Customer\n\n";
        cout << setw(72) << "5. Search Customer\n\n";
        cout << setw(62) << "6. Back\n\n\n";
        cout << setw(86) << "-------------------------------------------\n" << endl;
        cout << setw(76) << "Please Enter Your Choice : ";
        cin >> choose;

        if (choose == 1)
        {
            AddCust();
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
            menu();
        }
        break;

    case 2:
        system("cls");

        cout << setw(1000) << " " << endl;
        welcomehHeader(" \t\t\t                        Car Rental Management System                  ");
        cout << setw(69) << "1. Add Car\n\n";
        cout << setw(72) << "2. Update Car\n\n";
        cout << setw(72) << "3. Delete Car\n\n";
        cout << setw(73) << "4. Display Car\n\n";
        cout << setw(72) << "5. Search Car\n\n";
        cout << setw(67) << "6. Back\n\n\n";
        cout << setw(86) << "-------------------------------------------\n" << endl;
        cout << setw(76) << "Please Enter Your Choice : ";
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
            menu();
        }
        break;

    case 3:
        system("cls");
        cout << setw(1000) << " " << endl;
        welcomehHeader(" \t\t\t                        Car Rental Management System                  ");
        cout << setw(69) << "1. Rent Car\n\n";
        cout << setw(67) << "2. Cancel\n\n";
        cout << setw(71) << "3. Return Car\n\n";
        cout << setw(72) << "4. Rented List\n\n";
        cout << setw(67) << "5. Search\n\n";
        cout << setw(65) << "6. Back\n\n";
        cout << setw(86) << "-------------------------------------------\n" << endl;
        cout << setw(76) << "Please Enter Your Choice : ";
        cin >> choose;

        if (choose == 1)
        {
            RentalData();

        }
        else if (choose == 2)
        {
            DeleteRental();
        }
        else if (choose == 3) {
            ReturnCar();

        }
        else if (choose == 4)
        {
            RentedList();
        }
        else if (choose == 5)
        {
            SearchRentData();

        }
        else if (choose == 6)
        {

            menu();
        }
        break;

    case 4:

        system("cls");
        cout << setw(1000) << " " << endl;
        welcomehHeader(" \t\t\t                        Car Rental Management System                  ");
        report();
       
        break;

    case 5:
        system("cls");
        cout << setw(1000) << " " << endl;
        welcomehHeader(" \t\t\t                      Car Rental Management System                  ");
        std::cout << "\n                                   Do you want to logout the system? (yes/no): ";
        std::cin >> answer;

        for (char& c : answer) {
            c = std::tolower(c);
        }

        system("cls");

        if (answer == "yes") {
            ifstream inputFile("thanks.txt"); //displaying thank you ASCII image text on output screen fn1353
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
            menu();
        }
    default:
        system("cls");
        system("pause");
        //other IDs' will be an error
        cout << "Please Enter a Valid Input!\n\n";
        break;


    }



} //mainmenu end hereee!!!!

void table()
{
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
        printf("      %-*s  ", max_widths[i], field->name);
    }
    printf("\n");


    // Print table data
    for (const auto& row : data) {
        for (int i = 0; i < col_count; ++i) {
            printf("    | %-*s ", max_widths[i] + 2, row[i].c_str());
        }
        printf("|\n");
    }

}  //for table

void cartable()
{
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


//Create Customer
void AddCust()

{
    system("cls");
    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");



    char option;
    string CustIC = "";
    string CustName = "";
    string LicenseNo = "";
    string PhoneNo = "";

    while (true) {  // Loop until valid CustIC is entered
        cout << "\n\n" << setw(67) << "Customer IC:";
        cin.ignore(1, '\n');
        getline(cin, CustIC);

        if (CustIC.length() == 12 && all_of(CustIC.begin(), CustIC.end(), ::isdigit)) {
            break;  // Valid CustIC
        }

        cout << "\n                                      Invalid CustIC. Please enter 12 numeric characters." << endl;
        _getch();
        system("cls");
        AddCust();
    }
    cout << "\n\n" << setw(69) << "Customer Name:";
    getline(cin, CustName);
    cout << "\n\n" << setw(70) << "License Number:";
    getline(cin, LicenseNo);
    cout << "\n\n" << setw(68) << "Phone Number:";
    getline(cin, PhoneNo);

    if (!qstate)
    {
        string insertCust_query = "INSERT into customer (CustIC,CustName,LicenseNo,PhoneNo) values ('" + CustIC + "', '" + CustName + "', '" + LicenseNo + "', '" + PhoneNo + "')";
        const char* c = insertCust_query.c_str();
        qstate = mysql_query(conn, c);

        if (!qstate)
        {
            cout << "\n\n\n" << setw(75) << "Succesfully Insert!!!!.\n\n";
            cout << setw(79) << "Do you want to add other customer? (y/n): ";
            cin >> option;

            if (option == 'y' || option == 'Y')
            {
                AddCust();
            }
            else
            {
                ListCust();
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

//Update Customer
void UpdateCust()
{
    system("cls");
    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");

    string c_data;
    int choose;
    char option;

    qstate = mysql_query(conn, "SELECT * FROM customer");

    if (!qstate)
    {
        

        int pilih; //pilihan untuk update atau tak..
        printf("\n");
        cout << setw(75) << "1. Update Customer\n\n\n";
        cout << setw(69) << "2. Back\n\n\n";
        cout << setw(87) << "----------------------------------------------\n" << endl;
        cout << setw(77) << "Please Enter Your Choice : ";
        cin >> pilih;

        std::string answer;
        switch (pilih) {
        case 1:
            system("cls");
            welcomehHeader(" \t\t\t                  Car Rental Management System                  ");
            table();
            cout << endl << endl << setw(70) << "Choose Customer ID: ";
            cin >> CustID;
            break;
        case 2:

            std::cout << "\n                                         Do you want to go back to mainmenu? (yes/no):";
            std::cin >> answer;

            for (char& c : answer) {
                c = std::tolower(c);
            }

            system("cls");

            if (answer == "yes") {

                menu(); //back to mainmenu
            }
            else {
                UpdateCust(); //stay in update page
            }
        default:
            system("cls");
            system("pause");
            //other IDs' will be an error
            cout << "Please Enter a Valid Input!\n\n";
            break;
        }

        system("cls");
        welcomehHeader(" \t\t\t                  Car Rental Management System                  ");

        cout << setw(71) << "1. Customer IC \n\n";
        cout << setw(73) << "2. Customer Name \n\n";
        cout << setw(74) << "3. License Number \n\n";
        cout << setw(72) << "4. Phone Number \n\n";
        cout << setw(86) << "-------------------------------------------\n" << endl;
        cout << setw(78) << "Choose info you want to update: ";
        cin >> choose;

        cin.ignore(1, '\n');
        if (choose == 1)
        {
            cout << endl << endl << setw(75) << "Update Customer IC: ";
            getline(cin, c_data);
            string updateCust_query = "UPDATE customer SET CustIC ='" + c_data + "'WHERE CustID='" + CustID + "'";
            const char* c = updateCust_query.c_str();
            qstate = mysql_query(conn, c);
        }
        else if (choose == 2)
        {
            cout << endl << endl << setw(75) << "Update Customer Name:";
            getline(cin, c_data);
            string updateCust_query = "UPDATE customer SET CustName ='" + c_data + "'WHERE CustID='" + CustID + "'";
            const char* c = updateCust_query.c_str();
            qstate = mysql_query(conn, c);

        }
        else if (choose == 3)
        {
            cout << endl << endl << setw(75) << "Update License Number:";
            getline(cin, c_data);
            string updateCust_query = "UPDATE customer SET LicenseNo ='" + c_data + "'WHERE CustID='" + CustID + "'";
            const char* c = updateCust_query.c_str();
            qstate = mysql_query(conn, c);

        }
        else if (choose == 4)
        {
            cout << endl << endl << setw(75) << "Update Phone Number:";
            getline(cin, c_data);
            string updateCust_query = "UPDATE customer SET PhoneNo ='" + c_data + "'WHERE CustID='" + CustID + "'";
            const char* c = updateCust_query.c_str();
            qstate = mysql_query(conn, c);
        }

        cout << "\n\n\n" << setw(75) << "Succesfully Update!!!!.\n\n";
        cout << endl << setw(85) << "Do you want to update other customer? (y/n):";
        cin >> option;

        if (option == 'y' || option == 'Y')
        {
            UpdateCust();
        }
        else
        {
            ListCust();
        }


    }
    else
    {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }


}

//Delete Customer
void DeleteCust()
{
    system("cls");
    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");

    char choose;
    string cust_id;

    qstate = mysql_query(conn, "SELECT * FROM customer");
    if (!qstate)
    {
        table();

        cout << endl << setw(70) << "Customer ID you want to delete: ";
        cin >> cust_id;

        bool check = false;

        string deleteCust_query = "SELECT CustID FROM customer";
        const char* cd = deleteCust_query.c_str();
        qstate = mysql_query(conn, cd);
        res = mysql_store_result(conn);

        //validation to check whether custid is found or not
        if (!qstate)
        {
            while ((row = mysql_fetch_row(res)))
            {
                if (row[0] == cust_id)
                {
                    check = true;
                    break;
                }
            }
        }

        if (check)
        {
            string deleteCust_query = "DELETE FROM customer WHERE CustID='" + cust_id + "'";
            const char* cd = deleteCust_query.c_str();
            qstate = mysql_query(conn, cd);
            if (!qstate)
            {
                cout << endl << setw(75) << "Delete Successfull!" << endl;
            }
            else
            {
                cout << "Error : Delete Failed!" << mysql_errno(conn) << endl;
            }

        }
        else
        {
            cout << endl << setw(78) << "Please Enter a valid ID. Try Again? (y/n): ";
            cin >> choose;
            if (choose == 'y' || choose == 'Y')
            {
                DeleteCust();
            }
            else
            {
                menu();
            }
        }

        cout << endl << setw(78) << "Do you want to Delete other customer ?(y/n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
        {
            DeleteCust();
        }
        else
        {
            ListCust(); //listcust()
        }
    }
}

//Read customer from database
void ListCust()
{
    system("cls");
    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");
    qstate = mysql_query(conn, "SELECT * FROM customer");
    if (!qstate)
    {
        
        table();

    }
    else
    {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }
    cout << "Press Enter To Back....";
    _getch();
    menu();
}

//Search Customer
void SearchCust()
{
    system("cls");
    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");

    char choose;
    string cust_name;

    cin.ignore(1, '\n');
    cout << setw(60) << "SEARCH customer by CustName: ";
    getline(cin, cust_name);

    system("cls");
    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");

    string searchCust_query = "SELECT * FROM customer WHERE CustName like '" + cust_name + "%'";
    const char* sc = searchCust_query.c_str();
    qstate = mysql_query(conn, sc);

    if (!qstate)
    {
        table();

        cout << endl << setw(60) << "search other custommer?(y/n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
            SearchCust();
        else if (choose == 'n' || choose == 'N')
            menu();
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }
}

//---------------------------------------------------CUSTOMER PAGE END HERE!!---------------------------------------------------------

//Create Car
void AddCar()
{
    system("cls");
    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");

    char option, pilih;
    string CarPlate = "";
    string Price = "";
    string availability_status = "Available";

    qstate = mysql_query(conn, "SELECT * FROM car_category");

    cartable();

    cout << "\n\n" << setw(68) << "Car Category: ";
    cin.ignore(1, '\n');
    getline(cin, cat_Id);
    cout << "\n\n" << setw(71) << "Car Plate Number:";
    getline(cin, CarPlate);
    cout << "\n\n" << setw(74) << "Price(per hour): RM ";
    getline(cin, Price);




    if (!qstate)
    {
        string insertCar_query = "INSERT into car (CarPlate,Price,Status,cat_ID) values ('" + CarPlate + "', '" + Price + "', '" + availability_status + "', '" + cat_Id + "') ";
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
void UpdateCar()
{
    system("cls");
    welcomehHeader(" \t\t\t                        Car Rental Management System                  ");

    string c_data;
    int choose;
    char option;

    qstate = mysql_query(conn, "SELECT car.*, car_category.CarBrand, car_category.Model FROM car JOIN car_category ON car.cat_Id = car_category.cat_Id");

    if (!qstate) //car table from database
    {

        int pilih; //pilihan untuk update atau tak..
        printf("\n");
        cout << setw(75) << "1. Update Car\n\n\n";
        cout << setw(69) << "2. Back\n\n\n";
        cout << setw(87) << "----------------------------------------------\n" << endl;
        cout << setw(77) << "Please Enter Your Choice : ";
        cin >> pilih;


        system("cls");
        welcomehHeader(" \t\t\t                        Car Rental Management System                  ");
        std::string answer;
        switch (pilih) {
        case 1:
            cartable();
            cout << endl << endl << setw(70) << "Choose Car ID: ";
            cin >> CarID;
            break;
        case 2:

            menu();
            
        default:
            system("cls");
            break;
        }

        //another page to update...
        system("cls");
        welcomehHeader(" \t\t\t                          Car Rental Management System                  ");

        cout << setw(71) << "1. Car Plate \n\n";
        cout << setw(71) << "2. Car Price \n\n";
        cout << setw(72) << "3. Car Status \n\n";
        cout << setw(86) << "-------------------------------------------\n" << endl;
        cout << setw(78) << "Choose info you want to update: ";
        cin >> choose;

        cin.ignore(1, '\n');
        if (choose == 1)
        {
            cout << endl << endl << setw(75) << "Update Car Plate: ";
            getline(cin, c_data);
            string updateCar_query = "UPDATE car SET CarPlate ='" + c_data + "'WHERE CarID='" + CarID + "'";
            const char* ca = updateCar_query.c_str();
            qstate = mysql_query(conn, ca);
        }
        else if (choose == 2)
        {
            cout << endl << endl << setw(75) << "Update Car Price:";
            getline(cin, c_data);
            string updateCar_query = "UPDATE car SET Price ='" + c_data + "'WHERE CarID='" + CarID + "'";
            const char* ca = updateCar_query.c_str();
            qstate = mysql_query(conn, ca);

        }
        else if (choose == 3)
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


        cout << endl << setw(85) << "Do you want to update other car? (y/n):";
        cin >> option;

        if (option == 'y' || option == 'Y')
        {
            UpdateCar();
        }
        else
        {
            ListCar();
        }


    }
    else
    {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }

} //update car end here!!

//Delete Car
void DeleteCar()
{
    system("cls");

    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");

    char choose;
    string car_id;

    qstate = mysql_query(conn, "SELECT * FROM car");

    if (!qstate)
    {
        cartable();

        cout << endl << setw(70) << "Car ID you want to delete: ";
        cin >> car_id;

        bool check = false;

        string deleteCar_query = "SELECT CarID FROM car";
        const char* cad = deleteCar_query.c_str();
        qstate = mysql_query(conn, cad);
        res = mysql_store_result(conn);

        //validation to check whether carid is found or not
        if (!qstate)
        {
            while ((row = mysql_fetch_row(res)))
            {
                if (row[0] == car_id)
                {
                    check = true;
                    break;
                }
            }
        }

        if (check)
        {
            string deleteCar_query = "DELETE FROM car WHERE CarID='" + car_id + "'";
            const char* cad = deleteCar_query.c_str();
            qstate = mysql_query(conn, cad);
            if (!qstate)
            {
                cout << endl << setw(75) << "Delete Successfull!" << endl;
            }
            else
            {
                cout << "Error : Delete Failed!" << mysql_errno(conn) << endl;
            }

        }
        else
        {
            cout << endl << setw(78) << "Please Enter a valid ID. Try Again? (y/n): ";
            cin >> choose;
            if (choose == 'y' || choose == 'Y')
            {
                DeleteCar();
            }
            else
            {
                menu();
            }
        }

        cout << endl << setw(78) << "Do you want to Delete other car ?(y/n): ";
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

//List data car in database
void ListCar()
{
    system("cls");
    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");
    qstate = mysql_query(conn, "SELECT car.*, car_category.CarBrand, car_category.Model FROM car JOIN car_category ON car.cat_Id = car_category.cat_Id");
    if (!qstate)
    {
        res = mysql_store_result(conn);
        MYSQL_ROW row;

        cout << left << setw(10) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << '+' << '+' << endl;

        cout << setfill(' ') << '|' << left << setw(9)
            << "CarID"
            << setfill(' ') << '|' << setw(20) << "Plate Number"
            << setfill(' ') << '|' << setw(20) << "Car Price (Per Hour)"
            << setfill(' ') << '|' << setw(20) << "Status"
            << setfill(' ') << '|' << setw(20) << "Catergory Id"
            << setfill(' ') << '|' << right << setw(20)
            << "Car Brand" << '|' << endl;


        cout << left << setw(10) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << '+' << '+' << endl;

        if (res) {
            while ((row = mysql_fetch_row(res))) {
                cout << setfill(' ') << '|' << left << setw(9) << row[0]
                    << setfill(' ') << '|' << setw(20) << row[1]
                    << setfill(' ') << '|' << setw(20) << row[2]
                    << setfill(' ') << '|' << setw(20) << row[3]
                    << setfill(' ') << '|' << setw(20) << row[4]
                    << setfill(' ') << '|' << right << setw(20)
                    << row[5] << '|' << endl;
            }

            printf("\n");
        }
        else {
            mysql_free_result(res);
        }


    }
    else
    {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }
    cout << "Press Enter To Back....";
    _getch();
    menu();

}

//Search for car
void SearchCar()
{
    system("cls");
    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");

    char choose;
    string car_plate;

    cin.ignore(1, '\n');
    cout << setw(60) << "SEARCH Car by Plate Number: ";
    getline(cin, car_plate);

    system("cls");
    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");


    string searchCust_query = "SELECT car.*, car_category.CarBrand, car_category.Model FROM car JOIN car_category ON car.cat_Id = car_category.cat_Id WHERE CarPlate like '" + car_plate + "%'";
    const char* sc = searchCust_query.c_str();
    qstate = mysql_query(conn, sc);

    if (!qstate)
    {
        cartable();

        cout << endl << setw(60) << "search other car ?(y/n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
            SearchCar();
        else if (choose == 'n' || choose == 'N')
            menu();
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }
}

//---------------------------------------------------CAR PAGE END HERE!!---------------------------------------------------------

//this is where to rent a car
void RentalData() {
    db_response::ConnectionFunction();

    system("cls");
    welcomehHeader(" \t\t\t                    Car Rental Management System                  ");

    string custID, carID;
    std::string rentalDateStr, returnDateStr;
    string pickUpTimeStr = "10:00";
    string dropOffTimeStr = "10:00";

    // Prompt user for customer ID
    qstate = mysql_query(conn, "SELECT * FROM customer");
    table();
    std::cout << "\n\n" << setw(65) << "Enter Customer ID : ";
    std::cin >> custID;

    // Validate customer ID exists
    bool validCustomer = false;
    string query = "SELECT 1 FROM customer WHERE CustID = '" + custID + "'";
    if (mysql_query(conn, query.c_str()) != 0) {
        std::cerr << "Error executing query: " << mysql_error(conn) << std::endl;
        // mysql_close(conn);
        RentalData();
        return;
    }
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    if (row) {
        validCustomer = true;
    }
    mysql_free_result(res);

    string custName;
    if (validCustomer) {
        string cust_query = "SELECT CustName FROM customer WHERE CustID = '" + custID + "'";
        if (mysql_query(conn, cust_query.c_str()) != 0) {
            std::cerr << "Error fetching customer name: " << mysql_error(conn) << std::endl;
        }
        else {
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);
            if (row) {
                custName = row[0];  // Store the customer name
            }
            mysql_free_result(res);
        }
        printf("\n\n");
        // Prompt user for car ID
        qstate = mysql_query(conn, "SELECT * FROM car");
        cartable();
        cout << "\n\n" << setw(68) << "Choose Car ID: ";
        cin.ignore(1, '\n');
        getline(cin, CarID);

        // Validate car ID exists and fetch price,status,model and car plate
        string query = "SELECT car.Price,car.Status,car.CarPlate, car_category.CarBrand, car_category.Model FROM car JOIN car_category ON car.cat_Id = car_category.cat_Id WHERE CarID ='" + CarID + "' LIMIT 1";
        if (mysql_query(conn, query.c_str()) != 0) {
            std::cerr << "Error executing query: " << mysql_error(conn) << std::endl;
            mysql_close(conn);
            return;
        }
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);
        float carPrice = 0.0;
        string carStatus = "", carPlate = "", brand = "", model ="";
        if (row) {
            carPrice = atof(row[0]);
            carStatus = row[1];
            carPlate = row[2];  // Store car plate number
            brand = row[3];
            model = row[4];
        }
        else {
            std::cerr << "Car with ID " << CarID << " not found!" << std::endl;
            RentalData();

        }
        mysql_free_result(res);

        bool isAvailable = (carStatus == "Available");
        printf("\n\n");
        //rest of the code for rental calculations and display ...
        if (isAvailable) {
            // Prompt for rental and return dates
            std::cout << setw(77) << "Enter Rental Date (YYYY-MM-DD): ";
            std::cin >> rentalDateStr;
            std::cout << setw(77) << "Enter Return Date (YYYY-MM-DD): ";
            std::cin >> returnDateStr;

            // Convert dates to time_t values
            tm rentalDate = {}, returnDate = {}, pickUpTime = {}, dropOffTime = {};
            istringstream(rentalDateStr) >> get_time(&rentalDate, "%Y-%m-%d");
            istringstream(returnDateStr) >> get_time(&returnDate, "%Y-%m-%d");
            istringstream(pickUpTimeStr) >> get_time(&pickUpTime, "%H:%M");
            istringstream(dropOffTimeStr) >> get_time(&dropOffTime, "%H:%M");

            // Calculate number of hours rented
            time_t rentalTimeT = mktime(&rentalDate) + mktime(&pickUpTime);
            time_t returnTimeT = mktime(&returnDate) + mktime(&dropOffTime);
            int totalSeconds = difftime(returnTimeT, rentalTimeT);
            int totalHours = totalSeconds / 3600;

            // Calculate total price
            int totalDays = totalHours / 24;
            float totalPrice = totalHours * carPrice;
            if (totalDays >= 3) {
                float discountRate = 0.10; // Example discount of 10%
                float discountedPrice = totalPrice * (1 - discountRate);

                // Display discounted price
                welcomehHeader(" \t\t\t                        Car Rental Management System                  ");
                int nameWidth = custName.length() > 18 ? custName.length() + 1 : 18;

                std::cout << setw(92) << "     |-------------------------------------------------------|" << std::endl;
                std::cout << setw(92) << "     |                  Car Rental Details                   |" << std::endl;
                std::cout << setw(92) << "     |-------------------------------------------------------|" << std::endl;
                std::cout << std::setw(59) << " Customer Name:    " << std::setw(nameWidth) << custName << "\n" << std::endl;
                std::cout << std::setw(59) << " Car Brand: " << std::setw(nameWidth) << brand << "\n" << std::endl;
                std::cout << std::setw(59) << " Car Plate Number: " << std::setw(nameWidth) << carPlate << "\n" << std::endl;
                std::cout << std::setw(59) << " Pickup Time:      " << std::setw(nameWidth) << rentalDateStr << "\n" << std::endl;
                std::cout << std::setw(59) << " Drop Off Time:    " << std::setw(nameWidth) << returnDateStr << "\n" << std::endl;
                std::cout << std::setw(61) << " Total Price:      RM" << std::setw(nameWidth - 2) << totalPrice << "\n" << std::endl;
                std::cout << std::setw(63) << " Total price (with 10% discount):  RM" << std::setw(static_cast<std::streamsize>(nameWidth) - 2) << discountedPrice << "\n" << std::endl;
                std::cout << std::setw(92) << "   |-------------------------------------------------------|" << std::endl;
            }
            else {
                // Display regular price
                welcomehHeader(" \t\t\t                        Car Rental Management System                  ");
                int nameWidth = custName.length() > 18 ? custName.length() + 1 : 18;

                std::cout << setw(92) << "     |-------------------------------------------------------|" << std::endl;
                std::cout << setw(92) << "     |                  Car Rental Details                   |" << std::endl;
                std::cout << setw(92) << "     |-------------------------------------------------------|" << std::endl;
                std::cout << std::setw(59) << " Customer Name:    " << std::setw(nameWidth) << custName << "\n" << std::endl;
                std::cout << std::setw(59) << " Car Brand: " << std::setw(nameWidth) << brand << "\n" << std::endl;
                std::cout << std::setw(59) << " Car Plate Number: " << std::setw(nameWidth) << carPlate << "\n" << std::endl;
                std::cout << std::setw(59) << " Rental Date:      " << std::setw(nameWidth) << rentalDateStr << "\n" << std::endl;
                std::cout << std::setw(60) << " Return Date:     " << std::setw(nameWidth) << returnDateStr << "\n" << std::endl;
                std::cout << std::setw(61) << " Total Price:      RM" << std::setw(nameWidth - 2) << totalPrice << "\n" << std::endl;
                std::cout << std::setw(92) << "   |-------------------------------------------------------|" << std::endl;
            }
            // Prompt for rental confirmation
            std::cout << setw(80) << "\n                                                 Confirm Rental (y/n): ";
            char confirm;
            std::cin >> confirm;


            // If confirmed, insert rental into database
            if (confirm == 'y') {
                string rental_status = "Approved";
                query = "INSERT INTO rental (CustID, CarID, rental_date, return_date,rental_status,TotalPrice) VALUES ('" + custID + "', '" + CarID + "', '" + rentalDateStr + "','" + returnDateStr + "','" + rental_status + "', " + std::to_string(totalPrice) + ")";
                if (mysql_query(conn, query.c_str()) != 0) {
                    std::cerr << "Error adding rental: " << mysql_error(conn) << std::endl;

                }
                else {
                    std::cout << setw(79) << "\t\tRental successfully! Please make sure customer pay at the counter!" << std::endl;
                    // Fetch and display transaction details
                    std::string fetchQuery = "SELECT rental.RentalID, customer.CustName, car_category.CarBrand, car.CarPlate, rental.rental_date, rental.return_date, rental.TotalPrice FROM rental JOIN customer ON rental.CustID = customer.CustID JOIN car ON rental.CarID = car.CarID JOIN car_category ON car.cat_Id = car_category.cat_Id ORDER BY RentalID DESC LIMIT 1";
                    if (mysql_query(conn, fetchQuery.c_str()) != 0) {
                        std::cerr << "Error executing query: " << mysql_error(conn) << std::endl;
                        mysql_close(conn);
                        return;
                    }
                    res = mysql_store_result(conn);
                    row = mysql_fetch_row(res);
                    float total_price = 0.0;
                    string rentalID = "", custName = "", carStatus = "", carPlate = "", brand = "", rental_date = "", return_date="";
                    if (row) {
                        rentalID = row[0];
                        custName = row[1];
                        brand = row[2];  // Store car plate number
                        carPlate = row[3];
                        rental_date = row[4];
                        return_date = row[5];
                        total_price = atof(row[6]);


                        std::cout << "\nTransaction Details:\n";
                        std::cout << "-----------------------------------------------------\n";
                        std::cout << "Rental ID: " << rentalID << std::endl;
                        std::cout << "Customer Name: " << custName << std::endl;
                        std::cout << "Car Brand: " << brand << std::endl;
                        std::cout << "Car Plate: " << carPlate << std::endl;
                        std::cout << "Rental Date: " << rental_date << std::endl;
                        std::cout << "Return Date: " << return_date << std::endl;
                        std::cout << "Total Price: RM" << totalPrice << std::endl;
                        std::cout << "-----------------------------------------------------\n";
                    }
                    else {
                        std::cerr << "Error fetching transaction" << std::endl;
                        mysql_close(conn);

                    }
                    mysql_free_result(res);
                    cout << setw(79) << "Update the car status ? (y/n): ";
                    char option;
                    cin >> option;

                    if (option == 'y' || option == 'Y')
                    {
                        UpdateCar();
                    }
                    else
                    {

                        menu();
                    }

                }

            }
            else {

                menu();


            }

        }
        else {
            std::cerr << "      \n                             Error: Car is not available! Please check car's status........";
            Sleep(2500); //function which waits for (n) seconds
            system("cls");
            RentalData();

        }


    }

}

void ReturnCar()
{
    system("cls");
    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");
    qstate = mysql_query(conn, "SELECT rental.RentalID,  car.CarID, car_category.CarBrand, car.CarPlate, rental.rental_date, rental.return_date, rental.rental_status FROM car_category JOIN car ON car_category.cat_Id = car.cat_Id JOIN rental ON car.CarID = rental.CarID");

    if (!qstate) {
        // display table
         res = mysql_store_result(conn);
         MYSQL_ROW row;

    cout << left << setw(10) << setfill('-') << left << '+'
        << setw(21) << setfill('-') << left << '+'
        << setw(21) << setfill('-') << left << '+'
        << setw(21) << setfill('-') << left << '+'
        << setw(21) << setfill('-') << left << '+'
        << setw(21) << setfill('-') << left << '+'
        << setw(21) << setfill('-') << '+' << '+' << endl;

    cout << setfill(' ') << '|' << left << setw(9)
        << "Rental ID"
        << setfill(' ') << '|' << setw(20) << "Car ID"
        << setfill(' ') << '|' << setw(20) << "Brand"
        << setfill(' ') << '|' << setw(20) << "Plate"
        << setfill(' ') << '|' << setw(20) << "Rental Date"
        << setfill(' ') << '|' << setw(20) << "Return Date"
        << setfill(' ') << '|' << right << setw(20)
        << "Rental Status" << '|' << endl;


    cout << left << setw(10) << setfill('-') << left << '+'
        << setw(21) << setfill('-') << left << '+'
        << setw(21) << setfill('-') << left << '+'
        << setw(21) << setfill('-') << left << '+'
        << setw(21) << setfill('-') << left << '+'
        << setw(21) << setfill('-') << left << '+'
        << setw(21) << setfill('-') << '+' << '+' << endl;

    if (res) {
        while ((row = mysql_fetch_row(res))) {
            cout << setfill(' ') << '|' << left << setw(9) << row[0]
                << setfill(' ') << '|' << setw(20) << row[1]
                << setfill(' ') << '|' << setw(20) << row[2]
                << setfill(' ') << '|' << setw(20) << row[3]
                << setfill(' ') << '|' << setw(20) << row[4]
                << setfill(' ') << '|' << setw(20) << row[5]
                << setfill(' ') << '|' << right << setw(20)
                << row[6] << '|' << endl;
        }

        printf("\n");
    }
    else {
        mysql_free_result(res);
    }

        cout << "\n\n" << setw(68) << "Choose Rental ID: ";
        cin.ignore(1, '\n');  // Discard any leftover newline from previous input
        getline(cin, RentalID);

        // Update rental status to 'returned'
        std::string update_query = "UPDATE rental SET rental_status = 'returned' WHERE RentalID = '" + RentalID + "' ";
        if (mysql_query(conn, update_query.c_str())) {
            std::cerr << "Error updating rental status: " << mysql_error(conn) << std::endl;

        }
        // Log the return date and time
        std::string log_query = "INSERT into car_return (RentalID, return_date) VALUES ('" + RentalID + "', NOW())";
        if (mysql_query(conn, log_query.c_str())) {
            std::cerr << "Error logging return date: " << mysql_error(conn) << std::endl;
            mysql_close(conn);

           
        }
        cout << setw(79) << "Rental car returned successfully! Update the car status ? (y/n): ";
        char option;
        cin >> option;

        if (option == 'y' || option == 'Y')
        {
            UpdateCar();
        }
        else
        {

            menu();
        }


    }
    else {
        // Handle query error
        std::cerr << "Error executing query: " << mysql_error(conn) << std::endl;
    }
   
}


//this is where to cancel a car already rented
void DeleteRental()
{
    system("cls");

    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");

    char choose;
    string rent_id;

    qstate = mysql_query(conn, "SELECT * FROM rental");
    if (!qstate)
    {
        res = mysql_store_result(conn);
        MYSQL_ROW row;

        cout << left << setw(10) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << '+' << '+' << endl;

        cout << setfill(' ') << '|' << left << setw(9)
            << "Rental ID"
            << setfill(' ') << '|' << setw(20) << "Customer ID"
            << setfill(' ') << '|' << setw(20) << "Car ID"
            << setfill(' ') << '|' << setw(20) << "Rental Date"
            << setfill(' ') << '|' << setw(20) << "Return Date"
            << setfill(' ') << '|' << setw(20) << "Rental Status"
            << setfill(' ') << '|' << right << setw(20)
            << "Total Price" << '|' << endl;


        cout << left << setw(10) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << '+' << '+' << endl;

        if (res) {
            while ((row = mysql_fetch_row(res))) {
                cout << setfill(' ') << '|' << left << setw(9) << row[0]
                    << setfill(' ') << '|' << setw(20) << row[1]
                    << setfill(' ') << '|' << setw(20) << row[2]
                    << setfill(' ') << '|' << setw(20) << row[3]
                    << setfill(' ') << '|' << setw(20) << row[4]
                    << setfill(' ') << '|' << setw(20) << row[5]
                    << setfill(' ') << '|' << right << setw(20)
                    << row[6] << '|' << endl;
            }

            printf("\n");
        }
        else {
            mysql_free_result(res);
        }

        cout << endl << setw(70) << "Rental ID you want to delete: ";
        cin >> rent_id;

        bool check = false;

        string deleteCar_query = "SELECT RentalID FROM rental";
        const char* cad = deleteCar_query.c_str();
        qstate = mysql_query(conn, cad);
        res = mysql_store_result(conn);

        //validation to check whether custid is found or not
        if (!qstate)
        {
            while ((row = mysql_fetch_row(res)))
            {
                if (row[0] == rent_id)
                {
                    check = true;
                    break;
                }
            }
        }

        if (check)
        {
            string deleteCar_query = "DELETE FROM rental WHERE RentalID='" + rent_id + "'";
            const char* cad = deleteCar_query.c_str();
            qstate = mysql_query(conn, cad);
            if (!qstate)
            {
                cout << endl << setw(75) << "\tDelete Successfull! ***** UPDATE CAR STATUS  (y/n) ? " << endl;
                char option;
                cin >> option;

                if (option == 'y' || option == 'Y')
                {
                    UpdateCar();
                }
                else
                {

                    menu();
                }
            }
            else
            {
                cout << "Error : Delete Failed!" << mysql_errno(conn) << endl;
            }

        }
        else
        {
            cout << endl << setw(78) << "Please Enter a valid ID. Try Again? (y/n): ";
            cin >> choose;
            if (choose == 'y' || choose == 'Y')
            {
                DeleteRental();
            }
            else
            {
                menu();
            }
        }

        cout << endl << setw(78) << "Do you want to Delete other rental data ?(y/n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
        {
            DeleteRental();
        }
        else
        {
            // RentalList();
            menu();
        }
    }

}

//list rented from database
void RentedList()
{
    system("cls");
    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");
    qstate = mysql_query(conn, "SELECT * FROM rental");
    if (!qstate)
    {
        res = mysql_store_result(conn);
        MYSQL_ROW row;

        cout << left << setw(10) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << '+' << '+' << endl;

        cout << setfill(' ') << '|' << left << setw(9)
            << "Rental ID"
            << setfill(' ') << '|' << setw(20) << "Customer ID"
            << setfill(' ') << '|' << setw(20) << "Car ID"
            << setfill(' ') << '|' << setw(20) << "Rental Date"
            << setfill(' ') << '|' << setw(20) << "Return Date"
            << setfill(' ') << '|' << setw(20) << "Rental Status"
            << setfill(' ') << '|' << right << setw(20)
            << "Total Price" << '|' << endl;


        cout << left << setw(10) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << '+' << '+' << endl;

        if (res) {
            while ((row = mysql_fetch_row(res))) {
                cout << setfill(' ') << '|' << left << setw(9) << row[0]
                    << setfill(' ') << '|' << setw(20) << row[1]
                    << setfill(' ') << '|' << setw(20) << row[2]
                    << setfill(' ') << '|' << setw(20) << row[3]
                    << setfill(' ') << '|' << setw(20) << row[4]
                    << setfill(' ') << '|' << setw(20) << row[5]
                    << setfill(' ') << '|' << right << setw(20)
                    << row[6] << '|' << endl;
            }

            printf("\n");
        }
        else {
            mysql_free_result(res);
        }


    }
    else
    {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }
    cout << "\n\nPress Enter To Back....";
    _getch();
    menu();

}

//searching rental data from database
void SearchRentData()
{
    system("cls");
    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");

    char choose;
    string rental_id;

    cin.ignore(1, '\n');
    cout << setw(60) << "SEARCH rental by RentalID: ";
    getline(cin, rental_id);

    system("cls");
    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");

    string searchRent_query = "SELECT * FROM rental WHERE RentalID like '" + rental_id + "'";
    const char* sr = searchRent_query.c_str();
    qstate = mysql_query(conn, sr);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        MYSQL_ROW row;

        cout << left << setw(10) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << '+' << '+' << endl;

        cout << setfill(' ') << '|' << left << setw(9)
            << "Rental ID"
            << setfill(' ') << '|' << setw(20) << "Customer ID"
            << setfill(' ') << '|' << setw(20) << "Car ID"
            << setfill(' ') << '|' << setw(20) << "Rental Date"
            << setfill(' ') << '|' << setw(20) << "Return Date"
            << setfill(' ') << '|' << setw(20) << "Rental Status"
            << setfill(' ') << '|' << right << setw(20)
            << "Total Price" << '|' << endl;


        cout << left << setw(10) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << left << '+'
            << setw(21) << setfill('-') << '+' << '+' << endl;

        if (res) {
            while ((row = mysql_fetch_row(res))) {
                cout << setfill(' ') << '|' << left << setw(9) << row[0]
                    << setfill(' ') << '|' << setw(20) << row[1]
                    << setfill(' ') << '|' << setw(20) << row[2]
                    << setfill(' ') << '|' << setw(20) << row[3]
                    << setfill(' ') << '|' << setw(20) << row[4]
                    << setfill(' ') << '|' << setw(20) << row[5]
                    << setfill(' ') << '|' << right << setw(20)
                    << row[6] << '|' << endl;
            }

            printf("\n");
        }
        else {
            mysql_free_result(res);
        }

        cout << endl << setw(60) << "search other Rental Data ?(y/n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
            SearchRentData();
        else if (choose == 'n' || choose == 'N')
            menu();
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }
}

//---------------------------------------------------Report here!---------------------------------------------



void report()
{
    db_response::ConnectionFunction();
    system("cls");
    welcomehHeader(" \t\t\t                    Car Rental Management System                  ");

    int choose;
    cout << setw(71) << "1. Sales Revenue \n\n";
    cout << setw(73) << "2. Most rented car \n\n";
    cout << setw(62) << "3. Back \n\n";
    cout << setw(87) << "----------------------------------------------\n" << endl;
    cout << setw(77) << "Please Enter Your Choice : ";
    cin >> choose;
    if (choose == 1) {
        system("cls");
        welcomehHeader(" \t\t\t                    Car Rental Management System                  ");
        // Prompt the user for start date and end date
        std::string startDate, endDate;
        std::cout << setw(71) << "Enter start date (YYYY-MM-DD): ";
        std::cin >> startDate;
        std::cout << setw(71) << "Enter end date (YYYY-MM-DD): ";
        std::cin >> endDate;

        if (calculateSalesRevenueByDateRange(conn, startDate, endDate)) {
            std::cout << "Sales revenue calculation successful" << std::endl;
        }
        else {
            std::cerr << "Sales revenue calculation failed" << std::endl;
        }
        char choose;
        cout << endl << setw(78) << "Continue ?(y/n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
        {
            report();
        }
        else
        {
            // RentalList();
            menu();
        }

    }
    else if (choose == 2) {
        system("cls");
        welcomehHeader(" \t\t\t                    Car Rental Management System                  ");

        std::string startDate, endDate;
        std::cout << setw(71) << "Enter start date (YYYY-MM-DD): ";
        std::cin >> startDate;
        std::cout << setw(71) << "Enter end date (YYYY-MM-DD): ";
        std::cin >> endDate;

        if (findMostRentedCarByDateRange(conn, startDate, endDate)) {
            std::cout << "Most rented car calculation successful" << std::endl;
        }
        else {
            std::cerr << "Most rented car calculation failed" << std::endl;
        }
        char choose;
        cout << endl << setw(78) << "Continue ?(y/n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
        {
            report();
        }
        else
        {
            
            menu();
        }


    }
    else if (choose == 3) {
        std::cout << "\n                                         Do you want to go back to mainmenu? (yes/no):";
        string answer;
        std::cin >> answer;

        for (char& c : answer) {
            c = std::tolower(c);
        }

        system("cls");

        if (answer == "yes") {

            menu(); //back to mainmenu
        }
        else {
            report(); 
        }

    }
    
   
   
   
}
