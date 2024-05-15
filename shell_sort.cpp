#include "shell_sort.h"
#include <random>

TData_unit::TData_unit(std::string type, std::string date, std::string address, std::string surname_chief, std::string organisation_name) { // конструктор рабочей структуры
    this->type = delete_extraspace_string(type);
    this->date = delete_extraspace_string(date);
    this->organisation_name = delete_extraspace_string(organisation_name);
    this->surname_chief = delete_extraspace_string(surname_chief);
    this->address = delete_extraspace_string(address);
}

std::string sorting_func(std::string& path_to_file, std::string& sort_direction, int& field_for_sort) { // мета-функция сортировки, администрирующая её процесс и сообщающая коды завершения в функцию меню
    std::string sorted_file_name; // Переменная для хранения названия созданного файла, т.к. он создается с именем, зависящим от времени, мы должны запомнить название для дальнейшего использования

    // проверка и обработка возможных ошибок при вызове функции
    if (not(std::filesystem::exists(path_to_file))) return "input_file_error"; // возвращения ошибки существования файла
    if (sort_direction == "-" or field_for_sort == -1) return "incorrect_settings"; // возвращение ошибки некорректных настроек (направления/поля)

    // получение информации для сортировки
    std::vector<TData_unit> data;
    file_parsing(path_to_file, data); // заполнение массива структур за счет парсинга файла Report_File_Organisation

    if (!data_correct(data)) return "incorrect_data"; // если введенный файл содержит некорректные данные, то отправить код ошибки

    shell_sort(data, sort_direction, field_for_sort); // Shell-сортировка по заданному полю в заданном направлении
    create_file(data, field_for_sort, sorted_file_name, sort_direction); // создание и заполнения финального файла отсортированных структур
    return sorted_file_name;
}

// функции для парсинга файла
void first_string_pars(std::string& file_string, std::string& org_name, std::string& org_addr, std::string& chief_sur) {
    const std::string text_org_name = "Organisation name: "; // парсинг строки вида "Organisation name: TEXT | address: TEXT | chief surname: TEXT"
    const std::string text_addr = "address: ";
    const std::string text_chief_surname = "chief surname: ";

    file_string.erase(0, text_org_name.length());
    org_name = file_string.substr(0, file_string.find('|'));
    file_string.erase(0, file_string.find('|')+2);

    file_string.erase(0, text_addr.length());
    org_addr = file_string.substr(0, file_string.find('|'));
    file_string.erase(0, file_string.find('|')+2);

    file_string.erase(0, text_chief_surname.length());
    chief_sur = file_string.substr(0, file_string.length());
}

void block_pars(std::string& file_string, std::string& type, std::string& date, std::string& org_name, bool with_org_name = true) {
    file_string.erase(0, file_string.find('.')+2);                                 // парсинг блока вида
    type = file_string.substr(0, file_string.find('.')-2);                         //1. TYPE DD.MM.YYYY
    file_string.erase(0, file_string.find('.')-2);

    if (with_org_name) {                                                                // если блок вида TYPE DD.MM.YYYY ORG_NAME
        date = file_string.substr(0, file_string.find(' '));
        file_string.erase(0, file_string.find(' ') + 1);
        org_name = file_string;
    }
    else {
        date = file_string;
    }
}

void file_parsing(const std::string& path_to_file, std::vector<TData_unit>& file_information) {
    std::ifstream file(path_to_file);
    std::string file_string;
    std::string org_name="-", org_addr="-", chief_sur="-", type="-", date="-"; // переменные для всех возможных полей структур, по умолчанию принимающие значение -
    while (!file.eof()) {
        std::getline(file, file_string);
        if (file_string == "") break;

        if (file_string.find("Organisation name:") != -1) { // парсинг блока с информацией об организации
            bool end_of_block = false;
            org_name="-", org_addr="-", chief_sur="-", type="-", date="-";

            first_string_pars(file_string, org_name, org_addr, chief_sur); // распарсим первую строку блока
            std::getline(file, file_string); // проверим вторую строку блока
            if (file_string.find("we haven't information about") != -1) {
                std::getline(file, file_string);
                file_information.emplace_back(TData_unit(type, date, org_addr, chief_sur, org_name)); // Emplace_back, т.к. создаем объект непосредственно в векторе
                end_of_block = true;
            }

            while (!end_of_block) {                                                         // парсинг блока вида
                std::getline(file, file_string);                                            //1. TYPE DD.MM.YYYY
                if (file_string.find("----------------------") != -1) {                     //2. TYPE DD.MM.YYYY
                    end_of_block = true;                                                    //-------------------
                    break;
                }
                block_pars(file_string, type, date, org_name, false);
                file_information.emplace_back(type, date, org_addr, chief_sur, org_name);
            }
        }
        else { // парсинг блока с корреспонденцией без информации об организации
            org_name="-", org_addr="-", chief_sur="-", type="-", date="-";
            if (file_string.find("correspondence without information")!=-1) continue;
            block_pars(file_string, type, date, org_name);
            file_information.emplace_back(type, date, org_addr, chief_sur, org_name);
        }
    }
}

