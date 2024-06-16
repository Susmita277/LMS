#include <iostream>
#include <fstream>
#include <cstring>
#include <conio.h>

using namespace std;

class Book
{
    int id;
    char title[50];
    char author[50];
    bool available;

public:
    Book()
    {
        available = true; // By default, book is available when created
    }

public:
    void createBook()
    {
        cout << "Enter book ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter book title: ";
        cin.getline(title, 50);
        cout << "Enter book author: ";
        cin.getline(author, 50);
    }

    void displayBook() const
    {
        cout << "ID: " << id << "\n";
        cout << "Title: " << title << "\n";
        cout << "Author: " << author << "\n";
    }

    int getId() const
    {
        return id;
    }
    const char *getTitle() const
    {
        return title;
    }

    const char *getAuthor() const
    {
        return author;
    }

    void modifyBook()
    {
        cout << "Enter new title: ";
        cin.ignore();
        cin.getline(title, 50);
        cout << "Enter new author: ";
        cin.getline(author, 50);
        cout << "Is the book available? (1 for yes, 0 for no): ";
        cin >> available;
    }

    bool isAvailable() const
    {
        return available;
    }

    void setAvailable(bool status)
    {
        available = status;
    }

    void saveToFile(ofstream &outFile) const
    {
        outFile << id << "\n";
        outFile << title << "\n";
        outFile << author << "\n";
        outFile << available << "\n"; // Save availability status
    }

    bool loadFromFile(ifstream &inFile)
    {
        if (inFile >> id)
        {
            inFile.ignore();
            inFile.getline(title, 50);
            inFile.getline(author, 50);
            inFile >> available;
            inFile.ignore(); // Ignore the newline after the availability
            return true;
        }
        return false;
    }
};

bool checkPassword(const char *password)
{
    return strcmp(password, "library") == 0;
}


void displayMenu(); // Function declaration
void bookIssue();
void bookDeposit();
void administrativeMenu();
void createBook();
void displayBooks();
void searchBook();
void modifyBook();
void deleteBook();

