#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <assert.h>
using namespace std;

void Reverse(vector<int>& v) {
    for (size_t i = 0; i < v.size() / 2; ++i) {
        swap(v[i], v[v.size() - i - 1]);
    }
}

#define newCmd(cmd) {#cmd, cmd}

enum cmdsNum {
    ADD,
    DUMP,
    NEXT
};


struct ToDoList {
private:
    vector<vector<string>> toDoList;
    vector<size_t> numbOfDates;
    size_t currMonth;


    void resizeToDoList(size_t month) {
        toDoList.resize(numbOfDates[month]);
    }
    size_t getNextMonth() {
        return (currMonth + 1) % 12;
    }



public:
    ToDoList() {
        currMonth = 0;
        numbOfDates.assign(12, 31);
        numbOfDates[1] = 28;
        for(int i = 3; i < 12; i+=2) {
            numbOfDates[i] = 30;
        }
        resizeToDoList(currMonth);
    }

    void nextMonth() {
        size_t nxtMnt = getNextMonth();
        for (int i = numbOfDates[nxtMnt], j = i - 1; i < numbOfDates[currMonth]; ++i) {
             toDoList[j].insert(end(toDoList[j]), begin(toDoList[i]), end(toDoList[i]));
        }
        currMonth = nxtMnt;
        resizeToDoList(currMonth);
    }

    void addTask(size_t date, string task) {
        assert(date > 0);
        assert(date <= toDoList.size());
        toDoList[date - 1].push_back(task);
    }

    void dumpTasks(size_t date) {
        assert(date > 0);
        size_t inxDate = date - 1;
        assert(inxDate < toDoList.size());

        cout << toDoList[inxDate].size() << " ";
        for (const auto& task : toDoList[inxDate]) {
            cout << task << " ";
        }
        cout << endl;
    }
};

int main() {
    int Q = 0;
    cin >> Q;

    map<string, size_t> cmds = {newCmd(ADD),
                                newCmd(DUMP),
                                newCmd(NEXT)};

    string  currCmd;
    size_t date;
    string task;
    ToDoList tdlist;
    while (Q--) {
        cin >> currCmd;

        switch (cmds[currCmd]) {
            case ADD:
                cin >> date >> task;
                tdlist.addTask(date, task);
                break;
            case DUMP:
                cin >> date;
                tdlist.dumpTasks(date);
            case NEXT:
                tdlist.nextMonth();
            default :
                break;
        }

    }
}