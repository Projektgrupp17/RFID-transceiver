#include "../src/config.hpp"
#include "../src/interpreter.hpp"
#include "../src/util.hpp"
#include "../src/tag_collection.hpp"
#include "../src/tag_data.hpp"
#include "../src/transmitter.hpp"
#include "gtest/gtest.h"
#include <thread>

class interpreterTest : public ::testing::Test {
    protected:
        interpreterTest() {
            std::vector<std::string> name_pair;
            name_pair.push_back("cmd");
            name_pair.push_back("^\\s*(::\\s*(protocol|rotate|url|delay|repeat|sim\\s*:\\s*seed|quit)|(protocol|rotate|url|delay|repeat|sim\\s*:\\s*seed|list)\\s*::)[\\w\\s]*$");
            name_pair.push_back("getter");
            name_pair.push_back("^\\s*((protocol|rotate|url|delay|repeat|sim\\s*:\\s*seed|list)\\s*::)\\s*$");
            name_pair.push_back("setter");
            name_pair.push_back("^\\s*(::\\s*(protocol|rotate|url|delay|repeat|sim\\s*:\\s*seed|quit))[\\w\\s]*$");
            name_pair.push_back("opt");
            name_pair.push_back("[\\w]");

            interpret = interpreter(name_pair);
        }

        void SetUp() override {
            
        }

        interpreter interpret;
};

TEST(ConfigTest, SetDelay) {
    config config;
    
    EXPECT_EQ(config.get_scan_delay(), "1000");
    config.set_scan_delay("0");
    EXPECT_EQ(config.get_scan_delay(), "0");
    config.set_scan_delay("100");
    EXPECT_EQ(config.get_scan_delay(), "100");
}

TEST(UtilTest, HexToBin) {
    // various expected valid input
    char temp[16];

    strcpy(temp, "00");
    EXPECT_EQ(util::hex_to_bin(temp), "00000000");
    strcpy(temp, "01");
    EXPECT_EQ(util::hex_to_bin(temp), "00000001");
    strcpy(temp, "02");
    EXPECT_EQ(util::hex_to_bin(temp), "00000010");
    strcpy(temp, "34");
    EXPECT_EQ(util::hex_to_bin(temp), "00110100");
    strcpy(temp, "77");
    EXPECT_EQ(util::hex_to_bin(temp), "01110111");
    strcpy(temp, "FF");
    EXPECT_EQ(util::hex_to_bin(temp), "11111111");
    strcpy(temp, "AA");
    EXPECT_EQ(util::hex_to_bin(temp), "10101010");
    strcpy(temp, "F1");
    EXPECT_EQ(util::hex_to_bin(temp), "11110001");
    strcpy(temp, "EC");
    EXPECT_EQ(util::hex_to_bin(temp), "11101100");
    strcpy(temp, "BD");
    EXPECT_EQ(util::hex_to_bin(temp), "10111101");
    strcpy(temp, "F0");
    EXPECT_EQ(util::hex_to_bin(temp), "11110000");
    strcpy(temp, "0F");
    EXPECT_EQ(util::hex_to_bin(temp), "00001111");
}

TEST(UtilTest, ReverseTag) {
    char test_string_15693[] = "A2601BD86FE202E0";
    char test_string_15693_second[] = "1903C4E984B00EE0";
    char empty_string[] = "";

    EXPECT_EQ(util::reverse_tag(test_string_15693), "E002E26FD81B60A2");
    EXPECT_EQ(util::reverse_tag(test_string_15693_second), "E00EB084E9C40319");
    EXPECT_EQ(util::reverse_tag(empty_string), "");
}

