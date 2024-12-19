#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <conio.h>
#include <mysql.h>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <limits>
#include <sstream>
#include <boost/date_time/gregorian/gregorian.hpp>


using namespace std;

int qstate;
MYSQL* conn;
MYSQL_STMT stmt;
MYSQL_ROW row;
MYSQL_RES* res;
string items[20000000];
string adminId,CustID,CarID;

//method declaration
void login();
void menu();

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
void DeleteRental();

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
    std::cout << "\t\t\t###########################################################################\n";
    std::cout << "\t\t\t############                                                   ############\n";
    std::cout << "\t\t\t############      CAR RENTAL MANAGEMENT SYSTEM BY SYUSYU...     ###########\n";
    std::cout << "\t\t\t############                                                   ############\n";
    std::cout << "\t\t\t###########################################################################\n";
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
    Sleep(10);
    cout << "\nStarting the program please wait....." << endl;
    Sleep(10);
    cout << "\nloading up the system....." << endl;
    Sleep(2500); //function which waits for (n) seconds
    system("CLS"); //cleares screen
}


//main page
int main() {

    int mainmenu;
    db_response::ConnectionFunction();
    header();
    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");
    cout << setw(80) << "Welcome To Car Rental Management System\n\n\n";
    cout << setw(63) << "1. Login\n\n\n";
    cout << setw(63) << "2. Exit\n\n\n\n";
    cout << setw(83) << "----------------------------------------------\n" << endl;
    cout << setw(71) << "Please Enter Your Choice : ";
    cin >> mainmenu;

    switch (mainmenu) {
    case 1:
        login();
        break;
    case 2:
        exit(0);
    default:
        system("cls");
        system("pause");
        //other IDs' will be an error
        cout << "Please Enter a Valid Input!\n\n";
        break;
    }
    return 0;
}


/*void fileReader(char carFile[20])
{
    ifstream inputFile;
    inputFile.open(carFile); //displaying details of model A
    if (!inputFile) {
        cout << "Error in printing file. File not found!" << endl;
    }
    char str[200];
    while (inputFile) {
        inputFile.getline(str, 200);
        if (inputFile)
            cout << str << endl;
    }
}*/


