#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cstring>
#include <conio.h>
#include "mysql.h"
#include <iomanip>
#include <cstdlib>
#include <limits>
#include <ctime>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>


using namespace std;

int qstate;
MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES* res;
string staffname,adminname;
string items[20000000];
string StaffID, AdminID, BookID, StudID, FineID, StudNoMatric;


void adminlogin();
void stafflogin();
void AdminMenu();
void StaffPage();
void StaffMenu();

void RegStaff();
void UpdateStaff();
void DeleteStaff();
void ListStaff();
void SearchStaff();

void BookMenu();
void AddBook();
void UpdateBook();
void DeleteBook();
void ListBook();
void SearchBook();

void studmenu();
void studreg();
void studupdate();
void studdelete();
void studlist();
void studsearch();

void bookissuemenu();
void viewbookissue();
void viewallbookissue();
void issuebook();
void returnbook();

void RegStud();
void StudMenu();
void UpdateStud();
void DeleteStud();
void ListStud();
void SearchStud();

void calculateFine();
void UpdateLateReturns();
void BookIssueMenu();
void ViewBookIssue();
void ViewAllBookIssue();
void IssueBook();
void ReturnBook();

void ReportMenu();
void OverallReport();
void ReportByMonth();

void studlogin();
void processPayment(string, int, double);
void displayReceipt(string paymentMethod, int amountPaid);
void replaceSubString(string&, string, string);
void processLateBooks(string);
void generateTransactionReceipt();
string getInputFromUser(vector<string>, string, string);
int getInputFromUser(vector<int>, string, string);

string getInputOfLength(int, string, string);
string getPassword();
string getValidEmail();


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

        conn = mysql_real_connect(conn, "localhost", "root", "", "library", 3306, NULL, 0);
        if (conn)
            cout << "Welcome to Library Management System" << endl;
        else
            cout << "Failed To Connect!" << endl;
    }
};

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

void headMessage(const char* message)
{
    system("cls");
    std::cout << "\t\t\t###########################################################################\n";
    std::cout << "\t\t\t############                                                   ############\n";
    std::cout << "\t\t\t############      Library management System Project in C++      ###########\n";
    std::cout << "\t\t\t############                                                   ############\n";
    std::cout << "\t\t\t###########################################################################\n";
    std::cout << "\t\t\t---------------------------------------------------------------------------\n";
    printMessageCenter(message);
    std::cout << "\n\t\t\t---------------------------------------------------------------------------\n\n";
}

int main()
{
    int menu;
    db_response::ConnectionFunction();
    headMessage(" \t\t\t                Welcome to the Library Management System                    ");
    //cout << setw(60) << "Login As :-\n\n\n";
    cout << setw(63) << "1. Admin\n\n\n";
    cout << setw(63) << "2. Staff\n\n\n";
    cout << setw(65) << "3. Student\n\n\n";
    cout << setw(63) << "4. Exit\n\n\n\n";
    cout << setw(83) << "----------------------------------------------\n" << endl;
    cout << setw(71) << "Please Enter Your Choice : ";
    cin >> menu;

    switch (menu)
    {
    case 1:
        adminlogin();
        break;
    case 2:
        stafflogin();
        break;
    case 3:
        studlogin();
        break;
    case 4:
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


//ADMIN LOGIN//
void adminlogin()
{
    headMessage("\t\t\t                  Welcome To Admin Page              ");
    string adminpassword;
    string adminusername;
    //cout << "                Login as:" << admin << "\n\n";
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
    while ((ch = _getch()) != 13)
    {
        adminpassword += ch;
        cout << '*';
    }

    // Validate password input
    if (adminpassword.empty())
    {
        cout << endl << "Invalid password! Please try again." << endl;
        adminlogin(); // Repeat the function to allow re-entering the password
        return;
    }

    string checkUser_query = "SELECT admin_id FROM admin WHERE admin_username = '" + adminusername + "' AND admin_pswd = '" + adminpassword + "'";
    const char* cu = checkUser_query.c_str();
    qstate = mysql_query(conn, cu);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (res->row_count == 1)
        {
            while (row = mysql_fetch_row(res))
                AdminID = row[0];
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
}


//STAFF LOGIN//
void stafflogin()
{
    headMessage("\t\t\t                 Welcome To Staff Page        ");
    string staffpassword;
    string staffusername;
    //cout << "                Login as:" << staff << "\n\n";
    cout << setw(63) << "Username: ";
    cin >> staffusername;

    // Validate username input
    if (staffusername.empty())
    {
        cout << "Invalid username! Please try again." << endl;
        stafflogin(); // Repeat the function to allow re-entering the username
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
        stafflogin(); // Repeat the function to allow re-entering the password
        return;
    }

    string checkUser_query = "SELECT * , staff_name FROM staff WHERE staff_username = '" + staffusername + "' AND staff_pswd = '" + staffpassword + "'";
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
                staffname = row[3];
            }

            if (user == staffusername && pw == staffpassword)
                StaffPage();
        }

        char c;
        cout << "\n\n\n" << setw(86) << "Invalid username or password. Want to try again? (y/n): ";
        cin >> c;
        if (c == 'y' || c == 'Y')
            stafflogin();
        else
            main();

    }
    else
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
}


//ADMIN MENU//
void AdminMenu()
{
    int menu;
    system("cls");
    headMessage("\t\t\t                 Welcome To Admin Menu Page           ");
   // cout << "                Login as:" << adminname << "\n\n";
    cout << setw(72) << "1. Manage Staff\n\n";
    cout << setw(71) << "2. Manage Book\n\n";
    cout << setw(74) << "3. Manage Student\n\n";
    cout << setw(70) << "4. Book Issue\n\n";
    cout << setw(73) << "5. Manage Report\n\n";
    cout << setw(66) << "6. Logout\n\n";
    cout << setw(86) << "-------------------------------------------\n" << endl;
    cout << setw(77) << "Please Enter Your Choice : ";
    cin >> menu;

    switch (menu)
    {
    case 1:

        StaffMenu();
        break;

    case 2:

        BookMenu();
        break;

    case 3:

        studmenu();
        break;

    case 4:

        bookissuemenu();
        break;

    case 5:

        ReportMenu();
        break;


    case 6:

        system("cls");
        main();
        break;
    }


}


