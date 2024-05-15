#ifndef PROGRAM3_MENU_H
#define PROGRAM3_MENU_H

#include "shell_sort.h"

// выбор настроек сортировки
void choose_file(std::string&, const std::string&);
void choose_field(int&);
void choose_direction(std::string&);

// сообщения об ошибке нахождения файлов
void error_with_quanity_files(const std::string&);

//информационные сообщения
void sort_message(const std::string&);
void menu_message(std::string&, std::string&, int);

// функция менеджмента программы
void menu_manager();

#endif //PROGRAM3_MENU_H