//login
void login()
{
    welcomehHeader(" \t\t\t                  Welcome to the CRMS                    ");
    string username;
    string password;
    
    cout<< setw(80)<< "\n\n\t\t\t\t\t\tUsername: ";
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
    cout << setw(70) << "3. Rent Car\n\n";
    cout << setw(68) << "4. Report\n\n";
    cout << setw(68) << "5. Logout\n\n";
    cout << setw(86) << "-------------------------------------------\n" << endl;
    cout << setw(77) << "Please Enter Your Choice : ";
    cin >> mainmenu;

    switch (mainmenu) 

    {
    case 1:
        int choose;
        system("cls");

        cout << setw(1000) << " " << endl;
        welcomehHeader(" \t\t\t                      Car Rental Management System                  ");
        cout << setw(69) << "1. Add Customer\n\n";
        cout << setw(72) << "2. Update Customer\n\n";
        cout << setw(72) << "3. Delete Customer\n\n";
        cout << setw(73) << "4. Display Customer\n\n";
        cout << setw(72) << "5. Search Customer\n\n";
        cout << setw(62) << "6. Back\n\n\n";
        cout << setw(86) << "-------------------------------------------\n" << endl;
        cout << setw(76) << "Please Enter Your Choice : ";
        cin >> choose;

        if ( choose == 1)
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
        welcomehHeader(" \t\t\t                      Car Rental Management System                  ");
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
        welcomehHeader(" \t\t\t                      Car Rental Management System                  ");
        cout << setw(69) << "1. Rent Car\n\n";
        cout << setw(72) << "2. Cancel Rent\n\n";
        cout << setw(72) << "3. Car's Rented List\n\n";
        cout << setw(73) << "4. Search\n\n";
        cout << setw(72) << "5. Back\n\n";
        cout << setw(86) << "-------------------------------------------\n" << endl;
        cout << setw(76) << "Please Enter Your Choice : ";
        cin >> choose;

        if (choose == 1)
        {
            RentalData();

        }
        else if (choose == 2);
        {
            DeleteRental();
        }



    }

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

    cout << "\n\n" << setw(68) << "Customer IC:";
    cin.ignore(1, '\n');
    getline(cin, CustIC);
    cout << "\n\n" << setw(69) << "Customer Name:";
    getline(cin, CustName);
    cout << "\n\n" << setw(68) <<  "License Number:";
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
        cout << endl << endl << setw(73) << "Choose Customer ID: ";
        cin >> CustID;


        system("cls");
        welcomehHeader(" \t\t\t                  Car Rental Management System                  ");

        cout << setw(71) << "1. Customer IC \n\n";
        cout << setw(72) << "2. Customer Name \n\n";
        cout << setw(72) << "3. License Number \n\n";
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
            string updateCust_query = "UPDATE customer SET PhoneNo ='" + c_data + "WHERE CustID='" + CustID + "'";
            const char* c = updateCust_query.c_str();
            qstate = mysql_query(conn, c);
        }

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

        if(check)
        {
            string deleteCust_query = "DELETE FROM customer WHERE CustID='" + cust_id + "'";
            const char* cd = deleteCust_query.c_str();
            qstate = mysql_query(conn, cd);
            if(!qstate)
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

        cout  << endl << setw(78) << "Do you want to Delete other customer ?(y/n): ";
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
        printf("\n\n");


        // Print table data
        for (const auto& row : data) {
            for (int i = 0; i < col_count; ++i) {
                printf("    | %-*s ", max_widths[i] + 2, row[i].c_str());
            }
            printf("|\n");
        }

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



//Create Car
void AddCar()
{
    system("cls");
    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");

    char option;
    string CarBrand = "";
    string CarPlate = "";
    string Model = "";
    string CarCategory = "";
    string Price = "";
    string availability_status = "";

    cout << "\n\n" << setw(68) << "Car Brand:";
    cin.ignore(1, '\n');
    getline(cin, CarBrand);
    cout << "\n\n" << setw(68) << "Car Model:";
    getline(cin, Model);
    cout << "\n\n" << setw(68) << "Car Category:";
    getline(cin, CarCategory);
    cout << "\n\n" << setw(68) << "Car Plate Number:";
    getline(cin, CarPlate);
    cout << "\n\n" << setw(68) << "Price(per day): RM ";
    getline(cin, Price);
    cout << "\n\n" << setw(68) << "Car Availability Status: ";
    getline(cin, availability_status);

    if (!qstate)
    {
        string insertCar_query = "INSERT into car (CarBrand,Model,CarCategory,CarPlate,Price,Status) values ('" + CarBrand + "', '" + Model + "', '" + CarCategory + "', '" + CarPlate + "', '" + Price + "', '" + availability_status + "' ) ";
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
    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");

    string c_data;
    int choose;
    char option;

    qstate = mysql_query(conn, "SELECT * FROM car");

    if (!qstate)
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
            printf("        %-*s ", max_widths[i] , field->name);
        }
        printf("\n\n");


        // Print table data
        for (const auto& row : data) {
            for (int i = 0; i < col_count; ++i) {
                printf("      | %-*s ", max_widths[i] + 2, row[i].c_str());
            }
            printf("|\n");
        }

        cout << endl << endl << setw(70) << "Choose Car ID: ";
        cin >> CarID;

        system("cls");
        welcomehHeader(" \t\t\t                  Car Rental Management System                  ");

        cout << setw(71) << "1. Car Brand \n\n";
        cout << setw(72) << "2. Car Model \n\n";
        cout << setw(72) << "3. Car Category \n\n";
        cout << setw(72) << "4. Car Plate \n\n";
        cout << setw(72) << "5. Car Price \n\n";
        cout << setw(72) << "6. Car Status \n\n";
        cout << setw(86) << "-------------------------------------------\n" << endl;
        cout << setw(78) << "Choose info you want to update: ";
        cin >> choose;

        cin.ignore(1, '\n');
        if (choose == 1)
        {
            cout << endl << endl << setw(75) << "Update Car Brand: ";
            getline(cin, c_data);
            string updateCar_query = "UPDATE car SET CarBrand ='" + c_data + "'WHERE CarID='" + CarID + "'";
            const char* ca = updateCar_query.c_str();
            qstate = mysql_query(conn, ca);
        }
        else if (choose == 2)
        {
            cout << endl << endl << setw(75) << "Update Car Model:";
            getline(cin, c_data);
            string updateCar_query = "UPDATE car SET Model ='" + c_data + "'WHERE CarID='" + CarID + "'";
            const char* ca = updateCar_query.c_str();
            qstate = mysql_query(conn, ca);

        }
        else if (choose == 3)
        {
            cout << endl << endl << setw(75) << "Update Car Category:";
            getline(cin, c_data);
            string updateCar_query = "UPDATE car SET CarCategory ='" + c_data + "'WHERE CarID='" + CarID + "'";
            const char* ca = updateCar_query.c_str();
            qstate = mysql_query(conn, ca);

        }
        else if (choose == 4)
        {
            cout << endl << endl << setw(75) << "Update Car Plate:";
            getline(cin, c_data);
            string updateCar_query = "UPDATE car SET CarPlate ='" + c_data + "'WHERE CarID='" + CarID + "'";
            const char* ca = updateCar_query.c_str();
            qstate = mysql_query(conn, ca);
        }
        else if (choose == 5)
        {
            cout << endl << endl << setw(75) << "Update Car Price:";
            getline(cin, c_data);
            string updateCar_query = "UPDATE car SET Price ='" + c_data + "'WHERE CarID='" + CarID + "'";
            const char* ca = updateCar_query.c_str();
            qstate = mysql_query(conn, ca);
        }
        else if (choose == 6)
        {
            cout << endl << endl << setw(75) << "Update Car Status:";
            getline(cin, c_data);
            string updateCar_query = "UPDATE car SET Status ='" + c_data + "'WHERE CarID='" + CarID + "'";
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
            //ListCar();
            menu();
        }


    }
    else
    {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }

}

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

        cout << endl << setw(70) << "Car ID you want to delete: ";
        cin >> car_id;

        bool check = false;

        string deleteCar_query = "SELECT CustID FROM car";
        const char* cad = deleteCar_query.c_str();
        qstate = mysql_query(conn, cad);
        res = mysql_store_result(conn);

        //validation to check whether custid is found or not
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
            string deleteCar_query = "DELETE FROM customer WHERE CustID='" + car_id + "'";
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
                DeleteCust();
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
            DeleteCust();
        }
        else
        {
            ListCust(); //listcust()
        }


    }


}

