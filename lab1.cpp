#include <iostream>
#include <string>
#include <set>
#include <map>
#include <tuple>
#include <vector>
#include <algorithm>

using namespace std;

int STUDENT_ID = 1;
int GROUP_ID = 1;

//Class for description student
class Student
{
private:
    string name_;

public:
    //Constructors
    Student() : name_("New student " + to_string(STUDENT_ID++)) {};

    Student(const string& name) : name_(name) { STUDENT_ID++; };

    //Method that return name of student
    string GetName() const
    {
        return name_;
    }
};

//Class for description different groups
class Group
{
private:
    string name_;
    vector<Student> students_;
public:
    //Constructors
    Group() : name_("NULL") {};
    Group(const string& name) : name_(name) { GROUP_ID++; };
    Group(const vector<Student>& students) : name_("New group " + to_string(GROUP_ID++)), students_(students) {};
    Group(const string& name, const vector<Student>& students) : name_(name), students_(students) { GROUP_ID++; };

    //Method for setting list of students
    void SetStudents(const vector<Student>& students)
    {
        this->students_ = students;
    }

    //Method for adding new students
    void AddStudents(const Student& student)
    {
        students_.insert(students_.end(), student);
    }

    //Method for adding list of students
    void AddStudents(const vector<Student>& students)
    {
        students_.insert(students_.end(), students.begin(), students.end());
    }

    //Method for getting name of group
    string GetName() const
    {
        return name_;
    }

    //Method for getting list of students
    vector<Student> GetStudents() const
    {
        return students_;
    }
};

//Structure for description parameters of hashing
struct Params
{
    int m;
    int a;
    int b;
    int p;
};

//overloads of operator for output different classes
ostream& operator<<(ostream& os, const Params& p)
{
    os << "p: " << p.p << ", m: " << p.m << ", a: " << p.a << ", b: " << p.b;
    return os;
}

ostream& operator<<(ostream& os, const Student& student)
{
    os << "Student: " << student.GetName();
    return os;
}

template<class K, class V>
ostream& operator<<(ostream& os, const pair<K, V> p)
{
    os << "([" << p.first << "]: " << p.second << ")\n";
    return os;
}

template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    os << "{";
    bool first = true;
    for (auto it = v.begin(); it != v.end(); it++)
    {

        if (!first)
            os << ", ";
        first = false;
        os << *it;
    }
    os << "}";
    return os;
}

ostream& operator<< (ostream& os, const Group& m)
{
    os << "Group: " << m.GetName() << "(" << m.GetStudents() << ")";
    return os;
}

//Function for finding next simple number
int NextSimple(int number)
{
    number++;
    if (number == 1 || number == 2)
        return number;
    if (number % 2 == 0)
        return NextSimple(number);
    for (int i = 3; i * i <= number; i += 2)
        if (number % i == 0)
            return NextSimple(number);
    return number;
}

//Function for hashing strings
int Hash(const string& t)
{
    int res = 0;
    for (int i = 0; i < static_cast<int>(t.size()); i++)
        res += i * t[i];
    return res;
}

//Function for hashing group
int HashGroup(const Group& m)
{
    int res = 0;
    res += Hash(m.GetName());
    for (auto i : m.GetStudents())
    {
        res += Hash(i.GetName());
    }
    return res;
}

//Function that returns hash-table
pair<Params, vector<pair<Params, vector<Group>>>> IdealHash(const vector<Group>& v)
{
    int p = NextSimple(HashGroup(*v.rbegin()));
    int m = static_cast<int>(v.size());
    vector<pair<Params, vector<Group>>> res(m);
    int a = rand() % p;
    int b = rand() % p;
    for (auto i : v)
    {
        int h = ((a * HashGroup(i) + b) % p) % m;
        res[h].second.push_back(i);
    }
    Params p1;
    p1.p = p;
    p1.a = a;
    p1.b = b;
    p1.m = m;
    for (size_t i = 0; i < res.size(); i++)
    {
        if (res[i].second.size() > 1)
        {
            int m2 = static_cast<int>(res[i].second.size());
            m2 = m2 * m2;
            while (true)
            {
                bool cont = true;
                set<int> diff;
                vector<Group> temp(m2);
                Params p2;
                p2.p = p;
                p2.m = m2;
                p2.a = rand() % p;
                p2.b = rand() % p;
                for (auto j : res[i].second)
                {
                    int h = ((p2.a * HashGroup(j) + p2.b) % p) % m2;
                    auto in = diff.insert(h);
                    temp[h] = j;
                    cont = cont && in.second;
                }
                if (cont)
                {
                    res[i] = make_pair(p2, temp);
                    break;
                }
                else continue;
            }
        }
        else
        {
            res[i].first.m = static_cast<int>(res[i].second.size());
        }
    }
    return make_pair(p1, res);
}

pair<Params, vector<pair<Params, vector<int>>>> IdealHash(const vector<int>& v)
{
    int p = NextSimple(*v.rbegin());
    int m = static_cast<int>(v.size());
    vector<pair<Params, vector<int>>> res(m);
    int a = rand() % p;
    int b = rand() % p;
    for (auto i : v)
    {
        int h = ((a * i + b) % p) % m;
        res[h].second.push_back(i);
    }
    Params p1;
    p1.p = p;
    p1.a = a;
    p1.b = b;
    p1.m = m;
    for (size_t i = 0; i < res.size(); i++)
    {
        if (res[i].second.size() > 1)
        {
            int m2 = static_cast<int>(res[i].second.size());
            m2 = m2 * m2;
            while (true)
            {
                bool cont = true;
                set<int> diff;
                vector<int> temp(m2);
                Params p2;
                p2.p = p;
                p2.m = m2;
                p2.a = rand() % p;
                p2.b = rand() % p;
                for (auto j : res[i].second)
                {
                    int h = ((p2.a * j + p2.b) % p) % m2;
                    auto in = diff.insert(h);
                    temp[h] = j;
                    cont = cont && in.second;
                }
                if (cont)
                {
                    res[i] = make_pair(p2, temp);
                    break;
                }
                else continue;
            }
        }
        else
        {
            res[i].first.m = static_cast<int>(res[i].second.size());
        }
    }
    return make_pair(p1, res);
}

int main()
{
    Group g1("group1");
    Group g2("group2");
    Group g3("group3");
    Group g4("group4");
    Group g5("group5");
    Group g6("group6");
    Group g7("group7");
    cout << IdealHash({ g1,g2,g3,g4,g5,g6, g7}) << endl;
    cout << endl << endl;
    cout << IdealHash({ 10,15,23,46,57,83 }) << endl;
    return 0;
}