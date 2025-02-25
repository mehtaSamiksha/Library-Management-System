// Library Management System

// User Roles & Authentication
// Librarian password: lib123 | Counter password: counter123
// Locks out after 5 failed attempts, logs all login attempts (login_log.txt)

// Data Validation
// Email: Must end with @gmail.com, @outlook.com, or @lpu.in
// Phone: 10 digits, starts with 6-9
// Reg. No: 8-digit number
// ISBN: 13-digit number
// Book & Author: Letters, numbers, spaces, basic punctuation only

// Functionalities
// - Librarian: Add/Delete/Update books & students, view logs
// - Counter Staff: Issue/Return books, update inventory
// - Students: Max 3 books

// File Management
// - Stores books (books.txt), students (students.txt), issued books (issued_books.txt), and login logs (login_log.txt)
// - Loads data on start, saves before exit

// Error Handling
// - Displays errors for invalid input, unavailable books, max books issued, incorrect login, etc.

// Success Messages
// - Shown after adding/updating/deleting books/students, issuing/returning books, logging in/out

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>   // For email validation
#include <ctime>   // For getting current timestamp
#include <iomanip> // For table formatting
#include <limits>
#include <unordered_map>
#include <unordered_set>

using namespace std;

// Book Record Class
class Record
{
public:
    string bookName;
    string author;
    string isbn;
    int copies;

    Record(string bName, string auth, string id, int num)
    {
        bookName = bName;
        author = auth;
        isbn = id;
        copies = num;
    }
};

// Student Class
class Student
{
public:
    string firstName;
    string lastName;
    string regNumber;
    string phone;
    string email;

    Student(string fName, string lName, string reg, string ph, string em)
    {
        firstName = fName;
        lastName = lName;
        regNumber = reg;
        phone = ph;
        email = em;
    }
};

// Library Management System Class
class LMS
{
private:
    string librarianPassword = "lib123";
    string counterPassword = "counter123";
    int librarianAttempts = 5;
    int counterAttempts = 5;
    vector<Record> books;
    vector<Student> students;
    unordered_map<string, int> studentBookCount;
    unordered_map<string, unordered_set<string>> studentIssuedBooks;

public:
    LMS()
    {
        loadBooks();
        loadStudents();
    }

    void loadBooks()
{
    ifstream file("books.txt");
    if (!file)
    {
        //cout << "Error: Could not open books.txt\n";
        return;
    }

    string bName, auth, id;
    int num;

    while (getline(file, bName, ',')) // Read book name until comma
    {
        getline(file, auth, ','); // Read author until comma
        file >> id >> num;        // Read ISBN and copies
        file.ignore();            // Ignore newline at end

        books.push_back(Record(bName, auth, id, num));
        //cout << "Loaded: " << bName << " | " << auth << " | " << id << " | " << num << endl; // Debug Output
    }

    file.close();
}

    void loadStudents()
    {
        ifstream file("students.txt");
        if (!file)
            return;
        string fName, lName, regNum, phone, email;
        while (file >> fName >> lName >> regNum >> phone >> email)
        {
            students.push_back(Student(fName, lName, regNum, phone, email));
        }
        file.close();
    }

    void saveBooks()
{
    ofstream file("books.txt");
    if (!file)
    {
        cout << "Error: Could not open books.txt for writing\n";
        return;
    }

    for (auto &book : books)
    {
        file << book.bookName << "," << book.author << "," << book.isbn << " " << book.copies << endl;
        //cout << "Saving: " << book.bookName << " | " << book.author << " | " << book.isbn << " | " << book.copies << endl; // Debug output
    }

    file.close();
}


    void saveStudents()
    {
        ofstream file("students.txt");
        for (auto &student : students)
        {
            file << student.firstName << " " << student.lastName << " " << student.regNumber << " "
                 << student.phone << " " << student.email << endl;
        }
        file.close();
    }

