#include<iostream>
#include<vector>

using namespace std;

class Library;

class Book {
public:
    string name, author;
    int pages;
    int copies;
    int bookid;
    Book(string name, string author, int pages, int copies, int bookid) {
        this->name = name;
        this->author = author;
        this->pages = pages;
        this->copies = copies;
        this->bookid = bookid;
    }

    void display();
};

class Person {
public:
    string name;
    string address;
    int age;
    int id;
    //Library* lib;
    Person(string name, string address, int age, int id) {
        this->name = name;
        this->address = address;
        this->age = age;
        this->id = id;
    }

    int searchABook(int bookid, Library* lib);
};

class User : public Person {
public:
    string type;
    vector<int> issuedBooks;
    User(string name, string address, int age, int id) : Person(name, address, age, id) {
        this->type = "user";
    }
    void addBookToUser(int id, int bookid, Library* lib);

    void takeABook(int bookid, Library* lib);

    int findIssuedBooks();
    
    void removeBookFromUser(int id, int bookid, Library* lib);

    void dropABook(int bookid, Library* lib);

};

class Administrator : public Person {
public:
    string type;
    Administrator(string name, string address, int age, int id) : Person(name, address, age, id) {
        this->type = "admin";
    }
    Book* addABook(string name, string author, int pages, int copies, int bookid, Library* lib);

    void addUser(User* user, Library* lib);

    void issueToUser(int id, int bookid, Library* lib);

    int findIssuedBooksOfUser(int id, Library* lib);

    void displayBooks(Library* lib);
};

class Library {
    static Library* lib;
    Library() {}
public:
    vector<Book*> books;
    vector<User*> users; // 0, 1
    static Library* getLibrary() {
        if (lib == nullptr) {
            lib = new Library();
            return lib;
        }
        else return lib;
    }
};

Library* Library::lib = nullptr;

// Implement the functions after the class definitions

void Book::display() {
    cout<<"Book details:"<<endl;
    cout<<this->bookid<<" "<<this->name<<" "<<this->author<<" "<<this->copies<<endl;
}
int Person::searchABook(int bookid, Library* lib) {
    vector<Book*> b = lib->books;
    for(int i=0; i<b.size(); i++) {
        if(b[i]->bookid == bookid) return i;
    }
    return -1;
}

void User::addBookToUser(int id, int bookid, Library* lib) {
    for(int i=0; i<lib->users.size(); i++) {
        if(lib->users[i]->id == id) {
            lib->users[i]->issuedBooks.push_back(bookid);
            break;
        }
    }
}

void User::removeBookFromUser(int id, int bookid, Library* lib) {
    bool removed = false;
    for(int i=0; i<lib->users.size(); i++) {
        if(lib->users[i]->id == id) {
            for(int j=0; j<lib->users[i]->issuedBooks.size(); j++) {
                if(lib->users[i]->issuedBooks[j] == bookid) {
                    lib->users[i]->issuedBooks.erase(lib->users[i]->issuedBooks.begin() + i);
                    removed = true;
                }
            } 
            if(removed) break;
        }
    }      
}

void User::takeABook(int bookid, Library* lib) {
    int i = searchABook(bookid, lib);
    if(i != -1) {
        if(lib->books[i]->copies > 0) {
            addBookToUser(id, bookid, lib);
            lib->books[i]->copies -= 1;
            cout<<"Reduced copies to: "<<lib->books[i]->copies<<endl;
            cout<<"Assigned bookid: "<<bookid<<" to user: "<<id<<endl;
        } else {
            cout<<"Book has no copies left"<<endl;
        }
    } else {
        cout<<"No such book exists"<<endl;
    }
}

void User::dropABook(int bookid, Library* lib) {
    int i = searchABook(bookid, lib);
    if(i != -1) {
        lib->books[i]->copies += 1;
        removeBookFromUser(id, bookid, lib);
        cout<<"Dropped bookid: "<<bookid<<" by user: "<<id<<endl;
    } else {
        cout<<"No such book exists"<<endl;
    }
}

int User::findIssuedBooks() {
    cout<<"Issued books for user: "<<id<<" is "<<issuedBooks.size()<<endl;
    return issuedBooks.size();
}


Book* Administrator::addABook(string name, string author, int pages, int copies, int bookid, Library* lib) {
    Book* b = new Book(name, author, pages, copies, bookid);
    lib->books.push_back(b);
    cout<<"Added a book: bookId"<<bookid<<endl;
    return b;
}

void Administrator::addUser(User* user, Library* lib) {
    lib->users.push_back(user);
}

void Administrator::issueToUser(int id, int bookid, Library* lib) {
    int i = searchABook(bookid, lib);
    bool issued = false;
    if(i != -1) {
        if(lib->books[i]->copies > 0) {
            for(int j=0; j<lib->users.size(); j++) {
                if(lib->users[j]->id == id) {
                    lib->users[j]->addBookToUser(id, bookid, lib);
                    lib->books[i]->copies -= 1;
                    cout<<"Reduced copies to: "<<lib->books[i]->copies<<endl;
                    issued = true;
                    cout<<"Assigned bookid: "<<bookid<<" to user: "<<id<<endl;
                    break;
                }
            }
            if(!issued) cout<<"No such user exists"<<endl;
        } else {
            cout<<"Book has no copies left"<<endl;
        }
    } else{
        cout<<"No such book exists"<<endl;
    }
        
}

int Administrator :: findIssuedBooksOfUser(int id, Library* lib) {
    if(id >= 0 && id < lib->users.size()) {
        int books = lib->users[id]->issuedBooks.size();
        cout<<"Issued books of user: "<<id<<"is "<<books;
        return books;
    } else return 0;
}

void Administrator::displayBooks(Library* lib) {
    for(int i=0; i<lib->books.size(); i++) {
        Book* b = lib->books[i];
        b->display();
    }
}

int main() {
    Library* lib = Library::getLibrary();
    Administrator* admin = new Administrator("A", "add1", 23, 1);
    User* user1 = new User("B", "add2", 23, 2);
    User* user2 = new User("C", "add3", 23, 3);
    admin->addUser(user1, lib);
    admin->addUser(user2, lib);
    admin->addABook("Jane Eyre", "Charlotte Bronte", 100, 5, 0, lib);
    admin->addABook("Little Women", "Louisa May alcott", 100, 10, 1, lib);
    admin->issueToUser(2, 1, lib);
    user1->takeABook(1, lib);
    admin->issueToUser(3, 0, lib);
    user1->findIssuedBooks();
    user2->findIssuedBooks();
    admin->displayBooks(lib);
    // Rest of the main function
}
