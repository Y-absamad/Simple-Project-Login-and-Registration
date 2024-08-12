#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

class User;
const int MaX_Users = 100;
const string file = "data.txt";

void readFromFileToArray(User[], int &); // reads information frome the file and adds it to arraye.
void Registation(User[], int &); // takes the new user's information.  
string getName(const string& ); // check for name is not empty
string checkEmail(User[], int &); // verify that the email is valid
bool isValidEmail(const string& ); // Check for invalid characters
string checkPassword(); // ensure that the password is valid
string checkSQ(); // choose a security question
void login(User[], int &); //that the login data is correct
void forgetpassword(User[], int &); //retrieve the password via email
string deletespaces(const string&); // Delete the spaces from string
void createAccount(User); // send user data to save

class User
{
private:
    string fName;
    string lName;
    string email;
    string password;
    string securityQuestion;
    string questionAnswer;

public:
    void setValue(string fName, string lName, string email, string password, string securityQuestion, string questionAnswer)
    {
        this->fName = fName;
        this->lName = lName;
        this->email = email;
        this->password = password;
        this->securityQuestion = securityQuestion;
        this->questionAnswer = questionAnswer;
    }
    string getfName() { return fName; }
    string getlName() { return lName; }
    string getsecurityQuestion() { return securityQuestion; }
    string getquestionAnswer() { return questionAnswer; }
    string getPassword() { return password; }

    static User loadFromFile(ifstream &file)
    {
        User user;
        getline(file, user.fName);
        getline(file, user.lName);
        getline(file, user.email);
        getline(file, user.password);
        getline(file, user.securityQuestion);
        getline(file, user.questionAnswer);
        return user;
    }

    void saveToFile(ofstream &file)
    {
        file << fName << "\n"
             << lName << "\n"
             << email << "\n"
             << password << "\n"
             << securityQuestion << "\n"
             << questionAnswer<< endl;
    }

    bool checkLogin(string Email, string Pass)
    {
        return Email == email && Pass == password;
    }

    bool checkEmailUser(string Email)
    {
        return Email == email;
    }
};

int main()
{
    User users[MaX_Users];
    int numUser = 0;

    readFromFileToArray(users, numUser);

    bool running = true;
    while (running)
    {
        cout << "1) Login\n2) Registration\n3) forget password\n4)Exit\n";
        cout << "Enter your option ";
        int option;
        cin >> option;
        switch (option)
        {
        case 1:
            login(users, numUser);
            break;
        case 2:
            Registation(users, numUser);
            break;
        case 3:
            forgetpassword(users, numUser);
            break;
        case 4:
            running = false;
            break;
        default:
            cout << "--------------- Enter your option 1 to 4 ---------------\n";
            break;
        }
    }
    return 0;
}