int main()
{
    char password[20] = "library";
    char inputPassword[20];
    cout << "\n \n  ";
    cout << "Enter the password to access the system: ";
    cin >> inputPassword;

    if (!checkPassword(inputPassword))
    {
        cout << "Wrong password. Press any key to exit.";
        _getch();
        return 0;
    }

    while (true)
    {
        displayMenu();
        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            bookIssue();
            break;
        case 2:
            bookDeposit();
            break;
        case 3:
            administrativeMenu();
            break;
        case 4:
            cout << "Exiting the program.\n";
            exit(0);
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}

void displayMenu()
{
    cout << "\n \n";
    cout << "\n ***Welcome to Library Management System***\n";
    cout << "1. Book Issue\n";
    cout << "2. Book Deposit\n";
    cout << "3. Administrative Menu\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}

void bookIssue()
{
    int bookId;
    cout << "\n \n";
    cout << "Enter book ID to issue: ";
    cin >> bookId;

    // Open the books file for reading and writing
    ifstream inFile("books.txt");
    ofstream outFile("temp.txt");

    if (!inFile || !outFile)
    {
        cout << "Error opening file.\n";
        return;
    }

    Book book;
    bool found = false;
    while (book.loadFromFile(inFile))
    {
        if (book.getId() == bookId)
        {
            if (book.isAvailable())
            {
                book.setAvailable(false); // Mark book as issued
                cout << "Book issued successfully.\n";
            }
            else
            {
                cout << "Book is already issued.\n";
            }
            found = true;
        }
        book.saveToFile(outFile);
    }

    inFile.close();
    outFile.close();

    if (!found)
    {
        cout << "Book with ID " << bookId << " not found.\n";
    }
    else
    {
        remove("books.txt");
        rename("temp.txt", "books.txt");
    }
}

void bookDeposit()
{
    int bookId;
    cout << "Enter book ID to deposit: ";
    cin >> bookId;

    ifstream inFile("books.txt");
    ofstream outFile("temp.txt");

    if (!inFile || !outFile)
    {
        cout << "Error opening file.\n";
        return;
    }

    Book book;
    bool found = false;
    while (book.loadFromFile(inFile))
    {
        if (book.getId() == bookId)
        {
            if (!book.isAvailable())
            {
                book.setAvailable(true); // Mark book as available
                cout << "Book deposited successfully.\n";
            }
            else
            {
                cout << "Book is already available.\n";
            }
            found = true;
        }
        book.saveToFile(outFile);
    }

    inFile.close();
    outFile.close();

    if (!found)
    {
        cout << "Book with ID " << bookId << " not found.\n";
    }
    else
    {
        remove("books.txt");
        rename("temp.txt", "books.txt");
    }
}

void administrativeMenu()
{
    while (true)
    {
        cout << "\nAdministrative Menu:\n";
        cout << "1. Create Book\n";
        cout << "2. Display Books\n";
        cout << "3. Search Book\n";
        cout << "4. Modify Book\n";
        cout << "5. Delete Book\n";
        cout << "6. Return to Main Menu\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            createBook();
            break;
        case 2:
            displayBooks();
            break;
        case 3:
            searchBook();
            break;
        case 4:
            modifyBook();
            break;
        case 5:
            deleteBook();
            break;
        case 6:
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}
void createBook()
{
    Book newBook;
    int newId;
    bool idExists;

    do
    {
        newBook.createBook();
        newId = newBook.getId();
        idExists = false; // Initialize idExists to false for each iteration

        ifstream inFile("books.txt");
        Book existingBook;

        while (existingBook.loadFromFile(inFile))
        {
            if (existingBook.getId() == newId)
            {
                idExists = true;
                break;
            }
        }
        inFile.close();

        if (idExists)
        {
            cout << "A book with ID " << newId << " already exists. Please enter another ID.\n";
        }
        else
        {
            ofstream outFile("books.txt", ios::app);
            newBook.saveToFile(outFile);
            outFile.close();
            cout << "Book created successfully.\n";
        }
    } while (idExists); // Loop until a unique ID is entered
}

void displayBooks()
{
    ifstream inFile("books.txt");
    if (!inFile)
    {
        cout << "No books available.\n";
        return;
    }

    Book book;
    int totalBooks = 0;
    int availableBooks = 0;
    int issuedBooks = 0;

    cout << "\nBooks List:\n";
    while (book.loadFromFile(inFile))
    {
        book.displayBook();
        cout << "-------------------\n";
        totalBooks++;
        if (book.isAvailable())
        {
            availableBooks++;
        }
        else
        {
            issuedBooks++;
        }
    }
    inFile.close();

    cout << "Total books: " << totalBooks << "\n";
    cout << "Available books: " << availableBooks << "\n";
    cout << "Issued books: " << issuedBooks << "\n";
}

void searchBook()
{
    int id;
    cout << "Enter book ID to search: ";
    cin >> id;

    ifstream inFile("books.txt");
    if (!inFile)
    {
        cout << "No books available.\n";
        return;
    }

    Book book;
    bool found = false;
    while (book.loadFromFile(inFile))
    {
        if (book.getId() == id)
        {
            cout << "Book found:\n";
            book.displayBook();
            found = true;
        }
    }
    inFile.close();

    if (!found)
    {
        cout << "Book with ID " << id << " not found.\n";
    }
}

void modifyBook()
{
    int id;
    cout << "Enter book ID to modify: ";
    cin >> id;

    ifstream inFile("books.txt");
    ofstream outFile("temp.txt");

    if (!inFile || !outFile)
    {
        cout << "Error opening file.\n";
        return;
    }

    Book book;
    bool found = false;
    while (book.loadFromFile(inFile))
    {
        if (book.getId() == id)
        {
            book.modifyBook();
            cout << "Book modified successfully.\n";
            found = true;
        }
        book.saveToFile(outFile);
    }

    inFile.close();
    outFile.close();

    remove("books.txt");
    rename("temp.txt", "books.txt");

    if (!found)
    {
        cout << "Book with ID " << id << " not found.\n";
    }
}

void deleteBook()
{
    int id;
    cout << "Enter book ID to delete: ";
    cin >> id;

    ifstream inFile("books.txt");
    ofstream outFile("temp.txt");
    if (!inFile || !outFile)
    {
        cout << "Error opening file.\n";
        return;
    }

    Book book;
    bool found = false;
    while (book.loadFromFile(inFile))
    {
        if (book.getId() == id)
        {
            found = true;
        }
        else
        {
            book.saveToFile(outFile);
        }
    }

    inFile.close();
    outFile.close();

    remove("books.txt");
    rename("temp.txt", "books.txt");

    if (found)
    {
        cout << "Book deleted successfully.\n";
    }
    else
    {
        cout << "Book with ID " << id << " not found.\n";
    }
}
