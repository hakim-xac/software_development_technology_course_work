#pragma once
#include "include_modules.h"
//#include "Calculate.h"

namespace COURSE_WORK {   



    template <typename Type
        , typename = std::enable_if_t <std::is_same_v<Type, std::string>>>
        class Interface
    {
                                        /* �����-��������� */

        /*	������������ ������ ������	*/
        static constexpr const size_t widthTable{ 100 };
        std::queue<Type> buffer;
        std::queue<Type> calcBuffer;
        Type filename;
    protected:                                               /* PROTECTED SECTIONS */

        std::istream& in;
        std::ostream& out;
        std::ostream& err;
        
    private:                                                /* PRIVATE SECTIONS */

        /* ���������:
            ������ �����������
            , ����������� �����������
            , ����������� �����������
            , �������� �����������
            , ������� ����������� 
        */
        Interface() = delete;
        Interface(const Interface&) = delete;
        Interface(const Interface&&) = delete;
        Interface& operator = (const Interface&) = delete;
        Interface& operator = (const Interface&&) = delete;


        /* ������-����������� */
        constexpr Type 
        delimiter(char del = '=') const {
            Type result{ Type(widthTable, del) };
            result.front() = result.back() = '#';
            return result;
        }

        /*  ������� ������ �� ������������� ������� */
        //  ��������� 1 ����� �������� 
        constexpr Type
            generatingString(Type& str
                , char del = ' '
                , FormatingType ft = FormatingType::JUSTIFY) {
            return generatingString(std::move(str), Type(""), del, ft);
        }

        /*  ������� ������ �� ������������� ������� */
        //  ��������� 2 ������ ��������� 
        constexpr Type
        generatingString(Type&& str
            , Type&& str2 = Type("")
            , char del = ' '
            , FormatingType ft = FormatingType::JUSTIFY) {

            /// <param name="first_str">������ ������, ������������� ����� (��� �� ������, ���� 2 ������ �����)</param>
            /// <param name="second_str"> ������ ������</param>
            /// <param name="del"> ����������� </param>
            /// <param name="ft"> FormatingType::STANDART - ���������� �� ������ </param>
            /// <param name="ft"> FormatingType::JUSTIFY - ��������� �� ����� </param>
            /// <returns> ��������������� ������</returns>


            auto tmp{ Type(widthTable, del) };
            tmp.front() = tmp.back() = '#';
            if (str.empty()) return tmp;						// ���� str �����, ����������� delimiter
            if (str.size() > widthTable) throw;
            


            auto first_offset{ str.size() % 2 ? 0 : 1 };

            if (str2.empty()) {
                insertInString(tmp, (widthTable >> 1) + first_offset - (str.size() >> 1), str);
            }
            else {

                if (str.size() + str2.size() > widthTable) throw;
                if (ft == FormatingType::STANDART) {
                    auto first_pos{ widthTable / 3 };
                    insertInString(tmp, first_pos + first_offset - (str.size() >> 1), str);
                    auto second_offset{ str2.size() % 2 ? 0 : 1 };
                    insertInString(tmp, first_pos + first_pos - (str2.size() >> 1) + second_offset, str2);
                }
                else {
                    insertInString(tmp, 5, str);
                    insertInString(tmp, widthTable - str2.size() - 5, str2);
                }
            }
            return tmp;
            
        }


        /* ��������� � ������ to � ������� pos, ������ in, ������� ����� ������� ������ to */
        constexpr void 
        insertInString(Type& to, std::size_t pos, const Type& in ) const {
            if (to.empty())						throw;
            if (to.size() < pos)				throw;
            if (to.size() < in.size())			throw;
            if ((to.size() - pos) < in.size())	throw;

            to.replace(pos, in.size(), in);
        }


        /* �������  � �����, ������ elem */
        template <typename T>
        constexpr void
        toBuffer(T&& elem) {
            buffer.emplace(std::forward<T>(elem));
        }


        /* "��������" � ����� out, ����� */
        constexpr void 
        flush() {
            while (!buffer.empty())
            {
                out << buffer.front() << "\n";
                buffer.pop();
            }
        }


