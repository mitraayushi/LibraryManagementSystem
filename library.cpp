#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Book {
private:
    int id;
    string title, author;
    bool isIssued;

public:
    Book() {}
    Book(int id, string title, string author, bool isIssued = false)
        : id(id), title(title), author(author), isIssued(isIssued) {}

    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool getIsIssued() const { return isIssued; }

    void issue() { isIssued = true; }
    void returnBook() { isIssued = false; }

    void display() const {
        cout << "ID: " << id << ", Title: " << title
             << ", Author: " << author
             << ", Issued: " << (isIssued ? "Yes" : "No") << endl;
    }

    string toFileString() const {
        return to_string(id) + "|" + title + "|" + author + "|" + (isIssued ? "1" : "0");
    }

    static Book fromFileString(const string &line) {
        stringstream ss(line);
        string idStr, title, author, issuedStr;
        getline(ss, idStr, '|');
        getline(ss, title, '|');
        getline(ss, author, '|');
        getline(ss, issuedStr, '|');
        return Book(stoi(idStr), title, author, issuedStr == "1");
    }
};

class Member {
private:
    int memberId;
    string name;

public:
    Member() {}
    Member(int id, string name) : memberId(id), name(name) {}

    int getId() const { return memberId; }
    string getName() const { return name; }

    void display() const {
        cout << "ID: " << memberId << ", Name: " << name << endl;
    }

    string toFileString() const {
        return to_string(memberId) + "|" + name;
    }

    static Member fromFileString(const string &line) {
        stringstream ss(line);
        string idStr, name;
        getline(ss, idStr, '|');
        getline(ss, name, '|');
        return Member(stoi(idStr), name);
    }
};

class LibrarySystem {
private:
    vector<Book> books;
    vector<Member> members;

    void loadBooks() {
        books.clear();
        ifstream file("books.txt");
        string line;
        while (getline(file, line)) {
            if (!line.empty())
                books.push_back(Book::fromFileString(line));
        }
        file.close();
    }

    void loadMembers() {
        members.clear();
        ifstream file("members.txt");
        string line;
        while (getline(file, line)) {
            if (!line.empty())
                members.push_back(Member::fromFileString(line));
        }
        file.close();
    }

    void saveBooks() {
        ofstream file("books.txt");
        for (auto &book : books) {
            file << book.toFileString() << endl;
        }
        file.close();
    }

    void saveMembers() {
        ofstream file("members.txt");
        for (auto &member : members) {
            file << member.toFileString() << endl;
        }
        file.close();
    }

public:
    LibrarySystem() {
        loadBooks();
        loadMembers();
    }

    void addBook() {
        int id;
        string title, author;
        cout << "Enter Book ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter Title: ";
        getline(cin, title);
        cout << "Enter Author: ";
        getline(cin, author);
        books.push_back(Book(id, title, author));
        saveBooks();
        cout << "Book added successfully!\n";
    }

    void registerMember() {
        int id;
        string name;
        cout << "Enter Member ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
        members.push_back(Member(id, name));
        saveMembers();
        cout << "Member registered successfully!\n";
    }

    void issueBook() {
        int bookId;
        cout << "Enter Book ID to issue: ";
        cin >> bookId;
        for (auto &book : books) {
            if (book.getId() == bookId) {
                if (book.getIsIssued()) {
                    cout << "Book already issued.\n";
                    return;
                }
                book.issue();
                saveBooks();
                cout << "Book issued successfully!\n";
                return;
            }
        }
        cout << "Book not found.\n";
    }

    void returnBook() {
        int bookId;
        cout << "Enter Book ID to return: ";
        cin >> bookId;
        for (auto &book : books) {
            if (book.getId() == bookId) {
                if (!book.getIsIssued()) {
                    cout << "Book is not issued.\n";
                    return;
                }
                book.returnBook();
                saveBooks();
                cout << "Book returned successfully!\n";
                return;
            }
        }
        cout << "Book not found.\n";
    }

    void showBooks() const {
        if (books.empty()) {
            cout << "No books available.\n";
        } else {
            for (const auto &book : books) {
                book.display();
            }
        }
    }

    void showMembers() const {
        if (members.empty()) {
            cout << "No members registered.\n";
        } else {
            for (const auto &member : members) {
                member.display();
            }
        }
    }
};

int main() {
    LibrarySystem lib;
    int choice;
    do {
        cout << "\n===== Library Menu =====\n";
        cout << "1. Add Book\n2. Register Member\n3. Issue Book\n4. Return Book\n";
        cout << "5. Show Books\n6. Show Members\n0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: lib.addBook(); break;
            case 2: lib.registerMember(); break;
            case 3: lib.issueBook(); break;
            case 4: lib.returnBook(); break;
            case 5: lib.showBooks(); break;
            case 6: lib.showMembers(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice.\n"; break;
        }
    } while (choice != 0);

    return 0;
}