TEST(UtilTest, IdentifyManufacturer) {
    char one[] = "2947BC3D951F01E0";
    char five[] = "2BAF04BC38E505E0";
    char twelve[] = "A1B891E676EE0CE0";
    char fourteen[] = "ABC0BBCC32660EE0";
    char sixteen[] = "BEE55661E00910E0";
    char thirtythree[] = "AD12321EEE3333E0";
    char empty[] = "";
    char non_existent[] = "AC0900003B9999E0";
    std::string protocol = "15693";

    EXPECT_EQ(util::identify_manufacturer(protocol, one), "Motorola");
    EXPECT_EQ(util::identify_manufacturer(protocol, five), "Infineon Technologies");
    EXPECT_EQ(util::identify_manufacturer(protocol, twelve), "Toshiba");
    EXPECT_EQ(util::identify_manufacturer(protocol, fourteen), "Samsung Electronics");
    EXPECT_EQ(util::identify_manufacturer(protocol, sixteen), "LG Semiconductors");
    EXPECT_EQ(util::identify_manufacturer(protocol, thirtythree), "AMIC");
    EXPECT_ANY_THROW(util::identify_manufacturer(protocol, empty));
    EXPECT_ANY_THROW(util::identify_manufacturer(protocol, non_existent));
}

TEST(UtilTest, ArrSliceAndArrClear) {
    char test_array[64], part_array[64];
    strcpy(test_array, "This is an experimental array for thorough quality assurance...");
    std::string temp;
    util::arr_clear(part_array, 64);

    // valid slices including start and end of array
    util::arr_slice(test_array, part_array, 0, 4);
    temp = part_array;
    EXPECT_EQ(temp, "This");
    EXPECT_EQ(part_array[0], 'T');
    EXPECT_EQ(part_array[1], 'h');
    EXPECT_EQ(part_array[2], 'i');
    EXPECT_EQ(part_array[3], 's');
    util::arr_clear(part_array, 64);
    util::arr_slice(test_array, part_array, 23, 52);
    temp = part_array;
    EXPECT_EQ(temp, " array for thorough quality a");
    EXPECT_EQ(part_array[0], ' ');
    EXPECT_EQ(part_array[1], 'a');
    EXPECT_EQ(part_array[2], 'r');
    EXPECT_EQ(part_array[3], 'r');
    EXPECT_EQ(part_array[4], 'a');
    EXPECT_EQ(part_array[5], 'y');
    EXPECT_EQ(part_array[6], ' ');
    EXPECT_EQ(part_array[7], 'f');
    EXPECT_EQ(part_array[8], 'o');
    EXPECT_EQ(part_array[9], 'r');
    EXPECT_EQ(part_array[26], 'y');
    EXPECT_EQ(part_array[27], ' ');
    EXPECT_EQ(part_array[28], 'a');
    util::arr_clear(part_array, 64);
    util::arr_slice(test_array, part_array, 4, 17);
    temp = part_array;
    EXPECT_EQ(temp, " is an experi");
    EXPECT_EQ(part_array[0], ' ');
    EXPECT_EQ(part_array[1], 'i');
    EXPECT_EQ(part_array[2], 's');
    EXPECT_EQ(part_array[3], ' ');
    EXPECT_EQ(part_array[4], 'a');
    EXPECT_EQ(part_array[5], 'n');
    EXPECT_EQ(part_array[6], ' ');
    EXPECT_EQ(part_array[7], 'e');
    EXPECT_EQ(part_array[8], 'x');
    EXPECT_EQ(part_array[9], 'p');
    EXPECT_EQ(part_array[10], 'e');
    EXPECT_EQ(part_array[11], 'r');
    EXPECT_EQ(part_array[12], 'i');
}

TEST(UtilTest, Trim) {
    std::string test_string = " this  string incl udes irre gul  ar spaces   ";
    std::string test_string_no_spaces = "thisstringdoesntincludespaces";
    std::string test_string_empty = "";
    std::string test_string_only_spaces = "    ";
    std::string trimmed_string;

    util::trim(test_string, trimmed_string);
    EXPECT_EQ(trimmed_string, "thisstringincludesirregularspaces");
    trimmed_string.clear();
    util::trim(test_string_no_spaces, trimmed_string);
    EXPECT_EQ(trimmed_string, "thisstringdoesntincludespaces");
    trimmed_string.clear();
    util::trim(test_string_empty, trimmed_string);
    EXPECT_EQ(trimmed_string, "");
    trimmed_string.clear();
    util::trim(test_string_only_spaces, trimmed_string);
    EXPECT_EQ(trimmed_string, "");
}