        /* ������� ����������� */
        template <typename... Args>
        constexpr void
        to_log(Args&&... args) {
            err << delimiter('-') << "\n";
            ((err << generatingString(std::forward<Args>(args), "", ' ') << "\n"), ...);
            err << delimiter('-') << "\n";
        }


    protected:                                              /* PROTECTED SECTIONS */


        /* �������� ������ �� ����� */
        constexpr void
            showError(ErrorCodes key) {
            static std::map<ErrorCodes, std::string> base{
                {   ErrorCodes::FileOpenError,      "������ �������� �����"                 }
                , { ErrorCodes::FileReadError,      "������ ������ �����"                   }
                , { ErrorCodes::FileEmpty,          "����, �� �������� ���������"           }
                , { ErrorCodes::FileParseError,     "���� �������� �� �������� ���������"   }
                , { ErrorCodes::StackOverflow,      "������������ ����!"                    }
                , { ErrorCodes::DivideOrModByZero,  "������� �� ���� ���������!"            }
            };
            if (base.find(key) == base.end()) {
                to_log("����������� ������!");
                return;
            } 
            to_log(std::move(base[key]));            
        }


        /* ��������� ������ �� ������������ (���� � ����� � ���������� ����������� (��������) ������ ������) */
        constexpr Type
        inputRead() {
            Type filename{};
            out << delimiter(' ') << "\n";
            out << generatingString("������� ���� � �����:") << "\n";
            out << delimiter(' ') << "\n";
            out << std::string(widthTable, '_');
            out << std::string(widthTable, '\b');            
            
            in >> filename;
            return filename;
        }


        /* �������� � ����� ��������� � ��� ��������� */
        template <typename VecString, typename Sz, typename = std::enable_if_t<is_container_v<VecString>>>
        constexpr void
        showBody(VecString& con, Sz fileSize, double resultExpression) {
            auto it{ con.begin() };
            toBuffer(delimiter(' '));
            toBuffer(delimiter());
            toBuffer(generatingString("���������:"));
            toBuffer(delimiter('-'));
            while ( it != con.end() ) {
                if ((*it).size() > widthTable) {
                    showError(ErrorCodes::StackOverflow);
                    return;
                }
                toBuffer(generatingString(*it));
                ++it;
            }
            toBuffer(delimiter('-'));
            toBuffer(generatingString("��������� ���������:"));
            toBuffer(delimiter('-'));

            if (static_cast<double>(static_cast<long>(resultExpression)) == resultExpression) {
                toBuffer(generatingString(std::to_string(static_cast<long>(resultExpression))));
            }
            else {
                std::string resultExpressionString{ std::to_string(resultExpression) };
                if (resultExpressionString.size() > widthTable) {
                    showError(ErrorCodes::StackOverflow);
                    return;
                }
                toBuffer(generatingString(resultExpressionString));
            }
            toBuffer(delimiter('-'));
            toBuffer(generatingString("������ �����:", std::to_string(fileSize)));
            toBuffer(delimiter());

            flush();
        }


        /* ������� ��������� � ���������� � ����� */
        constexpr void
            showHeader() {
            toBuffer(delimiter());
            toBuffer(generatingString("������", "��-21", ' ', FormatingType::JUSTIFY));
            toBuffer(delimiter('-'));
            toBuffer(generatingString("�������:", "������� �.�.", ' ', FormatingType::JUSTIFY));
            toBuffer(delimiter('-'));
            toBuffer(generatingString("�������� ������", "���������� ���������� ��", ' ', FormatingType::JUSTIFY));
            toBuffer(delimiter('-'));
            toBuffer(generatingString("������� �:", "1", ' ', FormatingType::JUSTIFY));
            toBuffer(delimiter('-'));
            toBuffer(generatingString("����", "����������� ��� ��������� � ����������� (��������) ������ ������", ' ', FormatingType::JUSTIFY));
            toBuffer(delimiter());

            flush();
        }
    public:


        /* ����������� */
        Interface(std::istream& in, std::ostream& out, std::ostream& err)
        : in(in)
        , out(out)
        , err(err)
        , buffer() {}

                
        /* ���������� */
        ~Interface() {}




        
    };
}
