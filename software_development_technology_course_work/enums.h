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
    , FileNotValid
    , FileOpenError
    , FileReadError
    , FileParseError
};

