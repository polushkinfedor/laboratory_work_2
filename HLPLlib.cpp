#include "HLPLlib.h"

void set_color(std::string text, int color) { // выбор цвета (в значение color подставлять из перечисления serv_colors)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    std::cout << text;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

std::string make_date() { // возвращает время системы
    time_t curr_time = time(nullptr);
    tm* time_info = localtime(&curr_time);

    return (std::to_string(time_info->tm_mday) + "_"
            + std::to_string(time_info->tm_mon + 1) + "_"
            + std::to_string(time_info->tm_year + 1900) + "_"
            + std::to_string(time_info->tm_hour) + "-"
            + std::to_string(time_info->tm_min) + "-"
            + std::to_string(time_info->tm_sec));
}

std::string random_date(int tm) {
    std::srand(tm);
    std::string day = std::to_string(std::rand()%28);
    std::string mounth = std::to_string(std::rand()%12);
    std::string year = std::to_string(1960 + std::rand()%80);

    if (mounth.length() == 1) mounth = "0" + mounth;
    if (day.length() == 1) day = "0" + day;

    std::string date =  day + "." + mounth + "." + year;
    return date;
}

std::string delete_extraspace_string (std::string str) {
    int index = 0;
    for (int i = str.length()-1; i>=0; i--) { // удаление пробелов в конце строки
        if (str[str.length()-1] != ' ') break; // если последний символ не пробел, то выходим из цикла
        if (str[i] != ' ')  {   // встречаем пробел и удаляем все после него
            index = i+1;        // после чего выходим из цикла
            str.erase(index, str.length()-index);
            break;
        }
    }

    index = 0;
    if (str[0] != ' ') return str; // если первый символ не пробел, то выходим функции
    for (int i = 0; i<str.length(); i++) {
        if (str[i] != ' ')  {    // встречаем пробел и удаляем все до него
            index = i;           // после чего выходим из функции
            str.erase(0, index);
            break;
        }
    }
    return str;
}

std::string get_path_to_files() {
    std::string path = std::filesystem::current_path().string();

    int index = 0;
    for (int i = path.length()-1; i>0; i--) {
        if (path[i] == '\\') {
            index = i;
            break;
        }
    }
    path.erase(index);

    path += "\\Files";

    std::filesystem::create_directories(path);

    return path;
}

void str_tolower (std::string& str) {
    for (auto& symb : str) {
        symb = tolower(symb);
    }
    return;
}

bool date_correct(std::string date) { // является ли строка корректной датой
    static int MAX_VALID_YR = 9999; // константа максимального года
    static int MIN_VALID_YR = 1800; // константа минимального года

    if (date.length() != 10 or date[2] != '.' or date[5] != '.') return 0; // проверяем что дата верна по длине и разделена точками

    std::string day = "00"; // заполняем отдельные строковые переменные для дня месяца и года
    day[0] = date[0];
    day[1] = date[1];

    std::string mounth = "00";
    mounth[0] = date[3];
    mounth[1] = date[4];

    std::string year = "0000";
    year[0] = date[6];
    year[1] = date[7];
    year[2] = date[8];
    year[3] = date[9];

    if (not(number_correct(day) and number_correct(mounth) and number_correct(year))) return 0;// проверка на то, что все переменные которые должны быть числами ими и являются

    int d = stoi(day); // после проверки переводим день месяц и год в число
    int m = stoi(mounth);
    int y = stoi(year);

    if (y > MAX_VALID_YR || y < MIN_VALID_YR) return 0; // границы ввода года
    if (m < 1 || m > 12) return 0; // границы ввода месяца
    if (d < 1 || d > 31) return 0; // границы ввода дня

    // В високосный год в феврале 29 дней
    if (m == 2) {
        if (is_leap(y)) return (d <= 29);
        else return (d <= 28);
    }

    // Месяца апрель, июнь, сентябрь и октябрь состоят из 30 или меньше дней
    if (m == 4 || m == 6 || m == 9 || m == 11) return (d <= 30);

    return 1;
}

bool word_correct(std::string word) { // строка не должна содержать цифр и спец.символов
    if (word.length() < 1) return 0; // строка не пуста
    for (int i = 0; i < word.length(); i++) {
        if (not((word[i] >= 'A' && word[i] <= 'Z') or (word[i] >= 'a' && word[i] <= 'z'))) return 0; // проверка по таблице ACHII
    }
    return 1;
}

bool number_correct(std::string number) { // строка не должна содержать цифр и спец.символов
    if (number.length() < 1) return 0; // строка не пуста
    for (int i = 0; i < number.length(); i++) {
        if (not(number[i] >= '0' && number[i] <= '9')) return 0; // проверка по таблице ACHII
    }
    return 1;
}

bool noun_correct(std::string noun) { // строка не должна содержать цифр и спец.символов кроме пробелов
    if (noun.length() < 1) return 0; // строка не пуста
    for (int i = 0; i < noun.length(); i++) {
        if (not((noun[i] >= 'A' && noun[i] <= 'Z') or (noun[i] >= 'a' && noun[i] <= 'z') or noun[i] == ' '))
            return 0;// проверка по таблице ACHII
    }
    return 1;
}

bool without_special_symbols(std::string str) { // строка не должна содержать спец.символов
    if (str.length() < 1) return 0; // строка не пуста
    for (int i = 0; i < str.length(); i++) {
        if (not((str[i] >= 'A' && str[i] <= 'Z') or (str[i] >= 'a' && str[i] <= 'z') or
                (str[i] >= '0' && str[i] <= '9') or str[i] == ' '))
            return 0; // проверка по таблице ACHII
    }
    return 1;
}

bool is_leap(int year) { // функция определяет является ли год високосным
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}