bool data_correct(const std::vector<TData_unit>& Data) {
    bool is_data_correct = true;
    for (const auto& el : Data) {
        if (el.type != "-"  and !date_correct(el.date)) is_data_correct = false;
        if (el.surname_chief != "-"  and !word_correct(el.surname_chief)) is_data_correct = false;
        if (el.address != "-"  and !without_special_symbols(el.address)) is_data_correct = false;
        if (el.organisation_name != "-" and !noun_correct(el.organisation_name)) is_data_correct = false;
        if (el.date != "-"  and !date_correct(el.date)) is_data_correct = false;
    }
    return is_data_correct; // если данные целые
}

// функции для сортировки данных
bool date_comparator (std::string date1, std::string date2, std::string comp_sign) {
    if (date1 == "-" or date2 == "-") return (date1 < date2); // <<выталкивание>> пустой даты наверх

    int d1 = std::stoi(date1.substr(0,2)), d2 = std::stoi(date2.substr(0,2)); // парсинг даты вида "DD.MM.YYYY"
    int m1 = std::stoi(date1.substr(3,5)), m2 = std::stoi(date2.substr(3,5));
    int y1 = std::stoi(date1.substr(6,10)), y2 = std::stoi(date2.substr(6,10));

    bool ans;

    //
    bool sign;
    if (comp_sign == ">") sign = false;
    else if (comp_sign == "<") sign = true;

    if (((y1 > y2) xor sign) and y1 != y2) { // Сравнение годов. От базового (date1 > date2) знак изменяется за счет xor от переменной sign
            ans = true;
        } else if (y1 == y2) { // при равенстве годов сравниваются месяца
        if (((m1 > m2) xor sign) and m1 != m2) { // сравнение месяцев
            ans = true;
        } else if (m1 == m2) { // при равенстве месяцев сравниваются дни
            if (((d1 > d2) xor sign) and d1 != d2) { // сравнение дней
                ans = true;
            } else ans = false; // отношение дней не удовлетворяют знаку
        } else ans = false; // отношение месяцев не удовлетворяют знаку
    } else ans = false; // отношение годов не удовлетворяют знаку

    return ans;
}

bool comparator (TData_unit& unit1, TData_unit& unit2, int field, std::string comp_sign) { // функция сравнения структур по выбранным полям
    std::string el1, el2;
    bool ans;
    switch (field) { // выбор элементов для сравнения
        case type_field:
            el1 = unit1.type;
            el2 = unit2.type;
            break;
        case orgname_field:
            el1 = unit1.organisation_name;
            el2 = unit2.organisation_name;
            break;
        case surname_field:
            el1 = unit1.surname_chief;
            el2 = unit2.surname_chief;
            break;
        case addr_field:
            el1 = unit1.address;
            el2 = unit2.address;
            break;
        case date_field: // если элементы -- даты, то лексикографическое сравнение не подходит, поэтому используем отдельную функцию сравнения даты
            return date_comparator(unit1.date, unit2.date, comp_sign);
    }

    if (comp_sign == "<" or el2 == "-" or el1 == "-") { // лексикографическое сравнение в зависимости от знака
        ans = (el1 < el2);
    } else if (comp_sign == ">") {
        ans = (el1 > el2);
    }

    return ans;
}

void shell_sort(std::vector<TData_unit>& data, const std::string& sort_direction, const int& field_for_sort) {
    std::string sort_sym;
    int size = data.size();

    if (sort_direction == "from smaller to larger")  sort_sym = ">"; // перевод информации о направлении сортировки в символ для сортировки
    else if (sort_direction == "from larger to smaller")  sort_sym = "<";

    for (int k = size/2; k>0; k/=2) { // сортировка с шагами представленными рядом от data.size() до 1, каждый последующий элемент ряда является половиной предыдущего
        for (int i = k; i<size; ++i) { // сортировка вставкой для подмассива
            for (int j = i-k; j>=0 and comparator(data[j], data[j+k], field_for_sort, sort_sym); j-=k) {
                std::swap(data[j], data[j+k]);
            }
        }
    }
}


