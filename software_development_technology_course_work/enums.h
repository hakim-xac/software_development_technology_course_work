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
    , FileOpenError
    , FileReadError
    , FileParseError
    , StackOverflow
    , DivideOrModByZero
};


/* Коды ошибок, используемые в проекте */
enum class ErrorOperations {
    Plus
    , Minus
    , Multiply
    , Divide
    , Sin
    , Cos
};

