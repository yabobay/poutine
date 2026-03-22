#include <iostream>
#include <regex>
#include <array>
#include <charconv>

#include "task.hpp"

using namespace std;

const array<string, 7> days({"sun", "mon", "tue", "wed", "thu", "fri", "sat"});
const vector<pair<int, char>> time_units({ {3600, 'h'}, {60, 'm'}, {1, 's'} });

int day_number(const string_view& day_) {
    string_view day(day_);
    auto space = day.find_first_of(" ");
    if (day.size() > space)
        day.remove_suffix(day.size() - space);
    for (int i = 0; i < days.size(); i++) {
        if (day == days[i])
            return i;
    }
    return -1;
}

unsigned short parse_timestamp(const string_view& timestamp) {
    if (!isdigit(timestamp[0]))
        return 0;
    int time = 0, n = 0;
    for (const char c : timestamp) {
        for (const auto& i : time_units) {
            if (c == i.second) {
                time += n * i.first;
                n = 0;
                goto next;
            }
        }
        if (n > 0) n *= 10;
        n += c - '0';
    next:;
    }
    return time;
}

vector<pair<string_view, int>> split(const string_view& s, bool keep_spaces=false) {
    vector<pair<string_view, int>> v;
    const auto len = s.length();
    int word_start = 0;
    for (int i = 0; i < len; i++) {
        if (isspace(s[i])) {
            auto word_length = i-word_start;
            if (keep_spaces) ++word_length;
            if (word_length)
                v.push_back({string_view(s.data() + word_start, word_length), word_start});
            word_start = i + 1;
        }
    }
    v.push_back({string_view(s.data() + word_start, len-word_start), word_start});
    return v;
}

void trim_end(string& s) {
    if (!isspace(s.back()))
        return;
    for (auto i = s.end()-1; i >= s.begin(); i--) {
        if (!isspace(*i)) {
            s.erase(i+1, s.end());
            break;
        }
    }
}

// 🍝🍝🍝
optional<task> task::parse(const string& line) {
    task t;
    if (line.front() == '#')
        return {};

    static const regex day { "mon|tue|wed|thu|fri|sat|sun" };
    smatch match;
    bool day_found = regex_search(line, match, day);

    if (!day_found) {
        unsigned short n = true;

        auto words = split(line, true);

        auto timestamp = parse_timestamp(words.back().first);
        if (timestamp) {
            n = timestamp;
            t.name.assign(line, 0, words.back().second);
            t.noun.emplace("");
        } else {
            for (auto i = words.rbegin() + 1; i < words.rend(); i++) {
                if (isdigit(i->first.front())) {
                    t.name.assign(line, 0, i->second);
                    from_chars(i->first.begin(), i->first.end(), n);
                    auto x = i->second + i->first.size();
                    t.noun.emplace(line, x, line.length() - x);
                    break;
                }
            }
        }

        for (auto& i : t.days)
            i = n;
        trim_end(t.name);
        return t;
    }

    t.name = match.prefix();

    bool all_same = false;
    int n = -1;

    string_view rest(line.data()+match.position(), line.size()-match.position());
    auto words = split(rest, true);
    for (auto i = words.begin(); i < words.end(); i++) {
        auto& token = i->first;
        int day_num = day_number(token);
        if (day_num != -1) {
        back:
            ++i;
            auto next_day_num = day_number(i->first);
            if (next_day_num != -1) {
                all_same = true;
                t.days[day_num] = true;
                day_num = next_day_num;
                goto back;
            } else if (all_of(i->first.begin(), i->first.end(), [] (auto i) { return isdigit(i) || isspace(i); })) {
                from_chars(i->first.begin(), i->first.end(), t.days[day_num]);
                n = t.days[day_num];
            } else if (isdigit(i->first[0]))
               n = t.days[day_num] = parse_timestamp(i->first);
            else return {};
        } else {
            if (!t.noun.has_value())
                t.noun.emplace(token);
            else
                t.noun->append(token);
        }
    }

    if (all_same)
        for (auto& i : t.days)
            if (i)
                i = n;
    trim_end(t.name);

    return t;
}

string task::timestamp(unsigned short n) {
    string r;
    for (const auto& u : time_units) {
        if (n > u.first) {
            r += to_string(n / u.first);
            r += u.second;
            n %= u.first;
        }
    }
    return r;
}

void task::print() const {
    cout << "------" << endl;
    cout << "name: \"" << name << '"' << endl;
    cout << "days: ";
    for (auto i : days)
        cout << '[' << i << ']';
    cout << endl;
    if (noun)
        cout << "noun: \"" << *noun << '"' << endl;
}

bool task::is_today(boost::gregorian::greg_weekday day) const {
    return days[day.as_number()];
}

bool task::is_time_based() const {
    return noun.has_value() && !noun->size();
}