// функции для вывода данных в файл
void table_print (std::ofstream& fout, int start_field) { // вывод границ таблицы
    switch (start_field) {
        case type_field:
            fout << "|-----------------|-----------|----------------------------------|------------------------|-----------------|\n";
            break;
        case date_field:
            fout << "|-----------|----------------------------------|------------------------|-----------------|-----------------|\n";
            break;
        case addr_field:
            fout << "|----------------------------------|------------------------|-----------------|-----------------|-----------|\n";
            break;
        case orgname_field:
            fout << "|------------------------|-----------------|-----------------|-----------|----------------------------------|\n";
            break;
        case surname_field:
            fout << "|-----------------|-----------------|-----------|----------------------------------|------------------------|\n";
            break;
    }
}

void print_to_file (std::ofstream& fout, TData_unit& el, int start_field, bool is_it_head = 0) { // вывод информации в файл начиная с поля, по которому проводилась сортировка
    const int field_quanity = 5;
    int curr_field;

    if (is_it_head) { // если выводится заголовок таблицы, то необходимо вывести дополнительную служебную границу
        table_print(fout, start_field);
    }

    fout << "|"; // перед каждой строкой выводится символ для создания левой границы таблицы

    for (int j = 0; j<field_quanity; j++) { // цикл прохождения по всем полям структуры, начиная с поля по которому проводилась сортировка
        curr_field = (start_field + j) % field_quanity; // если индекс выходит за границу, то индекс будет находиться через остаток от деления

        switch (curr_field) { // вывод информации по индексу
            case type_field:
                fout << std::setw(18) << el.type + " |";
                break;
            case date_field:
                fout << std::setw(12) << el.date + " |";
                break;
            case addr_field:
                fout << std::setw(35) << el.address + " |";
                break;
            case orgname_field:
                fout << std::setw(25) << el.organisation_name + " |";
                break;
            case surname_field:
                fout << std::setw(18) << el.surname_chief + " |";
                break;
        }
    } fout << "\n";
}

void type_sort_enter (const std::string& sort_direction, int start_field, std::string& out, bool is_first_file_string = false) {
    // получение параметров сортировки
    // sort_direction -- отвечает за определение направления сортировки, start_field -- отвечает за поле сортировки
    // out -- дополняемая строка, is_first_file_string -- флаг, определяющий, заполняется ли первая строка файла или название файла

    if (is_first_file_string) out += "field of sort: ";
    switch (start_field) { // вывод информации по индексу
        case type_field:
            if (is_first_file_string) out += "Type |";
            else out += "TYPE(";
            break;
        case date_field:
            if (is_first_file_string) out += "Date |";
            else out += "DATE(";
            break;
        case addr_field:
            if (is_first_file_string) out += "Address |";
            else out += "ADDR(";
            break;
        case orgname_field:
            if (is_first_file_string) out += "Organisation name |";
            else out += "ORG_NAME(";
            break;
        case surname_field:
            if (is_first_file_string) out += "Chief surname |";
            else out += "CHIEF_SUR(";
            break;
    }

    // блок операторов, для добавления в названия направления, по которому проводилась сортировка
    if (is_first_file_string) out += " direction of sorting is: ";

    if (sort_direction == "from smaller to larger")  {
        if (is_first_file_string) out += "From smaller to larger\n\n";
        else out += "L)-";
    }
    else if (sort_direction == "from larger to smaller") {
        if (is_first_file_string) out += "From larger to smaller\n\n";
        else out += "R)-";
    }
}

void create_file(const std::vector<TData_unit>& data, int start_field, std::string& sorted_file_name, const std::string& sort_direction) {
    system("cls");
    TData_unit table_head("TYPE:", "DATE:", "ADDRESS:", "CHIEF SURNAME:", "ORGANISATION NAME:"); // структура создана для стандартизированного вывода заголовка таблицы
    std::string first_file_string = "";
    //TData_unit table_board("-----------------|", "-----------|", "----------------------------------|", "------------------------|", "-----------------|");

    std::string path = get_path_to_files().substr(0, get_path_to_files().length()-5) + "Sorted Files"; // получение пути до папки Sorted Files
    std::filesystem::create_directories(path);                                                         // и ее создание по этому пути

    std::string name_type_sort = "\\SortBy"; // переменная названия файла

    type_sort_enter(sort_direction, start_field, name_type_sort); // получение параметров сортировки в виде строки, добавляемой в название файла

    sorted_file_name = path + name_type_sort + make_date() + ".txt"; // получение полного названия файла через конкатенацию пути, названия, даты и времени создания и расширения
    std::ofstream fout(sorted_file_name);  // создания файла для заполнения по полному названию

    type_sort_enter(sort_direction, start_field, first_file_string, true); // получение параметров сортировки в виде строки, выводимой в файл
    fout << first_file_string;

    // вывод заголовка таблицы
    print_to_file (fout, table_head, start_field, true);
    table_print(fout, start_field);

    // вывод отсортированного массива структур в файл
    for (auto el: data) {
        print_to_file(fout, el, start_field);
    } table_print(fout, start_field);

    fout.close();
}