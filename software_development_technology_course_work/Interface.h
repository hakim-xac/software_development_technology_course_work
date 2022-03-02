#pragma once
#include "include_modules.h"
//#include "Calculate.h"

namespace COURSE_WORK {
    /// <summary>
    /// namespace COURSE_WORK
    /// </summary>



    // ��������.. �������� �� ��������������
    template <typename T, typename = void>
    struct is_iterable : std::false_type {};

    // this gets used only when we can call std::begin() and std::end() on that type
    template <typename T>
    struct is_iterable<T
        , std::void_t<decltype(std::begin(std::declval<T>()))
        , decltype(std::end(std::declval<T>()))
        >> : std::true_type {};

    // Here is a helper:
    template <typename T>
    constexpr bool is_iterable_v = is_iterable<T>::value;


    /// <summary>
    /// 
    /// </summary>
    /// <typeparam name="Type"></typeparam>
    /// <typeparam name=""></typeparam>

    template <typename Type
        , typename = std::enable_if_t <std::is_same_v<Type, std::string>>>
        class Interface
    {





        /*	maxWidthTable	*/
        static constexpr const size_t widthTable{ 100 };
        std::istream& in;
        std::ostream& out;
        std::ostream& err;
        std::queue<Type> buffer;
        std::queue<Type> calcBuffer;
        Type filename;

        
    private:

        Interface() = delete;
        Interface(const Interface&) = delete;
        Interface(const Interface&&) = delete;
        Interface& operator = (const Interface&) = delete;
        Interface& operator = (const Interface&&) = delete;

        /* ����������� */
        constexpr Type 
        delimiter(char del = '=') {
            Type result{ Type(widthTable, del) };
            result.front() = result.back() = '#';
            return result;
        }


        /*  ������� ������ �� ������������� ������� */
        constexpr Type 
        generatingString( Type&& str
                        , Type&& str2 = Type("")
                        , char del = '='
                        , FormatingType ft = FormatingType::STANDART) {

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
                insertInSitring(tmp, (widthTable >> 1) + first_offset - (str.size() >> 1), str);
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
        constexpr void 
        insertInSitring(Type& to, std::size_t pos, const Type& in ) {
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


    protected:


        /* ��������� ������ �� ������������ */
        constexpr void
        showError(ErrorCodes key) {
            static std::map<ErrorCodes, std::string> base{
                {ErrorCodes::FileOpenError, "������ �������� �����" },
                {ErrorCodes::FileReadError, "������ ������ �����" }
            };
            if (base.find(key) == base.end()) {
                to_log("����������� ������!");
                return;
            }
            to_log(std::move(base[key]));
        }

        /* */
        constexpr void
            showBody() {

        }

        /* ��������� ������ �� ������������ (���� � ����� � ���������� ����������� (��������) ������ ������) */
        constexpr std::decay_t<Type>
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


        /*  */
        template <typename Iter, typename = std::enable_if_t<is_iterable<Iter>>>
        constexpr void
        toCaclBuffer(Iter begin, Iter end) {
            auto it{ begin };
            while (it != end) {
                calcBuffer.push(*it);
            }
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
