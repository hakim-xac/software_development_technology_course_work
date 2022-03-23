#pragma once

/* ���������� ������ ��������� ������ �� �������, ������� ���������� ������ ������������ */
enum class FormatingType{
    STANDART, 	JUSTIFY
};


/* ���� ������, ������������ � ��������� */
enum class ErrorCodes {
    AllGood
    , FileIsOpen
    , FileEmpty
    , FileOpenError
    , FileReadError
    , FileParseError
    , StackOverflow
    , DivideOrModByZero
    , IsInfinity
    , ConversionError
};

