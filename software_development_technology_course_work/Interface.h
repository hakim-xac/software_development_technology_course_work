#pragma once
#include "include_modules.h"
#include "benchmark.h"

namespace COURSE_WORK {   



    template <typename Type
        , typename = std::enable_if_t <std::is_same_v<Type, std::string>>>
        class Interface
    {
                                        /* �����-��������� */
        /// ����� �������� � ������� ������ � ���������� ������ � �����, � ���� ��������������� �����.

        /*	������������ ������ ������	*/
        int widthTable;
        std::queue<Type> buffer;
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
            , �������� ����������� 
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
            if (str.size() > widthTable) 
                throw std::exception(std::string("����� ������ ������ ��������� �������!\t" + std::to_string(widthTable)).c_str());
            


            auto first_offset{ str.size() % 2 ? 0 : 1 };

            if (str2.empty()) {
                insertInString(tmp, (widthTable >> 1) + first_offset - (str.size() >> 1) - 1, str);
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
            if (to.empty())						throw std::exception("������ �����!");
            if (to.size() < pos)				throw std::exception("������ ������ ������ ������� �������!");
            if (to.size() < in.size())			throw std::exception("������ ����� ������ ����������� ������ !");
            if ((to.size() - pos) < in.size())	throw std::exception("������ ����� ������ ����������� ������ !");

            to.replace(pos, in.size(), in);
        }


        /* �������  � �����, ������ elem */
        template <typename T>
        constexpr void
        toBuffer(T&& elem) {
            buffer.emplace(std::forward<T>(elem));
        }


        /* "��������" � ����� out - ����� */
        constexpr void 
        flush() {
            while (!buffer.empty())
            {
                out << buffer.front() << "\n";
                buffer.pop();
            }
        }


        /* ������� ����� */
        constexpr void clearBuffer() {
            std::queue<Type> tmp;
            buffer.swap(tmp);
        }


        /* ������� ����������� */
        template <typename... Args>
        constexpr void
        to_log(Args&&... args) {
            err << delimiter(' ') << "\n";
            err << delimiter('-') << "\n";
            err << generatingString("������!!!") << "\n";
            ((err << generatingString(std::forward<Args>(args), "", ' ') << "\n"), ...);
            err << delimiter('-') << "\n";
        }


    protected:                                              /* PROTECTED SECTIONS */

        /* ����������� */
        Interface(int width, std::istream& in, std::ostream& out, std::ostream& err)
            : widthTable(width)
            , buffer()
            , in(in)
            , out(out)
            , err(err) {}

        /* ���������� */
        ~Interface() {}


        /* �������� ������ �� ����� */
        constexpr void
            showError(ErrorCodes key) {
            static std::map<ErrorCodes, std::string> base{
                {   ErrorCodes::FileOpenError,      "������ �������� �����"                     }
                , { ErrorCodes::FileReadError,      "������ ������ �����"                       }
                , { ErrorCodes::FileEmpty,          "����, �� �������� ���������"               }
                , { ErrorCodes::FileParseError,     "���� �������� �� �������� ���������"       }
                , { ErrorCodes::StackOverflow,      "������������ ����!"                        }
                , { ErrorCodes::DivideOrModByZero,  "������� �� ���� ���������!"                }
                , { ErrorCodes::IsInfinity,         "��������� ��������� ����� �������������!"  }
                , { ErrorCodes::ConversionError,    "������ �������������� ����!"               }
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
        showBody(VecString& con, Sz fileSize, long double resultExpression) {
            auto it{ con.begin() };
            toBuffer(delimiter(' '));
            toBuffer(delimiter());
            toBuffer(generatingString("���������:"));
            toBuffer(delimiter('-'));
            while ( it != con.end() ) {
                if ((*it).size() > widthTable) {
                    clearBuffer();
                    showError(ErrorCodes::StackOverflow);
                    return;
                }
                toBuffer(generatingString(*it));
                ++it;
            }
            toBuffer(delimiter('-'));
            toBuffer(generatingString("��������� ���������:"));
            toBuffer(delimiter('-'));
            toBuffer(delimiter(' '));

            auto [resultExpressionString, isConvert] = to_string(resultExpression);
            if (!isConvert) {
                clearBuffer();
                showError(ErrorCodes::ConversionError);
                return;
            }
            auto count{ resultExpressionString.size() / widthTable };
            if (count) {
                for (decltype(count) i{ 0 }, i_end{ count + 1 }; i < i_end; ++i) {
                    std::string result;
                    auto page{ widthTable * i };
                    auto begin{ resultExpressionString.begin() + page };
                    auto offset{ resultExpressionString.end() - begin };
                    auto end{ begin + (offset > widthTable ? widthTable : offset) };
                    if (resultExpressionString.end() < end) end = resultExpressionString.end();
                    std::copy(begin, end, std::back_inserter(result));
                    toBuffer(generatingString(result));
                }
            }
            else {
                toBuffer(generatingString(resultExpressionString));
            }
            
            toBuffer(delimiter(' '));
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


        /* ������� ���������� � ������� ������ ���������, ����� ����� */
        template <typename T>
        constexpr void
            showBenchmark(T&& start, T&& end) noexcept {
            toBuffer(delimiter('-'));
            toBuffer(delimiter(' '));
            auto diff{ end.diffirence(std::forward<T>(start)) };
            toBuffer(generatingString("����� ������ ���������:", std::to_string(diff.count()) + std::string("ms")));

            toBuffer(delimiter(' '));
            toBuffer(delimiter('-'));
            flush();
        }


        /* ��������������� ��� T � ������ */
        template <typename T>
        constexpr std::pair<std::string, bool>
            to_string(T&& type) {
            std::ostringstream ss_T;
            ss_T << std::forward<T>(type);
            if (ss_T.fail()) return { std::string(""), false };
            return { ss_T.str(), true };
        }
    };
}