    void addStudent()
    {
        string fName, lName, regNum, phone, email;
        cout << "\nEnter first name: ";
        cin >> fName;
        cout << "Enter last name: ";
        cin >> lName;
        cout << "Enter registration number (8 digits): ";
        cin >> regNum;
        while (regNum.length() != 8 || !all_of(regNum.begin(), regNum.end(), ::isdigit))
        {
            cout << "Invalid registration number! It must be an 8-digit number: ";
            cin >> regNum;
        }

        // Check if student already exists
        for (const auto &student : students)
        {
            if (student.regNumber == regNum)
            {
                cout << "\nError: A student with this registration number already exists!\n";
                return;
            }
        }

        cout << "Enter phone number (starting with 6, 7, 8, or 9): ";
        cin >> phone;
        while (!(phone[0] == '6' || phone[0] == '7' || phone[0] == '8' || phone[0] == '9') || phone.length() != 10)
        {
            cout << "Invalid phone number! It must start with 6, 7, 8, or 9 and be 10 digits long: ";
            cin >> phone;
        }

        cout << "Enter email address: ";
        cin >> email;
        while (!regex_match(email, regex("^[a-zA-Z0-9._%+-]+@(gmail\\.com|outlook\\.com|lpu\\.in)$")))
        {
            cout << "Invalid email! It must end with @gmail.com, @outlook.com, or @lpu.in: ";
            cin >> email;
        }

        students.push_back(Student(fName, lName, regNum, phone, email));
        saveStudents();
        cout << "\nThe student has been successfully registered.\n";
    }

    void addBook()
    {
        string bName, auth, id;
        int num;

        cout << "\nEnter book name: ";
        cin.ignore();
        getline(cin, bName);
        while (!regex_match(bName, regex("^[a-zA-Z0-9 .,-]+$")))
        {
            cout << "Invalid book name! Only letters, numbers, spaces, and basic punctuation allowed: ";
            getline(cin, bName);
        }

        cout << "Enter author name: ";
        getline(cin, auth);
        while (!regex_match(auth, regex("^[a-zA-Z .,-]+$")))
        {
            cout << "Invalid author name! Only letters, spaces, and basic punctuation allowed: ";
            getline(cin, auth);
        }

        cout << "Enter ISBN (13 digits): ";
        cin >> id;
        while (id.length() != 13 || !all_of(id.begin(), id.end(), ::isdigit))
        {
            cout << "Invalid ISBN! It must be exactly 13 digits: ";
            cin >> id;
        }

        cout << "Enter number of copies: ";
        num = getIntInput();

        // Check if book already exists
        for (auto &book : books)
        {
            if (book.isbn == id)
            {
                book.copies += num;
                saveBooks();
                cout << "\nThe book already exists. Updated copies count.\n";
                return;
            }
        }

        books.push_back(Record(bName, auth, id, num));
        saveBooks();
        cout << "\nThe book has been successfully added.\n";
    }

    void deleteBook()
    {
        string id;
        cout << "\nEnter ISBN to delete: ";
        cin >> id;

        for (auto it = books.begin(); it != books.end(); ++it)
        {
            if (it->isbn == id)
            {
                cout << "Enter number of copies to remove: ";
                int num;
                num = getIntInput();

                if (num >= it->copies)
                {
                    books.erase(it);
                    cout << "\nAll copies of the book have been removed from the inventory.\n";
                }
                else
                {
                    it->copies -= num;
                    cout << "\n"
                         << num << " copies have been removed. Remaining: " << it->copies << endl;
                }

                saveBooks();
                return;
            }
        }
        cout << "\nBook not found in the inventory.\n";
    }

    void updateBook()
    {
        string id;
        cout << "\nEnter ISBN to update: ";
        cin >> id;
        for (auto &book : books)
        {
            if (book.isbn == id)
            {
                cout << "Enter new number of copies: ";
                cin >> book.copies;
                saveBooks();
                cout << "\nThe book details have been successfully updated.\n";
                return;
            }
        }
        cout << "\nBook not found in the inventory.\n";
    }

    void showAllBooks()
    {
        if (books.empty())
        {
            cout << "\nThe library inventory is currently empty.\n";
        }
        else
        {
            cout << "\n===============================\nList of All Books\n===============================\n";
            cout << left << setw(30) << "Book Name" << setw(25) << "Author" << setw(20) << "ISBN" << setw(10)<< "Copies\n";
            cout << "---------------------------------------------------------------\n";
            for (const auto &book : books)
            {
                cout << left << setw(30) << book.bookName << setw(25) << book.author << setw(20) << book.isbn << setw(10)<< book.copies << endl;
            }
        }
    }

