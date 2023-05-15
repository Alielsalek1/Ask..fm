#include <bits/stdc++.h>
using namespace std;

map<string, int> taken_users; //to make users unique
int curr_user_id; //set User-ID by counter
int curr_question_id; //set question-ID by counter
//input validators
//valid integer and check its limits
int valid_integer(int &n, int mn = INT_MIN, int mx = INT_MAX)
{
    while (true)
    {
        cin>>n;
        if(cin.fail() || n < mn || n > mx)
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input" << endl;
            continue;
        }
        else return n;
    }

}
//check if Username is valid credentials
string valid_username(string &s)
{
    while (true)
    {
        getline(cin, s);
        if (s == "" || s == "\n") continue;

        //cannot start with a digit and user is not taken
        if (s.empty() || isdigit(s[0]) || taken_users[s] ){
            cout << "Invalid input" << endl;
            continue;
        }

        //cannot contain spaces
        bool valid = true;
        for (char i : s){
            if (i == ' '){
                cout << "Invalid input" << endl;
                valid = false;
            }
        }
        if (valid)
        {
            taken_users[s]++;
            return s;
        }
    }
}
//check if password is digits only (will be updated later to be more secure)
string valid_password(string &s)
{
    //regex pattern for digits only
    regex pattern(R"(^[0-9]+$)");
    while (true)
    {
        getline(cin, s);
        if (s == "" || s == "\n" || !regex_match(s, pattern)){
            cout << "Invalid input" << endl;
            continue;
        }
        return s;
    }
}
//getline sometimes takes an empty space or an empty string, so we validate it
string validate_getline(string &s){
    while (s == "\n" || s.empty())
        getline(cin, s);
    return s;
}
//check if the given boolean expression is Valid
int valid_boolean_expression(int &x)
{
    cin >> x;
    while (x != 1 && x != 0)
    {
        cout << "Invalid input" << endl;
        cin >> x;
    }
    return x;
}
//check if the question ID is valid
int valid_question_id(int &x){
    cin >> x;
    while (x < 0 && x >= curr_question_id) {
        cout << "Please Enter a Valid ID: ";
        cin >> x; cout << endl;
    }
    return x;
}
//______________________________________________________________________________________________________________________
class user{
private:
    string name;
    string password{};
    int id{};
    bool anonymous{};
public:
    vector<int> from_questions;
    vector<int> to_questions;