//List data car in database
void ListCar()
{
    system("cls");
    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");
    qstate = mysql_query(conn, "SELECT * FROM car");
    if (!qstate)
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
            printf("       %-*s  ", max_widths[i], field->name);
        }
        printf("\n\n");


        // Print table data
        for (const auto& row : data) {
            for (int i = 0; i < col_count; ++i) {
                printf("      | %-*s ", max_widths[i] + 2, row[i].c_str());
            }
            printf("|\n\n");
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
    string car_brand;

    cin.ignore(1, '\n');
    cout << setw(60) << "SEARCH car by CarBrand: ";
    getline(cin, car_brand);

    system("cls");
    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");


    string searchCust_query = "SELECT * FROM car WHERE CarBrand like '" + car_brand + "%'";
    const char* sc = searchCust_query.c_str();
    qstate = mysql_query(conn, sc);

    if (!qstate)
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
            printf("       %-*s ", max_widths[i], field->name);
        }
        printf("\n\n");


        // Print table data
        for (const auto& row : data) {
            for (int i = 0; i < col_count; ++i) {
                printf("      | %-*s ", max_widths[i] + 2, row[i].c_str());
            }
            printf("|\n\n");
        }

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


//this is where to rent a car
void RentalData() {
    db_response::ConnectionFunction();

    system("cls");
    welcomehHeader(" \t\t\t                  Car Rental Management System                  ");

    string custID, carID;
    std::string rentalDateStr, returnDateStr;


    // Prompt user for customer ID
    std::cout << "Enter Customer ID: ";
    std::cin >> custID;

    // Validate customer ID exists
    bool validCustomer = false;
    string query = "SELECT 1 FROM customer WHERE CustID = '" + custID + "'";
    if (mysql_query(conn, query.c_str()) != 0) {
        std::cerr << "Error executing query: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return;
    }
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    if (row) {
        validCustomer = true;
    }
    mysql_free_result(res);

    if (validCustomer) {
        // Prompt user for car ID
        std::cout << "Enter Car ID: ";
        std::cin >> carID;

        // Validate car ID exists and fetch price
        string query = "SELECT Price,Status FROM car WHERE CarID = '" + carID + "' LIMIT 1";
        if (mysql_query(conn, query.c_str()) != 0) {
            std::cerr << "Error executing query: " << mysql_error(conn) << std::endl;
            mysql_close(conn);
            return;
        }
        res = mysql_store_result(conn);
        row = mysql_fetch_row(res);
        double carPrice = 0.0;
        string carStatus = "";
        if (row) {
            carPrice = atof(row[0]);
            carStatus = row[1];
        }
        else {
            std::cerr << "Car with ID " << carID << " not found!" << std::endl;
            mysql_free_result(res);
            mysql_close(conn);
            return;
        }
        mysql_free_result(res);

        bool isAvailable = (carStatus == "Available");

        //rest of the code for rental calculations and display ...
        if (isAvailable) {
            // Prompt for rental and return dates
            std::cout << "Enter Rental Date (YYYY-MM-DD): ";
            std::cin >> rentalDateStr;
            std::cout << "Enter Return Date (YYYY-MM-DD): ";
            std::cin >> returnDateStr;

            // Convert dates to time_t values
            std::tm rentalDate = {};
            std::istringstream rentalDateStream(rentalDateStr);
            rentalDateStream >> std::get_time(&rentalDate, "%Y-%m-%d");
            std::tm returnDate = {};
            std::istringstream returnDateStream(returnDateStr);
            returnDateStream >> std::get_time(&returnDate, "%Y-%m-%d");

            // Calculate number of days rented
            std::time_t rentalTime = std::mktime(&rentalDate);
            std::time_t returnTime = std::mktime(&returnDate);
            int daysRented = std::difftime(returnTime, rentalTime) / (60 * 60 * 24);

            // Calculate total price
            double totalPrice = daysRented * carPrice;

            system("cls");

            // Display results
            welcomehHeader(" \t\t\t                  Car Rental Management System                  ");
            std::cout << "Customer ID: " << custID << std::endl;
            std::cout << "Car ID: " << carID << std::endl;
            std::cout << "Days Rented: " << daysRented << std::endl;
            std::cout << "Car Price: RM" << carPrice << std::endl;
            std::cout << "Total Price: RM" << totalPrice << std::endl;

            // Prompt for rental confirmation
            std::cout << "Confirm Rental (y/n): ";
            char confirm;
            
            std::cin >> confirm;


            // If confirmed, insert rental into database
            if (confirm == 'y') {
                query = "INSERT INTO rental (CustID, CarID, dateFrom, dateTo, TotalPrice) VALUES ('" + custID + "', '" + carID + "', '" + rentalDateStr + "', '" + returnDateStr + "', " + std::to_string(totalPrice) + ")";
                if (mysql_query(conn, query.c_str()) != 0) {
                    std::cerr << "Error adding rental: " << mysql_error(conn) << std::endl;
                }
                else {
                    std::cout << "Rental successfully! Please make sure customer pay at the counter!" << std::endl;
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
            
        }
        else {
            std::cerr << "Car is not available!";
            Sleep(2500); //function which waits for (n) seconds
            //mysql_close(conn);
            system("cls");
            RentalData();
           
        }

      
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
            printf("     %-*s ", max_widths[i], field->name);
        }
        printf("\n");


        // Print table data
        for (const auto& row : data) {
            for (int i = 0; i < col_count; ++i) {
                printf("    | %-*s", max_widths[i] + 2, row[i].c_str());
            }
            printf("|\n");
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
   