    void issueBook()
    {
        string id;
        cout << "\nEnter ISBN: ";
        cin >> id;

        auto bookIt = find_if(books.begin(), books.end(), [&](const Record &b)
                              { return b.isbn == id && b.copies > 0; });

        if (bookIt == books.end())
        {
            cout << "\nThe requested book is not available or not found in the inventory.\n";
            return;
        }

        // Get student details
        string regNum;
        cout << "Enter student registration number (8 digits): ";
        cin >> regNum;
        while (regNum.length() != 8 || !all_of(regNum.begin(), regNum.end(), ::isdigit))
        {
            cout << "Invalid registration number! It must be an 8-digit number: ";
            cin >> regNum;
        }

        // Check if the student already has 3 books issued
        if (studentBookCount[regNum] >= 3)
        {
            cout << "\nThis student has already issued 3 books and cannot issue more.\n";
            return;
        }

        // Check if the student has already issued this book
        if (studentIssuedBooks[regNum].find(id) != studentIssuedBooks[regNum].end())
        {
            cout << "\nThis student has already issued this book.\n";
            return;
        }

        // Issue the book
        bookIt->copies--;
        studentBookCount[regNum]++;
        studentIssuedBooks[regNum].insert(id); // Add the book to the student's issued books set

        saveBooks();
        logIssuedBook(regNum, bookIt->bookName, bookIt->author, bookIt->isbn);

        // Display receipt for the issued book
        cout << "\nThe book has been successfully issued.\n";
        cout << "============================================\n";
        cout << "Receipt for Book Issue\n";
        cout << "--------------------------------------------\n";
        cout << "Book Name: " << bookIt->bookName << "\nAuthor: " << bookIt->author << "\nISBN: " << bookIt->isbn << "\n";
        cout << "Student Registration Number: " << regNum << "\n";
        cout << "--------------------------------------------\n";
    }

    void logIssuedBook(const string &regNum, const string &bookName, const string &author, const string &isbn)
    {
        ofstream logFile("issued_books.txt", ios::app);
        if (logFile.is_open())
        {
            time_t now = time(0);
            char *dt = ctime(&now);
            dt[strlen(dt) - 1] = '\0';

            logFile << regNum << " " << bookName << " " << author << " " << isbn << " " << dt << endl;
            logFile.close();
        }
        else
        {
            cout << "Error: Unable to open issued books log file.\n";
        }
    }

    void returnBook()
    {
        string id;
        cout << "\nEnter ISBN of the book to return: ";
        cin >> id;

        bool bookFound = false;
        for (auto &book : books)
        {
            if (book.isbn == id)
            {
                book.copies++;
                bookFound = true;

                // Get student details to display
                string fName, lName, regNum;
                cout << "\nEnter student first name: ";
                cin >> fName;
                cout << "Enter student last name: ";
                cin >> lName;
                cout << "Enter student registration number: ";
                cin >> regNum;

                cout << "\nThe book has been successfully returned.\n";
                cout << "============================================\n";
                cout << "Receipt for Book Return\n";
                cout << "--------------------------------------------\n";
                cout << "Book Name: " << book.bookName << "\nAuthor: " << book.author << "\nISBN: " << book.isbn << "\n";
                cout << "Student Name: " << fName << " " << lName << "\nRegistration Number: " << regNum << "\n";
                cout << "--------------------------------------------\n";

                // Update the issued books log file
                removeIssuedBookLog(regNum, id);

                saveBooks();
                saveStudents();
                return;
            }
        }

        if (!bookFound)
        {
            cout << "\nInvalid ISBN. Book not found in the inventory.\n";
        }
    }

    void removeIssuedBookLog(const string &regNum, const string &isbn)
    {
        ifstream inFile("issued_books.txt");
        ofstream outFile("temp.txt");

        if (!inFile || !outFile)
        {
            cout << "Error: Unable to open issued books log file.\n";
            return;
        }

        string line;
        bool bookFound = false;

        while (getline(inFile, line))
        {
            stringstream ss(line);
            string student, bookName, author, bookIsbn;
            getline(ss, student, ' ');
            getline(ss, bookName, ' ');
            getline(ss, author, ' ');
            getline(ss, bookIsbn, ' ');

            if (student == regNum && bookIsbn == isbn)
            {
                bookFound = true;
            }
            else
            {
                outFile << line << endl; // Write the line if it's not the returned book
            }
        }

        inFile.close();
        outFile.close();

        if (bookFound)
        {
            remove("issued_books.txt");             // Remove the original log file
            rename("temp.txt", "issued_books.txt"); // Rename the temp file to update the log
            cout << "\nIssued book log updated successfully.\n";
        }
        else
        {
            cout << "\nError: No record of this book being issued to this student.\n";
        }
    }