//MANAGE STAFF IN ADMIN//
void StaffMenu() {
    int choose;
    system("cls");

    cout << setw(1000) << " " << endl;
    headMessage("\t\t\t                 Welcome To Staff Menu Page         ");
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
    headMessage("\t\t\t                 Welcome To Register Staff Page       ");

    char option;
    string STAFFNAME = "";
    string STAFFPHONE = "";
    string STAFFUSER = "";
    string STAFFPASS = "";
    string STAFFEMAIL = "";
    string STAFFPOSITION = "";

    int count = 0;
    int specials = 0;
    bool valid_password = true;

    //cout << setw(1000) << " " << endl;
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
    getline(cin, STAFFUSER);
    STAFFPASS = getPassword();

    string update_query = "update staff set status staff_pswd='" + STAFFPASS + "'";
    const char* q = update_query.c_str();
    qstate = mysql_query(conn, q);

    cout << "\n\n" << setw(61) << "Enter Name: ";
    cin.ignore();
    getline(cin, STAFFNAME);
    STAFFPHONE = getInputOfLength(10, "Phone number must be 10 digits!", "Enter Phone: ");
    STAFFEMAIL = getValidEmail();
    cout << setw(65) << "Enter Position: ";
    cin.ignore();
    getline(cin, STAFFPOSITION);

    string checkUser_query = "select * from staff where staff_username = '" + STAFFUSER + "'";
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
            string insertCustomer_query = "insert into staff (staff_username, staff_pswd,staff_name,staff_contact,staff_email,staff_position) values ('" + STAFFUSER + "', '" + STAFFPASS + "','" + STAFFNAME + "','" + STAFFPHONE + "','" + STAFFEMAIL + "','" + STAFFPOSITION + "')";
            const char* q = insertCustomer_query.c_str();
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

void UpdateStaff() {
    system("cls");
    headMessage("\t\t\t                 Welcome To Update Staff Page       ");

    string dataStaff;
    int choose;
    char option;

    qstate = mysql_query(conn, "select * from staff");

    if (!qstate)
    {

        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(112) << "---------------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(12) << "";
        printf("| %-3s | %-25s | %-10s | %-20s | %-25s |\n", "ID", "Name", "Phone", "Email", "Position");
        cout << setw(112) << "---------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {

            cout << setw(12) << "";
            printf("| %-3s | %-25s | %-10s | %-20s | %-25s |\n", row[0], row[3], row[4], row[5], row[6]);
            cout << setw(112) << "---------------------------------------------------------------------------------------------------\n";
        }
        cout << endl << endl << setw(35) << "  Choose Staff ID : ";
        cin >> StaffID;

        system("cls");

        string findbyid_query = "SELECT * FROM staff where staff_id='" + StaffID + "'";
        const char* qi = findbyid_query.c_str();
        qstate = mysql_query(conn, qi);

        if (!qstate)
        {
            res = mysql_store_result(conn);
            cout << endl;
            while ((row = mysql_fetch_row(res)))
            {
                system("cls");
                headMessage("\t\t\t                 Welcome To Update Staff Page       ");

                cout << setw(82) << "---------------------------------------" << endl;
                cout << setw(57) << "Staff Email:" << row[5] << endl;
                cout << setw(57) << "Staff Phone:" << row[4] << endl;
                cout << setw(60) << "Staff Position:" << row[6] << endl;
                cout << setw(82) << "---------------------------------------" << endl << endl;
            }
        }
        else
        {
            cout << "Query Execution Problem!" << mysql_error(conn) << endl;
            system("pause");
        }

        cout << setw(71) << "1. Staff Email\n\n";
        cout << setw(71) << "2. Staff Phone\n\n";
        cout << setw(74) << "3. Staff Position\n\n";
        cout << setw(64) << "4. Back\n\n";
        cout << setw(86) << "-------------------------------------------\n" << endl;
        cout << setw(78) << "Please Enter Your Choice : ";
        cin >> choose;

        cin.ignore(1, '\n');
        if (choose == 1)
        {
            cout << endl << endl << setw(70) << "1. New Staff Email: ";
            getline(cin, dataStaff);
            string update_query = "update staff set staff_email='" + dataStaff + "' where staff_id='" + StaffID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (choose == 2)
        {
            cout << endl << endl << setw(70) << "2. New Staff Phone: ";
            getline(cin, dataStaff);
            string update_query = "update staff set staff_contact='" + dataStaff + "' where staff_id='" + StaffID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);

        }
        else  if (choose == 3)
        {
            cout << endl << endl << setw(73) << "3. New Staff Position: ";
            getline(cin, dataStaff);
            string update_query = "update staff set staff_position='" + dataStaff + "' where staff_id='" + StaffID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else  if (choose == 4)
        {
            StaffMenu();
        }
        cout << endl << setw(86) << "Do you want to update other staff? (y|n): ";
        cin >> option;

        if (option == 'y' || option == 'Y')
            UpdateStaff();
        else
            ListStaff();
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }

}

void DeleteStaff() {
    system("cls");
    headMessage("\t\t\t                  Welcome To Delete Staff Page              ");

    char choose;
    string id;

    qstate = mysql_query(conn, "select * from staff");
    if (!qstate)
    {

        res = mysql_store_result(conn);


        cout << setw(112) << "---------------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(12) << "";
        printf("| %-3s | %-25s | %-10s | %-20s | %-25s |\n", "ID", "Name", "Phone", "Email", "Position");
        cout << setw(112) << "---------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {

            cout << setw(12) << "";
            printf("| %-3s | %-25s | %-10s | %-20s | %-25s |\n", row[0], row[3], row[4], row[5], row[6]);
            cout << setw(112) << "---------------------------------------------------------------------------------------------------\n";
        }

        cout << endl << endl << setw(70) << "Enter Staff ID to delete: ";
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
            cout << endl << setw(92) << "Please Enter a valid ID.Do You Want To Try Again? (y|n): ";
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
        cout << endl << setw(78) << "         Do you want to Delete another Staff?(y|n): ";
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
}

void ListStaff() {
    system("cls");
    headMessage("\t\t\t                  Welcome To List Staff Page              ");

    qstate = mysql_query(conn, "SELECT * FROM staff");
    if (!qstate)
    {
        res = mysql_store_result(conn);

        cout << setw(109) << "---------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(15) << "";
        printf("| %-25s | %-10s | %-20s | %-25s |\n", "Name", "Phone", "Email", "Position");
        cout << setw(109) << "---------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {

            cout << setw(15) << "";
            printf("| %-25s | %-10s | %-20s | %-25s |\n", row[3], row[4], row[5], row[6]);
            cout << setw(109) << "---------------------------------------------------------------------------------------------\n";
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

void SearchStaff() {
    system("cls");
    headMessage("\t\t\t                     Welcome To Search Staff Page              ");

    char choose;
    string name;

    cin.ignore(1, '\n');
    cout << endl << setw(72) << "Search Staff by name: ";
    getline(cin, name);

    string search_query = "select * from staff where staff_name like '%" + name + "%'";
    const char* q = search_query.c_str();
    qstate = mysql_query(conn, q);

    if (!qstate)
    {
        res = mysql_store_result(conn);


        cout << setw(109) << "---------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(15) << "";
        printf("| %-25s | %-10s | %-20s | %-25s |\n", "Name", "Phone", "Email", "Position");
        cout << setw(109) << "---------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {

            cout << setw(15) << "";
            printf("| %-25s | %-10s | %-20s | %-25s |\n", row[3], row[4], row[5], row[6]);
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


//MANAGE STUDENT IN ADMIN//
void studmenu() {
    int choose;
    system("cls");

    cout << setw(1000) << " " << endl;
    headMessage("\t\t\t                 Welcome To Student Menu Page      ");
    cout << setw(74) << "1. Register Student\n\n";
    cout << setw(72) << "2. Update Student\n\n";
    cout << setw(72) << "3. Delete Student\n\n";
    cout << setw(73) << "4. Display Student\n\n";
    cout << setw(72) << "5. Search Student\n\n";
    cout << setw(63) << "6. Back\n\n\n";
    cout << setw(86) << "-------------------------------------------\n" << endl;
    cout << setw(77) << "Please Enter Your Choice : ";
    cin >> choose;

    if (choose == 1)
    {
        RegStud();
    }
    else if (choose == 2)
    {
        UpdateStud();
    }
    else if (choose == 3)
    {
        DeleteStud();
    }
    else if (choose == 4)
    {
        ListStud();
    }
    else if (choose == 5)
    {
        SearchStud();
    }
    else if (choose == 6)
    {
        AdminMenu();
    }

}

void studreg() {
    system("cls");
    headMessage("\t\t\t                    Welcome To Register Student Menu Page       ");

    char option;
    string MEMName = "";
    string MEMPHONE = "";
    string MEMNOMATRIC = "";
    string MEMGENDER = "";
    string MEMEmail = "";


    cout << "\n" << setw(64) << "Enter Name  : ";
    cin.ignore(1, '\n');
    getline(cin, MEMName);
    MEMPHONE = getInputOfLength(10, "Phone number must be 10 digits long.", "Enter Phone : ");
    MEMNOMATRIC = getInputOfLength(10, "Matric No must be 10 digits long", "Enter Matric Num.: ");
    MEMEmail = getValidEmail();
    cout << setw(64) << "Enter Gender : ";
    cin.ignore();
    getline(cin, MEMGENDER);



    if (!qstate)
    {
        string insertMember_query = "insert into student (stud_name,stud_nomatric,stud_contact,stud_email,stud_gender) values ('" + MEMName + "', '" + MEMNOMATRIC + "','" + MEMPHONE + "','" + MEMEmail + "','" + MEMGENDER + "')";
        const char* q = insertMember_query.c_str();
        qstate = mysql_query(conn, q);

        if (!qstate)
        {
            cout << "\n\n\n" << setw(76) << "You have been registered.\n\n";
            cout << setw(81) << "Do you want to add other staff? (y|n): ";
            cin >> option;

            if (option == 'y' || option == 'Y')
                studreg();
            else
                studlist();
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

void studupdate() {
    system("cls");
    headMessage("\t\t\t                      Welcome To Update Student Page              ");

    string data;
    int choose;
    char option;

    qstate = mysql_query(conn, "select * from student");

    if (!qstate)
    {

        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", "No Matric", "Name", "No.Phone", "Email", "Gender");
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(13) << "";
            printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", row[1], row[0], row[2], row[3], row[4]);
            cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        }
        cout << endl << endl << setw(42) << "  Choose Student No. Matric : ";
        cin >> StudNoMatric;

        system("cls");

        string findbyid_query = "SELECT * FROM student where stud_nomatric='" + StudNoMatric + "'";
        const char* qi = findbyid_query.c_str();
        qstate = mysql_query(conn, qi);

        if (!qstate)
        {
            res = mysql_store_result(conn);
            cout << endl;
            while ((row = mysql_fetch_row(res)))
            {
                system("cls");
                headMessage("\t\t\t                 Welcome To Update Student Page       ");

                cout << setw(82) << "---------------------------------------" << endl;
                cout << setw(54) << "No. Phone:" << row[2] << endl;
                cout << setw(50) << "Email:" << row[3] << endl;
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
            string update_query = "update student set stud_contact='" + data + "' where stud_nomatric='" + StudNoMatric + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }

        else if (choose == 2)
        {
            cout << endl << endl << setw(66) << "2. Email: ";
            getline(cin, data);
            string update_query = "update student set stud_email='" + data + "' where stud_nomatric='" + StudNoMatric + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }


        cout << endl << setw(83) << "Do you want to update other student? (y|n): ";
        cin >> option;

        if (option == 'y' || option == 'Y')
            studupdate();
        else
            studlist();
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
}

void studdelete() {
    system("cls");
    headMessage("\t\t\t                      Welcome To Delete Student Page              ");

    char choose;
    string id;

    qstate = mysql_query(conn, "select * from student");
    if (!qstate)
    {

        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", "No Matric", "Name", "No.Phone", "Email", "Gender");
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(13) << "";
            printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", row[1], row[0], row[2], row[3], row[4]);
            cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        }
        cout << endl << endl << setw(43) << "  Choose Student No. Matric : ";
        cin >> StudNoMatric;

        bool found = false;

        string query = "select stud_nomatric from student";
        const char* qd = query.c_str();
        qstate = mysql_query(conn, qd);
        res = mysql_store_result(conn);

        if (!qstate)
        {
            while ((row = mysql_fetch_row(res)))
            {
                if (row[0] == StudNoMatric)
                {
                    found = true;
                    break;
                }
            }
        }

        if (found)
        {
            string query = "delete from student where stud_nomatric='" + StudNoMatric + "'";
            const char* qd = query.c_str();
            qstate = mysql_query(conn, qd);
            if (!qstate)
            {
                cout << endl << setw(35) << "         Successfully deleted from member record" << endl;
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
                DeleteBook();
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
            studdelete();
        }
        else
        {
            studlist();
        }
    }
}

void studlist() {
    system("cls");
    headMessage("\t\t\t                       Welcome To List Student Page              ");

    qstate = mysql_query(conn, "SELECT * FROM student");
    if (!qstate)
    {
        res = mysql_store_result(conn);


        cout << "\n";
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", "No Matric", "Name", "No.Phone", "Email", "Gender");
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(13) << "";
            printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", row[1], row[0], row[2], row[3], row[4]);
            cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        }


    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
    cout << setw(75) << "Press Enter To Back....";
    _getch();
    studmenu();
}

void studsearch() {
    system("cls");
    headMessage("\t\t\t                     Welcome To Search Student Page              ");

    char choose;
    string name;

    cin.ignore(1, '\n');
    cout << setw(24) << "";
    cout << setw(24) << "Search Student by name: ";
    getline(cin, name);

    string search_query = "select * from student where stud_name like '%" + name + "%'";
    const char* q = search_query.c_str();
    qstate = mysql_query(conn, q);

    if (!qstate)
    {
        res = mysql_store_result(conn);


        cout << "\n";
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", "No Matric", "Name", "No.Phone", "Email", "Gender");
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(13) << "";
            printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", row[1], row[0], row[2], row[3], row[4]);
            cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        }


        cout << endl << setw(50) << "";
        cout << setw(10) << "Do you want to search other student?(y|n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
            studsearch();
        else if (choose == 'n' || choose == 'N')
            studmenu();
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
}


//BOOK ISSUE IN ADMIN//
void bookissuemenu() {
    UpdateLateReturns();
    int choose;
    system("cls");
    cout << setw(1000) << " " << endl;
    headMessage("\t\t\t                 Welcome To Book Menu Page           ");
    cout << setw(74) << "1. View Book Issue\n\n";
    cout << setw(79) << "2. View All Book Issues\n\n";
    cout << setw(70) << "3. Borrow Book\n\n";
    cout << setw(72) << "4. Return a Book\n\n";
    cout << setw(64) << "5. Back\n\n\n";
    cout << setw(86) << "-------------------------------------------\n" << endl;
    cout << setw(77) << "Please Enter Your Choice : ";
    cin >> choose;
    if (choose == 1)
    {
        viewbookissue();
    }
    else if (choose == 2)
    {
        viewallbookissue();
    }
    else if (choose == 3)
    {
        issuebook();
    }
    else if (choose == 4)
    {
        returnbook();
    }
    else if (choose == 5)
    {
        AdminMenu();
    }
}

void viewbookissue() {
    system("cls");
    headMessage("\t\t\t                  Welcome To View Book Issue Page              ");

    qstate = mysql_query(conn, "SELECT * FROM borrow WHERE status = 'ISSUED' OR status = 'LATE'");
    if (!qstate)
    {
        res = mysql_store_result(conn);


        cout << setw(24) << "";
        cout << setw(25) << "--------------------------------------------------------------------------------\n";
        // Display the data in a formatted way
        cout << setw(25) << "";
        printf("| %-8s | %-10s | %-7s | %-10s | %-10s | %-15s |\n", "Issue ID", "Member ID", "Book ID", "Issue Date", "Due Date", "Status");

        cout << setw(24) << "";
        cout << setw(25) << "--------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {
            cout << setw(25) << "";
            printf("| %-8s | %-10s | %-7s | %-10s | %-10s | %-15s |\n", row[0], row[1], row[2], row[3], row[4], row[6]);

            cout << setw(24) << "";
            cout << setw(25) << "--------------------------------------------------------------------------------\n";
        }
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }

    cout << endl << setw(70) << "Press Enter To Go Back....";
    _getch();
    bookissuemenu();
}

void viewallbookissue() {
    system("cls");
    headMessage("\t\t\t                  Welcome To View Book Issue Page              ");

    qstate = mysql_query(conn, "SELECT * FROM borrow");
    if (!qstate)
    {
        res = mysql_store_result(conn);


        cout << setw(10) << "";
        cout << setw(22) << "----------------------------------------------------------------------------------------------------------------\n";
        // Display the data in a formatted way
        cout << setw(10) << "";
        printf("| %-10s | %-10s | %-10s | %-15s | %-15s | %-15s | %-15s |\n", "Issue ID", "Member ID", "Book ID", "Issue Date", "Due Date", "Return Date", "Status", "Pay Status");

        cout << setw(10) << "";
        cout << setw(22) << "----------------------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {
            cout << setw(10) << "";
            printf("| %-10s | %-10s | %-10s | %-15s | %-15s | %-15s | %-15s |\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);

            cout << setw(10) << "";
            cout << setw(22) << "----------------------------------------------------------------------------------------------------------------\n";
        }
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }

    cout << endl << setw(70) << "Press Enter To Go Back....";
    _getch();
    bookissuemenu();
}

void issuebook() {
    system("cls");
    headMessage("\t\t\t                  Welcome To Book Issue Page              ");

    string studId, bookId;
    cout << "\n\n" << setw(70) << "Enter Student No Matric: ";
    cin >> studId;

    string name;

    cin.ignore(1, '\n');
    cout << setw(70) << endl << "Search Book by name: ";
    getline(cin, name);

    string search_query = "select * from book where book_title like '" + name + "%'";
    const char* q = search_query.c_str();
    qstate = mysql_query(conn, q);

    if (!qstate)
    {
        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(99) << "-------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(25) << "";
        printf("| %-5s | %-20s | %-20s | %-15s |\n", "ID", "Book Title", "Book Author", "Book Genre");
        cout << setw(99) << "-------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(25) << "";
            printf("| %-5s | %-20s | %-20s | %-15s |\n", row[0], row[1], row[2], row[3]);
            cout << setw(99) << "-------------------------------------------------------------------------\n";
        }
    }
    cout << "\n\n" << setw(70) << "Enter Book ID: ";
    cin >> bookId;

    // Check if the book is available
    string bookStatusQuery = "SELECT book_status FROM book WHERE book_id = '" + bookId + "'";
    qstate = mysql_query(conn, bookStatusQuery.c_str());

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (mysql_num_rows(res) > 0) {
            row = mysql_fetch_row(res);
            string status = row[0];
            if (status == "AVAILABLE") {
                // Issue the book
                string issueBookQuery = "INSERT INTO borrow (stud_nomatric, book_id, start_date, end_date) VALUES ('" + studId + "', '" + bookId + "', CURDATE(), DATE_ADD(CURDATE(), INTERVAL 7 DAY))";
                qstate = mysql_query(conn, issueBookQuery.c_str());

                if (!qstate) {
                    cout << "\n\n" << setw(75) << "Book Issued Successfully!\n\n";

                    // Update book status to 'ISSUED'
                    string updateBookStatusQuery = "UPDATE book SET book_status = 'ISSUED' WHERE book_id = '" + bookId + "'";
                    qstate = mysql_query(conn, updateBookStatusQuery.c_str());
                }
                else {
                    cout << "Query Execution Problem!" << mysql_error(conn) << endl;
                }
            }
            else {
                cout << "\n\n" << setw(75) << "Book is not available for issue.\n\n";
            }
        }
        else {
            cout << "\n\n" << setw(70) << "No such book found.\n\n";
        }
    }
    else {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }

    cout << endl << setw(74) << "Press Enter To Go Back....";
    _getch();
    bookissuemenu();
}

void returnbook() {
    system("cls");
    headMessage("\t\t\t                  Welcome To Book Return Page              ");

    string studMatric, bookId;
    cout << "\n\n" << setw(75) << "Enter Member No Matric: ";
    cin >> studMatric;

    // Display all issued books to the member
    string issuedBooksQuery = "SELECT book_id, start_date, end_date, status, fine, return_date FROM borrow WHERE stud_nomatric = '" + studMatric + "' AND (status = 'ISSUED' OR status = 'LATE')";
    qstate = mysql_query(conn, issuedBooksQuery.c_str());

    if (!qstate) {
        res = mysql_store_result(conn);
        if (mysql_num_rows(res) > 0) {

            cout << setw(24) << "";
            cout << setw(24) << "List of Issued Books:\n\n";

            cout << setw(24) << "";
            cout << setw(24) << "--------------------------------------------------------------------------------------\n";
            // Display the data in a formatted way
            cout << setw(24) << "";
            printf("| %-10s | %-10s | %-10s | %-20s | %-20s |\n", "Book ID", "Issued On", "Due On", "Status", "Fine (RM)");

            cout << setw(24) << "";
            cout << setw(24) << "--------------------------------------------------------------------------------------\n";

            while ((row = mysql_fetch_row(res)))
            {
                cout << setw(24) << "";
                printf("| %-10s | %-10s | %-10s | %-20s | %-20s |\n", row[0], row[1], row[2], row[3], row[4]);

                cout << setw(24) << "";
                cout << setw(24) << "--------------------------------------------------------------------------------------\n";
            }

            // Ask for the book ID to return
            cout << "\n\n" << setw(75) << "Enter Book ID to Return: ";
            cin >> bookId;

            // Check if the book is late
            string checkLateQuery = "SELECT end_date, DATEDIFF(CURDATE(), end_date) AS days, CURDATE() FROM borrow WHERE stud_nomatric = '" + studMatric + "' AND book_id = '" + bookId + "'";

            qstate = mysql_query(conn, checkLateQuery.c_str());
            if (!qstate) {
                res = mysql_store_result(conn);
                if (mysql_num_rows(res) > 0) {
                    row = mysql_fetch_row(res);
                    //cout << "END DATE " << row[0] << endl;
                    //cout << "DAYS " << row[1] << endl;
                    //cout << "CUR DATE " << row[2] << endl;
                    int daysLate = atoi(row[1]);
                    //cout << "DAYS LATE" << daysLate << endl;
                    if (daysLate > 0) {
                        int fine = daysLate * 2; // Assuming the fine is 2 units per day

                        // Update the borrow table with fine and status
                        string updateQuery = "UPDATE borrow SET status = 'RETURNED LATE', return_date = CURDATE(), fine = " + std::to_string(fine) + " WHERE stud_nomatric = '" + studMatric + "' AND book_id = '" + bookId + "'";
                        qstate = mysql_query(conn, updateQuery.c_str());
                        if (qstate) cout << "Query Execution Problem!" << mysql_error(conn) << endl;
                    }
                    else {
                        // Update the borrow table with status only
                        string updateQuery = "UPDATE borrow SET status = 'RETURNED', return_date = CURDATE() WHERE stud_nomatric = '" + studMatric + "' AND book_id = '" + bookId + "'";
                        qstate = mysql_query(conn, updateQuery.c_str());
                        if (qstate) cout << "Query Execution Problem!" << mysql_error(conn) << endl;
                    }
                }

                string updateBookStatusQuery = "UPDATE book SET book_status = 'AVAILABLE' WHERE book_id = '" + bookId + "'";
                qstate = mysql_query(conn, updateBookStatusQuery.c_str());

                if (!qstate) {
                    cout << "\n\n" << setw(75) << "Book Returned Successfully!\n\n";
                }
                else {
                    cout << "Query Execution Problem!" << mysql_error(conn) << endl;
                }
            }
            else {
                cout << "Query Execution Problem!" << mysql_error(conn) << endl;
            }
        }
        else {
            cout << "\n\n" << setw(75) << "No issued books found for this student.\n\n";
        }
    }
    else {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }

    cout << endl << setw(75) << "Press Enter To Go Back....";
    _getch();
    bookissuemenu();
}



//MANAGE BOOK IN ADMIN//
void BookMenu() {
    int choose;
    system("cls");

    cout << setw(1000) << " " << endl;
    headMessage("\t\t\t                 Welcome To Book Menu Page         ");
    cout << setw(69) << "1. Add Book\n\n";
    cout << setw(72) << "2. Update Book\n\n";
    cout << setw(72) << "3. Delete Book\n\n";
    cout << setw(73) << "4. Display Book\n\n";
    cout << setw(72) << "5. Search Book\n\n";
    cout << setw(66) << "6. Back\n\n\n";
    cout << setw(86) << "-------------------------------------------\n" << endl;
    cout << setw(76) << "Please Enter Your Choice : ";
    cin >> choose;

    if (choose == 1)
    {
        AddBook();
    }
    else if (choose == 2)
    {
        UpdateBook();
    }
    else if (choose == 3)
    {
        DeleteBook();
    }
    else if (choose == 4)
    {
        ListBook();
    }
    else if (choose == 5)
    {
        SearchBook();
    }
    else if (choose == 6)
    {
        AdminMenu();
    }
}

void AddBook() {
    system("cls");
    headMessage("\t\t\t                  Welcome To Add Book Page         ");

    char option;
    string BOOKTITLE = "";
    string BOOKAUTHOR = "";
    string BOOKGENRE = "";
    string BOOKSHELF = "";

    cout << "\n\n" << setw(68) << "Enter Book Title: ";
    cin.ignore(1, '\n');
    getline(cin, BOOKTITLE);
    cout << "\n\n" << setw(69) << "Enter Book Author: ";
    getline(cin, BOOKAUTHOR);
    cout << "\n\n" << setw(68) << "Enter Book Genre: ";
    getline(cin, BOOKGENRE);
    cout << "\n\n" << setw(68) << "Enter Book Shelf: ";
    getline(cin, BOOKSHELF);

    if (!qstate)
    {
        string insertBook_query = "insert into book (book_title,book_author,book_genre, book_shelf) values ('" + BOOKTITLE + "', '" + BOOKAUTHOR + "', '" + BOOKGENRE + "', '" + BOOKSHELF + "')";
        const char* q = insertBook_query.c_str();
        qstate = mysql_query(conn, q);

        if (!qstate)
        {
            cout << "\n\n\n" << setw(75) << "Succesfully Insert!!!!.\n\n";
            cout << setw(79) << "Do you want to add other book? (y|n): ";
            cin >> option;

            if (option == 'y' || option == 'Y')
                AddBook();
            else
                ListBook();
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

void UpdateBook() {
    system("cls");
    headMessage("\t\t\t                  Welcome To Update Book Page              ");

    string data;
    int choose;
    char option;

    qstate = mysql_query(conn, "select * from book");

    if (!qstate)
    {

        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(18) << ""; cout << "------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(18) << "";
        printf("| %-3s | %-20s | %-20s | %-15s | %-15s | \n", "ID", "Book Title", "Book Author", "Book Genre", "Book Shelf");
        cout << setw(18) << ""; cout << "------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(18) << "";
            printf("| %-5s | %-20s | %-20s | %-15s | %-15s |\n", row[0], row[1], row[2], row[3], row[5]);
            cout << setw(18) << ""; cout << "------------------------------------------------------------------------------------------\n";
        }
        cout << endl << endl << setw(73) << "  Choose Book ID : ";
        cin >> BookID;

        system("cls");

        string findbyid_query = "SELECT * FROM book where book_id='" + BookID + "'";
        const char* qi = findbyid_query.c_str();
        qstate = mysql_query(conn, qi);

        if (!qstate)
        {
            res = mysql_store_result(conn);
            cout << endl;
            while ((row = mysql_fetch_row(res)))
            {
                system("cls");
                headMessage("\t\t\t                 Welcome To Update Book Page       ");

                cout << setw(82) << "---------------------------------------" << endl;
                cout << setw(55) << "Book Title:" << row[1] << endl;
                cout << setw(56) << "Book Author:" << row[2] << endl;
                cout << setw(55) << "Book Genre:" << row[3] << endl;
                cout << setw(55) << "Book Shelf:" << row[5] << endl;
                cout << setw(82) << "---------------------------------------" << endl << endl;
            }
        }
        else
        {
            cout << "Query Execution Problem!" << mysql_error(conn) << endl;
            system("pause");
        }

        cout << setw(71) << "1. Book Title\n\n";
        cout << setw(72) << "2. Book Author\n\n";
        cout << setw(71) << "3. Book Genre\n\n";
        cout << setw(71) << "4. Book Shelf\n\n";
        cout << setw(65) << "5. Back\n\n";
        cout << setw(86) << "-------------------------------------------\n" << endl;
        cout << setw(78) << "Please Enter Your Choice : ";
        cin >> choose;

        cin.ignore(1, '\n');
        if (choose == 1)
        {
            cout << endl << endl << setw(75) << "1. New Book Title: ";
            getline(cin, data);
            string update_query = "update book set book_title ='" + data + "' where book_id='" + BookID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (choose == 2)
        {
            cout << endl << endl << setw(75) << "2. New Book Author: ";
            getline(cin, data);
            string update_query = "update book set book_author='" + data + "' where book_id='" + BookID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (choose == 3)
        {
            cout << endl << endl << setw(75) << "2. New Book Genre: ";
            getline(cin, data);
            string update_query = "update book set book_genre='" + data + "' where book_id='" + BookID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else if (choose == 4)
        {
            cout << endl << endl << setw(75) << "2. New Book Shelf: ";
            getline(cin, data);
            string update_query = "update book set book_shelf='" + data + "' where book_id='" + BookID + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }
        else  if (choose == 5)
        {
            BookMenu();
        }

        cout << endl << setw(85) << "Do you want to update other book? (y|n): ";
        cin >> option;

        if (option == 'y' || option == 'Y')
            UpdateBook();
        else
            ListBook();
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
}

void DeleteBook() {
    system("cls");
    headMessage("\t\t\t                  Welcome To Delete Book Page              ");

    char choose;
    string id;

    qstate = mysql_query(conn, "select * from book");
    if (!qstate)
    {

        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(99) << "-------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(25) << "";
        printf("| %-5s | %-20s | %-20s | %-15s |\n", "ID", "Book Title", "Book Author", "Book Genre");
        cout << setw(99) << "-------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {

            cout << setw(25) << "";
            printf("| %-5s | %-20s | %-20s | %-15s |\n", row[0], row[1], row[2], row[3]);
            cout << setw(99) << "-------------------------------------------------------------------------\n";
        }

        cout << endl << endl << setw(70) << "Enter Book ID to delete: ";
        cin >> id;

        bool found = false;

        string query = "select book_id from book";
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
            string query = "delete from book where book_id='" + id + "'";
            const char* qd = query.c_str();
            qstate = mysql_query(conn, qd);
            if (!qstate)
            {
                cout << endl << setw(75) << "Successfully deleted from book record" << endl;
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
                DeleteBook();
            }
            else if (choose == 'n' || choose == 'N')
            {
                BookMenu();
            }
        }
        cout << endl << setw(78) << "Do you want to Delete another Book?(y|n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
        {
            DeleteBook();
        }
        else
        {
            ListBook();
        }
    }
}

void ListBook() {
    system("cls");
    headMessage("\t\t\t                  Welcome To List Book Page              ");

    qstate = mysql_query(conn, "SELECT * FROM book");
    if (!qstate)
    {
        res = mysql_store_result(conn);

        cout << "\n";
       cout << setw(109) << "---------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(15) << "";
        printf("| %-5s | %-20s | %-20s | %-15s | %-15s |\n", "ID", "Book Title", "Book Author", "Book Genre", "Book Shelf");
        cout << setw(109) << "---------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {

            cout << setw(15) << "";
            printf("| %-5s | %-20s | %-20s | %-15s | %-15s |\n", row[0], row[1], row[2], row[3], row[5]);
           cout << setw(109) << "---------------------------------------------------------------------------------------------\n";
        } 
        
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
    cout << endl << setw(70) << "Press Enter To Back....";
    _getch();
    BookMenu();
}

void SearchBook() {
    system("cls");
    headMessage("\t\t\t                  Welcome To Search Book Page              ");

    char choose;
    string name;

    cin.ignore(1, '\n');
    cout << setw(71) << "Search Book by name: ";
    getline(cin, name);

    string search_query = "select * from book where book_title like '" + name + "%'";
    const char* q = search_query.c_str();
    qstate = mysql_query(conn, q);

    if (!qstate)
    {
        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(99) << "-------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(25) << "";
        printf("| %-5s | %-20s | %-20s | %-15s |\n", "ID", "Book Title", "Book Author", "Book Genre");
        cout << setw(99) << "-------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(25) << "";
            printf("| %-5s | %-20s | %-20s | %-15s |\n", row[0], row[1], row[2], row[3]);
            cout << setw(99) << "-------------------------------------------------------------------------\n";
        }

        cout << endl << setw(75) << "Do you want to search other book?(y|n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
            SearchBook();
        else if (choose == 'n' || choose == 'N')
            BookMenu();
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
}



//STAFFMENU//
void StaffPage()
{
    int menu;
    system("cls");
    headMessage("\t\t\t                 Welcome To Staff Menu Page      ");
    cout << "                Login as:" << staffname << "\n\n";
    cout << setw(71) << "1. Manage Student\n\n" << endl;
    cout << setw(68) << "2. Manage Book\n\n" << endl;
    cout << setw(61) << "3. Logout" << endl;
    cout << setw(83) << "      -----------------------------------------\n" << endl;
    cout << setw(75) << "Please Enter Your Choice : ";
    cin >> menu;

    switch (menu)
    {
    case 1:
        StudMenu();
        break;

    case 2:
        BookIssueMenu();
        break;
    case 3:
        system("cls");
        main();
        break;
    }
}


//STUDENT IN STAFF//
void StudMenu() {
    int choose;
    system("cls");

    cout << setw(1000) << " " << endl;
    headMessage("\t\t\t                 Welcome To Student Menu Page      ");
    cout << setw(74) << "1. Register Student\n\n";
    cout << setw(72) << "2. Update Student\n\n";
    cout << setw(72) << "3. Delete Student\n\n";
    cout << setw(73) << "4. Display Student\n\n";
    cout << setw(72) << "5. Search Student\n\n";
    cout << setw(63) << "6. Back\n\n\n";
    cout << setw(86) << "-------------------------------------------\n" << endl;
    cout << setw(77) << "Please Enter Your Choice : ";
    cin >> choose;

    if (choose == 1)
    {
        RegStud();
    }
    else if (choose == 2)
    {
        UpdateStud();
    }
    else if (choose == 3)
    {
        DeleteStud();
    }
    else if (choose == 4)
    {
        ListStud();
    }
    else if (choose == 5)
    {
        SearchStud();
    }
    else if (choose == 6)
    {
        StaffPage();
    }
}

void RegStud() {
    system("cls");
    headMessage("\t\t\t                    Welcome To Register Student Menu Page       ");

    char option;
    string MEMName = "";
    string MEMPHONE = "";
    string MEMNOMATRIC = "";
    string MEMGENDER = "";
    string MEMEmail = "";


    cout << "\n" << setw(64) << "Enter Name  : ";
    cin.ignore(1, '\n');
    getline(cin, MEMName);
    MEMPHONE = getInputOfLength(10, "Phone number must be 10 digits long.", "Enter Phone : ");
    MEMNOMATRIC = getInputOfLength(10, "Matric No must be 10 digits long", "Enter Matric Num.: ");
    MEMEmail = getValidEmail();
    cout << setw(64) << "Enter Gender : ";
    cin.ignore();
    getline(cin, MEMGENDER);



    if (!qstate)
    {
        string insertMember_query = "insert into student (stud_name,stud_nomatric,stud_contact,stud_email,stud_gender) values ('" + MEMName + "', '" + MEMNOMATRIC + "','" + MEMPHONE + "','" + MEMEmail + "','" + MEMGENDER + "')";
        const char* q = insertMember_query.c_str();
        qstate = mysql_query(conn, q);

        if (!qstate)
        {
            cout << "\n\n\n" << setw(76) << "You have been registered.\n\n";
            cout << setw(81) << "Do you want to add other staff? (y|n): ";
            cin >> option;

            if (option == 'y' || option == 'Y')
                RegStud();
            else
                ListStud();
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

void UpdateStud() {
    system("cls");
    headMessage("\t\t\t                      Welcome To Update Student Page              ");

    string data;
    int choose;
    char option;

    qstate = mysql_query(conn, "select * from student");

    if (!qstate)
    {

        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", "No Matric", "Name", "No.Phone", "Email", "Gender");
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(13) << "";
            printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", row[1], row[0], row[2], row[3], row[4]);
            cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        }
        cout << endl << endl << setw(42) << "  Choose Student No. Matric : ";
        cin >> StudNoMatric;

        system("cls");

        string findbyid_query = "SELECT * FROM student where stud_nomatric='" + StudNoMatric + "'";
        const char* qi = findbyid_query.c_str();
        qstate = mysql_query(conn, qi);

        if (!qstate)
        {
            res = mysql_store_result(conn);
            cout << endl;
            while ((row = mysql_fetch_row(res)))
            {
                system("cls");
                headMessage("\t\t\t                 Welcome To Update Student Page       ");

                cout << setw(82) << "---------------------------------------" << endl;
                cout << setw(54) << "No. Phone:" << row[2] << endl;
                cout << setw(50) << "Email:" << row[3] << endl;
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
            string update_query = "update student set stud_contact='" + data + "' where stud_nomatric='" + StudNoMatric + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }

        else if (choose == 2)
        {
            cout << endl << endl << setw(66) << "2. Email: ";
            getline(cin, data);
            string update_query = "update student set stud_email='" + data + "' where stud_nomatric='" + StudNoMatric + "'";
            const char* q = update_query.c_str();
            qstate = mysql_query(conn, q);
        }


        cout << endl << setw(83) << "Do you want to update other student? (y|n): ";
        cin >> option;

        if (option == 'y' || option == 'Y')
            UpdateStud();
        else
            ListStud();
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
}

void DeleteStud() {
    system("cls");
    headMessage("\t\t\t                      Welcome To Delete Student Page              ");

    char choose;
    string id;

    qstate = mysql_query(conn, "select * from student");
    if (!qstate)
    {

        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", "No Matric", "Name", "No.Phone", "Email", "Gender");
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(13) << "";
            printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", row[1], row[0], row[2], row[3], row[4]);
            cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        }
        cout << endl << endl << setw(43) << "  Choose Student No. Matric : ";
        cin >> StudNoMatric;

        bool found = false;

        string query = "select stud_nomatric from student";
        const char* qd = query.c_str();
        qstate = mysql_query(conn, qd);
        res = mysql_store_result(conn);

        if (!qstate)
        {
            while ((row = mysql_fetch_row(res)))
            {
                if (row[0] == StudNoMatric)
                {
                    found = true;
                    break;
                }
            }
        }

        if (found)
        {
            string query = "delete from student where stud_nomatric='" + StudNoMatric + "'";
            const char* qd = query.c_str();
            qstate = mysql_query(conn, qd);
            if (!qstate)
            {
                cout << endl << setw(35) << "         Successfully deleted from member record" << endl;
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
                DeleteBook();
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
            DeleteStud();
        }
        else
        {
            ListStud();
        }
    }
}

void ListStud() {
    system("cls");
    headMessage("\t\t\t                       Welcome To List Student Page              ");

    qstate = mysql_query(conn, "SELECT * FROM student");
    if (!qstate)
    {
        res = mysql_store_result(conn);


        cout << "\n";
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", "No Matric", "Name", "No.Phone", "Email", "Gender");
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(13) << "";
            printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", row[1], row[0], row[2], row[3], row[4]);
            cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        }


    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
    cout << setw(75) << "Press Enter To Back....";
    _getch();
    StudMenu();
}

void SearchStud() {
    system("cls");
    headMessage("\t\t\t                     Welcome To Search Student Page              ");

    char choose;
    string name;

    cin.ignore(1, '\n');
    cout << setw(24) << "";
    cout << setw(24) << "Search Student by name: ";
    getline(cin, name);

    string search_query = "select * from student where stud_name like '%" + name + "%'";
    const char* q = search_query.c_str();
    qstate = mysql_query(conn, q);

    if (!qstate)
    {
        res = mysql_store_result(conn);


        cout << "\n";
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(13) << "";
        printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", "No Matric", "Name", "No.Phone", "Email", "Gender");
        cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(13) << "";
            printf("|%-15s | %-25s | %-15s | %-20s | %-10s |\n", row[1], row[0], row[2], row[3], row[4]);
            cout << setw(114) << "----------------------------------------------------------------------------------------------------\n";
        }


        cout << endl << setw(50) << "";
        cout << setw(10) << "Do you want to search other student?(y|n): ";
        cin >> choose;

        if (choose == 'y' || choose == 'Y')
            SearchStud();
        else if (choose == 'n' || choose == 'N')
            StudMenu();
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }

}


//FINE IN STAFF//

// Function to calculate the difference in days between two dates
int days_difference(const std::tm& endDate, const std::tm& startDate) {
    std::time_t end_t = std::mktime(const_cast<std::tm*>(&endDate));
    std::time_t start_t = std::mktime(const_cast<std::tm*>(&startDate));
    const int secondsPerDay = 86400;
    double difference = difftime(end_t, start_t) / secondsPerDay;
    return static_cast<int>(difference);
}

// Function to check if the book is returned late and calculate the fine
int calculateFine(const std::tm& issueDate, const std::tm& dueDate, const std::tm& returnDate) {
    int daysLate = days_difference(returnDate, dueDate);

    if (daysLate > 0) {
        // Assuming the fine is RM 2 per day late
        return daysLate * 2;
    }

    return 0; // No fine if returned on time or early
}


void BookIssueMenu() {
    UpdateLateReturns();
    int choose;
    system("cls");
    cout << setw(1000) << " " << endl;
    headMessage("\t\t\t                 Welcome To Book Menu Page           ");
    cout << setw(74) << "1. View Book Issue\n\n";
    cout << setw(79) << "2. View All Book Issues\n\n";
    cout << setw(70) << "3. Borrow Book\n\n";
    cout << setw(72) << "4. Return a Book\n\n";
    cout << setw(64) << "5. Back\n\n\n";
    cout << setw(86) << "-------------------------------------------\n" << endl;
    cout << setw(77) << "Please Enter Your Choice : ";
    cin >> choose;
    if (choose == 1)
    {
        ViewBookIssue();
    }
    else if (choose == 2)
    {
        ViewAllBookIssue();
    }
    else if (choose == 3)
    {
        IssueBook();
    }
    else if (choose == 4)
    {
        ReturnBook();
    }
    else if (choose == 5)
    {
        StaffPage();
    }

}

void ViewBookIssue() {
    system("cls");
    headMessage("\t\t\t                  Welcome To View Book Issue Page              ");

    qstate = mysql_query(conn, "SELECT * FROM borrow WHERE status = 'ISSUED' OR status = 'LATE'");
    if (!qstate)
    {
        res = mysql_store_result(conn);


        cout << setw(24) << "";
        cout << setw(25) << "--------------------------------------------------------------------------------\n";
        // Display the data in a formatted way
        cout << setw(25) << "";
        printf("| %-8s | %-10s | %-7s | %-10s | %-10s | %-15s |\n", "Issue ID", "Member ID", "Book ID", "Issue Date", "Due Date", "Status");

        cout << setw(24) << "";
        cout << setw(25) << "--------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {
            cout << setw(25) << "";
            printf("| %-8s | %-10s | %-7s | %-10s | %-10s | %-15s |\n", row[0], row[1], row[2], row[3], row[4], row[6]);

            cout << setw(24) << "";
            cout << setw(25) << "--------------------------------------------------------------------------------\n";
        }
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }

    cout << endl << setw(70) << "Press Enter To Go Back....";
    _getch();
    BookIssueMenu();
}

void ViewAllBookIssue() {
    system("cls");
    headMessage("\t\t\t                  Welcome To View Book Issue Page              ");

    qstate = mysql_query(conn, "SELECT * FROM borrow");
    if (!qstate)
    {
        res = mysql_store_result(conn);


        cout << setw(10) << "";
        cout << setw(22) << "----------------------------------------------------------------------------------------------------------------\n";
        // Display the data in a formatted way
        cout << setw(10) << "";
        printf("| %-10s | %-10s | %-10s | %-15s | %-15s | %-15s | %-15s |\n", "Issue ID", "Member ID", "Book ID", "Issue Date", "Due Date","Return Date" , "Status", "Pay Status");

        cout << setw(10) << "";
        cout << setw(22) << "----------------------------------------------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {
            cout << setw(10) << "";
            printf("| %-10s | %-10s | %-10s | %-15s | %-15s | %-15s | %-15s |\n", row[0], row[1], row[2], row[3], row[4], row[5], row[6]);

            cout << setw(10) << "";
            cout << setw(22) << "----------------------------------------------------------------------------------------------------------------\n";
        }
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }

    cout << endl << setw(70) << "Press Enter To Go Back....";
    _getch();
    BookIssueMenu();
}

void IssueBook() {
    system("cls");
    headMessage("\t\t\t                  Welcome To Book Issue Page              ");

    string studId, bookId;
    cout << "\n\n" << setw(70) << "Enter Student No Matric: ";
    cin >> studId;

    string name;

    cin.ignore(1, '\n');
    cout << setw(70) << endl << "Search Book by name: ";
    getline(cin, name);

    string search_query = "select * from book where book_title like '" + name + "%'";
    const char* q = search_query.c_str();
    qstate = mysql_query(conn, q);

    if (!qstate)
    {
        res = mysql_store_result(conn);

        cout << "\n";
        cout << setw(99) << "-------------------------------------------------------------------------\n";
        //data show in box
        cout << setw(25) << "";
        printf("| %-5s | %-20s | %-20s | %-15s |\n", "ID", "Book Title", "Book Author", "Book Genre");
        cout << setw(99) << "-------------------------------------------------------------------------\n";

        while ((row = mysql_fetch_row(res)))
        {


            cout << setw(25) << "";
            printf("| %-5s | %-20s | %-20s | %-15s |\n", row[0], row[1], row[2], row[3]);
            cout << setw(99) << "-------------------------------------------------------------------------\n";
        }
    }
    cout << "\n\n" << setw(70) << "Enter Book ID: ";
    cin >> bookId;

    // Check if the book is available
    string bookStatusQuery = "SELECT book_status FROM book WHERE book_id = '" + bookId + "'";
    qstate = mysql_query(conn, bookStatusQuery.c_str());

    if (!qstate)
    {
        res = mysql_store_result(conn);
        if (mysql_num_rows(res) > 0) {
            row = mysql_fetch_row(res);
            string status = row[0];
            if (status == "AVAILABLE") {
                // Issue the book
                string issueBookQuery = "INSERT INTO borrow (stud_nomatric, book_id, start_date, end_date) VALUES ('" + studId + "', '" + bookId + "', CURDATE(), DATE_ADD(CURDATE(), INTERVAL 7 DAY))";
                qstate = mysql_query(conn, issueBookQuery.c_str());

                if (!qstate) {
                    cout << "\n\n" << setw(75) << "Book Issued Successfully!\n\n";

                    // Update book status to 'ISSUED'
                    string updateBookStatusQuery = "UPDATE book SET book_status = 'ISSUED' WHERE book_id = '" + bookId + "'";
                    qstate = mysql_query(conn, updateBookStatusQuery.c_str());
                }
                else {
                    cout << "Query Execution Problem!" << mysql_error(conn) << endl;
                }
            }
            else {
                cout << "\n\n" << setw(75) << "Book is not available for issue.\n\n";
            }
        }
        else {
            cout << "\n\n" << setw(70) << "No such book found.\n\n";
        }
    }
    else {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }

    cout << endl << setw(74) << "Press Enter To Go Back....";
    _getch();
    BookIssueMenu();
}

void ReturnBook() {
    system("cls");
    headMessage("\t\t\t                  Welcome To Book Return Page              ");

    string studMatric, bookId;
    cout << "\n\n" << setw(75) << "Enter Member No Matric: ";
    cin >> studMatric;

    // Display all issued books to the member
    string issuedBooksQuery = "SELECT book_id, start_date, end_date, status, fine, return_date FROM borrow WHERE stud_nomatric = '" + studMatric + "' AND (status = 'ISSUED' OR status = 'LATE')";
    qstate = mysql_query(conn, issuedBooksQuery.c_str());

    if (!qstate) {
        res = mysql_store_result(conn);
        if (mysql_num_rows(res) > 0) {

            cout << setw(24) << "";
            cout << setw(24) << "List of Issued Books:\n\n";

            cout << setw(24) << "";
            cout << setw(24) << "--------------------------------------------------------------------------------------\n";
            // Display the data in a formatted way
            cout << setw(24) << "";
            printf("| %-10s | %-10s | %-10s | %-20s | %-20s |\n", "Book ID", "Issued On", "Due On", "Status", "Fine (RM)");

            cout << setw(24) << "";
            cout << setw(24) << "--------------------------------------------------------------------------------------\n";

            while ((row = mysql_fetch_row(res)))
            {
                cout << setw(24) << "";
                printf("| %-10s | %-10s | %-10s | %-20s | %-20s |\n", row[0], row[1], row[2], row[3], row[4]);

                cout << setw(24) << "";
                cout << setw(24) << "--------------------------------------------------------------------------------------\n";
            }

            // Ask for the book ID to return
            cout << "\n\n" << setw(75) << "Enter Book ID to Return: ";
            cin >> bookId;

            // Check if the book is late
            string checkLateQuery = "SELECT end_date, DATEDIFF(CURDATE(), end_date) AS days, CURDATE() FROM borrow WHERE stud_nomatric = '" + studMatric + "' AND book_id = '" + bookId + "'";

            qstate = mysql_query(conn, checkLateQuery.c_str());
            if (!qstate) {
                res = mysql_store_result(conn);
                if (mysql_num_rows(res) > 0) {
                    row = mysql_fetch_row(res);
                   
                    int daysLate = atoi(row[1]);
                   
                    if (daysLate > 0) {
                        int fine = daysLate * 2; // Assuming the fine is 2 units per day

                        // Update the borrow table with fine and status
                        string updateQuery = "UPDATE borrow SET status = 'RETURNED LATE', return_date = CURDATE(), fine = " + std::to_string(fine) + " WHERE stud_nomatric = '" + studMatric + "' AND book_id = '" + bookId + "'";
                        qstate = mysql_query(conn, updateQuery.c_str());
                        if (qstate) cout << "Query Execution Problem!" << mysql_error(conn) << endl;
                    }
                    else {
                        // Update the borrow table with status only
                        string updateQuery = "UPDATE borrow SET status = 'RETURNED', return_date = CURDATE() WHERE stud_nomatric = '" + studMatric + "' AND book_id = '" + bookId + "'";
                        qstate = mysql_query(conn, updateQuery.c_str());
                        if (qstate) cout << "Query Execution Problem!" << mysql_error(conn) << endl;
                    }
                }

                string updateBookStatusQuery = "UPDATE book SET book_status = 'AVAILABLE' WHERE book_id = '" + bookId + "'";
                qstate = mysql_query(conn, updateBookStatusQuery.c_str());

                if (!qstate) {
                    cout << "\n\n" << setw(75) << "Book Returned Successfully!\n\n";
                }
                else {
                    cout << "Query Execution Problem!" << mysql_error(conn) << endl;
                }
            }
            else {
                cout << "Query Execution Problem!" << mysql_error(conn) << endl;
            }
        }
        else {
            cout << "\n\n" << setw(75) << "No issued books found for this student.\n\n";
        }
    }
    else {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }

    cout << endl << setw(75) << "Press Enter To Go Back....";
    _getch();
    BookIssueMenu();
}

void UpdateLateReturns() {
    

    MYSQL_RES* res;
    MYSQL_ROW row;
    int qstate;

    string checkIssuedBooksQuery = "SELECT stud_nomatric, book_id, end_date, DATEDIFF(CURDATE(), end_date) AS days FROM borrow WHERE status = 'ISSUED'";
    qstate = mysql_query(conn, checkIssuedBooksQuery.c_str());

    if (!qstate) {
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res))) {
            int daysLate = atoi(row[3]);
            if (daysLate > 0) {
                string studMatric = row[0];
                string bookId = row[1];
                int fine = daysLate * 2; // Assuming the fine is 2 units per day

                // Update the borrow table with fine and status
                string updateQuery = "UPDATE borrow SET status = 'LATE', fine = " + std::to_string(fine) + " WHERE stud_nomatric = '" + studMatric + "' AND book_id = '" + bookId + "'";
                mysql_query(conn, updateQuery.c_str());
            }
        }
        mysql_free_result(res);
        
    }
    else {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }
}



//REPORT IN ADMIN//
void ReportMenu() {
    int choose;
    system("cls");

    cout << setw(1000) << " " << endl;
    headMessage("\t\t\t                 Welcome To Report Menu Page             ");
    cout << setw(75) << "1. View Overall Report\n\n";
    cout << setw(71) << "2. Report by Month\n\n";
    cout << setw(61) << "3. Back\n\n\n";
    cout << setw(86) << "-------------------------------------------\n" << endl;
    cout << setw(77) << "Please Enter Your Choice : ";
    cin >> choose;

    if (choose == 1)
    {
        OverallReport();

    }
    else if (choose == 2)
    {
        ReportByMonth();
    }
    else
    {
        AdminMenu();
    }

}

void OverallReport() {
    system("cls");

    headMessage("\t\t\t                  Overall System Report              ");

    MYSQL_RES* res;
    MYSQL_ROW row;
    int qstate;

    // Execute and process each query
    string queries[] = {
        "SELECT COUNT(*) FROM borrow", // Total Issuances
        "SELECT COUNT(*) FROM student", // Total Members
        "SELECT COUNT(*) FROM book", // Total Books
        "SELECT SUM(fine) FROM borrow", // Total Fines
        "SELECT a.book_id, b.book_title, b.book_author, COUNT(a.book_id) AS count FROM borrow a INNER JOIN book b WHERE a.book_id = b.book_id GROUP BY book_id ORDER BY count DESC LIMIT 1", // Most Popular Book
        "SELECT a.stud_nomatric, a.stud_name, COUNT(b.stud_nomatric) AS count FROM borrow b INNER JOIN student a WHERE a.stud_nomatric = b.stud_nomatric GROUP BY stud_nomatric ORDER BY count DESC LIMIT 1" // Top Reader
    };

    string reportTitles[] = {
        "Total Issuances: ",
        "Total Students: ",
        "Total Books: ",
        "Total Fines Collected: ",
        "Most Popular Book: ",
        "Top Reader: "
    };

    for (int i = 0; i < 6; ++i) { // Reduced the loop count to 6
        qstate = mysql_query(conn, queries[i].c_str());
        if (!qstate) {
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);
            cout << setw(75);
            if (i == 4) { // Most Popular Book
                cout << setw(40) << reportTitles[i]
                    << setw(15) << (row[0] ? row[0] : "0")
                    << " ( " << (row[1] ? row[1] : "No data")
                    << " by " << (row[2] ? row[2] : "No data")
                    << " ) [" << (row[3] ? row[3] : "No data") << " issuance(s)]" << endl;
            }
            else if (i == 3) { // Total Fine
                cout << setw(40) << reportTitles[i]
                    << setw(15) << (row[0] ? row[0] : "0") << endl;
            }
            else if (i == 5) { // Top Reader
                cout << setw(40) << reportTitles[i]
                    << setw(15) << (row[0] ? row[0] : "0")
                    << " ( " << (row[1] ? row[1] : "No data")
                    << " ) [" << (row[2] ? row[2] : "No data") << " book(s) borrowed]" << endl;
            }
            else { // Other metrics
                cout << setw(40) << reportTitles[i]
                    << setw(15) << (row[0] ? row[0] : "0") << endl;
            }
            mysql_free_result(res);
        }
        else {
            cout << "Query Execution Problem! Error: " << mysql_error(conn) << endl;
        }
    }

    cout << "\n" << setw(75) << "Press Enter To Go Back....";
    _getch();
    ReportMenu();
}

void ReportByMonth() {
    system("cls");
    headMessage("\t\t\t                  Monthly System Report              ");

    string year, month;
    cout << setw(44) << "Enter Year (YYYY): ";
    cin >> year;
    cout << setw(43) << "Enter Month (MM): ";
    cin >> month;


    MYSQL_RES* res;
    MYSQL_ROW row;
    int qstate;

    // Execute and process each query
    string queries[] = {
        "SELECT COUNT(*) FROM borrow WHERE YEAR(start_date) = '" + year + "' AND MONTH(start_date) = '" + month + "'", // Total Issuances in the month
        "SELECT SUM(fine) FROM borrow WHERE YEAR(start_date) = '" + year + "' AND MONTH(start_date) = '" + month + "'", // Total Fines in the month
        "SELECT book_id, COUNT(book_id) AS count FROM borrow WHERE YEAR(start_date) = '" + year + "' AND MONTH(start_date) = '" + month + "' GROUP BY book_id ORDER BY count DESC LIMIT 1", // Most Popular Book in the month
        "SELECT stud_nomatric, COUNT(stud_nomatric) AS count FROM borrow WHERE YEAR(start_date) = '" + year + "' AND MONTH(start_date) = '" + month + "' GROUP BY stud_nomatric ORDER BY count DESC LIMIT 1" // Top Reader in the month
    };
    string reportTitles[] = {
        "Total Issuances: ",
        "Total Fines Collected: ",
        "Most Popular Book (ID): ",
        "Top Reader (Member ID): "
    };

    for (int i = 0; i < 4; ++i) {
        qstate = mysql_query(conn, queries[i].c_str());
        if (!qstate) {
            res = mysql_store_result(conn);
            row = mysql_fetch_row(res);
            cout << setw(40) << reportTitles[i] << (row && row[0] ? row[0] : "0") << endl;
            mysql_free_result(res);
        }
        else {
            cout << "Query Execution Problem! Error: " << mysql_error(conn) << endl;
        }
    }

    cout << "\n" << setw(75) << "Press Enter To Go Back....";
    _getch();
    ReportMenu();
}

void studlogin() {
    system("cls");
    headMessage("\t\t\t                  Welcome To Student Page              ");
    char choose;
    string nomatric;

    cin.ignore(1, '\n');
    cout << setw(24) << "";
    cout << setw(24) << "Enter No. Matric: ";
    getline(cin, StudNoMatric);

    string search_query = "SELECT a.stud_nomatric, b.book_title, a.start_date, a.end_date FROM borrow a INNER JOIN book b ON b.book_id = a.book_id WHERE stud_nomatric LIKE '%" + StudNoMatric + "%'";
    const char* q = search_query.c_str();
    qstate = mysql_query(conn, q);

    if (!qstate) {
        res = mysql_store_result(conn);

        cout << setw(24) << "";
        cout << setw(24) << string(80, '-') << "\n";
        //data show in box
        cout << setw(24) << "";
        printf("| %-50s | %-10s | %-10s |\n", "Book Title", "Start Date", "End Date");

        cout << setw(24) << "";
        cout << setw(24) << string(80, '-') << "\n";
        while ((row = mysql_fetch_row(res))) {
            cout << setw(24) << "";
            printf("| %-50s | %-10s | %-10s |\n", row[1], row[2], row[3]);
            cout << setw(24) << "";
            cout << setw(24) << string(80, '-') << "\n";
        }

        processLateBooks(StudNoMatric);
    }
    else {
        cout << "Query Execution Problem!" << mysql_error(conn) << endl;
    }

    cout << setw(24) << " ";
    cout << "Press enter to go back";
    _getch();
    main();
}

void processLateBooks(string matricNumber) {
    map<int, double> idVSFine;
    vector<int> allBorrowIds;
    double amountPaid;
    string query = "SELECT a.borrow_id, b.book_title, a.start_date, a.end_date, a.fine FROM borrow a INNER JOIN book b ON b.book_id = a.book_id WHERE stud_nomatric LIKE '%@noMatric%' AND end_date < CURRENT_DATE() AND status LIKE '%LATE%'";
    replaceSubString(query, "@noMatric", matricNumber);
    qstate = mysql_query(conn, query.c_str());

    if (qstate != 0) {
        cout << mysql_error(conn) << endl;
    }

    res = mysql_store_result(conn);
    int numberOfLateBooks = mysql_num_rows(res);

    if (numberOfLateBooks == 0) {
        cout << setw(24) << " ";
        cout << "You have no late return fine" << endl;
        return;
    }
    else {
        cout << endl;
        cout << setw(24) << " ";
        cout << "You have " << numberOfLateBooks << " late return fine" << endl;
        cout << setw(24) << " ";
        cout << "Press enter to proceed with payment" << endl;
        _getch();

        system("cls");
        headMessage("\t\t\t                  Welcome To Student Page              ");
        cout << setw(24) << "";
        cout << setw(24) << string(72, '-') << "\n";
        //data show in box
        cout << setw(24) << "";
        printf("| %-15s | %-50s |\n", "Borrow ID", "Book Title");

        cout << setw(24) << "";
        cout << setw(24) << string(72, '-') << "\n";
        while ((row = mysql_fetch_row(res))) {
            idVSFine[stoi(row[0])] = stod(row[4]);
            allBorrowIds.push_back(stoi(row[0]));
            cout << setw(24) << "";
            printf("| %-15s | %-50s |\n", row[0], row[1]);
            cout << setw(24) << "";
            cout << setw(24) << string(72, '-') << "\n";
        }

        int borrowId;
        cout << endl;
        borrowId = getInputFromUser(allBorrowIds, "Invalid Borrow ID", "Enter Borrow ID: ");
        amountPaid = idVSFine[borrowId];

        processPayment(matricNumber, borrowId, amountPaid);
    }
}

string getInputFromUser(vector<string> validOptions, string warningMessage, string prompt) {
    string userInput;

    do {
        cout << setw(24) << " ";
        cout << prompt;
        cin >> userInput;

        auto it = find(validOptions.begin(), validOptions.end(), userInput);

        if (it != validOptions.end()) {
            break;  // Input is valid, exit the loop
        }
        else {
            cout << setw(24) << " ";
            cout << warningMessage << endl;
        }
    } while (true);

    return userInput;
}

int getInputFromUser(vector<int> validOptions, string warningMessage, string prompt) {
    int userInput;

    do {
        cout << setw(24) << " ";
        cout << prompt;
        cin >> userInput;

        auto it = find(validOptions.begin(), validOptions.end(), userInput);

        if (it != validOptions.end()) {
            break;  // Input is valid, exit the loop
        }
        else {
            cout << setw(24) << " ";
            cout << warningMessage << endl;
        }
    } while (true);

    return userInput;
}



// Function to process payment//
void processPayment(string StudNoMatric, int i_borrowId, double amountPaid) {
    string borrowId = to_string(i_borrowId);
    char choose;
    string returnDate;  // Add a variable to store the return date

    system("cls");
    headMessage("\t\t\t                  Make Payment             ");
    cout << setw(24) << left << " " << "[1] Pay with Cash" << endl;
    cout << setw(24) << left << " " << "[2] Pay Online" << endl;
    cout << setw(24) << left << " " << "[3] Cancel" << endl << endl;
    cout << setw(24) << left << " " << "Choose Your Preferred Method: ";
    cin.ignore();
    cin >> choose;

    if (choose == '1') {
        string updatepay_query = "update borrow set pay_status='Paid' where borrow_id = " + borrowId;
        const char* qu = updatepay_query.c_str();
        qstate = mysql_query(conn, qu);

        if (!qstate) {
            cout << setw(24) << "" << "Thank You For The Payment!!!" << endl;
        }
        else {
            cout << mysql_error(conn);
        }

        displayReceipt("Cash", amountPaid);  // Replace this with your receipt display function
    }
    else if (choose == '2') {
        cout << setw(24) << "" << "[1] Bank Islam" << endl;
        cout << setw(24) << "" << "[2] Maybank" << endl;
        cout << setw(24) << "" << "[3] Bank Rakyat" << endl;;
        cout << setw(24) << "" << "Choose: ";
        int i_choose;
        cin >> i_choose;

       
        if (i_choose >= 1 and i_choose <= 3)
        {
            generateTransactionReceipt();
            cout << setw(91) << endl << "Successfully Paid!" << endl;

            string updatepay3_query = "update borrow set pay_status = 'Paid', return_date = CURDATE() where borrow_id = " + borrowId;
            const char* qu = updatepay3_query.c_str();
            qstate = mysql_query(conn, qu);

            if (!qstate) {
                cout << "Payment successful..." << endl;
            }
            else {
                cout << mysql_error(conn);
            }
        }
    }
    else if (choose == '3') {
        cout << "Payment canceled.";
        studlogin();
    }
    else {
        cout << "Invalid choice. Payment canceled.";
    }

    _getch();
   
}

void generateTransactionReceipt() {
    stringstream receipt;

    receipt << setw(24) << "" << "-------------Transaction Receipt-----------" << endl << endl;

    // Generate a random reference number
    stringstream refNum;
    for (int i = 0; i < 10; ++i) {
        if (rand() % 2 == 0) {
            // Include a random letter (50% chance)
            refNum << char('A' + rand() % 26);
        }
        else {
            // Include a random digit (50% chance)
            refNum << char('0' + rand() % 10);
        }
    }

    // Get current date and time
    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);

    // Append reference number, date, and time to the receipt
    receipt << setw(24) << "" << "Enter Reference No. : " << refNum.str() << endl;
    receipt << setw(24) << "" << "Enter Date: " << put_time(localTime, "%Y-%m-%d") << endl;
    receipt << setw(24) << "" << "Enter Time: " << put_time(localTime, "%H:%M") << endl;

    cout << receipt.str();
}

void displayReceipt(string paymentMethod, int amountPaid) {
    if (paymentMethod == "Cash") {
        cout << setw(24) << "" << "Transaction Receipt\n";
        cout << setw(24) << "" << "---------------------\n";
    }
    cout << setw(24) << "" << "Payment Method: " << paymentMethod << endl;
    cout << setw(24) << "" << "Amount Paid: RM " << amountPaid << endl;
    cout << setw(24) << "" << "Thank you for your payment!\n";
    cout << setw(24) << "" << "---------------------\n";
}

void replaceSubString(string& str, string str2, string str3) {
    str.replace(str.find(str2), str2.length(), str3);
}

string getInputOfLength(int requiredLength, string warning, string prompt) {
    string userInput;

    do {
        cout << setw(64) << prompt;
        cin >> userInput;

        if (userInput.length() == requiredLength) {
            break;  // Input is valid, exit the loop
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
        cout << setw(64) << "Enter your email address : ";
        cin >> userEmail;

        if (userEmail.find('@') != string::npos) {
            break;  // Valid email, exit the loop
        }
        else {
            cout << "Invalid email address. It must contain the '@' symbol." << endl;
        }
    } while (true);

    return userEmail;
}

