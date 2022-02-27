#pragma once
#include "include_modules.h"

namespace COURSE_WORK {
    /// <summary>
    /// namespace COURSE_WORK
    /// </summary>
    template <typename Type
            , typename = std::enable_if_t<std::is_same_v<Type, std::string>>>
    class Interface
    {
        /*	widthTable	*/
        static constexpr const size_t widthTable{ 100 };
        std::istream& in;
        std::ostream& out;
        std::queue<std::string> buffer;

        
    private:

        Interface(const Interface&) = delete;
        Interface(const Interface&&) = delete;
        Interface& operator = (const Interface&) = delete;
        Interface& operator = (const Interface&&) = delete;


        /* ����������� */
        std::string delimiter(char del = '=') {
            std::string result{ std::string(widthTable, del) };
            result.front() = result.back() = '#';
            return result;
        }


        /*  ������� ������ �� ������������� ������� */
        std::string generatingString( std::string&& str 
                                    , std::string&& str2 = ""
                                    , char del = '='
                                    , FormatingType ft = FormatingType::STANDART) {

            /// <param name="first_str">������ ������, ������������� ����� (��� �� ������, ���� 2 ������ �����)</param>
            /// <param name="second_str"> ������ ������</param>
            /// <param name="del"> ����������� </param>
            /// <param name="ft"> FormatingType::STANDART - ���������� �� ������ </param>
            /// <param name="ft"> FormatingType::JUSTIFY - ��������� �� ����� </param>
            /// <returns> ��������������� ������</returns>


            auto tmp{ std::string(widthTable, del) };
            tmp.front() = tmp.back() = '#';
            if (str.empty()) return tmp;						// ���� str �����, ����������� delimiter
            if (str.size() > widthTable) throw;


            auto first_offset{ str.size() % 2 ? 0 : 1 };

            if (str2.empty()) {
                insertInSitring(tmp, (widthTable / 2) + first_offset, str);
            }
            else {

                if (str.size() + str2.size() > widthTable) throw;
                if (ft == FormatingType::STANDART) {
                    auto first_pos{ widthTable / 3 };
                    insertInSitring(tmp, first_pos + first_offset - (str.size() >> 1), str);
                    auto second_offset{ str2.size() % 2 ? 0 : 1 };
                    insertInSitring(tmp, first_pos + first_pos - (str2.size() >> 1) + second_offset, str2);
                }
                else {
                    insertInSitring(tmp, 5, str);
                    insertInSitring(tmp, widthTable - str2.size() - 5, str2);
                }
            }
            return tmp;
            
        }


        /* ��������� � ������ to � ������� pos, ������ in, ������� ����� ������� ������ to */
        void insertInSitring(std::string& to, std::size_t pos, const std::string& in ) {
            if (to.empty())						throw;
            if (to.size() < pos)				throw;
            if (to.size() < in.size())			throw;
            if ((to.size() - pos) < in.size())	throw;

            to.replace(pos, in.size(), in);
        }


        /* �������  � �����, ������ elem */
        template <typename T>
        void toBuffer(T&& elem) {
            buffer.emplace(std::move(elem));
        }


        /* "��������" � ����� out, ����� */
        void showBuffer() {
            while (!buffer.empty())
            {
                out << buffer.front() << "\n";
                buffer.pop();
            }
        }


        /* ������� ��������� � ���������� � ����� */
        void showHeader() {
            toBuffer(delimiter());
            toBuffer(generatingString(std::string("������"), std::string("��-21"), ' ', FormatingType::JUSTIFY));
            toBuffer(delimiter('-'));
            toBuffer(generatingString(std::string("�������:"), std::string("������� �.�."), ' ', FormatingType::JUSTIFY));
            toBuffer(delimiter('-'));
            toBuffer(generatingString(std::string("�������� ������"), std::string("���������� ���������� ��"), ' ', FormatingType::JUSTIFY));
            toBuffer(delimiter('-'));
            toBuffer(generatingString(std::string("������� �:"), std::string("1"), ' ', FormatingType::JUSTIFY));
            toBuffer(delimiter('-'));
            toBuffer(generatingString(std::string("����"), std::string("����������� ��� ��������� � ����������� (��������) ������ ������"), ' ', FormatingType::JUSTIFY));
            toBuffer(delimiter('-'));
        }


    public:


        /* ����������� */
        Interface()
        : in(std::cin)
        , out(std::cout)
        , buffer() {}


        /* ����������� */
        Interface(std::istream& in, std::ostream& out)
        : in(in)
        , out(out)
        , buffer() {}


        /* ���������� */
        ~Interface() {}


        /* ������� �������-����, ����������� ���� � ����� � ���������� � ��������� ���, ���� ��� ��� ������ */
        void loop() {
            std::ios::sync_with_stdio(0);
            setlocale(LC_ALL, "Russian");
            showHeader();
            //showBody();


            showBuffer();
        }

        
    };
}