    void showAllStudents()
    {
        if (students.empty())
        {
            cout << "\nNo students have been registered yet.\n";
        }
        else
        {
            cout << "\n===============================\nList of All Students\n===============================\n";
            cout << left << setw(25) << "First Name" << setw(25) << "Last Name" << setw(20) << "Reg. Number" << setw(15) << "Phone" << "Email\n";
            cout << "---------------------------------------------------------------\n";
            for (const auto &student : students)
            {
                cout << left << setw(25) << student.firstName << setw(25) << student.lastName
                     << setw(20) << student.regNumber << setw(15) << student.phone << student.email << endl;
            }
        }
    }

    bool authenticate(string role)
    {
        string password;
        int &attempts = (role == "Librarian") ? librarianAttempts : counterAttempts;

        while (attempts > 0)
        {
            cout << "\nEnter password: ";
            cin >> password;

            if ((role == "Librarian" && password == librarianPassword) ||
                (role == "Counter" && password == counterPassword))
            {
                cout << "\nWelcome " << role << "!\n";
                logLoginAttempt(role, true); // Log successful login
                return true;
            }
            else
            {
                attempts--;
                cout << "\nIncorrect password! Attempts left: " << attempts << "\n";
                logLoginAttempt(role, false); // Log failed login
            }
        }

        cout << "\nToo many failed attempts. Access denied.\n";
        return false;
    }

    void logLoginAttempt(const string &role, bool success)
    {
        ofstream logFile("login_log.txt", ios::app);
        if (logFile.is_open())
        {
            time_t now = time(0);
            char *dt = ctime(&now);
            dt[strlen(dt) - 1] = '\0';

            logFile << "Role: " << role << ", Success: " << (success ? "Yes" : "No") << ", Time: " << dt << endl;
            logFile.close();
        }
        else
        {
            cout << "Unable to open log file.\n";
        }
    }

    // Function to safely read an integer input
    int getIntInput()
    {
        int choice;
        while (true)
        {
            cin >> choice;
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a valid number: ";
            }
            else
            {
                break;
            }
        }
        return choice;
    }
};

int main()
{
    cout << "\n==============================\n Welcome to the Library Management System!\n==============================\n";
    LMS library;
    int choice;
    do
    {
        cout << "\n1. Librarian\n2. Counter\n3. Exit\nEnter choice: ";
        choice = library.getIntInput();

        if (choice == 1)
        {
            if (library.authenticate("Librarian"))
            {
                int libChoice;
                do
                {
                    cout << "\n1. Add Book\n2. Delete Book\n3. Update Book\n4. Show All Books\n5. Add Student\n6. Show All Students\n7. Logout\nEnter choice: ";
                    libChoice = library.getIntInput();
                    switch (libChoice)
                    {
                    case 1:
                        library.addBook();
                        break;
                    case 2:
                        library.deleteBook();
                        break;
                    case 3:
                        library.updateBook();
                        break;
                    case 4:
                        library.showAllBooks();
                        break;
                    case 5:
                        library.addStudent();
                        break;
                    case 6:
                        library.showAllStudents();
                        break;
                    case 7:
                        break;
                    default:
                        cout << "Invalid option! Please try again.\n";
                    }
                } while (libChoice != 7);
            }
        }
        else if (choice == 2)
        {
            if (library.authenticate("Counter"))
            {
                int counterChoice;
                do
                {
                    cout << "\n1. Issue Book\n2. Return Book\n3. Show All Books\n4. Show All Students\n5. Logout\nEnter choice: ";
                    counterChoice = library.getIntInput();
                    switch (counterChoice)
                    {
                    case 1:
                        library.issueBook();
                        break;
                    case 2:
                        library.returnBook();
                        break;
                    case 3:
                        library.showAllBooks();
                        break;
                    case 4:
                        library.showAllStudents();
                        break;
                    case 5:
                        break;
                    default:
                        cout << "Invalid option! Please try again.\n";
                    }
                } while (counterChoice != 5);
            }
        }
        else if (choice != 3)
        {
            cout << "Invalid choice, try again.\n";
        }

    } while (choice != 3);

    return 0;
}
