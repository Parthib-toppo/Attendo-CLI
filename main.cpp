#include<bits/stdc++.h>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
using namespace std;

sql::mysql::MySQL_Driver *driver;
sql::Connection *con;

driver = sql::mysql::get_mysql_driver_instance();
con = driver->connect("tcp://127.0.0.1:3306", "username", "password");
con->setSchema("attendance_management");

void signUp(sql::Connection *con) {
    string username, password, name;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    cout << "Enter name: ";
    cin >> name;

    sql::Statement *stmt = con->createStatement();
    string query = "INSERT INTO users (username, password, name) VALUES ('" + username + "', '" + password + "', '" + name + "')";
    stmt->execute(query);
    cout << "User registered successfully!" << endl;
    delete stmt;
}

bool login(sql::Connection *con, int &userId) {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    sql::Statement *stmt = con->createStatement();
    string query = "SELECT id FROM users WHERE username = '" + username + "' AND password = '" + password + "'";
    sql::ResultSet *res = stmt->executeQuery(query);

    if (res->next()) {
        userId = res->getInt("id");
        cout << "Login successful!" << endl;
        delete res;
        delete stmt;
        return true;
    } else {
        cout << "Login failed!" << endl;
        delete res;
        delete stmt;
        return false;
    }
}

void markAttendance(sql::Connection *con, int userId) {
    int subjectId;
    string status;

    cout << "Enter subject ID: ";
    cin >> subjectId;
    cout << "Enter attendance status (Present/Absent): ";
    cin >> status;

    sql::Statement *stmt = con->createStatement();
    string query = "INSERT INTO attendance (user_id, subject_id, attendance_date, status) VALUES (" +
                    to_string(userId) + ", " + to_string(subjectId) + ", CURDATE(), '" + status + "')";
    stmt->execute(query);
    cout << "Attendance marked successfully!" << endl;
    delete stmt;
}

int main() {
    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "username", "password");
    con->setSchema("attendance_management");

    int choice;
    int userId = -1;

    while (true) {
        cout << "1. Sign Up\n2. Log In\n3. Mark Attendance\n4. View Attendance\n5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            signUp(con);
        } else if (choice == 2) {
            if (login(con, userId)) {
                cout << "Welcome!" << endl;
            }
        } else if (choice == 3 && userId != -1) {
            markAttendance(con, userId);
        } else if (choice == 4 && userId != -1) {
            viewAttendance(con, userId); // Assuming you have a viewAttendance function
        } else if (choice == 5) {
            break;
        } else {
            cout << "Invalid choice or login required!" << endl;
        }
    }

    delete con;
    return 0;
}
