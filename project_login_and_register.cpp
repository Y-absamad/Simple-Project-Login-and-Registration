#include <iostream>
#include <fstream>
#include<string>
using namespace std;

class User;
const int MaX_Users = 100;
const string file = "data.txt";

void readFromFileToArray(User[], int&);
/*
    the frist function to be called in int main().
    this function reads information frome the file and adds it to arraye.
*/

void Registation(User[], int&);
/*
    This function takes the new user's information,
    adds it to the array, -------> setValue(information)
    and then saves it to a file.-------> saveToFile()----> in class User;
*/
string checkEmail(User[], int&);
/*
    This function is used to verify that the email is valid and contains an @ symbol.
    Then make sure that the email is not already in use. -----> checkEmailUser() ---> in class User
*/
string checkPassword();
/*
    This function is used to ensure that the password is valid
    and contains uppercase letters, lowercase letters, and numbers.
*/
string checkSQ();
/*
    This function is used to choose a security question
    that is used to recover the account in case you forget the password
*/
void login(User[], int&);
/*
    This function used that the login data is correct.----> checkLogin() ---> in class User
*/
void forgetpassword(User[], int&);
/*
    This function is used to retrieve the password via email
    and your security question.----> checkSQ();
*/


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

    static User loadFromFile(ifstream& file)
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

    void saveToFile(ofstream& file)
    {
        file << fName << "\n"
            << lName << "\n"
            << email << "\n"
            << password << "\n"
            << securityQuestion << "\n"
            << questionAnswer << endl;
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

    bool c = true;
    while (c)
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
            c = false;
            break;
        default:
            cout << "--------------- Enter your option 1 to 4 ---------------\n";
            break;
        }
    }
    return 0;
}


void readFromFileToArray(User users[], int& numUser)
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
void Registation(User users[], int& numUser)
{
    string fName, lName, email, password, securityQuestion, questionAnswer;
    cout << "-----Please enter information to create an account-----" << endl
        << "----------    Don't use spaces    ----------\n";

    cout << "Enter your first name : ";
    cin >> fName;
    cout << "Enter your last name : ";
    cin >> lName;
    email = checkEmail(users, numUser);
    password = checkPassword();
    securityQuestion = checkSQ();
    cout << securityQuestion << endl;
    cout << "Enter your answer : ";
    cin >> questionAnswer;

    users[numUser++].setValue(fName, lName, email, password, securityQuestion, questionAnswer);

    ofstream outfile(file, ios::app);
    if (outfile.is_open())
    {
        users[numUser - 1].saveToFile(outfile);
        cout << "------------Account successfully created----------\n";
        outfile.close();
    }
    else
        cout << "-----Unable to open file for saving user data.------\n";
}
string checkEmail(User users[], int& numUser)
{
    string email;
    while (true)
    {
    check:
        cout << "Enter your email : ";
        cin >> email;
        int ch_ar = email.find('@');
        if (ch_ar != string::npos && ch_ar != 0 && ch_ar != email.length() - 1)
        {
            for (int i = 0; i < numUser; i++)
            {
                if (users[i].checkEmailUser(email))
                {
                    cout << "-----This email is not available-----\nPlease enter another email\n";
                    goto check;
                }
            }
            return email;
        }
        cout << "Invalid email. It must contain the '@' symbol\n";
    }
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
        cin >> password;
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
    { "What is your mother's maiden name?",
     "What is the name of your first pet?",
     "In what city were you born?",
     "What is your favorite book/movie/TV show?",
     "What is the name of your childhood best friend?" };
select:
    cout << "-----Please choose a question that you can use to recover your account if you forget your password-----\n";
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
void login(User users[], int& numUser)
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
            cout << "--------Login successful! Welcome---------\n";
            log = true;
            break;
        }
    }
    if (!log)
    {
        cout << "Invalid email or password. Please try again." << endl;
    }
}
void forgetpassword(User users[], int& numUser)
{
    string email;
    cout << "Don't worrry, we will try to recover your password\n";
    cout << "Enter your email : ";
    cin >> email;

    bool forget = false;
    for (int i = 0; i < numUser; i++)
    {
        if (users[i].checkEmailUser(email))
        {
            string answer;
            cout << users[i].getsecurityQuestion() << endl;
            cout << "Enter your anser : ";
            cin >> answer;
            if (answer == users[i].getquestionAnswer())
            {
                cout << "----------your password is ( " << users[i].getPassword() << " ) ----------" << endl;
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