TEST(UtilTest, Replace) {
    std::string test_string = "thiis iis a striing with erroneous double iiii's that should be a siingle i";
    std::string test_string_no_i = "hello world";
    std::string test_string_empty_string = "";
    std::string test_string_only_spaces = "    ";
    std::string replaced_string;

    util::replace(test_string, replaced_string, "ii", "i");
    EXPECT_EQ(replaced_string, "this is a string with erroneous double i's that should be a single i");
    replaced_string.clear();
    util::replace(test_string_no_i, replaced_string, "ii", "i");
    EXPECT_EQ(replaced_string, "hello world");
    replaced_string.clear();
    util::replace(test_string_empty_string, replaced_string, "ii", "i");
    EXPECT_EQ(replaced_string, "");
    replaced_string.clear();
    util::replace(test_string_only_spaces, replaced_string, "ii", "i");
    EXPECT_EQ(replaced_string, "    ");
}

TEST(UtilTest, Slice) {
    std::string test_string = "this is a proper test string";
    std::string sliced_string;

    util::slice(test_string, sliced_string, 0, 4);
    EXPECT_EQ(sliced_string, "this");
    sliced_string.clear();
    util::slice(test_string, sliced_string, 7, 15);
    EXPECT_EQ(sliced_string, " a prope");
    sliced_string.clear();
    util::slice(test_string, sliced_string, 22, 28);
    EXPECT_EQ(sliced_string, "string");
}

