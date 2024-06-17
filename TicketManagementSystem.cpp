#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// Base User class
class User {
protected:
    string username;
    string password;

public:
    User(const string& username, const string& password) : username(username), password(password) {}

//	User(string username,string password){
//		this->username=username;
//		this->password=password;
//	}
    string getUsername() const {
        return username;
    }

    bool checkPassword(const string& password) const {
        return this->password == password;
    }

    virtual void displayMenu() const = 0;
};

// Customer class
class Customer : public User {
public:
    Customer(const string& username, const string& password) : User(username, password) {}

    void displayMenu() const {
        cout << "Customer Menu:\n1. Book Ticket\n2. View Tickets\n3. Logout" << endl;
    }
};

// Admin class
class Admin : public User {
public:
    Admin(const string& username, const string& password) : User(username, password) {}

    void displayMenu() const {
        cout << "Admin Menu:\n1. Add Movie\n2. Remove Movie\n3. View All Tickets\n4. Logout" << endl;
    }
};

// Movie class
class Movie {
private:
    string title;
    string genre;
    double duration; // in minutes

public:
    int row, col;

    Movie(const string& title, const string& genre, double duration) 
        : title(title), genre(genre), duration(duration), row(-1), col(-1) {}

    string getTitle() const {
        return title;
    }

    void displayInfo() const {
        cout << "Title: " << title << ", Genre: " << genre << ", Duration: " << duration << " mins" << endl;
    }
};

// Ticket class
class Ticket {
private:
    Movie movie;
    string customerUsername;

public:
    Ticket(const Movie& movie, const string& customerUsername) 
        : movie(movie), customerUsername(customerUsername) {}

    void displayInfo() const {
        cout << "Customer: " << customerUsername << ", Movie: ";
        movie.displayInfo();
    }

    string getCustomerUsername() const {
        return customerUsername;
    }
};

// TicketSystem class
class TicketSystem {
private:
    vector<User*> users;
    vector<Movie> movies;
    vector<Ticket> tickets;
    int seats[5][5];
    User* currentUser;
    int currentUserIndex;

    void signUp() {
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;
        if (username != "admin") {
            users.push_back(new Customer(username, password));
            cout << "Customer registered successfully!" << endl;
        } else {
            cout << "You can't use this username and password" << endl;
        }
    }

    void login() {
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        for (size_t i = 0; i < users.size(); ++i) {
            if (users[i]->getUsername() == username && users[i]->checkPassword(password)) {
                currentUser = users[i];
                currentUserIndex = i;
                cout << "Login successful!" << endl;
                return;
            }
        }
        cout << "Invalid username or password." << endl;
    }

    void displaySeats() const {
        cout << "0==> Available 1==> Occupied" << endl;
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                cout << seats[i][j] << "  ";
            }
            cout << endl;
        }
    }

    void logout() {
        currentUser = NULL;
        cout << "Logged out successfully." << endl;
    }

    void addMovie() {
        string title, genre;
        double duration;
        cout << "Enter title: ";
        cin >> title;
        cout << "Enter genre: ";
        cin >> genre;
        cout << "Enter duration (in minutes): ";
        cin >> duration;

        movies.push_back(Movie(title, genre, duration));
        cout << "Movie added successfully!" << endl;
    }

    void removeMovie() {
        string title;
        cout << "Enter title of the movie to remove: ";
        cin >> title;

        for (size_t i = 0; i < movies.size(); ++i) {
            if (movies[i].getTitle() == title) {
                seats[movies[i].row][movies[i].col] = 0;
                movies.erase(movies.begin() + i);
                displaySeats();
                cout << "Movie removed successfully!" << endl;
                return;
            }
        }
        cout << "Movie not found." << endl;
    }

    void viewAllTickets() const {
        for (size_t i = 0; i < tickets.size(); ++i) {
            tickets[i].displayInfo();
        }
    }

    void bookTicket() {
        displaySeats();
        string title;
        cout << "Enter title of the movie to book: ";
        cin >> title;
        Movie* selectedMovie = NULL;
        bool found = false;

        for (size_t i = 0; i < movies.size(); ++i) {
            if (movies[i].getTitle() == title) {
                selectedMovie = &movies[i];
                found = true;
                break;
            }
        }

        if (found) {
            int row, col;
            cout << "Enter Seat Row and Column: ";
            cin >> row >> col;
            if (row >= 5 || col >= 5) {
                cout << "Invalid Position..." << endl;
                return;
            }

            if (seats[row][col] == 0) {
                seats[row][col] = 1;
                selectedMovie->row = row;
                selectedMovie->col = col;
                tickets.push_back(Ticket(*selectedMovie, currentUser->getUsername()));
                cout << "Ticket booked successfully!" << endl;
                displaySeats();
            } else {
                cout << "Seat is Already Occupied..." << endl;
            }
        } else {
            cout << "Movie not found." << endl;
        }
    }

    void viewTickets() const {
        for (size_t i = 0; i < tickets.size(); ++i) {
            if (tickets[i].getCustomerUsername() == currentUser->getUsername()) {
                tickets[i].displayInfo();
            }
        }
        long l = rand() % 9000000000L + 1000000000L;
        cout << "UTR Number  : " << l << endl;
    }

public:
    TicketSystem() : currentUser(NULL), currentUserIndex(-1) {
        users.push_back(new Admin("admin", "123"));
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                seats[i][j] = 0;
            }
        }
    }

    ~TicketSystem() {
        for (size_t i = 0; i < users.size(); ++i) {
            delete users[i];
        }
    }

    void menu() {
        while (true) {
            if (currentUser == NULL) {
                cout << "1. Sign Up\n2. Login\n3. Exit" << endl;
                int choice;
                cin >> choice;

                switch (choice) {
                    case 1:
                        signUp();
                        break;
                    case 2:
                        login();
                        break;
                    case 3:
                        cout << "Thank You....." << endl;
                        return;
                    default:
                        cout << "Invalid choice." << endl;
                }
            } else {
                currentUser->displayMenu();
                int choice;
                cin >> choice;

                if (dynamic_cast<Customer*>(currentUser) != NULL) {
                    cout << "Movies available in theaters now: " << endl;
                    for (size_t i = 0; i < movies.size(); ++i) {
                        cout << movies[i].getTitle() << endl;
                    }
                    switch (choice) {
                        case 1:
                            bookTicket();
                            break;
                        case 2:
                            viewTickets();
                            break;
                        case 3:
                            logout();
                            break;
                        default:
                            cout << "Invalid choice." << endl;
                    }
                } else if (dynamic_cast<Admin*>(currentUser) != NULL) {
                    switch (choice) {
                        case 1:
                            addMovie();
                            break;
                        case 2:
                            removeMovie();
                            break;
                        case 3:
                            viewAllTickets();
                            break;
                        case 4:
                            logout();
                            break;
                        default:
                            cout << "Invalid choice." << endl;
                    }
                }
            }
        }
    }
};

int main() {
    srand(time(0));  
    cout << "Welcome to Movie Ticket Management System...." << endl;
    TicketSystem obj;
    obj.menu();
    return 0;
}