void readFromFileToArray(User users[], int &numUser)
{
    ifstream inFile(file);
    if (inFile.is_open())
    {
        while (numUser < MaX_Users && !inFile.eof())
        {
            users[numUser++] = User::loadFromFile(inFile);
        }
        inFile.close();
    }
}
void Registation(User users[], int &numUser)
{
    string fName, lName, email, password, securityQuestion, questionAnswer;
    cout << "\n-----Please enter information to create an account-----" << endl;

    cin.ignore();
    fName = getName("Enter your first name : ");
    
    lName = getName("Enter your last name : ");
    
    email = checkEmail(users, numUser);
    password = checkPassword();
    securityQuestion = checkSQ();
    cout << securityQuestion << endl;
    cin.ignore();
    cout << "Enter your answer : ";
    getline(cin, questionAnswer);
    questionAnswer = deletespaces(questionAnswer);

    users[numUser++].setValue(fName, lName, email, password, securityQuestion, questionAnswer);
    createAccount(users[numUser-1]);
    
}
string checkEmail(User users[], int &numUser) {
    string email;

    while (true) {
        cout << "Enter your email: ";
        getline(cin, email);

        email = deletespaces(email);

        if (isValidEmail(email)) {
            bool emailExists = false;

            for (int i = 0; i < numUser; i++) {
                if (users[i].checkEmailUser(email)) {
                    cout << "\n-----This email is not available-----\nPlease enter another email\n";
                    emailExists = true;
                    break;
                }
            }

            if (!emailExists) {
                return email;
            }
        } else {
            cout << "\nInvalid email. Ensure it contains '@', a valid domain, and no invalid characters.\n";
        }
    }
}
bool isValidEmail(const string& email) {
    int atPosition = email.find('@');
    int dotPosition = email.find('.', atPosition + 1);
    
    if (atPosition == string::npos || atPosition == 0 || atPosition == email.length() - 1 || 
        dotPosition == string::npos || dotPosition == email.length() - 1) {
        return false;
    }
    
    for (char c : email) {
        if (!isalnum(c) && c != '@' && c != '.' && c != '_' && c != '-') {
            return false;
        }
    }

    return true;
}
string checkPassword()
{
    bool upperL, lowerL, num;
    upperL = lowerL = num = false;

    string password, confirmPassword;
    ;
    while (true)
    {
        cout << "Enter your password : ";
        getline(cin, password);
        password = deletespaces(password);
        if (password.length() >= 7)
        {
            for (int i = 0; i < password.length(); i++)
            {
                if (password[i] >= 'A' && password[i] <= 'Z')
                    upperL = true;
                if (password[i] >= 'a' && password[i] <= 'z')
                    lowerL = true;
                if (password[i] >= '0' && password[i] <= '9')
                    num = true;
            }
            if (upperL && lowerL && num)
            {
            confirm:
                cout << "Confirm password : ";
                cin >> confirmPassword;
                if (confirmPassword != password)
                {
                    cout << "invalid confirmation. try again\n";
                    goto confirm;
                }
                return password;
            }
        }
        cout << "Invalid password. It must contain (8) characters and (uppercases / lowercases / number)\n";
    }
}
string checkSQ()
{
    int option;
    const int size = 5;
    string securityQuestion;
    string questions[size] =
        {"What is your mother's maiden name?",
         "What is the name of your first pet?",
         "In what city were you born?",
         "What is your favorite book/movie/TV show?",
         "What is the name of your childhood best friend?"};
select:
    cout << "\n-----Please choose a question that you can use to recover your account if you forget your password-----\n";
    for (int i = 0; i < size; i++)
    {
        cout << i + 1 << "- " << questions[i] << endl;
    }
    cout << "Enter your option ";
    cin >> option;
    if (option > 0 && option <= 5)
    {
        securityQuestion = questions[option - 1];
        return securityQuestion;
    }
    else
    {
        cout << "--------------- Enter your number of question from 1 to 5 ---------------\n";
        goto select;
    }
}
void login(User users[], int &numUser)
{
    string email, password;
    cout << "Enter email : ";
    cin >> email;
    cout << "Enter password : ";
    cin >> password;

    ifstream inFile(file);
    bool log = false;
    for (int i = 0; i < numUser; i++)
    {
        if (users[i].checkLogin(email, password))
        {
            cout << "\n--------Login successful! Welcome---------\n";
            log = true;
            break;
        }
    }
    if (!log)
    {
        cout << "\nInvalid email or password. Please try again.\n"
             << endl;
    }
}
void forgetpassword(User users[], int &numUser)
{
    string email;
    cout << "Don't worrry, we will try to recover your password\n";
    cout << "Enter your email : ";
    cin.ignore();
    getline(cin, email);
    email = deletespaces(email);

    bool forget = false;
    for (int i = 0; i < numUser; i++)
    {
        if (users[i].checkEmailUser(email))
        {
            string answer;
            cout << users[i].getsecurityQuestion() << endl;
            cout << "Enter your anser : ";
            getline(cin,answer);
            answer = deletespaces(answer);
            if (answer == users[i].getquestionAnswer())
            {
                cout << "\n----------your password is ( " << users[i].getPassword() << " ) ----------\n"
                     << endl;
                forget = true;
                break;
            }
            else
            {
                cout << "-----Wrong anser!-----\nPlease try again later.\n";
                forget = true;
            }
        }
    }
    if (!forget)
    {
        cout << "----------Sorry, the email not found----------\n";
    }
}
string deletespaces(const string& str) {
    string result; 
    for (char c : str) {
        if (!isspace(c)) {
            result += c;
        }
    }
    return result;
}
void createAccount(User user)
{
    ofstream outfile(file, ios::app);
    if (outfile.is_open())
    {
        user.saveToFile(outfile);
        cout << "\n------------Account successfully created----------\n";
        outfile.close();
    }
    else
        cout << "\n-----Unable to open file for saving user data.------\n";
}
string getName(const string& prompt) {
    string name;
    while (true) {
        cout << prompt;
        getline(cin, name);
        name = deletespaces(name);
        if (!name.empty()) {
            return name;
        }
        cout << "Invalid input. Name cannot be empty. Please try again.\n";
    }
}