TEST(UtilTest, Split) {
    std::string test_string = "this,is,test,string,for,ages";
    std::string test_string_separator_ending = "this.is.test.hey.";
    std::string test_string_separator_start = "|this|is|tester";
    std::string test_string_separator_both = "-this-is-another-test-message-";
    std::string test_string_lone_separator = ",";
    std::string test_string_separator_only_spaces = "  ";
    std::vector<std::string> results;
    std::vector<std::string>::iterator iter;

    util::split(test_string, ",", &results);
    iter = results.begin();
    EXPECT_EQ(*iter, "this");
    iter++;
    EXPECT_EQ(*iter, "is");
    iter++;
    EXPECT_EQ(*iter, "test");
    iter++;
    EXPECT_EQ(*iter, "string");
    iter++;
    EXPECT_EQ(*iter, "for");
    iter++;
    EXPECT_EQ(*iter, "ages");
    results.clear();
    util::split(test_string_separator_ending, ".", &results);
    iter = results.begin();
    EXPECT_EQ(*iter, "this");
    iter++;
    EXPECT_EQ(*iter, "is");
    iter++;
    EXPECT_EQ(*iter, "test");
    iter++;
    EXPECT_EQ(*iter, "hey");
    iter++;
    EXPECT_EQ(*iter, "");
    results.clear();
    util::split(test_string_separator_start, "|", &results);
    iter = results.begin();
    EXPECT_EQ(*iter, "");
    iter++;
    EXPECT_EQ(*iter, "this");
    iter++;
    EXPECT_EQ(*iter, "is");
    iter++;
    EXPECT_EQ(*iter, "tester");
    results.clear();
    util::split(test_string_separator_both, "-", &results);
    iter = results.begin();
    EXPECT_EQ(*iter, "");
    iter++;
    EXPECT_EQ(*iter, "this");
    iter++;
    EXPECT_EQ(*iter, "is");
    iter++;
    EXPECT_EQ(*iter, "another");
    iter++;
    EXPECT_EQ(*iter, "test");
    iter++;
    EXPECT_EQ(*iter, "message");
    iter++;
    EXPECT_EQ(*iter, "");
    results.clear();
    util::split(test_string_lone_separator, ",", &results);
    iter = results.begin();
    EXPECT_EQ(*iter, "");
    iter++;
    EXPECT_EQ(*iter, "");
    results.clear();
    util::split(test_string_separator_only_spaces, " ", &results);
    iter = results.begin();
    EXPECT_EQ(*iter, "");
    iter++;
    EXPECT_EQ(*iter, "");
    iter++;
    EXPECT_EQ(*iter, "");
    results.clear();
    util::split(test_string_separator_only_spaces, ",", &results);
    iter = results.begin();
    EXPECT_EQ(*iter, "  ");
    results.clear();
    util::split(test_string, ".", &results);
    iter = results.begin();
    EXPECT_EQ(*iter, "this,is,test,string,for,ages");
}
/*
TEST(TagCollectionTest, ConnectParameterizedConstructorInstantRescans) {
    tag_data data, data_second, data_third;
    transmitter trns;
    config conf;
    data.tag_hex = "E002AB435591C4E1";
    data_second.tag_hex = "E002ACDE11119731";
    data_third.tag_hex = "E002BC43A2239184";
    tag_collection tag_col = tag_collection(data, trns, conf);
    EXPECT_EQ(tag_col.size(), 1);
    tag_col.connect(data_second);
    EXPECT_EQ(tag_col.size(), 2);
    tag_col.connect(data_third);
    EXPECT_EQ(tag_col.size(), 3);
    tag_col.connect(data);
    EXPECT_EQ(tag_col.size(), 3);
    std::this_thread::sleep_for(std::chrono::milliseconds(12000));
    tag_col.dump();
}

TEST(TagCollectionTest, ConnectInstantRescans) {
    tag_data data, data_second, data_third;
    transmitter trns;
    config conf;
    data.tag_hex = "E002AB435591C4E2";
    data_second.tag_hex = "E002ACDE11119732";
    data_third.tag_hex = "E002BC43A2239185";
    tag_collection t_col = tag_collection(trns, conf);
    EXPECT_EQ(t_col.size(), 0);
    t_col.connect(data_third);
    EXPECT_EQ(t_col.size(), 1);
    t_col.connect(data_third);
    EXPECT_EQ(t_col.size(), 1);
    std::this_thread::sleep_for(std::chrono::milliseconds(7000));
    t_col.dump();
}

TEST(TagCollectionTest, ConnectVariableRescans) {
    tag_data data, data_second, data_third, data_forth, data_fifth, data_sixth, data_seventh;
    transmitter trns;
    config conf;
    data.tag_hex = "E002AB435591C4E2";
    data_second.tag_hex = "E002ACDE11119732";
    data_third.tag_hex = "E002BC43A2239185";
    data_forth.tag_hex = "E002BD43A2239186";
    data_fifth.tag_hex = "E002CD43A2239187";
    data_sixth.tag_hex = "E002CE43A2239188";
    data_seventh.tag_hex = "E002DF43A2239189";
    tag_collection t_col = tag_collection(trns, conf);
    EXPECT_EQ(t_col.size(), 0);
    t_col.connect(data);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    EXPECT_EQ(t_col.size(), 1);
    t_col.connect(data_second);
    std::this_thread::sleep_for(std::chrono::milliseconds(885));
    EXPECT_EQ(t_col.size(), 2);
    t_col.connect(data_third);
    std::this_thread::sleep_for(std::chrono::milliseconds(1250));
    EXPECT_EQ(t_col.size(), 3);
    t_col.connect(data_forth);
    std::this_thread::sleep_for(std::chrono::milliseconds(1625));
    EXPECT_EQ(t_col.size(), 4);
    t_col.connect(data_fifth);
    std::this_thread::sleep_for(std::chrono::milliseconds(2250));
    EXPECT_EQ(t_col.size(), 3);
    t_col.connect(data_sixth);
    std::this_thread::sleep_for(std::chrono::milliseconds(3700));
    EXPECT_EQ(t_col.size(), 1);
    t_col.connect(data_seventh);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_EQ(t_col.size(), 2);
    std::this_thread::sleep_for(std::chrono::milliseconds(7000));
    t_col.dump();
}
*/
TEST_F(interpreterTest, TestCommandMatch) {
    // setters
    EXPECT_EQ(interpret.test("::url", "cmd"), true);
    EXPECT_EQ(interpret.test("::protocol", "cmd"), true);
    EXPECT_EQ(interpret.test("::delay", "cmd"), true);
    EXPECT_EQ(interpret.test("::sim:seed", "cmd"), true);
    EXPECT_EQ(interpret.test("::repeat", "cmd"), true);
    EXPECT_EQ(interpret.test("::quit", "cmd"), true);

    // getters
    EXPECT_EQ(interpret.test("url::", "cmd"), true);
    EXPECT_EQ(interpret.test("protocol::", "cmd"), true);
    EXPECT_EQ(interpret.test("delay::", "cmd"), true);
    EXPECT_EQ(interpret.test("list::", "cmd"), true);
    EXPECT_EQ(interpret.test("sim:seed::", "cmd"), true);
    EXPECT_EQ(interpret.test("repeat::", "cmd"), true);

    // setters / getters without valid prefix/suffix
    EXPECT_EQ(interpret.test("url", "cmd"), false);
    EXPECT_EQ(interpret.test("protocol", "cmd"), false);
    EXPECT_EQ(interpret.test("delay", "cmd"), false);
    EXPECT_EQ(interpret.test("quit", "cmd"), false);
    EXPECT_EQ(interpret.test("list", "cmd"), false);
    EXPECT_EQ(interpret.test(":url", "cmd"), false);
    EXPECT_EQ(interpret.test(":protocol", "cmd"), false);
    EXPECT_EQ(interpret.test(":delay", "cmd"), false);
    EXPECT_EQ(interpret.test(":quit", "cmd"), false);
    EXPECT_EQ(interpret.test(":list", "cmd"), false);
    EXPECT_EQ(interpret.test("url:", "cmd"), false);
    EXPECT_EQ(interpret.test("protocol:", "cmd"), false);
    EXPECT_EQ(interpret.test("delay:", "cmd"), false);
    EXPECT_EQ(interpret.test("quit:", "cmd"), false);
    EXPECT_EQ(interpret.test("list:", "cmd"), false);
    EXPECT_EQ(interpret.test("sim:seed", "cmd"), false);
    EXPECT_EQ(interpret.test(":sim:seed", "cmd"), false);
    EXPECT_EQ(interpret.test("sim:seed:", "cmd"), false);
    EXPECT_EQ(interpret.test("repeat", "cmd"), false);
    EXPECT_EQ(interpret.test(":repeat", "cmd"), false);
    EXPECT_EQ(interpret.test("repeat:", "cmd"), false);

    // invalid but presumed common inputs
    EXPECT_EQ(interpret.test(" ", "cmd"), false);
    EXPECT_EQ(interpret.test(".", "cmd"), false);
    EXPECT_EQ(interpret.test("--help", "cmd"), false);
    EXPECT_EQ(interpret.test(" --help", "cmd"), false);
    EXPECT_EQ(interpret.test("cd..", "cmd"), false);
    EXPECT_EQ(interpret.test("cd ..", "cmd"), false);
    EXPECT_EQ(interpret.test("q", "cmd"), false);
}