    //sign up functions
    void enter_username(){
        cout << "Enter Username (No spaces): ";
        string temp_name;
        set_name(valid_username(temp_name)); cout << endl;
    }
    void enter_password(){
        cout << "Enter Your Password (digits only): ";
        string temp_password;
        set_pass(valid_password(temp_password)); cout << endl;
    }
    void check_anonymous_questions(){
        int temp_anonymous = 0;
        cout << "Allow Anonymous Questions (0 or 1): ";
        set_anonymous(valid_boolean_expression(temp_anonymous)); cout << endl;
    }
    //constructor AKA sign up
    user(){
        enter_username();
        enter_password();
        check_anonymous_questions();
        set_id(curr_user_id++);
    }
    //check if the entered username and password exists
    int valid_user(const string &username,const string &pass){
        if (username == name && pass == password) return id;
        return -1;
    }
    void print(){
        cout << "ID: " << get_id() << "         Name: " << get_name() << endl;
    }
    //setters
    void set_name(string x){
        name = x;
    }
    void set_pass(string x){
        password = x;
    }
    void set_anonymous(bool x){
        anonymous = x;
    }
    void set_id(int x){
        id = x;
    }
    //getters
    string get_name(){
        return name;
    }
    int get_id(){
        return id;
    }
    bool get_anonymous(){
        return anonymous;
    }
};
vector<user> users;
//parent class for common members and member functions for threads and question
//base class for threads and question
class post{
protected:
    int from{};
    int to{};
    bool anonymous{};
    string question_itself;
    string answer;
public:
    void anonymous_question_or_not(){
        if (!anonymous) cout << "From User ID (" << from << ") ";
    }
    void print_questions_itself(){
        cout << "to User ID (" << to << "):    Question: " << get_question_itself() << endl;
    }
    void print_answer(){
        string temp = get_answer();
        if (!temp.empty()) cout << "    Answer: " << temp << endl;
    }
    void print_question()
    {
        anonymous_question_or_not();
        print_questions_itself();
        print_answer();
    }
    //asking a question
    void adjust_from_to(const int &author_id){
        cout << "Enter User ID: ";
        int temp_to = 0;
        set_from(author_id);
        //check if the ID is valid
        set_to(valid_integer(temp_to, 0, curr_user_id)); cout << endl;
    }
    void allow_anonymous(const int &author_id){
        //check if user allows anonymous question
        if (users[author_id].get_anonymous()){
            cout << "Note: Anonymous Questions are not allowed for this user" << endl;
            set_anonymous(0);
        }
        else{
            cout << "Do You Want to ask Anonymously? (0 or 1)";
            int temp_anonymous = 0; cout << endl;
            set_anonymous(valid_boolean_expression(temp_anonymous));
        }
    }
    void ask_the_question(){
        cout << "Enter Your Question: ";
        string temp_question;
        getline(cin, temp_question);
        validate_getline(temp_question);
        set_question_itself(temp_question);
        cout << endl;
    }
    void ask(const int &author_id){
        adjust_from_to(author_id);
        allow_anonymous(author_id);
        ask_the_question();
    }
    //setters
    void set_to(int x){
        to = x;
    }
    void set_from(int x){
        from = x;
    }
    void set_anonymous(int x){
        anonymous = x;
    }
    void set_question_itself(string s){
        question_itself = s;
    }
    void set_answer(string s){
        answer = s;
    }
    //getters
    string get_answer(){
        return answer;
    }
    string get_question_itself(){
        return question_itself;
    }
    int get_from(){
        return from;
    }
    int get_to(){
        return to;
    }
};
class Thread: public post{
public:
    //constructor AKA ask a thread
    Thread(const int &author_id){
        ask(author_id);
    }
};
class question: public post{
private:
    int id{};
public:
    vector<Thread> threads;
    //constructor AKA ask a question
    void adjust_from_to_User(){
        users[get_from()].from_questions.push_back(curr_question_id);
        users[get_to()].to_questions.push_back(curr_question_id);
    }
    question(const int &author_id){
        ask(author_id);
        adjust_from_to_User();
        set_id(curr_question_id++);
    }
    //print all threads in a vector
    void print_threads(){
        for (auto threadd : threads){
            cout << "Thread Question: ";
            threadd.print_question();
        }
    }
    //Feed
    void print_feed()
    {
        cout << "Question ID (" << id << ") ";
        print_question();
        print_threads();
    }
    //setters
    void set_id(int x){
        id = x;
    }
};
vector<question> questions;
//______________________________________________________________________________________________________________________
//menus
int menu1()
{
    int choice = 0;
    cout << "Menu: \n"
            "   1: Login\n"
            "   2: Sign Up\n"
            "   3: Exit\n"
            "Choose a Number from 1 - 3";
    cout << endl;

    //input validation
    choice = valid_integer(choice, 1, 3);
    return choice;
}
int menu2()
{
    int choice = 0;
    cout << "Menu: \n"
            "   1: Print Questions to me\n"
            "   2: Print Questions from me\n"
            "   3: Answer a Question\n"
            "   4: Ask a Question\n"
            "   5: List System Users\n"
            "   6: Feed\n"
            "   7: Ask a Thread\n"
            "   8: Answer a Thread\n"
            "   9: Logout\n"
            "Choose a number from 1 - 9";
    cout << endl;

    //input validation
    choice = valid_integer(choice, 1, 9);
    return choice;
}
//Helper Functions
//get parent question for thread question
int get_parent_question()
{
    cout << "Enter Question ID: ";
    int parent_id = -1;
    valid_question_id(parent_id);
    return parent_id;
}
//validate answer of a question or a thread
void get_answer(string &s)
{
    cout << "Enter your Answer: ";
    validate_getline(s); cout << endl;
}
int choose_thread_for_answer(const int &size){
    cout << "Which Thread would you like to answer from 0 - " << size - 1 << "\n";
    int choice = -1;
    return valid_integer(choice, 0, size - 1);
}
//main functions
//login returns user id or -1 to go back to first menu
int login()
{
    while (true)
    {
        string username, password;
        cout << "Enter Username and Password: ";
        cin >> username >> password;
        for (auto user : users)
        {
            int curr = user.valid_user(username, password);
            if (curr > -1) return curr;
        }
        cout << "Invalid Input, if you wish to try again press 1 or press 0 to go back to main menu" << endl;
        int choice = -1;
        int actual_choice = valid_boolean_expression(choice); cout << endl;
        if (actual_choice == 0) return -1;
    }
}
void sign_up()
{
    user person;
    users.push_back(person);
}
void list_system_users()
{
    for (auto user : users)
        user.print();
}
void feed()
{
    for (auto question : questions)
        question.print_feed();
}
void print_questions_to_me(const int &author_id)
{
    for (auto question_id : users[author_id].to_questions){
        questions[question_id].print_feed();
    }
}
void print_questions_from_me(const int &author_id){
    for (auto question_id : users[author_id].from_questions){
        questions[question_id].print_feed();
    }
}
void answer_a_question()
{
    //if no questions
    if (curr_question_id == 0)
    {
        cout << "No Questions to answer\n";
        return;
    }
    int question_id = get_parent_question();

    string temp_ans;
    get_answer(temp_ans);
    questions[question_id].set_answer(temp_ans);
}
void answer_a_thread()
{
    int question_id = get_parent_question(), size = (int)questions[question_id].threads.size();
    questions[question_id].print_threads();

    //check if parent question has threads already
    if (questions[question_id].threads.empty()){
        cout << "Question has no Threads" << endl;
        return;
    }
    int choice = choose_thread_for_answer(size);

    //answer
    string temp_ans;
    get_answer(temp_ans);
    questions[question_id].threads[choice].set_answer(temp_ans);
}
void ask_fm()
{
    while (true)
    {
        int choice = menu1();
        //login
        if (choice == 1)
        {
            int user_id = login();
            if (user_id == -1) continue; //Go back to Main Menu
            while (true)
            {
                int choice2 = menu2();
                if (choice2 == 1){
                    print_questions_to_me(user_id);
                }
                if (choice2 == 2){
                    print_questions_from_me(user_id);
                }
                if (choice2 == 3){
                    answer_a_question();
                }
                if (choice2 == 4){
                    questions.emplace_back(user_id);
                }
                if (choice2 == 5){
                    list_system_users();
                }
                if (choice2 == 6){
                    feed();
                }
                if (choice2 == 7){
                    int parent = get_parent_question();
                    questions[parent].threads.emplace_back(Thread(user_id));
                }
                if (choice2 == 8){
                    answer_a_thread();
                }
                if (choice2 == 9){
                    break;
                }
            }
        }
        //sign up
        else if (choice == 2){
            sign_up();
        }
        //Exit
        else{
            exit(1);
        }
    }
}
int main() {
    ask_fm();
}
