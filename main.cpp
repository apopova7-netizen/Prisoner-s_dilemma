#include <iostream>

#include "Common.h"
#include "Input_data_processing.h"



int main(int argc, char *argv[]) {
    ParsedInputData parsed_data = parse_input_data(argc, argv);;// распарс. входные данные
    //ловим исключения
    //если что-то не так завершаем
    ValidatedInputData validated_data = validate_input_data(parsed_data);
    //ловим исключения
    //если что-то не так завершаем
    if (validated_data.matrix_file != "")
        GameMatrix game_matrix = load_game_matrix(validated_data.matrix_file);
        //ловим исключения, в случае неудачи выходи из программы совсем.
    return 0;
}