TEST_F(interpreterTest, EvalAndLookBehindAndFlushCommandMatch) {
    std::string match_str;
    std::vector<std::string> match_vector;
    std::vector<std::string>::iterator iter;

    // setters
    EXPECT_EQ(interpret.eval("::url", match_str, "cmd", 1, true), true);
    EXPECT_EQ(interpret.look_behind(), "::url");
    match_str.clear();
    EXPECT_EQ(interpret.eval("::protocol", match_str, "cmd", 1, true), true);
    EXPECT_EQ(match_str, "::protocol");
    EXPECT_EQ(interpret.look_behind(), "::protocol");
    EXPECT_EQ(interpret.look_behind(1), "::protocol");
    EXPECT_EQ(interpret.look_behind(2), "::url");
    match_str.clear();
    EXPECT_EQ(interpret.eval("::delay", match_vector, "cmd", 1, true), true);
    iter = match_vector.begin();
    EXPECT_EQ(*iter, "::delay");
    iter++;
    EXPECT_EQ(*iter, "::delay");
    iter++;
    EXPECT_EQ(*iter, "delay");
    EXPECT_EQ(interpret.look_behind(), "::delay");
    EXPECT_EQ(interpret.look_behind(2), "::protocol");
    match_vector.clear();
    EXPECT_EQ(interpret.eval("::sim:seed", match_str, "cmd", 1, true), true);
    EXPECT_EQ(interpret.look_behind(), "::sim:seed");
    EXPECT_EQ(interpret.look_behind(1), "::sim:seed");
    match_str.clear();
    EXPECT_EQ(interpret.eval("::repeat", match_str, "cmd"), true);
    EXPECT_EQ(match_str, "::repeat");
    EXPECT_EQ(interpret.look_behind(), "::sim:seed");
    match_str.clear();
    EXPECT_EQ(interpret.eval("::repeat", match_str, "cmd", 1, false), true);
    EXPECT_EQ(match_str, "::repeat");
    EXPECT_EQ(interpret.look_behind(), "::sim:seed");
    match_str.clear();
    EXPECT_EQ(interpret.eval("::repeat", match_vector, "cmd"), true);
    iter = match_vector.begin();
    EXPECT_EQ(*iter, "::repeat");
    iter++;
    EXPECT_EQ(*iter, "::repeat");
    iter++;
    EXPECT_EQ(*iter, "repeat");
    EXPECT_EQ(interpret.look_behind(), "::sim:seed");
    match_vector.clear();
    EXPECT_EQ(interpret.eval("::repeat", match_vector, "cmd", 1, false), true);
    iter = match_vector.begin();
    EXPECT_EQ(*iter, "::repeat");
    iter++;
    EXPECT_EQ(*iter, "::repeat");
    iter++;
    EXPECT_EQ(*iter, "repeat");
    EXPECT_EQ(interpret.look_behind(), "::sim:seed");
    match_vector.clear();
    EXPECT_EQ(interpret.eval("::quit", match_str, "cmd", 1, true), true);
    EXPECT_EQ(interpret.look_behind(), "::quit");
    interpret.flush_last(1);
    EXPECT_EQ(interpret.look_behind(), "::sim:seed");
    interpret.flush_last(2);
    EXPECT_EQ(interpret.look_behind(), "::protocol");
    EXPECT_EQ(interpret.look_behind(1), "::protocol");
    EXPECT_EQ(interpret.look_behind(2), "::url");
    interpret.flush();

    // getters
    EXPECT_EQ(interpret.test("url::", "cmd"), true);
    EXPECT_EQ(interpret.test("protocol::", "cmd"), true);
    EXPECT_EQ(interpret.test("delay::", "cmd"), true);
    EXPECT_EQ(interpret.test("list::", "cmd"), true);
    EXPECT_EQ(interpret.test("sim:seed::", "cmd"), true);
    EXPECT_EQ(interpret.test("repeat::", "cmd"), true);
}

