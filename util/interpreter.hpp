#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <string>
#include <vector>
#include <regex>
#include <map>

class interpreter {
    private:
        std::map<std::string, std::regex> pattern;
        std::vector<std::string> l_behind;

    public:
        interpreter();
        interpreter(std::string name, std::string pattern);
        interpreter(std::map<std::string, std::string> name_pattern_pairs);
        interpreter(std::vector<std::string> name_pattern_pairs);
        bool test(std::string input, std::string regex, int group = 0, bool retain = false);
        bool test(std::string input, std::regex regex);
        bool eval(std::string input, std::string &match_str, std::string regex, int group = 0, bool retain = false);
        bool eval(std::string input, std::string &match_str, std::regex regex, int group = 0);
        bool eval(std::string input, std::vector<std::string> &match_str, std::string regex, int group = 0, bool retain = false);
        bool eval(std::string input, std::vector<std::string> &match_str, std::regex regex);
        std::string look_behind();
        std::string look_behind(int hops);
        bool look_behind_eval(std::string input, std::string name_replaced = "", std::string name_to_replace = "", int hops = 0);
        void flush();
        void flush_last(int no);
        bool has_pattern(std::string name);
        bool has_pattern(std::vector<std::string> name);
        void remove_pattern(std::string name, bool force = true);
        void remove_pattern(std::vector<std::string> name, bool force = true);
        void set_pattern(std::string name, std::string pat);
        void set_pattern(std::map<std::string, std::string> name_pattern_pairs);
        void set_pattern(std::vector<std::string> name_pattern_pairs);
};

#endif