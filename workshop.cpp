
#include <mysql/jdbc.h>
#include <iostream>
#include <conio.h>
#include <iomanip>
#include <regex>
#include <string>
#include <set>

using namespace std;
using namespace sql;

void Menu();
void admin();
void staff(const string& staffName);
void FStaff();
void order(const string& staffName);
void customer(const string& customerName);
void FCustomer();
void anotherOrder(string customerName);
void customer(const string& customerName);
void searchCustomer(Connection* connection);
void searchStaff(Connection* connection);
void Sales();
void SMenu();

Connection* databaseCon() {
    try {
        // Create a MySQL Connector/C++ driver
        Driver* driver = get_driver_instance();

        // Establish a connection to the MySQL database
        Connection* con = driver->connect("tcp://127.0.0.1:3306 ", "root", "");
        con->setSchema("workshop 1");

        return con;
    }
    catch (SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState : " << e.getSQLState() << endl;
        return nullptr;
    }
}

void staffprofile(Connection* connection, const string& staffName)
{
    system("cls");
    string newName, newUsername, newPassword, newEmail, searchName, newContact;
    char choice;

    try {
        while (true) {

            PreparedStatement* ps = connection->prepareStatement("SELECT * FROM staff WHERE Staff_Name = ?");
            ps->setString(1, staffName);
            ResultSet* res = ps->executeQuery();

            if (res->next()) {
                string maskedPassword(res->getString("Staff_Password").length(), '*');
                cout << "\n\t\tCurrent Details:\n";
                cout << "\t\tName    : " << staffName << endl;
                cout << "\t\tUsername: " << res->getString("Staff_Username") << endl;
                cout << "\t\tPassword: " << maskedPassword << endl;
                cout << "\t\tIC      : " << res->getString("Staff_IC") << endl;
                cout << "\t\tContact Number: " << res->getString("Staff_Contact") << endl;
                cout << "\t\tEmail   : " << res->getString("Staff_Email") << endl;


                char a;
                cout << "\n\t\tDo you want to change password (Y/N): ";
                cin >> a;

                if (a == 'Y' || a == 'y')
                {
                    bool validPassword = false;
                    while (!validPassword) {
                        cout << "\n\t\tEnter new password \n\t\t(at least 6 characters with at include alphabet and number): ";
                        newPassword = "";
                        char ch;
                        while ((ch = _getch()) != '\r') {
                            if (ch == '\b') {
                                if (!newPassword.empty()) {
                                    cout << "\b \b";
                                    newPassword.pop_back();
                                }
                            }
                            else {
                                newPassword += ch;
                                cout << '*';
                            }
                        }

                        if (newPassword.length() < 6) {
                            cerr << "\n\n\t\tPassword must be at least 6 characters long." << endl;
                        }
                        else if (!regex_search(newPassword, regex("[a-zA-Z]"))) {
                            cerr << "\n\n\t\tPassword must contain at least one alphabet." << endl;
                        }
                        else if (!regex_search(newPassword, regex("[0-9]"))) {
                            cerr << "\n\n\t\tPassword must contain at least one number." << endl;
                        }
                        else {
                            validPassword = true;
                        }
                    }

                    PreparedStatement* psUpdate = connection->prepareStatement("UPDATE staff SET Staff_Password = COALESCE(?, Staff_Password) WHERE Staff_Name = ?");
                    psUpdate->setString(1, newPassword);
                    psUpdate->setString(2, staffName);


                    psUpdate->executeUpdate();

                    cout << "\n\t\tStaff details updated successfully!" << endl;

                    delete psUpdate;
                    delete ps;
                    cout << "\n\t\tPress Enter to continue...";
                    while (_getch() != 13);
                    staff(staffName);
                    
                }
                else
                {
                    staff(staffName);
                }

            }
            else 
            {
                cout << "\n\t\tStaff with name " << staffName << " not found." << endl;
            }
           delete res; 
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState: " << e.getSQLState() << endl;
    }
}
void registerStaff() {

    
    string name, username, password, email, IC,c_number;
    while (true)
    {   system("cls");
        try {
            Connection* connection = databaseCon();
            if (!connection) {
                cerr << "Failed to connect to the database." << endl;

            }

            cout << "\n\t\t\tEnter name: ";
            cin.ignore();
            getline(cin, name);

            cout << "\t\t\tEnter username: ";
            cin >> username;

            bool validPassword = false;
            while (!validPassword) {
                cout << "\t\t\tEnter password \n\t\t\t(at least 6 characters with at include alphabet and number): ";
                password = "";
                char ch;
                while ((ch = _getch()) != '\r') {
                    if (ch == '\b') {
                        if (!password.empty()) {
                            cout << "\b \b";
                            password.pop_back();
                        }
                    }
                    else {
                        password += ch;
                        cout << '*';
                    }
                }

                if (password.length() < 6) {
                    cerr << "\n\n\t\t\tPassword must be at least 6 characters long." << endl;
                }
                else if (!regex_search(password, regex("[a-zA-Z]"))) {
                    cerr << "\n\n\t\t\tPassword must contain at least one alphabet." << endl;
                }
                else if (!regex_search(password, regex("[0-9]"))) {
                    cerr << "\n\n\t\t\tPassword must contain at least one number." << endl;
                }
                else {
                    validPassword = true;
                }
            };

            cout << "\n\t\t\tIC number  : ";
            cin >> IC;

            cout << "\t\t\tEnter contact number: ";
            cin >> c_number;

            cout << "\t\t\tEnter email: ";
            cin >> email;

            PreparedStatement* ps = connection->prepareStatement("INSERT INTO staff (Staff_Name, Staff_Username, Staff_Password, Staff_IC, Staff_Contact, Staff_Email) VALUES (?, ?, ?, ?, ?, ?)");
            ps->setString(1, name);
            ps->setString(2, username);
            ps->setString(3, password);
            ps->setString(4, IC);
            ps->setString(5, c_number);
            ps->setString(6, email);

            ps->execute();

            cout << "\n\t\t\tRegistration successful!" << endl;


            delete ps;
            delete connection;
        }
        catch (const sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
            cerr << "SQLState: " << e.getSQLState() << endl;
        }
        catch (...) {
            cerr << "An exception occurred." << endl;
        }
        char choice4;
        cout << "\n\t\tDo you want to register another customer (Y/N): ";
        cin >> choice4;
        if (choice4 != 'Y' && choice4 != 'y') {
            FStaff();
        }
    }
}
void displayStaff(Connection* connection) {

    system("cls");
    char  cont;
    try {
        Statement* stmt = connection->createStatement();
        ResultSet* res = stmt->executeQuery("SELECT Staff_id, Staff_Name, Staff_Username, Staff_Password, Staff_IC, Staff_Contact, Staff_Email FROM staff");

        cout << "\n----------------------------------------------------------------------------------------------------------------------------------------------------\n";
        cout << setw(10) << "Staff ID" << setw(20) << "Name" << setw(20) << "Username" << setw(20) << "Password" << setw(20) << "IC" << setw(20) << "Contact" << setw(20) << "Email" << endl;
        cout << "\n------------------------------------------------------------------------------------------------------------------------------------------------------\n";

        while (res->next()) {
            string maskedPassword(res->getString("Staff_Password").length(), '*');
            cout << setw(10) << res->getInt("Staff_id") << setw(20) << res->getString("Staff_Name") << setw(20) << res->getString("Staff_Username") << setw(20) << maskedPassword << setw(20) << res->getString("Staff_IC") << setw(20) << res->getString("Staff_Contact") << setw(20) << res->getString("Staff_Email") << endl<<endl;
        }

        cout << "\n-------------------------------------------------------------------------------------------------------------------------------------------------------\n";

        delete res;
        delete stmt;

        do {
            cout << "\nDo you want to search the staff name (Y/N) : ";
            cin >> cont;

            if (cont == 'Y' || cont == 'y')
            {
                searchStaff(connection);
            }
            else if (cont == 'N' || cont == 'n')
            {
                FStaff();
            }
            else
            {
                cout << "\n\t\tWrong input please enter Y or N only\n";
            }
        } while (cont != 'Y' || cont != 'y' || cont == 'N' || cont == 'n');
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState: " << e.getSQLState() << endl;
    }
}
void updateStaff(Connection* connection) {

    system("cls");
    string newName, newUsername, newPassword, newEmail,searchName,newContact;
    char choice;

    try {
        while (true) {
            cout << "\n\t\tEnter the Staff Name to update: ";
            cin.ignore();
            getline(cin, searchName);

            PreparedStatement* ps = connection->prepareStatement("SELECT * FROM staff WHERE Staff_Name = ?");
            ps->setString(1, searchName);
            ResultSet* res = ps->executeQuery();

            if (res->next()) {
                string maskedPassword(res->getString("Staff_Password").length(), '*');
                cout << "\n\t\tCurrent Details:\n";
                cout << "\t\tName    : " << res->getString("Staff_Name") << endl;
                cout << "\t\tUsername: " << res->getString("Staff_Username") << endl;
                cout << "\t\tPassword: " << maskedPassword << endl;
                cout << "\t\tIC      : " << res->getString("Staff_IC") << endl;
                cout << "\t\tContact Number: " << res->getString("Staff_Contact") << endl;
                cout << "\t\tEmail   : " << res->getString("Staff_Email") << endl;

                cout << "\n\t\tEnter new details (Press Enter to keep existing values):\n\n";
              
                cout << "\t\tNew Username: ";
                cin >> newName;
              
                bool validPassword = false;
                while (!validPassword) {
                    cout << "\t\tEnter new password \n\t\t(at least 6 characters with at include alphabet and number): ";
                    newPassword = "";
                    char ch;
                    while ((ch = _getch()) != '\r') {
                        if (ch == '\b') {
                            if (!newPassword.empty()) {
                                cout << "\b \b";
                                newPassword.pop_back();
                            }
                        }
                        else {
                            newPassword += ch;
                            cout << '*';
                        }
                    }

                    if (newPassword.length() < 6) {
                        cerr << "\n\n\t\tPassword must be at least 6 characters long." << endl;
                    }
                    else if (!regex_search(newPassword, regex("[a-zA-Z]"))) {
                        cerr << "\n\n\t\tPassword must contain at least one alphabet." << endl;
                    }
                    else if (!regex_search(newPassword, regex("[0-9]"))) {
                        cerr << "\n\n\t\tPassword must contain at least one number." << endl;
                    }
                    else {
                        validPassword = true;
                    }
                }
                cout << endl;
                cout << "\t\tNew Contact Number: ";
                cin >> newContact;
                cout << "\t\tNew Email: ";
                cin >> newEmail;

                PreparedStatement* psUpdate = connection->prepareStatement("UPDATE staff SET Staff_Username = COALESCE(?, Staff_Username), Staff_Password = COALESCE(?, Staff_Password), Staff_Contact = COALESCE(?, Staff_Contact), Staff_Email = COALESCE(?, Staff_Email) WHERE Staff_Name = ?");
                psUpdate->setString(1, newUsername);
                psUpdate->setString(2, newPassword);
                psUpdate->setString(3, newContact);
                psUpdate->setString(4, newEmail);
                psUpdate->setString(5, searchName);
              

                psUpdate->executeUpdate();

                cout << "\n\t\tStaff details updated successfully!" << endl;

                delete psUpdate;
                delete ps;
            }
            else {
                cout << "\n\t\tStaff with name " << searchName << " not found." << endl;
            }

            delete res;
            

            cout << "\n\t\tDo you want to update another staff (Y/N): ";
            cin >> choice;
            if (choice != 'Y' && choice != 'y') {
                FStaff();
                
            }
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState: " << e.getSQLState() << endl;
    }
}
void deleteStaff(Connection* connection) {

    system("cls");
    string staffName;
    char confirmDelete;

    try {
        while (true) {
            cout << "\n\t\tEnter the Staff Name to delete: ";
            cin.ignore(); 
            getline(cin, staffName);

            PreparedStatement* ps = connection->prepareStatement("DELETE FROM staff WHERE Staff_Name = ?");
            ps->setString(1, staffName);

            int rowsAffected = ps->executeUpdate();
            if (rowsAffected > 0) {
                cout << "\n\t\tStaff with Name " << staffName << " deleted successfully." << endl;

                // Decrement staff_id for remaining staff members
                PreparedStatement* updatePs = connection->prepareStatement("UPDATE Staff SET Staff_id = Staff_id - 1 WHERE Staff_id > ?");
                updatePs->setInt(1, rowsAffected);
                updatePs->executeUpdate();
                delete updatePs;
            }
            else {
                cout << "\n\t\tStaff with Name " << staffName << " not found." << endl;
            }

            delete ps;

            cout << "\n\t\tDo you want to delete another staff (Y/N): ";
            cin >> confirmDelete;
            if (confirmDelete != 'Y' && confirmDelete != 'y') {
                FStaff();
            }
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState: " << e.getSQLState() << endl;
    }
}
void searchStaff(Connection* connection) {
    
    system("cls");
    string staffName;

    try {
        while (true) {
          
            cout << "\n\t\tEnter the Staff Name to search: ";
            cin.ignore(); 
            getline(cin, staffName);

            PreparedStatement* ps = connection->prepareStatement("SELECT * FROM staff WHERE Staff_Name = ?");
            ps->setString(1, staffName);

            ResultSet* res = ps->executeQuery();

            if (res->next()) {
                string maskedPassword(res->getString("Staff_Password").length(), '*');
                cout << "\n\t\tStaff Details:\n";
                cout << "\t\tStaff ID: " << res->getInt("Staff_id") << endl;
                cout << "\t\tName    : " << res->getString("Staff_Name") << endl;
                cout << "\t\tUsername: " << res->getString("Staff_Username") << endl;
                cout << "\t\tPassword: " << maskedPassword << endl;
                cout << "\t\tIC      : " << res->getString("Staff_IC") << endl;
                cout << "\t\tContact Number: " << res->getString("Staff_Contact") << endl;
                cout << "\t\tEmail   : " << res->getString("Staff_Email") << endl;
            }
            else {
                cout << "\n\t\tStaff with name " << staffName << " not found." << endl;
            }

            delete res;
            delete ps;

            char choice;
            cout << "\n\t\tDo you want to search for another staff (Y/N): ";
            cin >> choice;
            if (choice != 'Y' && choice != 'y') {
                FStaff();
            }
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState : " << e.getSQLState() << endl;
    }
}
void registerCustomer() {

    system("cls");
    string name, username, password, email, IC,c_number;
    while (true)
    {
        try {
            Connection* connection = databaseCon();
            if (!connection) {
                cerr << "Failed to connect to the database." << endl;

            }

            cout << "\n\t\t\tEnter name: ";
            cin.ignore();
            getline(cin, name);

            cout << "\t\t\tEnter username: ";
            cin >> username;

            bool validPassword = false;
            while (!validPassword) {
                cout << "\t\t\tEnter password \n\t\t\t(at least 6 characters with at include alphabet and number): ";
                password = "";
                char ch;
                while ((ch = _getch()) != '\r') {
                    if (ch == '\b') {
                        if (!password.empty()) {
                            cout << "\b \b";
                            password.pop_back();
                        }
                    }
                    else {
                        password += ch;
                        cout << '*';
                    }
                }

                if (password.length() < 6) {
                    cerr << "\n\n\t\t\tPassword must be at least 6 characters long." << endl;
                }
                else if (!regex_search(password, regex("[a-zA-Z]"))) {
                    cerr << "\n\n\t\t\tPassword must contain at least one alphabet." << endl;
                }
                else if (!regex_search(password, regex("[0-9]"))) {
                    cerr << "\n\n\t\t\tPassword must contain at least one number." << endl;
                }
                else {
                    validPassword = true;
                }
            };

            cout << "\n\t\t\tIC number  : ";
            cin >> IC;

            cout << "\t\t\tEnter contact number: ";
            cin >> c_number;

            cout << "\t\t\tEnter email: ";
            cin >> email;

            PreparedStatement* ps = connection->prepareStatement("INSERT INTO customer (Cust_Name, Cust_Username, Cust_Password, Cust_IC, Cust_Contact, Cust_Email) VALUES (?, ?, ?, ?, ?, ?)");
            ps->setString(1, name);
            ps->setString(2, username);
            ps->setString(3, password);
            ps->setString(4, IC);
            ps->setString(5, c_number);
            ps->setString(6, email);

            ps->execute();

            cout << "\n\t\t\tRegistration successful!" << endl;


            delete ps;
            delete connection;
        }
        catch (const sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
            cerr << "SQLState: " << e.getSQLState() << endl;
        }
        catch (...) {
            cerr << "An exception occurred." << endl;
        }
        char choice4;
        cout << "\n\t\tDo you want to register another customer (Y/N): ";
        cin >> choice4;
        if (choice4 != 'Y' && choice4 != 'y') {
            FCustomer();
        }
    }

}
void displayCustomer(Connection* connection) {

    system("cls");
    char  cont;
    try {
        Statement* stmt = connection->createStatement();
        ResultSet* res = stmt->executeQuery("SELECT Cust_id, Cust_Name, Cust_Username, Cust_Password, Cust_IC, Cust_Contact, Cust_Email FROM customer");

        cout << "\n----------------------------------------------------------------------------------------------------------------------------------------------------\n";
        cout << setw(10) << "Customer ID" << setw(10) << "Name" << setw(20) << "Username" << setw(20) << "Password" << setw(15) << "IC" << setw(25) << "Contact" << setw(20) << "Email" << endl;
        cout << "\n------------------------------------------------------------------------------------------------------------------------------------------------------\n";

        while (res->next()) {
            string maskedPassword(res->getString("Cust_Password").length(), '*');
            cout << setw(10) << res->getInt("Cust_id") << setw(10) << res->getString("Cust_Name") << setw(20) << res->getString("Cust_Username") << setw(20) << maskedPassword << setw(20) << res->getString("Cust_IC") << setw(20) << res->getString("Cust_Contact") << setw(25) << res->getString("Cust_Email") << endl << endl;
        }

        cout << "\n-------------------------------------------------------------------------------------------------------------------------------------------------------\n";

        delete res;
        delete stmt;

        do{
        cout << "Do you want to search the customer name (Y/N) : ";
        cin >> cont;

        if (cont == 'Y' || cont == 'y')
        {
            searchCustomer(connection);
        }
        else if (cont == 'N' || cont == 'n')
        {
            FCustomer();
        }
        else
        {
            cout << "\n\t\tWrong input please enter Y or N only";
        }
        } while (cont != 'Y' || cont != 'y');
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState : " << e.getSQLState() << endl;
    }
}
void updateCustomer(Connection* connection) {

    system("cls");
    string newName, newUsername, newPassword, newEmail, newIC,custName,newContact;
    char choice;

    try {
        while (true) {
            cout << "\n\n\t\tEnter the Customer name to update: ";
            cin.ignore();
            getline(cin, custName);

            Statement* stmt = connection->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM customer WHERE Cust_Name = '" + custName + "'");

            if (res->next()) {
                string maskedPassword(res->getString("Cust_Password").length(), '*');
                cout << "\n\t\tCurrent Details\n\n";
                cout << "\t\tName    : " << res->getString("Cust_Name") << endl;
                cout << "\t\tUsername: " << res->getString("Cust_Username") << endl;
                cout << "\t\tPassword: " << maskedPassword << endl;
                cout << "\t\tIC      : " << res->getString("Cust_IC") << endl;
                cout << "\t\tContact Number: " << res->getString("Cust_Contact") << endl;
                cout << "\t\tEmail   : " << res->getString("Cust_Email") << endl;

                cout << "\n\t\tEnter new details (Press Enter to keep existing values):\n";
                
                cout << "\t\tNew Username: ";
                getline(cin >> ws, newUsername);

                bool validPassword = false;
                while (!validPassword) {
                    cout << "\t\tEnter new password \n\t\t(at least 6 characters with at include alphabet and number): ";
                    newPassword = "";
                    char ch;
                    while ((ch = _getch()) != '\r') {
                        if (ch == '\b') {
                            if (!newPassword.empty()) {
                                cout << "\b \b";
                                newPassword.pop_back();
                            }
                        }
                        else {
                            newPassword += ch;
                            cout << '*';
                        }
                    }

                    if (newPassword.length() < 6) {
                        cerr << "\n\n\t\tPassword must be at least 6 characters long." << endl;
                    }
                    else if (!regex_search(newPassword, regex("[a-zA-Z]"))) {
                        cerr << "\n\n\t\tPassword must contain at least one alphabet." << endl;
                    }
                    else if (!regex_search(newPassword, regex("[0-9]"))) {
                        cerr << "\n\n\t\tPassword must contain at least one number." << endl;
                    }
                    else {
                        validPassword = true;
                    }
                }
                cout << "\n\t\tNew Contact Number: ";
                cin >> newContact;
                cout << "\t\tNew Email: ";
                cin >> newEmail;

                // Update only the fields that have been modified
                PreparedStatement* ps = connection->prepareStatement("UPDATE customer SET Cust_Username = COALESCE(?, Cust_Username), Cust_Password = COALESCE(?, Cust_Password), Cust_Contact = COALESCE(?, Cust_Contact), Cust_Email = COALESCE(?, Cust_Email) WHERE Cust_Name = ?");                
                ps->setString(1, newUsername);
                ps->setString(2, newPassword);
                ps->setString(3, newContact);
                ps->setString(4, newEmail);
                ps->setString(5, custName);

                ps->executeUpdate();

                cout << "\n\t\tStaff details updated successfully!" << endl;

                delete ps;
            }
            else {
                cout << "\n\t\tCustomer with that name " << custName << " not found." << endl;
            }

            delete res;
            delete stmt;

            cout << "\n\t\tDo you want to update another customer (Y/N): ";
            cin >> choice;
            if (choice != 'Y' && choice != 'y') {
                FCustomer();
            }
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState : " << e.getSQLState() << endl;
    }
}
void deleteCustomer(Connection* connection) {

    system("cls");
    string custName;
    char confirmDelete;

    try {
        while (true) {
            cout << "\n\t\tEnter the Customer Name to delete: ";
            cin.ignore(); 
            getline(cin, custName);

            PreparedStatement* ps = connection->prepareStatement("DELETE FROM customer WHERE Cust_Name = ?");
            ps->setString(1, custName);

            int rowsAffected = ps->executeUpdate();
            if (rowsAffected > 0) {
                cout << "\n\t\tStaff with Name " << custName << " deleted successfully." << endl;

                // Decrement staff_id for remaining staff members
                PreparedStatement* updatePs = connection->prepareStatement("UPDATE customer SET Cust_id = Cust_id - 1 WHERE Cust_id > ?");
                updatePs->setInt(1, rowsAffected);
                updatePs->executeUpdate();
                delete updatePs;
            }
            else {
                cout << "\n\t\tCustomer with Name " << custName << " not found." << endl;
            }

            delete ps;

            cout << "\n\t\tDo you want to delete another customer (Y/N): ";
            cin >> confirmDelete;
            if (confirmDelete != 'Y' && confirmDelete!= 'y') {
                FCustomer();
            }
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState: " << e.getSQLState() << endl;
    }
}
void searchCustomer(Connection* connection) {

    system("cls");
    string custName;

    try {
        while (true) {

            cout << "\n\t\tEnter the Customer Name to search: ";
            cin.ignore(); 
            getline(cin, custName);

            PreparedStatement* ps = connection->prepareStatement("SELECT * FROM customer WHERE Cust_Name = ?");
            ps->setString(1, custName);

            ResultSet* res = ps->executeQuery();

            if (res->next()) {
                string maskedPassword(res->getString("Cust_Password").length(), '*');
                cout << "\n\t\tStaff Details:\n";
                cout << "\t\tStaff ID: " << res->getInt("Cust_id") << endl;
                cout << "\t\tName    : " << res->getString("Cust_Name") << endl;
                cout << "\t\tUsername: " << res->getString("Cust_Username") << endl;
                cout << "\t\tPassword: " << maskedPassword << endl;
                cout << "\t\tIC      : " << res->getString("Cust_IC") << endl;
                cout << "\t\tContact Number: " << res->getString("Cust_Contact") << endl;
                cout << "\t\tEmail   : " << res->getString("Cust_Email") << endl;
            }
            else {
                cout << "\n\t\tCustomer with name " << custName << " not found." << endl;
            }

            delete res;
            delete ps;

            char choice;
            cout << "\n\t\tDo you want to search for another customer (Y/N): ";
            cin >> choice;
            if (choice != 'Y' && choice != 'y') {
                FCustomer();
            }
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState: " << e.getSQLState() << endl;
    }
}
void managePayment()
{
    string customerName;
    system("cls");
    cout << "\n\n\t\tEnter name: ";
    cin >> customerName;
    anotherOrder(customerName);
}
void addMenu() 
{
    system("cls");
    string fname, fstatus;
    double fprice;
    int fquantity;
    while (true)
    {
        try {
            Connection* connection = databaseCon();
            if (!connection) {
                cerr << "Failed to connect to the database." << endl;

            }
            
            cout << "\n\t\t\tEnter Menu Name: ";
            cin.ignore();
            getline(cin, fname);

            cout << "\t\t\tEnter Price: RM ";
            cin >> fprice;

            cout << "\t\t\tEnter Quantity: ";
            cin >> fquantity;

            cout << "\t\t\tAvailability of the Menu (Available/Unavailable): ";
            cin >> fstatus;

           

            PreparedStatement* ps = connection->prepareStatement("INSERT INTO menu (Menu_Name, Menu_Price, Menu_Quantity, Menu_Status) VALUES (?, ?, ?, ?)");
            ps->setString(1, fname);
            ps->setDouble(2, fprice);
            ps->setInt(3, fquantity);
            ps->setString(4, fstatus);
            

            ps->execute();

            cout << "\n\t\t\tMenu Added successful!" << endl;


            delete ps;
            delete connection;
        }
        catch (const sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
            cerr << "SQLState : " << e.getSQLState() << endl;
        }
        catch (...) {
            cerr << "An exception occurred." << endl;
        }
        char choice4;
        cout << "\n\t\t\tDo you want to add another menu (Y/N): ";
        cin >> choice4;
        if (choice4 != 'Y' && choice4 != 'y') {
            Menu();
        }
    }

}
void updateMenu(Connection* connection) 
{
    
    string fname, fstatus;
    double fprice;
    int fquantity;
    char choice;

    try {
        while (true) {
            system("cls");
            cout << "\n\n\t\tEnter the menu name to update: ";
            cin.ignore();
            getline(cin, fname);

            Statement* stmt = connection->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM menu WHERE Menu_Name = '" + fname + "'");

            if (res->next()) {

                cout << "\n\t\tMenu selected :\n\n";
                cout << "\t\tMenu Name    : " << res->getString("Menu_Name") << endl;
                cout << "\t\tMenu Price   : " << fixed << setprecision(2) << res->getDouble("Menu_Price") << endl;
                cout << "\t\tMenu Quantity: " << res->getInt("Menu_Quantity") << endl;
                cout << "\t\tMenu Status  : " << res->getString("Menu_Status") << endl;

                cout << "\n\t\tEnter new details (Press Enter to keep existing values):\n";

                cout << "\t\tNew Menu Price (RM): ";
                cin >> fprice;
                cout << "\t\tNew Quantity       : ";
                cin >> fquantity;
                cout << "\t\tNew Status (Available/Unavailable): ";
                cin >> fstatus;

                // Update only the fields that have been modified
                PreparedStatement* ps = connection->prepareStatement("UPDATE menu SET Menu_Price = COALESCE(?, Menu_Price), Menu_Quantity = COALESCE(?, Menu_Quantity), Menu_Status = COALESCE(?, Menu_Status) WHERE Menu_Name = ?");
                ps->setDouble(1, fprice);
                ps->setInt(2, fquantity);
                ps->setString(3, fstatus);
                ps->setString(4, fname);
                

                ps->executeUpdate();

                cout << "\n\t\tMenu details updated successfully!" << endl;

                delete ps;
            }
            else {
                cout << "\n\t\tMenu with that name " << fname << " not found." << endl;
            }

            delete res;
            delete stmt;

            cout << "\n\t\tDo you want to update another menu (Y/N): ";
            cin >> choice;
            if (choice != 'Y' && choice != 'y') {
                Menu();
            }
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState: " << e.getSQLState() << endl;
    }
}
void SupdateMenu(Connection* connection)
{
    system("cls");
    string fname, fstatus;
    double fprice;
    int fquantity;
    char choice;

    try {
        while (true) {
            cout << "\n\n\t\tEnter the menu name to update: ";
            cin.ignore();
            getline(cin, fname);

            Statement* stmt = connection->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM menu WHERE Menu_Name = '" + fname + "'");

            if (res->next()) {

                cout << "\n\t\tMenu selected :\n\n";
                cout << "\t\tMenu Name    : " << res->getString("Menu_Name") << endl;
                cout << "\t\tMenu Price   : " << fixed << setprecision(2) << res->getDouble("Menu_Price") << endl;
                cout << "\t\tMenu Quantity: " << res->getInt("Menu_Quantity") << endl;
                cout << "\t\tMenu Status  : " << res->getString("Menu_Status") << endl;

                cout << "\n\t\tEnter new details (Press Enter to keep existing values):\n";

                cout << "\t\tNew Quantity       : ";
                cin >> fquantity;
                cout << "\t\tNew Status (Available/Unavailable): ";
                cin >> fstatus;

                // Update only the fields that have been modified
                PreparedStatement* ps = connection->prepareStatement("UPDATE menu SET Menu_Quantity = COALESCE(?, Menu_Quantity), Menu_Status = COALESCE(?, Menu_Status) WHERE Menu_Name = ?");
                ps->setInt(1, fquantity);
                ps->setString(2, fstatus);
                ps->setString(3, fname);


                ps->executeUpdate();

                cout << "\n\t\tMenu details updated successfully!" << endl;

                delete ps;
            }
            else {
                cout << "\n\t\tMenu with that name " << fname << " not found." << endl;
            }

            delete res;
            delete stmt;

            cout << "\n\t\tDo you want to update another menu (Y/N): ";
            cin >> choice;
            if (choice != 'Y' && choice != 'y') {
                SMenu();
            }
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState: " << e.getSQLState() << endl;
    }
}
void deleteMenu(Connection* connection) 
{
    system("cls");
    string fname;
    char confirmDelete;

    try {
        while (true) {
            cout << "\n\t\tEnter the Menu Name to delete: ";
            cin.ignore();
            getline(cin, fname);

            PreparedStatement* ps = connection->prepareStatement("DELETE FROM menu WHERE Menu_Name = ?");
            ps->setString(1, fname);

            int rowsAffected = ps->executeUpdate();
            if (rowsAffected > 0) {
                cout << "\n\t\tMenu with Name " << fname << " deleted successfully." << endl;

                // Decrement staff_id for remaining staff members
                PreparedStatement* updatePs = connection->prepareStatement("UPDATE menu SET Menu_id = Menu_id - 1 WHERE Menu_id > ?");
                updatePs->setInt(1, rowsAffected);
                updatePs->executeUpdate();
                delete updatePs;
            }
            else {
                cout << "\n\t\tMenu with Name " << fname << " not found." << endl;
            }

            delete ps;

            cout << "\n\t\tDo you want to delete another menu (Y/N): ";
            cin >> confirmDelete;
            if (confirmDelete != 'Y' && confirmDelete != 'y') {
                Menu();
            }
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState : " << e.getSQLState() << endl;
    }
}
void searchMenu(Connection* connection) {

    system("cls");
    string fname;

    try {
        while (true) {

            cout << "\n\t\tEnter the Menu Name to search: ";
            cin.ignore();
            getline(cin, fname);

            PreparedStatement* ps = connection->prepareStatement("SELECT * FROM menu WHERE Menu_Name = ?");
            ps->setString(1, fname);

            ResultSet* res = ps->executeQuery();

            if (res->next()) {
                cout << "\n\t\tMenu selected \n\n";
                cout << "\t\tMenu Name    : " << res->getString("Menu_Name") << endl;
                cout << "\t\tMenu Price   : (RM) " << fixed << setprecision(2) << res->getDouble("Menu_Price") << endl;
                cout << "\t\tMenu Quantity: " << res->getInt("Menu_Quantity") << endl;
                cout << "\t\tMenu Status  : " << res->getString("Menu_Status") << endl;
            }
            else {
                cout << "\n\t\tMenu with name " << fname << " not found." << endl;
            }

            delete res;
            delete ps;

            char choice;
            cout << "\n\t\tDo you want to search for another menu (Y/N): ";
            cin >> choice;
            if (choice != 'Y' && choice != 'y') {
                Menu();
            }
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState : " << e.getSQLState() << endl;
    }
}
void SsearchMenu(Connection* connection) {

    system("cls");
    string fname;

    try {
        while (true) {

            cout << "\n\t\tEnter the Menu Name to search: ";
            cin.ignore();
            getline(cin, fname);

            PreparedStatement* ps = connection->prepareStatement("SELECT * FROM menu WHERE Menu_Name = ?");
            ps->setString(1, fname);

            ResultSet* res = ps->executeQuery();

            if (res->next()) {
                cout << "\n\t\tMenu selected \n\n";
                cout << "\t\tMenu Name    : " << res->getString("Menu_Name") << endl;
                cout << "\t\tMenu Price   : (RM) " << fixed << setprecision(2) << res->getDouble("Menu_Price") << endl;
                cout << "\t\tMenu Quantity: " << res->getInt("Menu_Quantity") << endl;
                cout << "\t\tMenu Status  : " << res->getString("Menu_Status") << endl;
            }
            else {
                cout << "\n\t\tMenu with name " << fname << " not found." << endl;
            }

            delete res;
            delete ps;

            char choice;
            cout << "\n\t\tDo you want to search for another menu (Y/N): ";
            cin >> choice;
            if (choice != 'Y' && choice != 'y') {
                SMenu();
            }
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState : " << e.getSQLState() << endl;
    }
}
void displayMenu(Connection* connection) {

    system("cls");
    char  cont;
    try {
        Statement* stmt = connection->createStatement();
        ResultSet* res = stmt->executeQuery("SELECT Menu_id, Menu_Name, Menu_Price, Menu_Quantity, Menu_Status FROM menu");

        cout << "\n--------------------------------------------------------------------------------------------\n";
        cout << setw(10) << "Menu ID" << setw(14) << "Food Name" << setw(20) << "Price (RM)" << setw(20) << "Quantity" << setw(20) << "Status" << endl;
        cout << "\n--------------------------------------------------------------------------------------------\n";

        while (res->next()) {
            cout << setw(10) << res->getInt("Menu_id") << setw(15) << res->getString("Menu_Name") << setw(20) << fixed << setprecision(2) << res->getDouble("Menu_Price") << setw(20) << res->getInt("Menu_Quantity") << setw(20) << res->getString("Menu_Status") << endl << endl;
        }

        cout << "\n---------------------------------------------------------------------------------------------\n";

        delete res;
        delete stmt;

        cout << "Do you want to search the menu name (Y/N) : ";
        cin >> cont;

        if (cont == 'Y' || cont == 'y')
        {
            searchMenu(connection);
        }
        else
        {
            Menu();
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState : " << e.getSQLState() << endl;
    }
}
void SdisplayMenu(Connection* connection) {

    system("cls");
    char  cont;
    try {
        Statement* stmt = connection->createStatement();
        ResultSet* res = stmt->executeQuery("SELECT Menu_id, Menu_Name, Menu_Price, Menu_Quantity, Menu_Status FROM menu");

        cout << "\n--------------------------------------------------------------------------------------------\n";
        cout << setw(10) << "Menu ID" << setw(14) << "Food Name" << setw(20) << "Price (RM)" << setw(20) << "Quantity" << setw(20) << "Status" << endl;
        cout << "\n--------------------------------------------------------------------------------------------\n";

        while (res->next()) {
            cout << setw(10) << res->getInt("Menu_id") << setw(15) << res->getString("Menu_Name") << setw(20) << fixed << setprecision(2) << res->getDouble("Menu_Price") << setw(20) << res->getInt("Menu_Quantity") << setw(20) << res->getString("Menu_Status") << endl << endl;
        }

        cout << "\n---------------------------------------------------------------------------------------------\n";

        delete res;
        delete stmt;

        cout << "Do you want to search the menu name (Y/N) : ";
        cin >> cont;

        if (cont == 'Y' || cont == 'y')
        {
            SsearchMenu(connection);
        }
        else
        {
            SMenu();
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState : " << e.getSQLState() << endl;
    }
}
void deleteOrder(Connection* connection, string const& staffName)
{
    system("cls");
    string fname,food1;
    char confirmDelete;

    try {
        while (true) {
            cout << "\n\t\tEnter the Customer Name to delete: ";
            cin.ignore();
            getline(cin, fname);

            cout << "\n\t\tEnter the Food Name to delete: ";
            getline(cin, food1);

            PreparedStatement* ps = connection->prepareStatement("DELETE FROM order1 WHERE order_name = ? AND order_fname = ?");
            ps->setString(1, fname);
            ps->setString(2, food1);

            int rowsAffected = ps->executeUpdate();
            if (rowsAffected > 0) {
                cout << "\n\t\tOrder with name " << fname << " and food name "<< food1 << " deleted successfully." << endl;

 
                PreparedStatement* updatePs = connection->prepareStatement("UPDATE order1 SET order_id = order_id - 1 WHERE order_id > ?");
                updatePs->setInt(1, rowsAffected);
                updatePs->executeUpdate();
                delete updatePs;
            }
            else {
                cout << "\n\t\tOrder with Name " << " and food name "<< food1 << " not found." << endl;
            }

            delete ps;

            cout << "\n\t\tDo you want to delete another order (Y/N): ";
            cin >> confirmDelete;
            if (confirmDelete != 'Y' && confirmDelete != 'y') {
                order(staffName);
            }
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState : " << e.getSQLState() << endl;
    }
}
void searchOrder(Connection* connection, string const& staffName) {

    system("cls");
    string fname,food1;

    try {
        while (true) {

            cout << "\n\t\tEnter the Customer Name to search: ";
            cin.ignore();
            getline(cin, fname);

            cout << "\n\t\tEnter the Food Name to search: ";
            getline(cin, food1);

            PreparedStatement* ps = connection->prepareStatement("SELECT * FROM order1 WHERE order_name = ? AND order_fname = ?");
            ps->setString(1, fname);
            ps->setString(2, food1);

            ResultSet* res = ps->executeQuery();

            if (res->next()) {
                cout << "\n\t\tOrder selected :\n\n";
                cout << "\t\tCustomer Name  : " << res->getString("order_name") << endl;
                cout << "\t\tFood Name      : " << res->getString("order_fname") << endl;
                cout << "\t\tQuantity       : " << res->getInt("order_quantity") << endl;
                cout << "\t\tOrder Time     : " << res->getString("order_time") << endl;
                cout << "\t\tOrder Status   : " << res->getString("order_status") << endl;
                cout << "\t\tPick Up Time   : " << res->getString("pickup_time") << endl;
            }
            else {
                cout << "\n\t\tOrder with customer name " << fname << " and food name" << food1 << " not found." << endl;
            }

            delete res;
            delete ps;

            char choice;
            cout << "\n\t\tDo you want to search for another order (Y/N): ";
            cin >> choice;
            if (choice != 'Y' && choice != 'y') {
                order(staffName);
            }
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState : " << e.getSQLState() << endl;
    }
}
void displayOrder(Connection* connection, const string& staffName) {

    system("cls");
    char  cont;
    try {
        Statement* stmt = connection->createStatement();
        ResultSet* res = stmt->executeQuery("SELECT order_id, order_name, order_fname, order_quantity, order_time, order_status, pickup_time FROM order1");

        cout << "\n------------------------------------------------------------------------------------------------------------------------\n\n";
        cout << setw(10) << "Order ID" << setw(18) << "Customer Name" << setw(15) << "Food Name" << setw(15) << "Quantity" << setw(15) << "Order Time" <<setw(20) << "Order Status" << setw(20) << "Pickup Time" << endl;
        cout << "\n------------------------------------------------------------------------------------------------------------------------\n\n";

        while (res->next()) {
            cout << setw(10) << res->getInt("order_id") << setw(13) << res->getString("order_name") << setw(18) << res->getString("order_fname") << setw(12) << res->getInt("order_quantity") << setw(25) << res->getString("order_time")<< setw(15) << res->getString("order_status") << setw(25) << res->getString("pickup_time") << endl << endl;
        }

        cout << "\n--------------------------------------------------------------------------------------------------------------------------\n";

        delete res;
        delete stmt;

        cout << "Do you want to search the order (Y/N) : ";
        cin >> cont;

        if (cont == 'Y' || cont == 'y')
        {
            searchOrder(connection, staffName);
        }
        else
        {
            order(staffName);
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState: " << e.getSQLState() << endl;
    }
}
void FStaff()
{
    system("cls");
    int choose;
    Connection* connection = databaseCon();

    cout << "\t\t****************************** Staff Function **************************\n\n";
    cout << "\t\t\t 1. Add Staff" << endl;
    cout << "\t\t\t 2. Update Staff" << endl;
    cout << "\t\t\t 3. Delete Staff" << endl;
    cout << "\t\t\t 4. View Staff" << endl;
    cout << "\t\t\t 5. Search Staff" << endl;
    cout << "\t\t\t 6. Back to Admin Menu" << endl;

    do{
    cout << "\n\t\t\tWhich option would you like to choose: ";
    cin >> choose;

    switch (choose)
    {
    case 1:
        registerStaff();
        break;
    case 2:
        updateStaff(connection);
        break;
    case 3:
        deleteStaff(connection);
        break;
    case 4:
        displayStaff(connection);
        break;
    case 5:
        searchStaff(connection);
        break;
    case 6:
        admin();
        break;
    default:
        cout << "\t\t\tInvalid choice. Please select a valid option (1-6).\n";
    }
    } while (choose != 0<6);
}
void FCustomer()
{
    system("cls");
    int choose;
    Connection* connection = databaseCon();

    cout << "\t\t****************************** Customer Function **************************\n\n";
    cout << "\t\t\t 1. Add Customer" << endl;
    cout << "\t\t\t 2. Update Customer" << endl;
    cout << "\t\t\t 3. Delete Customer" << endl;
    cout << "\t\t\t 4. View Customer" << endl;
    cout << "\t\t\t 5. Search Customer" << endl;
    cout << "\t\t\t 6. Manage Payement" << endl;
    cout << "\t\t\t 7. Back to Admin Menu" << endl;

    do{
    cout << "\n\t\t\tWhich option would you like to choose: ";
    cin >> choose;
    
    switch (choose)
    {
    case 1:
        registerCustomer();
        break;
    case 2:
        updateCustomer(connection);
        break;
    case 3:
        deleteCustomer(connection);
        break;
    case 4:
        displayCustomer(connection);
        break;
    case 5:
        searchCustomer(connection);
        break;
    case 6:
        managePayment();
        break;
    case 7:
        admin();
        break;
    default:
        cout << "\t\t\tInvalid choice. Please select a valid option (1-7).\n";
    }
    }while (choose != 7);
}
void SCustomer(const string& staffName)
{
    system("cls");
    int choose;
    Connection* connection = databaseCon();

    cout << "\t\t****************************** Customer Function **************************\n\n";
    cout << "\t\t\t 1. Add Customer" << endl;
    cout << "\t\t\t 2. Update Customer" << endl;
    cout << "\t\t\t 3. Delete Customer" << endl;
    cout << "\t\t\t 4. View Customer" << endl;
    cout << "\t\t\t 5. Search Customer" << endl;
    cout << "\t\t\t 6. Manage Payement" << endl;
    cout << "\t\t\t 7. Back to Staff Menu" << endl;

    do {
        cout << "\n\t\t\tWhich option would you like to choose: ";
        cin >> choose;

        switch (choose)
        {
        case 1:
            registerCustomer();
            break;
        case 2:
            updateCustomer(connection);
            break;
        case 3:
            deleteCustomer(connection);
            break;
        case 4:
            displayCustomer(connection);
            break;
        case 5:
            searchCustomer(connection);
            break;
        case 6:
            managePayment();
            break;
        case 7:
            staff(staffName);
            break;
        default:
            cout << "\t\t\tInvalid choice. Please select a valid option (1-7).\n";
        }
    } while (choose != 7);
}
void SMenu()
{
    system("cls");
    int choose;
    Connection* connection = databaseCon();
    string staffName;
    
    do{
    cout << "\t\t****************************** Menu Function **************************\n\n";
    cout << "\t\t\t 1. Update Menu" << endl;
    cout << "\t\t\t 2. View Menu" << endl;
    cout << "\t\t\t 3. Search Menu" << endl;
    cout << "\t\t\t 4. Back to Staff Menu" << endl;

    cout << "\n\t\t\tWhich option would you like to choose: ";
    cin >> choose;
    system("cls");
    switch (choose)
    {
    case 1:
        SupdateMenu(connection);
        break;
    case 2:
        SdisplayMenu(connection);
        break;
    case 3:
        SsearchMenu(connection);
        break;
    case 4:
        staff(staffName);
        break;
    default:
        cout << "\t\t\tInvalid choice. Please select a valid option (1-4).\n";
    }
    } while (choose != 4);

}
void Menu()
{
    system("cls");
    int choose;
    Connection* connection = databaseCon();

    cout << "\t\t****************************** Menu Function **************************\n\n";
    cout << "\t\t\t 1. Add Menu" << endl;
    cout << "\t\t\t 2. Update Menu" << endl;
    cout << "\t\t\t 3. Delete Menu" << endl;
    cout << "\t\t\t 4. View Menu" << endl;
    cout << "\t\t\t 5. Search Menu" << endl;
    cout << "\t\t\t 6. Back to Admin Menu" << endl;
    do
    {
    cout << "\n\t\t\tWhich option would you like to choose: ";
    cin >> choose;
    
    switch (choose)
    {
    case 1:
        addMenu();
        break;
    case 2:
        updateMenu(connection);
        break;
    case 3:
        deleteMenu(connection);
        break;
    case 4:
        displayMenu(connection);
        break;
    case 5:
        searchMenu(connection);
        break;
    case 6:
        admin();
        break;
    default:
        system("cls");
        cout << "\t\t\tInvalid choice. Please select a valid option (1-6).\n";
    }
    }while (choose != 6);

}
void order(const string& staffName)
{
    system("cls");
    int choose;
    Connection* connection = databaseCon();

    cout << "\t\t****************************** Order Function **************************\n\n";
    cout << "\t\t\t 1. Delete Order" << endl;
    cout << "\t\t\t 2. View Order" << endl;
    cout << "\t\t\t 3. Search Order" << endl;
    cout << "\t\t\t 4. Back to Staff Menu" << endl;
    do {
        cout << "\n\t\t\tWhich option would you like to choose: ";
        cin >> choose;

        switch (choose)
        {
        case 1:
            deleteOrder(connection, staffName);
            break;
        case 2:
            displayOrder(connection, staffName);
            break;
        case 3:
            searchOrder(connection, staffName);
            break;
        case 4:
            staff(staffName);
            break;
        default:
            cout << "\t\t\tInvalid choice. Please select a valid option (1-4).\n";
        }
    } while (choose != 4);

}
void salesDay()
{
    string time;

    system("cls");

    cout << "\t\t****************************** Sales by Day **************************\n";

    while (true)
    {
        try {
            Connection* connection = databaseCon();
            if (!connection) {
                cerr << "Failed to connect to the database." << endl;
                return;
            }
            Statement* stmt = connection->createStatement();

            cout << "\n\t\tPlease choose the date (YYYY-MM-DD): ";
            cin.ignore();
            getline(cin, time);
            

            ResultSet* resOrders = stmt->executeQuery("SELECT * FROM order1 WHERE order_date = '" + time + "'");
            double totalSales = 0.0;

            if (!resOrders->next()) {
                cout << "\n\t\tNo sales data available for the date: " << time << endl << endl;
            }
            else {
                do {
                    string orderName = resOrders->getString("order_fname");
                    int quantity = resOrders->getInt("order_quantity");

                    ResultSet* resMenu = stmt->executeQuery("SELECT Menu_Price FROM menu WHERE Menu_Name = '" + orderName + "'");

                    if (resMenu->next())
                    {
                        double price = resMenu->getDouble("Menu_Price");
                        totalSales += price * quantity;
                    }

                    delete resMenu;
                } while (resOrders->next());

                cout << "\n\t\tTotal sales on " << time << " is: RM " << totalSales << endl << endl;
            }

            delete resOrders;
            delete stmt;
            delete connection;
        }
        catch (const sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
            cerr << "SQLState : " << e.getSQLState() << endl;
        }
        catch (...) {
            cerr << "An exception occurred." << endl;
        }
        char choice4;
        cout << "\n\t\tDo you want to check sales for another date? (Y/N): ";
        cin >> choice4;
        if (choice4 != 'Y' && choice4 != 'y') {
            Sales();
        }
    }
}
void salesMonth()
{
    string month;

    system("cls");

    cout << "\t\t****************************** Sales by Month **************************\n";

    while (true)
    {
        try {
            Connection* connection = databaseCon();
            if (!connection) {
                cerr << "Failed to connect to the database." << endl;
                return;
            }
            Statement* stmt = connection->createStatement();

            cout << "\n\t\tPlease choose the month (YYYY-MM): ";
            cin.ignore();
            getline(cin, month);

            ResultSet* resOrders = stmt->executeQuery("SELECT * FROM order1 WHERE order_date LIKE '" + month + "-%'");
            double totalSales = 0.0;

            if (!resOrders->next()) {
                cout << "\n\t\tNo sales data available for the month: " << month << endl << endl;
            }
            else {
                do {
                    string orderName = resOrders->getString("order_fname");
                    int quantity = resOrders->getInt("order_quantity");

                    ResultSet* resMenu = stmt->executeQuery("SELECT Menu_Price FROM menu WHERE Menu_Name = '" + orderName + "'");

                    if (resMenu->next())
                    {
                        double price = resMenu->getDouble("Menu_Price");
                        totalSales += price * quantity;
                    }

                    delete resMenu;
                } while (resOrders->next());

                cout << "\n\t\tTotal sales in " << month << " is: RM " << totalSales << endl << endl;
            }

            delete resOrders;
            delete stmt;
            delete connection;
        }
        catch (const sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
            cerr << "SQLState : " << e.getSQLState() << endl;
        }
        catch (...) {
            cerr << "An exception occurred." << endl;
        }

        char choice;
        cout << "\n\t\tDo you want to check sales for another month? (Y/N): ";
        cin >> choice;
        if (choice != 'Y' && choice != 'y') {
            Sales();
        }
    }
}
void Sales()
{
    int choice;
    system("cls");
    cout << "\t\t 1. View Sales by Day" << endl;
    cout << "\t\t 2. View Sales by Month" << endl;
    cout << "\t\t 3. Back to Admin Menu" << endl;

    do
    {
        cout << "\n\t\tWhich one you want to view: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            salesDay();
            break;
        case 2:
            salesMonth();
            break;
        case 3:
            admin();
            break;
        default:
            cout << "\t\t\tInvalid choice. Please select a valid option (1-3).\n";
    }
    } while (choice != 0 < 3);
   
}
void admin() 
{
    system("cls");
    int choice1;
    
    cout << "\t\t****************************** WELCOME Admin Umar **************************\n\n";
    cout << "\t\t\t 1. Manage Staff" << endl;
    cout << "\t\t\t 2. Manage Customer" << endl;
    cout << "\t\t\t 3. Manage Menu" << endl;
    cout << "\t\t\t 4. View Sales" << endl;
    cout << "\t\t\t 5. Log out" << endl;
    
    do
    {
        cout << "\n\t\t\tWhat will you be doing today: ";
        cin >> choice1;

        switch (choice1) {
        case 1:
            FStaff();
            break;
        case 2:
            FCustomer();
            break;
        case 3:
            Menu();
            break;
        case 4:
            Sales();
            break;
        case 5:
            system("cls");
            exit(1);
            break;
        default:
            cout << "\t\t\tInvalid choice. Please select a valid option (1-5).\n";
        }
    } while (choice1 != 0<5);   
}
void adminLogin() {

    system("cls");
    string name, pass;
    char ch;
    string blankPassword;
    int attempts = 0;
    const int maxAttempts = 3;

    try {
        Connection* connection = databaseCon();
        if (!connection) {
            cerr << "Failed to connect to the database." << endl;
            return;
        }

        bool loggedIn = false;
        do {
            
            cout << "\t\tPlease Log In to continue (admin)" << endl;
            cout << "\n\t\tEnter username: ";
            cin >> name;

            cout << "\t\tEnter password: ";
            ch = _getch();
            pass.clear();
            blankPassword.clear();

            while (ch != 13) {
                if (ch == 8) {
                    if (!pass.empty()) {
                        pass.pop_back();
                        blankPassword.pop_back();
                        cout << "\b \b";
                    }
                }
                else {
                    pass.push_back(ch);
                    blankPassword.push_back('*');
                    cout << '*';
                }

                ch = _getch();
            }

            system("CLS");

            PreparedStatement* ps = connection->prepareStatement("SELECT Admin_id FROM Adminl WHERE Admin_User = ? AND Admin_Pass = ?");
            ps->setString(1, name);
            ps->setString(2, pass);

            ResultSet* rs = ps->executeQuery();
            if (rs->next()) {
                rs->getInt("Admin_id");
                loggedIn = true;
                
            }
            else {
                cout << "\n\t\tInvalid username or password. Please try again."<< " attempts number " << attempts+1 << " of " << maxAttempts<< endl;
                attempts++;
                if (attempts >= maxAttempts) {
                    cerr << "\n\t\tMaximum login attempts reached. Exiting." << endl;
                    
                    return;
                }

            }
  
        } while (!loggedIn);

        admin(); 
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState: " << e.getSQLState() << endl;
    }
}
void staff(const string& staffName) 
{
    system("cls");
    int choice1;
    Connection* connection = databaseCon();
    
    cout << "\t\t****************************** WELCOME Staff "<< staffName <<"**************************\n\n";
    cout << "\t\t\t 1. Profile" << endl;
    cout << "\t\t\t 2. Manage Customer" << endl;
    cout << "\t\t\t 3. Manage Menu" << endl;
    cout << "\t\t\t 4. Manage Order" << endl;
    cout << "\t\t\t 5. Log out" << endl;
    
    do 
    {
        cout << "\n\t\t\tWhat will you be doing today: ";
        cin >> choice1;

        switch (choice1) 
        {
        case 1:
            staffprofile(connection, staffName);
            break;
        case 2:
            SCustomer(staffName);
            break;
        case 3:
            SMenu();
            break;
        case 4:
            order(staffName);
            break;
        case 5:
            system("cls");
            exit(0);
            break;   
        default:
            cout << "\t\t\tInvalid choice. Please select a valid option (1-4).\n";
        }
    } while (choice1 != 0<5);
}
void staffLogin() {

    system("cls");
    string name, pass,staffName;
    char ch;
    string blankPassword;
    int attempts = 0;
    const int maxAttempts = 3;

    try {
        Connection* connection = databaseCon();
        if (!connection) {
            cerr << "Failed to connect to the database." << endl;
            return;
        }

        bool loggedIn = false;
        do {
            cout << "\t\tPlease Log In to continue (staff)" << endl;

            cout << "\n\t\tUsername: ";
            cin >> name;

            cout << "\t\tPassword: ";
            ch = _getch();
            pass.clear();
            blankPassword.clear();

            while (ch != 13) {
                if (ch == 8) {
                    if (!pass.empty()) {
                        pass.pop_back();
                        blankPassword.pop_back();
                        cout << "\b \b";
                    }
                }
                else {
                    pass.push_back(ch);
                    blankPassword.push_back('*');
                    cout << '*';
                }

                ch = _getch();
            }

            system("CLS");

            PreparedStatement* ps = connection->prepareStatement("SELECT Staff_id FROM staff WHERE Staff_Username = ? AND Staff_Password = ?");
            ps->setString(1, name);
            ps->setString(2, pass);

            ResultSet* rs = ps->executeQuery();
            if (rs->next()) {
                rs->getInt("Staff_id");
                loggedIn = true;
            }
            else {
                cout << "\n\t\tInvalid username or password. Please try again." << "\a attempts number " << attempts + 1 << " of " << maxAttempts << endl;
                attempts++;
                if (attempts >= maxAttempts) {
                    cerr << "\n\t\tMaximum login attempts reached. Exiting." << endl;
                    return;
                }
            }

        } while (!loggedIn);

        PreparedStatement* psName = connection->prepareStatement("SELECT Staff_Name FROM staff WHERE Staff_Username = ?");
        psName->setString(1, name);

        ResultSet* rsName = psName->executeQuery();
        if (rsName->next()) {
            staffName = rsName->getString("Staff_Name");
            staff(staffName);
        }
        else {
            cerr << "Error retrieving customer name." << endl;

        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState : " << e.getSQLState() << endl;
    }
}
double getFoodPrice(Connection* connection, const string& foodName) {
    PreparedStatement* priceStmt = connection->prepareStatement("SELECT Menu_Price FROM menu WHERE Menu_Name = ?");
    priceStmt->setString(1, foodName);
    ResultSet* priceRes = priceStmt->executeQuery();

    double price = 0.0;
    if (priceRes->next()) {
        price = priceRes->getDouble("Menu_Price");
    }

    delete priceStmt;
    delete priceRes;

    return price;
}
void reciept(Connection* connection) {
    system("cls");
    string customerName;
    double totalPrice1 = 0.0;
    cout << "\n\t\t*********************** Receipt ***********************\n\n";

    Statement* stmt = connection->createStatement();
    ResultSet* res = stmt->executeQuery("SELECT DISTINCT order_name, pickup_time FROM order1 WHERE order_time >= DATE_SUB(NOW(), INTERVAL 3 MINUTE)");

    while (res->next()) {
        string currentOrderName = res->getString("order_name");
        string currentPickupTime = res->getString("pickup_time");

        PreparedStatement* orderStmt = connection->prepareStatement("SELECT order_fname, order_quantity FROM order1 WHERE order_name = ? AND order_time >= DATE_SUB(NOW(), INTERVAL 3 MINUTE)");
        orderStmt->setString(1, currentOrderName);
        ResultSet* orderRes = orderStmt->executeQuery();

        cout << "\n\t\tOrder Name: " << currentOrderName << "\tPickup Time: " << currentPickupTime << endl;
        cout << "\n\t\t------------------------------------------------------------\n";
        cout << setw(20) << "\t\tFood Name" << setw(15) << "\t\tQuantity" << setw(5) << "Price" << endl;
        cout << "\t\t------------------------------------------------------------\n";



        string prevFoodName = "";
        int totalQuantity = 0;

        while (orderRes->next()) {
            string currentFoodName = orderRes->getString("order_fname");
            int currentQuantity = orderRes->getInt("order_quantity");
            double price = getFoodPrice(connection, currentFoodName);

            totalPrice1 += currentQuantity * price;


            if (currentFoodName == prevFoodName) {
                totalQuantity += currentQuantity;
            }
            else {
                if (prevFoodName != "") {
                    // Get food price and calculate total price
                    double price = getFoodPrice(connection, prevFoodName);
                    double totalPrice = totalQuantity * price;
                    cout << setw(35) << prevFoodName << setw(17) << totalQuantity << setw(14) << totalPrice << endl;
                }
                totalQuantity = currentQuantity;
                prevFoodName = currentFoodName;
            }
        }

        if (prevFoodName != "") {
            // Calculate price for last food item
            double price = getFoodPrice(connection, prevFoodName);
            double totalPrice = totalQuantity * price;
            cout << setw(35) << prevFoodName << setw(17) << totalQuantity << setw(14) << totalPrice << endl;
        }

        cout << "\t\t------------------------------------------------------------\n";


        cout << "\n\t\tOrder Total Price: RM " << totalPrice1 << endl;


        delete orderStmt;
        delete orderRes;
    }
    cout << "\n\t\t*********** End of Receipt ***********\n\n";
    char char1;

    cout << "\t\tDo you want to add another Order (Y/N): ";
    cin >> char1;

    if (char1 == 'Y' || char1 == 'y')
    {
        anotherOrder(customerName);
    }
    else
    {
        customer(customerName);
    }
}
bool isValidDateTime(const string& dateTime) {
    regex dateTimeFormat(R"((\d{4})/(0[1-9]|1[0-2])/(0[1-9]|[12][0-9]|3[01])\s(0[0-9]|1[0-9]|2[0-3]):[0-5][0-9])");
    if (!regex_match(dateTime, dateTimeFormat)) {
        return false;
    }

    string date = dateTime.substr(0, 10);
    string time = dateTime.substr(11, 5);
    int hour = stoi(time.substr(0, 2));
    int minute = stoi(time.substr(3, 2));

    // Check if the time is between 11:00 and 22:30
    if ((hour < 11) || (hour == 22 && minute > 30) || (hour > 22)) {
        return false;
    }

    return true;
}
string promptPickUpDateTime() {
    string dateTime; 
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (true) {
       
        cout << "\n\n\t\tEnter the pick-up date and time of the order (YYYY/MM/DD HH:MM): ";
        
        getline(cin, dateTime);

        if (isValidDateTime(dateTime)) {
            break;
        }
        else {
            cout << "Invalid date/time. Please enter a date in YYYY/MM/DD format and a time between 11:00 and 22:30 in HH:MM format." << endl;
        }
       
    }

    return dateTime;
}
void makePayment(Connection* connection) {
    double totalPrice = 0.0;
    string orderName;

    system("cls");
    cout << "\n\t\t*********************** Payment Details ***********************\n\n";

    Statement* stmt = connection->createStatement();
    ResultSet* res = stmt->executeQuery("SELECT DISTINCT order_name FROM order1 WHERE order_time >= DATE_SUB(NOW(), INTERVAL 3 MINUTE)");

    while (res->next()) {
        string currentOrderName = res->getString("order_name");
        PreparedStatement* orderStmt = connection->prepareStatement("SELECT order_fname, order_quantity FROM order1 WHERE order_name = ? AND order_time >= DATE_SUB(NOW(), INTERVAL 3 MINUTE)");
        orderStmt->setString(1, currentOrderName);
        ResultSet* orderRes = orderStmt->executeQuery();

        while (orderRes->next()) {
            string foodName = orderRes->getString("order_fname");
            int quantity = orderRes->getInt("order_quantity");
            double price = getFoodPrice(connection, foodName);



            totalPrice += quantity * price;
        }

        delete orderStmt;
        delete orderRes;
    }

    cout << "\n\t\tTotal Price to be Paid: RM " << totalPrice << endl;

    char paymentMethod;
    char choice;

    bool pay = true;
    while(pay)
    { 
    cout << "\n\t\tChoose Payment Method (O for Online Payment, C for Cash Payment, D for Card Payment): ";
    cin >> paymentMethod;

    if (paymentMethod == 'O' || paymentMethod == 'o') {
        cout << "\n\t\tInitiating Online Payment..."; 
    }
    else if (paymentMethod == 'C' || paymentMethod == 'c') {
        cout << "\n\t\tPlease pay in cash at the counter."; 
    }
    else if (paymentMethod == 'D' || paymentMethod == 'd') {
        cout << "\n\t\tPlease wave or enter card details."; 
    }
    else {
        cout << "\n\t\tDo you want cancel the order (Y/N) if no try again: ";
        cin >> choice;
        if (choice == 'Y' || choice == 'y')
        {
            cout << "\t\tOrder Canceled, Please come again." << endl;
            PreparedStatement* cancelStmt = connection->prepareStatement("DELETE FROM order1 WHERE order_time >= DATE_SUB(NOW(), INTERVAL 3 MINUTE)");
            cancelStmt->executeUpdate();
            delete cancelStmt;
            exit(0);
        }
        pay = true;
    }
    pay = false;
    }

    string pickupTime = promptPickUpDateTime();
    cout << "Pick-up date and time entered: " << pickupTime << endl;
    

    PreparedStatement* updateStmt = connection->prepareStatement("UPDATE order1 SET order_status = 'paid', pickup_time = ? WHERE order_time >= DATE_SUB(NOW(), INTERVAL 3 MINUTE)");
    updateStmt->setString(1, pickupTime);
    updateStmt->executeUpdate();
    delete updateStmt;

    reciept(connection);
}
void displayOrderedFood(Connection* connection) {
    system("cls");

    string customerName;
    char order, order1;
    cout << "\t\t****************************** Ordered Food **************************\n\n";

    Statement* stmt = connection->createStatement();
    ResultSet* res = stmt->executeQuery("SELECT DISTINCT order_name FROM order1 WHERE order_time >= DATE_SUB(NOW(), INTERVAL 3 MINUTE)");

    while (res->next()) {
        string orderName = res->getString("order_name");
        cout << "\n\t\tOrder Name: " << orderName << endl;

        PreparedStatement* orderStmt = connection->prepareStatement("SELECT order_fname, order_quantity FROM order1 WHERE order_name = ? AND order_time >= DATE_SUB(NOW(), INTERVAL 3 MINUTE)");
        orderStmt->setString(1, orderName);
        ResultSet* orderRes = orderStmt->executeQuery();

        cout << "\n\t\t------------------------------------------------------------\n";
        cout << setw(20) << "\t\tFood Name" << setw(15) << "\t\tQuantity" << setw(10) << "Price" << endl;
        cout << "\t\t------------------------------------------------------------\n";

        string prevFoodName = "";
        int totalQuantity = 0;

        while (orderRes->next()) {
            string currentFoodName = orderRes->getString("order_fname");
            int currentQuantity = orderRes->getInt("order_quantity");

            if (currentFoodName == prevFoodName) {
                totalQuantity += currentQuantity;
            }
            else {
                if (prevFoodName != "") {
                    // Get food price and calculate total price
                    double price = getFoodPrice(connection, prevFoodName);
                    double totalPrice = totalQuantity * price;
                    cout << setw(35) << prevFoodName << setw(17) << totalQuantity << setw(14) << totalPrice << endl;
                }
                totalQuantity = currentQuantity;
                prevFoodName = currentFoodName;
            }
        }

        if (prevFoodName != "") {
            // Calculate price for last food item
            double price = getFoodPrice(connection, prevFoodName);
            double totalPrice = totalQuantity * price;
            cout << setw(35) << prevFoodName << setw(17) << totalQuantity << setw(14) << totalPrice << endl;
        }

        cout << "\t\t------------------------------------------------------------\n";

        delete orderStmt;
        delete orderRes;
    }

    cout << "\n\t\tConfirm order? (Y/N) : ";
    cin >> order;

    if (order == 'Y' || order == 'y') {
        makePayment(connection);
    }
    else {
        cout << "\n\t\tDo you want to add more order? (Y/N): ";
        cin >> order1;
        if (order1 == 'Y' || order1 == 'y') {
            anotherOrder(customerName);
        }
        else {
            makePayment(connection);
        }
    }
}
void anotherOrder(string customerName)
{
    char choice;
    string fname;
    int quantity,menuId;

    Connection* connection = databaseCon();
    Statement* qstmt = connection->createStatement();
    ResultSet* qres = qstmt->executeQuery("SELECT Menu_id, Menu_Name, Menu_Price, Menu_Quantity, Menu_Status FROM menu WHERE Menu_Status = 'Unavailable'");

    if(qres->next())
    {
        system("cls");
        cout << "\n\t\tNo food items are currently unavailable. Please come later.\n\n";
        cout << "\t\tPress any key to return back..." << endl;
        cin.get();
        if (cin.get())
        {
            customer(customerName);
        }

    }
    do {
        system("cls");

        cout << "\t\t****************************** WELCOME **************************\n\n";

        Statement* stmt = connection->createStatement();
        ResultSet* res = stmt->executeQuery("SELECT Menu_id, Menu_Name, Menu_Price, Menu_Quantity, Menu_Status FROM menu");

        cout << "\n\t\t--------------------------------------------------------------------------------------------\n";
        cout << setw(10) << "\t\tMenu ID" << setw(15) << "Food Name" << setw(20) << "Price (RM)" << setw(20) << "Status" << endl;
        cout << "\n\t\t--------------------------------------------------------------------------------------------\n";

        while (res->next()) {
            cout <<"\t"<< setw(15) << res->getInt("Menu_id") <<"\t" << setw(15) << res->getString("Menu_Name") << fixed << setprecision(2) << setw(20) << res->getDouble("Menu_Price") << setw(20) << res->getString("Menu_Status") << endl << endl;
        }

        cout << "\t\t---------------------------------------------------------------------------------------------\n";

        bool validFood = false;
        while (!validFood)
        {
            cout << "\t\tMenu ID: ";
            cin >> menuId;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            PreparedStatement* checkStmt = connection->prepareStatement("SELECT Menu_id FROM menu WHERE Menu_id = ? AND Menu_Status = 'Available'");
            checkStmt->setInt(1, menuId);
            ResultSet* checkRes = checkStmt->executeQuery();

            PreparedStatement* getNameStmt = connection->prepareStatement("SELECT Menu_Name FROM menu WHERE Menu_id = ?");
            getNameStmt->setInt(1, menuId);
            ResultSet* nameRes = getNameStmt->executeQuery();
            if (checkRes->next())
            {
                do
                {
                    cout << "\t\tQuantity: ";
                    cin >> quantity;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    PreparedStatement* qcheckStmt = connection->prepareStatement("SELECT Menu_id, Menu_Quantity FROM menu WHERE Menu_id = ?");
                    qcheckStmt->setInt(1, menuId);
                    ResultSet* qcheckRes = qcheckStmt->executeQuery();

                    if (qcheckRes->next())
                    {
                        int availableQuantity = qcheckRes->getInt("Menu_Quantity");

                        if (quantity <= availableQuantity)
                        {
                            PreparedStatement* ps = connection->prepareStatement("INSERT INTO order1 (order_name, order_fname, order_quantity) SELECT ?, Menu_Name, ? FROM menu WHERE Menu_id = ?");
                            ps->setString(1, customerName);
                            ps->setInt(2, quantity);
                            ps->setInt(3, menuId);
                            ps->execute();

                            PreparedStatement* updateStmt = connection->prepareStatement("UPDATE menu SET Menu_Quantity = Menu_Quantity - ? WHERE Menu_id = ?");
                            updateStmt->setInt(1, quantity);
                            updateStmt->setInt(2, menuId);
                            updateStmt->execute();

                            PreparedStatement* checkZeroStmt = connection->prepareStatement("SELECT Menu_id, Menu_Quantity FROM menu WHERE Menu_id = ?");
                            checkZeroStmt->setInt(1, menuId);
                            ResultSet* checkZeroRes = checkZeroStmt->executeQuery();

                            if (checkZeroRes->next())
                            {
                                int updatedQuantity = checkZeroRes->getInt("Menu_Quantity");

                                if (updatedQuantity == 0)
                                {
                                    PreparedStatement* updateStatusStmt = connection->prepareStatement("UPDATE menu SET Menu_Status = 'Unavailable' WHERE Menu_id = ?");
                                    updateStatusStmt->setInt(1, menuId);
                                    updateStatusStmt->execute();
                                }
                            }

                            delete ps;
                            delete updateStmt;
                            delete checkZeroRes;
                            delete checkZeroStmt;
                            cout << "\n\t\tOrder Added!\n" << endl;

                            break;
                        }
                        else
                        {
                            cout << "\n\t\tQuantity is over the available quantity limit. Please choose again.\n";
                        }
                    }

                    delete qcheckRes;
                    delete qcheckStmt;

                } while (true);

                validFood = true;
            }
            else
            {
                cout << "\n\t\tFood name not available or status is 'Available'. Please choose again.\n";
            }

            delete checkRes;
            delete checkStmt;
        }
        cout << "\t\tDo you want to place another order? (Y/N): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        fflush(stdin);
    } while (choice == 'Y');
    displayOrderedFood(connection);
}
void history(const string& customerName) 
{
    Connection* connection = databaseCon(); 

    system("cls");

    cout << "\t\t****************************** ORDER HISTORY FOR " << customerName << " *************************\n\n";

    try {
        PreparedStatement* ps = connection->prepareStatement("SELECT order1.order_fname, order1.pickup_time, SUM(order1.order_quantity) AS total_quantity, menu.Menu_Price "
            "FROM order1 "
            "JOIN menu ON order1.order_fname = menu.Menu_Name "
            "WHERE order1.order_name = ? "
            "GROUP BY order1.order_fname, order1.pickup_time, menu.Menu_Price");
        ps->setString(1, customerName);

        ResultSet* rs = ps->executeQuery();

        map<string, pair<int, double>> orderSummary; 

        while (rs->next()) {
            string orderItem = rs->getString("order_fname") + " (" + rs->getString("pickup_time") + ")";
            int quantity = rs->getInt("total_quantity");
            double price = rs->getDouble("Menu_Price");
            double total = price * quantity;
            orderSummary[orderItem] = make_pair(quantity, total);
        }

        if (orderSummary.empty()) {
            char choice;
            cout << "\t\tNo history of food ordered for " << customerName << endl;
            cout << "\n\t\tDo you want to order? (Y/N): ";
            cin >> choice;

            if (choice == 'Y' || choice == 'y') {
                anotherOrder(customerName); 
            }
            else {
                system("cls");
                exit(0);
            }
        }
        else {
            cout << "\t\t-----------------------------------------------------------------------\n";
            cout << "\t\t|    Food Item                        | Quantity | Total Price (RM)  |\n";
            cout << "\t\t----------------------------------------------------------------------\n";

            for (const auto& entry : orderSummary) {
                cout << "\t\t| " << setw(35) << left << entry.first << " | " << setw(8) << entry.second.first << " | " << setw(17) << fixed << setprecision(2) << entry.second.second << " |\n";
            }

            cout << "\t\t-----------------------------------------------------------------------\n";
        }
        char chae;
        do
        {
            cout << "\n\t\tDo you want to order?  (Y/N): ";
            cin >> chae;

            if (chae == 'Y' || chae == 'y')
            {
                anotherOrder(customerName);
            }
            else if (chae == 'N' || chae == 'n')
            {
                customer(customerName);
            }
            else
            {
                cout << "\n\t\tWrong Input! TRY AGAIN\n" << endl;
                char a;
            }
        } while ('a');

    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState: " << e.getSQLState() << endl;
    }
}
void customerprofile(Connection* connection, const string& customerName)
{
    system("cls");
    string newName, newUsername, newPassword, newEmail, searchName, newContact;
    char choice;

    try {
        while (true) {

            PreparedStatement* ps = connection->prepareStatement("SELECT * FROM customer WHERE Cust_Name = ?");
            ps->setString(1, customerName);
            ResultSet* res = ps->executeQuery();

            if (res->next()) {
                string maskedPassword(res->getString("Cust_Password").length(), '*');
                cout << "\n\t\tCurrent Details:\n";
                cout << "\t\tName    : " << customerName << endl;
                cout << "\t\tUsername: " << res->getString("Cust_Username") << endl;
                cout << "\t\tPassword: " << maskedPassword << endl;
                cout << "\t\tIC      : " << res->getString("Cust_IC") << endl;
                cout << "\t\tContact Number: " << res->getString("Cust_Contact") << endl;
                cout << "\t\tEmail   : " << res->getString("Cust_Email") << endl;


                char a;
                cout << "\n\t\tDo you want to change password (Y/N): ";
                cin >> a;

                if (a == 'Y' || a == 'y')
                {
                    bool validPassword = false;
                    while (!validPassword) {
                        cout << "\n\t\tEnter new password \n\t\t(at least 6 characters with at include alphabet and number): ";
                        newPassword = "";
                        char ch;
                        while ((ch = _getch()) != '\r') {
                            if (ch == '\b') {
                                if (!newPassword.empty()) {
                                    cout << "\b \b";
                                    newPassword.pop_back();
                                }
                            }
                            else {
                                newPassword += ch;
                                cout << '*';
                            }
                        }

                        if (newPassword.length() < 6) {
                            cerr << "\n\n\t\tPassword must be at least 6 characters long." << endl;
                        }
                        else if (!regex_search(newPassword, regex("[a-zA-Z]"))) {
                            cerr << "\n\n\t\tPassword must contain at least one alphabet." << endl;
                        }
                        else if (!regex_search(newPassword, regex("[0-9]"))) {
                            cerr << "\n\n\t\tPassword must contain at least one number." << endl;
                        }
                        else {
                            validPassword = true;
                        }
                    }

                    PreparedStatement* psUpdate = connection->prepareStatement("UPDATE customer SET Cust_Password = COALESCE(?, Cust_Password) WHERE Cust_Name = ?");
                    psUpdate->setString(1, newPassword);
                    psUpdate->setString(2, customerName);


                    psUpdate->executeUpdate();

                    cout << "\n\t\tDetails updated successfully!" << endl;

                    delete psUpdate;
                    delete ps;
                    cout << "\n\t\tPress Enter to continue...";
                    while (_getch() != 13);
                    customer(customerName);

                }
                else
                {
                    customer(customerName);
                }

            }
            else
            {
                cout << "\n\t\tStaff with name " << customerName << " not found." << endl;
            }
            delete res;
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState: " << e.getSQLState() << endl;
    }
}
void customer(const string& customerName) {
   
    string name, fname;
    int check;

    Connection* connection = databaseCon();
    
    system("cls");

    cout << "\t\t****************************** WELCOME " << customerName << " * *************************\n\n";
    cout << "\t\t1. Profile" << endl;
    cout << "\t\t2. Order Food" << endl;
    cout << "\t\t3. Check History Ordered" << endl;
    cout << "\t\t4. Exit" << endl;
    do 
    {
        cout << "\t\tWhat you want to do? : ";
        cin >> check;
        if (check == 1)
        {
            customerprofile(connection, customerName);
        }
        else if (check == 2)
        {
            anotherOrder(customerName);
        }
        else if (check == 3)
        {
            history(customerName);
        }
        else if (check == 4)
        {
            exit(0);
        }
        else
        {
            cout << "\n\t\tWrong Input! TRY AGAIN\n" << endl;
            char a;
        }
    } while ('a');
}
void customerLogin() 
{
    system("cls");
    string name, pass,customerName;
    char ch;
    string blankPassword;
    int attempts = 0;
    const int maxAttempts = 3;

    try {
        Connection* connection = databaseCon();
        if (!connection) {
            cerr << "Failed to connect to the database." << endl;
            return;
        }

        bool loggedIn = false;
        do {
            cout << "\t\tPlease Log In to continue (customer)" << endl;

            cout << "\n\t\tUsername: ";
            cin >> name;

            cout << "\t\tPassword: ";
            ch = _getch();
            pass.clear();
            blankPassword.clear();

            while (ch != 13) {
                if (ch == 8) {
                    if (!pass.empty()) {
                        pass.pop_back();
                        blankPassword.pop_back();
                        cout << "\b \b";
                    }
                }
                else {
                    pass.push_back(ch);
                    blankPassword.push_back('*');
                    cout << '*';
                }

                ch = _getch();
            }

            system("CLS");

            PreparedStatement* ps = connection->prepareStatement("SELECT Cust_id FROM customer WHERE Cust_Username = ? AND Cust_Password = ?");
            ps->setString(1, name);
            ps->setString(2, pass);

            ResultSet* rs = ps->executeQuery();
            if (rs->next()) {
                rs->getInt("Cust_id");
                loggedIn = true;
            }
            else {
                cout << "\n\t\tInvalid username or password. Please try again." << "\a attempts number " << attempts + 1 << " of " << maxAttempts << endl;
                attempts++;
                if (attempts >= maxAttempts) {
                    cerr << "\n\t\tMaximum login attempts reached. Exiting." << endl;
                    return;
                }
            }

        } while (!loggedIn);

        PreparedStatement* psName = connection->prepareStatement("SELECT Cust_Name FROM customer WHERE Cust_Username = ?");
        psName->setString(1, name);

        ResultSet* rsName = psName->executeQuery();
        if (rsName->next()) {
            customerName = rsName->getString("Cust_Name");
            customer(customerName); 
        }
        else {
            cerr << "Error retrieving customer name." << endl;
          
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
        cerr << "SQLState : " << e.getSQLState() << endl;
    }
}
int main() {
    int choice;
    string A_user, A_pass, S_user, S_pass, C_user, C_pass;
    
    cout << "\t\t****************************** WELCOME**************************\n\n";
    cout << "\t\t\t1. Admin" << endl;
    cout << "\t\t\t2. Staff" << endl;
    cout << "\t\t\t3. Customer" << endl;
    cout << "\t\t\t4. Exit" << endl;

    do {
        cout << "\n\t\tPlease choose who to use me: ";
        cin >> choice;
        
        switch (choice) {
        case 1:
            adminLogin();
            break;
        case 2:
            staffLogin();
            break;
        case 3:
            customerLogin();
            break;
        case 4:
             system("cls");
            break;
        default:
           cout << "\n\t\tInvalid choice. Please select a valid option (1-4).\n";
        }

    } while (choice != 0<4);
    return 0;
}

