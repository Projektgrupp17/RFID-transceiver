#include "interpreter.hpp"
#include "util.hpp"
#include <stdexcept>
#include <iostream>
#include <utility>

interpreter::interpreter() {};

interpreter::interpreter(std::string p_name, std::string p_pattern) {
    pattern[p_name] = std::regex(p_pattern);
}

interpreter::interpreter(std::map<std::string, std::string> name_pattern) {
    for (std::map<std::string, std::string>::value_type& entry : name_pattern) {
        pattern[entry.first] = std::regex(entry.second);
    }
}

interpreter::interpreter(std::vector<std::string> name_pattern) {
    if ((int)name_pattern.size() % 2) {
        throw std::invalid_argument("Invalid argument. Non-matching name patterns.");
    }
    
    for (int i = 0; i < (int)name_pattern.size(); i += 2) {
 //       std::cout << "Setting key " << name_pattern[i] << " val " << name_pattern[i + 1] << std::endl;
        pattern[name_pattern[i]] = std::regex(name_pattern[i + 1]);
 //       std::cout << "Setting type " << typeid(pattern[name_pattern[i]]).name() << std::endl;
    }
}

bool interpreter::test(std::string input, std::string regex, int group, bool retain) {
    if (!pattern.count(regex)) {
 //       std::cout << "throw " << std::endl;
        throw std::invalid_argument("Invalid argument; non-existing pattern.");
    }
    
    std::map<std::string, std::regex>::iterator iter = pattern.find(regex);
    bool status = std::regex_search(input, iter->second);
    std::regex str = iter->second;
    std::regex tesss("^abc$");
    std::string test = "";
//    std::cout << "key " << iter->first.c_str() << " val " << std::regex_search(input, str) << " " << typeid(iter->second).name() << std::endl;
//    std::cout << "type " << typeid(tesss).name() << std::endl;

    if (retain && status) {
        std::smatch match;
        std::regex_search(input, match, iter->second);
        l_behind.push_back(match[group]);
    }

    return status;
}

bool interpreter::test(std::string input, std::regex regex) {
    return std::regex_search(input, regex);
}

bool interpreter::eval(std::string input, std::string &match_str, std::string regex, int group, bool retain) {
    std::smatch match;
    bool status = false;

    if (!pattern.count(regex)) {
        throw std::invalid_argument("Invalid argument; non-existing pattern.");
    }

    std::map<std::string, std::regex>::iterator iter = pattern.find(regex);

    status = std::regex_search(input, match, iter->second);

    if (!(group < (int)match.size())) {
        throw std::out_of_range("Specified group is out of range; non-existing group.");
    }

    if (status) {
        match_str = match[group];

        if (retain) {
            l_behind.push_back(match_str);
        }
    }
    
    return status;
}

bool interpreter::eval(std::string input, std::string &match_str, std::regex regex, int group) {
    std::smatch match;

    if (std::regex_search(input, match, regex)) {
        if (!(group < (int)match.size())) {
            throw std::out_of_range("Specified group is out of range; non-existing group.");
        }

        match_str = match[group];
        return true;
    }
    
    return false;
}

bool interpreter::eval(std::string input, std::vector<std::string> &match_str, std::string regex, int group, bool retain) {
    std::smatch match;
    bool status = false;

    if (!pattern.count(regex)) {
        throw std::invalid_argument("Invalid argument; non-existing pattern.");
    }

    std::map<std::string, std::regex>::iterator iter = pattern.find(regex);

    status = std::regex_search(input, match, iter->second);

    if (status) {
        for (int i = 0; i < (int)match.size(); i++) {
            match_str.push_back(match[i]);
        }

        if (retain) {
            l_behind.push_back(match_str[group]);
        }
    }
    
    return status;
}

bool interpreter::eval(std::string input, std::vector<std::string> &match_str, std::regex regex) {
    std::smatch match;

    if (std::regex_search(input, match, regex)) {
        for (int i = 0; i < (int)match.size(); i++) {
            match_str.push_back(match[i]);
        }

        return true;
    }
    
    return false;
}

std::string interpreter::look_behind() {
    if (!l_behind.size()) {
        throw std::logic_error("No existing look behind match.");
    }

    return l_behind[l_behind.size() - 1];
}

std::string interpreter::look_behind(int hops) {
    if (!l_behind.size() || !(hops <= (int)l_behind.size()) || hops < 1) {
        throw std::logic_error("No existing look behind match or look behind match out of range." + std::to_string(hops));
    }

    std::vector<std::string>::iterator iter = l_behind.end();

    for (int i = 0; i < hops; i++) {
        iter--;
    }

    return *iter;
}

bool interpreter::look_behind_eval(std::string input, std::string name_replaced, std::string name_to_replace, int hops) {
    if (name_replaced == "" && name_to_replace != "") {
        throw std::invalid_argument("Invalid argument; can't replace nothing with something.");
    }

    bool status = false;
    std::string new_name;
    util::replace(look_behind(hops), new_name, name_replaced, name_to_replace);

    if (!pattern.count(new_name)) {
        throw std::logic_error("Unable to evaluate expression; non-existing pattern.");
    }

    if ((status = std::regex_match(input, pattern.find(new_name)->second))) {
        // look behind
    }

    return status;
}

void interpreter::flush() {
    l_behind.clear();
}

void interpreter::flush_last(int no) {
    if (!(no < (int)l_behind.size())) {
        throw std::logic_error("No existing look behind match.");
    }
    std::vector<std::string>::iterator iter = l_behind.end();

    for (int i = 0; i < no; i++) {
        iter--;
        l_behind.erase(iter);
    }
}

bool interpreter::has_pattern(std::string name) {
    return pattern.count(name) ? true : false;
}

bool interpreter::has_pattern(std::vector<std::string> name) {
    for (int i = 0; i < (int)name.size(); i++) {
        if (!pattern.count(name[i])) {
            return false;
        }
    }

    return true;
}

void interpreter::remove_pattern(std::string name, bool force) {
    if (!force && !pattern.count(name)) {
        throw std::invalid_argument("No pattern with the name " + name + " exists.");
    }

    if (pattern.count(name)) {
        std::map<std::string, std::regex>::iterator iter = pattern.find(name);
        pattern.erase(iter);
    }
    
}

void interpreter::remove_pattern(std::vector<std::string> name, bool force) {
    if (!force) {
        for (int i = 0; i < (int)name.size(); i++) {
            if (!pattern.count(name[i])) {
                throw std::invalid_argument("No pattern with the name " + name[i] + " exists.");
            }
        }
    }

    for (int i = 0; i < (int)name.size(); i++) {
        if (pattern.count(name[i])) {
            std::map<std::string, std::regex>::iterator iter = pattern.find(name[i]);
            pattern.erase(iter);
        }
    } 
}

void interpreter::set_pattern(std::string name, std::string pat) {
    pattern[name] = std::regex(pat);
}

void interpreter::set_pattern(std::map<std::string, std::string> name_pattern_pairs) {
    for (std::map<std::string, std::string>::value_type& entry : name_pattern_pairs) {
        pattern[entry.first] = std::regex(entry.second);
    }
}

void interpreter::set_pattern(std::vector<std::string> name_pattern_pairs) {
    if ((int)name_pattern_pairs.size() % 2) {
        throw std::invalid_argument("Invalid argument. Non-matching name patterns.");
    }

    for (int i = 0; i < (int)name_pattern_pairs.size(); i += 2) {
        pattern[name_pattern_pairs[i]] = std::regex(name_pattern_pairs[i + 1]);
    }
}