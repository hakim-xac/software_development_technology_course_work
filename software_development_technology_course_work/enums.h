#pragma once

/* Определяет формат выводимой строки из функций, которые используют данное перечисление */
enum class FormatingType{
    STANDART, 	JUSTIFY
};


/* Коды ошибок, используемые в проекте */
enum class ErrorCodes {
    AllGood
    , FileIsOpen
    , FileEmpty
    , FileNotValid
    , FileOpenError
    , FileReadError
    , FileParseError
};

