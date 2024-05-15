#ifndef PROGRAM3_SHELL_SORT_H
#define PROGRAM3_SHELL_SORT_H
#include "HLPLlib.h"

struct TData_unit {
    TData_unit(std::string, std::string, std::string, std::string, std::string);
    std::string type = "-";
    std::string date = "-";
    std::string address = "-";
    std::string surname_chief = "-";
    std::string organisation_name = "-";
};

enum { type_field = 0, date_field, addr_field, orgname_field, surname_field};

std::string sorting_func(std::string& path_to_file, std::string& sort_direction, int& field_for_sort);

// функции для парсинга файла
void first_string_pars(std::string&, std::string&, std::string&, std::string&);
void block_pars(std::string& file_string, std::string& type, std::string& date, std::string& org_name, bool with_org_name);
void file_parsing(const std::string&, std::vector<TData_unit>&);
bool data_correct(const std::vector<TData_unit>&);

// функции для сортировки данных
bool date_comparator (std::string date1, std::string date2, std::string comp_sign);
bool comparator (TData_unit& unit1, TData_unit& unit2, int field, std::string comp_sign);
void shell_sort(std::vector<TData_unit>& data, const std::string& sort_direction, const int& field_for_sort);

// функции для вывода данных в файл
void table_print (std::ofstream& fout, int start_field);
void print_to_file (std::ofstream& fout, TData_unit& el, int start_field, bool is_it_head);
void type_sort_enter (const std::string& sort_direction, int start_field, std::string& out, bool is_first_file_string);
void create_file(const std::vector<TData_unit>& data, int start_field, std::string& sorted_file_name, const std::string& sort_direction);

#endif //PROGRAM3_SHELL_SORT_H