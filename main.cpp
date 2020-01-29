#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

struct Year {
    int year;

    explicit Year(int y) {
        year = y;
    }
};

struct Month {
    int month;

    explicit Month(int m) {
        month = m;
    }
};

struct Day {
    int day;

    explicit Day(int d) {
        day = d;
    }
};

class Date {
public:

    Date (Year y = Year(1), Month m = Month(1), Day d = Day(1)) {
        if (m.month < 1 || m.month > 12) {
            throw runtime_error("Month value is invalid: " + to_string(m.month));
        }

        if (d.day < 1 || d.day > 31) {
            throw runtime_error("Day value is invalid: " + to_string(d.day));
        }

        year  = y.year;
        month = m.month;
        day   = d.day;

    }
    int GetYear() const {
        return year;
    };
    int GetMonth() const {
        return month;
    };
    int GetDay() const {
        return day;
    };

private:
    int year;
    int month;
    int day;
};


istream& operator >> (istream& stream, Date& date) {
    string strdate;

    stream >> strdate;

    stringstream new_stream;

    new_stream << strdate;

    int d = 0, m = 0, y = 0;
    char c1, c2;

    new_stream >> y >> c1 >> m >> c2 >> d;
    if (!new_stream                ||
        c1 != '-' || c2 != '-' ||
        new_stream.peek() != EOF) {
        throw runtime_error("Wrong date format: " + strdate);
    }

    date = {Year(y), Month(m), Day(d)};

    return stream;
}


ostream& operator << (ostream& stream, const Date& date) {
    stream << setw(4) << setfill('0') << date.GetYear()  << '-'
           << setw(2) << setfill('0') << date.GetMonth() << '-'
           << setw(2) << setfill('0') << date.GetDay();

    return stream;
}

bool operator == (const Date& d1, const Date& d2) {

    return d1.GetYear()  == d2.GetYear()  &&
           d1.GetMonth() == d2.GetMonth() &&
           d1.GetDay()   == d2.GetDay();
}

bool operator < (const Date& d1, const Date& d2) {
    int year1 = d1.GetYear();
    int year2 = d2.GetYear();

    int month1 = d1.GetMonth();
    int month2 = d2.GetMonth();

    int day1 = d1.GetDay();
    int day2 = d2.GetDay();

    if (year1 == year2) {
        if (month1 == month2) {
            return day1 < day2;
        } else {
            return month1 < month2;
        }
    }
    return year1 < year2;
}

bool operator<(const Date& lhs, const Date& rhs);

class Database {
public:
    void AddEvent(const Date &date, const string &event) {
        dataBase[date].insert(event);
    }

    bool DeleteEvent(const Date &date, const string &event) {
        if (dataBase.count(date) != 0 && dataBase[date].count(event) != 0) {
            dataBase[date].erase(event);
            return true;
        }
        return false;
    }

    int DeleteDate(const Date &date) {
        int numEvents = 0;
        if (dataBase.count(date) != 0) {
            numEvents = dataBase[date].size();
            dataBase.erase(date);
        }
        return numEvents;

    }

    set<string> Find(const Date& date) const {
        if (dataBase.count(date) != 0) {
            return dataBase.at(date);
        }

        return {};
    }

    void Print() const {
        for (const auto& m : dataBase) {
            for(const auto& s : m.second) {
                cout << m.first << ' ' << s << endl;
            }
        }
    }

private:
    map<Date, set<string>> dataBase;
};

int main() {
    Database db;

    string command;
    try {
        while (getline(cin, command)) {

            if (command.empty()) {
                continue;
            }

            stringstream stream;
            stream << command;

            Date date;
            string event;
            stream >> command;

            if (command == "Add" ||
                command == "Del" ||
                command == "Find") {
                stream >> date >> event;

                if (command == "Add") {

                    db.AddEvent(date, event);

                } else if (command == "Del") {

                    if (event.empty()) {
                        cout << "Deleted "
                             << db.DeleteDate(date)
                             << " events" << endl;

                    } else {

                        if (db.DeleteEvent(date, event)) {
                            cout << "Deleted successfully" << endl;
                        } else {
                            cout << "Event not found" << endl;
                        }
                    }

                } else if (command == "Find") {

                    set<string> events = db.Find(date);

                    for (const auto &ev : events) {
                        cout << ev << endl;
                    }
                }
            } else if (command == "Print") {
                db.Print();
            } else {
                throw runtime_error("Unknown command: " + command);
            }
        }
    }
    catch (runtime_error& err) {
        cout << err.what();
    }

    return 0;
}