/*TEST_F(interpreterTest, UrlOptionsMatch) {
    // url setter with valid options
    interpret.test("::url", "cmd", 1, true);
    EXPECT_EQ(interpret.test("http://www.kth.se", "opt"), true);
    EXPECT_EQ(interpret.test("www.kth.se", "opt"), true);
    EXPECT_EQ(interpret.test("http://kth.se", "opt"), true);
    EXPECT_EQ(interpret.test("kth.se", "opt"), true);
    EXPECT_EQ(interpret.test("http://www.kth.se/", "opt"), true);
    EXPECT_EQ(interpret.test("www.kth.se/", "opt"), true);
    EXPECT_EQ(interpret.test("http://www.kth.se:80", "opt"), true);
    EXPECT_EQ(interpret.test("http://www.kth.se:80/", "opt"), true);
    EXPECT_EQ(interpret.test("http://.kth.se:80", "opt"), true);
    EXPECT_EQ(interpret.test("http://.kth.se:80/", "opt"), true);
    EXPECT_EQ(interpret.test("http://127.0.0.1", "opt"), true);
    EXPECT_EQ(interpret.test("127.0.0.1", "opt"), true);
    EXPECT_EQ(interpret.test("http://127.0.0.1/", "opt"), true);
    EXPECT_EQ(interpret.test("127.0.0.1/", "opt"), true);
    EXPECT_EQ(interpret.test("http://127.0.0.1:80", "opt"), true);
    EXPECT_EQ(interpret.test("http://127.0.0.1:80/", "opt"), true);

    // url getter validly without any options
    EXPECT_EQ(interpret.test("", "opt"), true);
    EXPECT_EQ(interpret.test(" ", "opt"), true);
    EXPECT_EQ(interpret.test("   ", "opt"), true);
    EXPECT_EQ(interpret.test("    ", "opt"), true);

    // setters with invalid options
    EXPECT_EQ(interpret.test("https://www.kth.se", "opt"), false);
    EXPECT_EQ(interpret.test("https://kth.se", "opt"), false);
    EXPECT_EQ(interpret.test("https://www.kth.se:80", "opt"), false);
    EXPECT_EQ(interpret.test("https://www.kth.se/", "opt"), false);
    EXPECT_EQ(interpret.test("https://kth.se/", "opt"), false);
    EXPECT_EQ(interpret.test("https://www.kth.se:80/", "opt"), false);
    EXPECT_EQ(interpret.test("https://www.kth.se/:80", "opt"), false);
    EXPECT_EQ(interpret.test("https://kth.se/:80", "opt"), false);
    EXPECT_EQ(interpret.test("https://www.kth.se/test:80", "opt"), false);
    EXPECT_EQ(interpret.test("https://kth.se//test", "opt"), false);
    EXPECT_EQ(interpret.test("https:/www.kth.se", "opt"), false);
    EXPECT_EQ(interpret.test("https:/kth.se", "opt"), false);
    EXPECT_EQ(interpret.test("https:/www.kth.se:80", "opt"), false);
    EXPECT_EQ(interpret.test("https:www.kth.se", "opt"), false);
    EXPECT_EQ(interpret.test("https:kth.se", "opt"), false);
    EXPECT_EQ(interpret.test("https:www.kth.se:80", "opt"), false);
    EXPECT_EQ(interpret.test("https//www.kth.se", "opt"), false);
    EXPECT_EQ(interpret.test("https//kth.se", "opt"), false);
    EXPECT_EQ(interpret.test("https//www.kth.se:80", "opt"), false);
    EXPECT_EQ(interpret.test("http:/www.kth.se", "opt"), false);
    EXPECT_EQ(interpret.test("http:/kth.se", "opt"), false);
    EXPECT_EQ(interpret.test("http:/www.kth.se:80", "opt"), false);
    EXPECT_EQ(interpret.test("http:www.kth.se", "opt"), false);
    EXPECT_EQ(interpret.test("http:kth.se", "opt"), false);
    EXPECT_EQ(interpret.test("http:www.kth.se:80", "opt"), false);
    EXPECT_EQ(interpret.test("http//www.kth.se", "opt"), false);
    EXPECT_EQ(interpret.test("http//kth.se", "opt"), false);
    EXPECT_EQ(interpret.test("http//www.kth.se:80", "opt"), false);
    
    // valid help flag for setters / getters
    EXPECT_EQ(interpreter.test("--help", "cmd"), true);
    EXPECT_EQ(interpreter.test(" --help", "cmd"), true);
}

TEST_F(interpreterTest, ProtocolOptionsMatch) {
    // url setter with valid options
    EXPECT_EQ(interpret.test("15693", "opt"), true);
    EXPECT_EQ(interpreter.test("14443A", "opt"), true);
    EXPECT_EQ(interpreter.test("14443B", "opt"), true);
    EXPECT_EQ(interpreter.test("118092", "opt"), true);

    // url getter validly without any options
    EXPECT_EQ(interpreter.test("", "opt"), true);
    EXPECT_EQ(interpreter.test(" ", "opt"), true);
    EXPECT_EQ(interpreter.test("   ", "opt"), true);
    EXPECT_EQ(interpreter.test("    ", "opt"), true);

    // setters with invalid options
    EXPECT_EQ(interpret.test("14240", "opt"), false);
    EXPECT_EQ(interpret.test("ISO15693", "opt"), false);
    
    // valid help flag for setters / getters
    EXPECT_EQ(interpreter.test("--help", "cmd"), true);
    EXPECT_EQ(interpreter.test(" --help", "cmd"), true);
} */