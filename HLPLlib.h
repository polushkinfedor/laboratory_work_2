#ifndef PROGRAM3_HLPLLIB_H
#define PROGRAM3_HLPLLIB_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <random>
#include <filesystem>

enum serv_colors
{
    serv_green = 2, serv_blue = 11, serv_red = 12, serv_yellow = 14
};
std::string make_date();
std::string random_date(int tm);
std::string delete_extraspace_string(std::string);
std::string get_path_to_files();

void str_tolower (std::string& str);
void set_color(std::string, int);

bool date_correct(std::string); // является ли строка корректной датой
bool word_correct(std::string); // строка не должна содержать цифр и спец.символов
bool number_correct(std::string); // должна содержать только цифры
bool noun_correct(std::string); // строка не должна содержать цифр и спец.символов кроме пробелов
bool without_special_symbols(std::string); // строка не содержит спец символов кроме пробелов
bool is_leap(int year);

#endif //PROGRAM3_HLPLLIB_H
