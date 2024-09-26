#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
using namespace std;

class Book {
    int book_id;
    string title;
    string author;
    bool is_issued;

public:
    Book() : is_issued(false) {}

    void getBookData() {
        cout << "\nEnter Book ID: ";
        cin >> book_id;
        cin.ignore();
        cout << "Enter Book Title: ";
        getline(cin, title);
        cout << "Enter Author Name: ";
        getline(cin, author);
        is_issued = false;  // Initially, the book is not issued
    }

    void displayBook() const {
        cout << setw(6) << book_id << setw(20) << title << setw(20) << author;
        if (is_issued)
            cout << setw(15) << "Issued" << endl;
        else
            cout << setw(15) << "Available" << endl;
    }

    int getBookID() const {
        return book_id;
    }

    bool checkIssued() const {
        return is_issued;
    }

    void issueBook() {
        is_issued = true;
    }

    void returnBook() {
        is_issued = false;
    }
};

// File management
fstream book_file;

// Function to add a new book
void addBook() {
    Book book;
    book.getBookData();
    book_file.open("books.dat", ios::out | ios::app | ios::binary);
    book_file.write((char*)&book, sizeof(Book));
    book_file.close();
    cout << "\nBook Added Successfully.\n";
}

// Function to display all books
void displayAllBooks() {
    Book book;
    book_file.open("books.dat", ios::in | ios::binary);
    if (!book_file) {
        cout << "Error in opening file.\n";
        return;
    }
    cout << "\n\nBooks in Library:\n";
    cout << setw(6) << "ID" << setw(20) << "Title" << setw(20) << "Author" << setw(15) << "Status\n";
    while (book_file.read((char*)&book, sizeof(Book))) {
        book.displayBook();
    }
    book_file.close();
}

// Function to search for a book
void searchBook(int id) {
    Book book;
    bool found = false;
    book_file.open("books.dat", ios::in | ios::binary);
    while (book_file.read((char*)&book, sizeof(Book))) {
        if (book.getBookID() == id) {
            book.displayBook();
            found = true;
        }
    }
    book_file.close();
    if (!found) {
        cout << "\nBook not found.\n";
    }
}

// Function to issue a book
void issueBook(int id) {
    Book book;
    bool found = false;
    book_file.open("books.dat", ios::in | ios::out | ios::binary);
    while (book_file.read((char*)&book, sizeof(Book)) && !found) {
        if (book.getBookID() == id && !book.checkIssued()) {
            book.issueBook();
            int pos = -1 * sizeof(book);
            book_file.seekp(pos, ios::cur);
            book_file.write((char*)&book, sizeof(Book));
            cout << "\nBook issued successfully.\n";
            found = true;
        }
    }
    book_file.close();
    if (!found) {
        cout << "\nBook not available or already issued.\n";
    }
}

// Function to return a book
void returnBook(int id) {
    Book book;
    bool found = false;
    book_file.open("books.dat", ios::in | ios::out | ios::binary);
    while (book_file.read((char*)&book, sizeof(Book)) && !found) {
        if (book.getBookID() == id && book.checkIssued()) {
            book.returnBook();
            int pos = -1 * sizeof(book);
            book_file.seekp(pos, ios::cur);
            book_file.write((char*)&book, sizeof(Book));
            cout << "\nBook returned successfully.\n";
            found = true;
        }
    }
    book_file.close();
    if (!found) {
        cout << "\nBook not found or not issued.\n";
    }
}

// Function to remove a book
void removeBook(int id) {
    Book book;
    fstream temp_file;
    bool found = false;
    book_file.open("books.dat", ios::in | ios::binary);
    temp_file.open("temp.dat", ios::out | ios::binary);

    while (book_file.read((char*)&book, sizeof(Book))) {
        if (book.getBookID() == id) {
            found = true;
        } else {
            temp_file.write((char*)&book, sizeof(Book));
        }
    }

    book_file.close();
    temp_file.close();

    remove("books.dat");
    rename("temp.dat", "books.dat");

    if (found) {
        cout << "\nBook removed successfully.\n";
    } else {
        cout << "\nBook not found.\n";
    }
}

// Main menu
void librarianMenu() {
    int choice;
    do {
        cout << "\n\n--- LIBRARIAN MENU ---\n";
        cout << "1. Add Book\n";
        cout << "2. Display All Books\n";
        cout << "3. Search Book\n";
        cout << "4. Remove Book\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayAllBooks(); break;
            case 3: {
                int id;
                cout << "\nEnter Book ID to search: ";
                cin >> id;
                searchBook(id);
                break;
            }
            case 4: {
                int id;
                cout << "\nEnter Book ID to remove: ";
                cin >> id;
                removeBook(id);
                break;
            }
            case 5: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 5);
}

void userMenu() {
    int choice;
    do {
        cout << "\n\n--- USER MENU ---\n";
        cout << "1. Search Book\n";
        cout << "2. Issue Book\n";
        cout << "3. Return Book\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int id;
                cout << "\nEnter Book ID to search: ";
                cin >> id;
                searchBook(id);
                break;
            }
            case 2: {
                int id;
                cout << "\nEnter Book ID to issue: ";
                cin >> id;
                issueBook(id);
                break;
            }
            case 3: {
                int id;
                cout << "\nEnter Book ID to return: ";
                cin >> id;
                returnBook(id);
                break;
            }
            case 4: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 4);
}

int main() {
    int role;
    cout << "--- LIBRARY MANAGEMENT SYSTEM ---\n";
    cout << "Select Role: \n";
    cout << "1. Librarian\n";
    cout << "2. User\n";
    cout << "Enter role: ";
    cin >> role;

    switch (role) {
        case 1: librarianMenu(); break;
        case 2: userMenu(); break;
        default: cout << "Invalid role selected.\n";
    }

    return 0;
}
