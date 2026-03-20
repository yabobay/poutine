#include <iostream>
#include <fstream>
#include <vector>

#include "task.hpp"

using namespace std;

int main() {
    string tasks_filename = getenv("HOME");
    tasks_filename += "/.poutine";
    ifstream tasks_file(tasks_filename);
    if (tasks_file.fail()) {
        cout << "Couldn't read poutine file at `" << tasks_filename << "`." << endl;
        // TODO: add instructions for writing one?
        return 0;
    }

    vector<task> tasks;
    string line;
    while (getline(tasks_file, line)) {
        auto t = task::parse(line);
        if (t.has_value())
            tasks.push_back(*t);
    }

    auto today = boost::gregorian::day_clock::local_day().day_of_week();
    auto today_int = today.as_number() - 1;
    cout << "Today is " << today.as_long_string() << endl;

    for (int i = 0; i < tasks.size(); i++) {
        const auto& task = tasks[i];
        if (!task.today(today))
            continue;
        cout << ' ' << (i+1) << ". " << task.name << ' ';
        if (task.is_time_based())
            cout << task::timestamp(task.days[today_int]);
        else
            cout << task.days[today_int];
        if (task.noun.has_value())
            cout << ' ' << *task.noun;
        cout << endl;
    }
}
