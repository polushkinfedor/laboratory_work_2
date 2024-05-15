#include "menu.h"

// выбор настроек сортировки
void choose_file (std::string & path_file, const std::string& path_to_dir) {
    std::string exit_return = "-1";
    bool need_exit = false;
    do { // пока пользователь не введет доступное число или не захочет выйти, мы будем запрашивать ввод
        system("cls");
        std::string user_input;
        std::cout << "You choose report organisation file:\n";

        int counter = 0;
        std::vector<std::filesystem::path> file_names;

        for (const auto & entry: std::filesystem::directory_iterator(path_to_dir)) { // получение массива путей до файлов вида Report_File_Organisation
            if (entry.path().string().find("Report_File_Organisation") != -1) {
                file_names.push_back(entry.path());
                counter++;
            }
        }

        if (counter == 0) {
            error_with_quanity_files(path_to_dir);
            return; // выход из функции при ошибке количества файлов
        }

        for (int i = 0; i < counter; i++) { // вывод массива путей до файлов с их порядковым номером
            std::cout << i + 1 << ": ";
            set_color(file_names[i].string(), serv_yellow);
            std::cout << std::endl;
        }
        std::cout << "0 -- "; // 0 для выхода из программы
        set_color("to exit\n", serv_red);


        getline(std::cin, user_input);
        user_input = delete_extraspace_string(user_input);

        if (user_input == "0") {
            need_exit = true;
        }

        if (number_correct(user_input)) { // проверка корректности введенного пользователем номера файла
            stoi(user_input);
            if (stoi(user_input) < counter + 1 and stoi(user_input) > 0) {
                path_file = file_names[stoi(user_input) - 1].string();
                need_exit = true;
            }
        }
    } while (need_exit != true);
}

void choose_direction(std::string& sort_direction) {
    system("cls");
    set_color("Select direction of sorting:\n\n", serv_yellow);
    std::cout << "l -- from smaller to larger\n";
    std::cout << "r -- from larger to smaller\n";
    std::cout << "\n0 -- to go back\n";
    char corr_dir;
    bool correct_enter = false;

    do {
        correct_enter = true;
        corr_dir = _getch();
        if (tolower(corr_dir) == 'l') {
            sort_direction = "from smaller to larger";
        } else if (tolower(corr_dir) == 'r') {
            sort_direction = "from larger to smaller";
        } else if (corr_dir != '0') {
            correct_enter = false;
        }
    }
    while (correct_enter != true);
}

void choose_field(int& field_for_sort) {
    system("cls");
    set_color("Select one field that will be the reference for sorting:\n\n", serv_yellow);
    std::cout << "1 -- type of correspondence";
    std::cout << "\t2 -- date of sending\n";
    std::cout << "3 -- address of organisation";
    std::cout << "\t4 -- chief surname\n";
    std::cout << "5 -- name of organisation\n";
    std::cout << "\n0 -- to go back\n";

    char field_num;
    bool correct_enter = false;

    do {
        correct_enter = true;
        field_num = _getch();
        if (field_num == '1') {
            field_for_sort = type_field;
        } else if (field_num == '2') {
            field_for_sort = date_field;
        } else if (field_num == '3') {
            field_for_sort = addr_field;
        } else if (field_num == '4') {
            field_for_sort = surname_field;
        } else if (field_num == '5') {
            field_for_sort = orgname_field;
        } else if (field_num == '0') {

        } else {
            correct_enter = false;
        }
    } while (correct_enter != true);
}

// сообщения об ошибке нахождения файлов
void error_with_quanity_files(const std::string& path_to_dir) {
    system("cls");
    set_color("Error\n", serv_red);
    std::cout << "you havent any file format ";
    set_color("Report_File_Organisation_DD_MM_YYYY_TT-TT-TT\n", serv_yellow);
    std::cout << "on path:";
    set_color(path_to_dir, serv_blue);
    std::cout << "\nplease, add files. To continue enter any key";
    _getch();
}

//информационные сообщения
void sort_message(const std::string& ans) {
    system ("cls");
    if (ans == "input_file_error") {
        std::cout << "File wasn't find. Please, make sure that file exist";
    } else if (ans == "incorrect_settings") {
        std::cout << "Settings is incorrect. Please, make sure that all settings was selected";
    } else if (ans == "incorrect_data") {
        std::cout << "Choosed file contained incorrect data.";
    }
    else {
        std::cout << "file ";
        set_color(ans, serv_blue);
        std::cout << " created.\n";
        std::cout << "Sorted ended successfull.\nTo continue enter any key";
    }
    _getch();
}

void menu_message(std::string& path_to_file, std::string& sort_direction, int field) {
    system("cls");
    std::string field_for_sort;
    switch (field) {
        default:
            field_for_sort = "-";
            break;
        case type_field:
            field_for_sort = "type";
            break;
        case date_field:
            field_for_sort = "date";
            break;
        case addr_field:
            field_for_sort = "address";
            break;
        case surname_field:
            field_for_sort = "chief surname";
            break;
        case orgname_field:
            field_for_sort = "organisation name";
            break;
    }

    std::cout << "You are working with program for sorting files ";
    set_color("Report_File_Organisation_DD_MM_YYYY_TT-TT-TT\n", serv_yellow);
    std::cout << "They must located on path ";
    set_color(get_path_to_files(), serv_yellow);
    std::cout << "\nNow choose file: ";
    set_color(path_to_file, serv_blue);
    std::cout << "\nDirection of sort: ";
    set_color(sort_direction, serv_blue);
    std::cout << "\nField for sort: ";
    set_color(field_for_sort, serv_blue);
    std::cout << "\n\nto start sorting with choosen settings enter: ";
    set_color("/start\n", serv_green);
    std::cout << "to exit enter: ";
    set_color("/exit\n", serv_green);
    set_color("\n\nSETTINGS\n", serv_green);
    std::cout << "-- To choose file enter ";
    set_color("1\n", serv_green);
    std::cout << "-- To choose direction of sort enter ";
    set_color("2\n", serv_green);
    std::cout << "-- To choose field for sort enter ";
    set_color("3\n", serv_green);
    std::cout << "-- To clear settings enter ";
    set_color("/clear\n", serv_green);
}

// функция менеджмента программы
void menu_manager() {
    bool program_end = false;
    std::string menu_position;
    std::string path_to_dir = get_path_to_files();

    std::string path_to_file = "-";
    std::string sort_direction = "-";
    int field_for_sort = -1;

    do {
        menu_message(path_to_file, sort_direction, field_for_sort); // функция, выводящая служебную информацию

        std::getline(std::cin, menu_position); // получение команды о взаимодействии пользователя с программой
        str_tolower(menu_position);

        if (menu_position == "1") {
            choose_file(path_to_file, path_to_dir);
        }
        else if (menu_position == "2") {
            choose_direction(sort_direction);
        }
        else if (menu_position == "3") {
            choose_field(field_for_sort);
        }
        else if (menu_position == "/start") {
            std::string ans = sorting_func(path_to_file, sort_direction, field_for_sort);
            sort_message(ans);
        }
        else if (menu_position == "/clear") {
            path_to_file = "-";
            sort_direction = "-";
            field_for_sort = -1;
        }
        else if (menu_position == "/exit") {
            program_end = true;
        }
    } while (not(program_end) == true);
}
