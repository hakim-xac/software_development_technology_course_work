#pragma once

/* ���������� ������ ��������� ������ �� �������, ������� ���������� ������ ������������ */
enum class FormatingType{
    STANDART, 	JUSTIFY
};


/* ���� ������, ������������ � ������� */
enum class ErrorCodes {
    AllGood
    , FileIsOpen
    , FileEmpty
    , FileOpenError
    , FileReadError
    , FileParseError
};


/* ���� ������, ������������ � ������� */
enum class ErrorOperations {
    Plus
    , Minus
    , Multiply
    , Divide
    , Sin
    , Cos
};

