#pragma once
#include "include_modules.h"
//#include "Calculate.h"

namespace COURSE_WORK {
    /// <summary>
    /// namespace COURSE_WORK
    /// </summary>

    


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
        std::queue<Type> buffer;
        std::queue<Type> calcBuffer;
        Type filename;
    protected:

        std::istream& in;
        std::ostream& out;
        std::ostream& err;
        
    private:

        Interface() = delete;
        Interface(const Interface&) = delete;
        Interface(const Interface&&) = delete;
        Interface& operator = (const Interface&) = delete;
        Interface& operator = (const Interface&&) = delete;

        /* разделитель */
        constexpr Type 
        delimiter(char del = '=') {
            Type result{ Type(widthTable, del) };
            result.front() = result.back() = '#';
            return result;
        }


        /*  создает строку по определенному формату */
        constexpr Type 
        generatingString( Type&& str
                        , Type&& str2 = Type("")
                        , char del = '='
                        , FormatingType ft = FormatingType::STANDART) {

            /// <param name="first_str">первая строка, располагается слева (или по центру, если 2 строка пуста)</param>
            /// <param name="second_str"> правая строка</param>
            /// <param name="del"> разделитель </param>
            /// <param name="ft"> FormatingType::STANDART - равномерно от центра </param>
            /// <param name="ft"> FormatingType::JUSTIFY - приклеено по краям </param>
            /// <returns> форматированная строка</returns>


            auto tmp{ Type(widthTable, del) };
            tmp.front() = tmp.back() = '#';
            if (str.empty()) return tmp;						// если str пусто, равносильно delimiter
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


        /* вставляет в строку to в позицию pos, строку in, замещая собой символы строки to */
        constexpr void 
        insertInString(Type& to, std::size_t pos, const Type& in ) {
            if (to.empty())						throw;
            if (to.size() < pos)				throw;
            if (to.size() < in.size())			throw;
            if ((to.size() - pos) < in.size())	throw;

            to.replace(pos, in.size(), in);
        }


        /* создает  в стеке, строку elem */
        template <typename T>
        constexpr void
        toBuffer(T&& elem) {
            buffer.emplace(std::forward<T>(elem));
        }


        /* "печатает" в поток out, буфер */
        constexpr void 
        flush() {
            while (!buffer.empty())
            {
                out << buffer.front() << "\n";
                buffer.pop();
            }
        }


        /* функция логирования */
        template <typename... Args>
        constexpr void
        to_log(Args&&... args) {
            err << delimiter('-') << "\n";
            ((err << generatingString(std::forward<Args>(args), "", ' ') << "\n"), ...);
            err << delimiter('-') << "\n";
        }


    protected:


        /* Принимаем данные от пользователя */
        constexpr void
        showError(ErrorCodes key) {
            static std::map<ErrorCodes, std::string> base{
                {ErrorCodes::FileOpenError, "Ошибка открытия файла" },
                {ErrorCodes::FileReadError, "Ошибка чтения файла" }
            };
            if (base.find(key) == base.end()) {
                to_log("Неизвестная ошибка!");
                return;
            }
            to_log(std::move(base[key]));
        }

        /* */
        constexpr void
        showBody() {


        }

        /* Принимаем данные от пользователя (путь к файлу с выражением постфиксной (польской) формой записи) */
        constexpr std::decay_t<Type>
        inputRead() {
            Type filename{};
            out << delimiter(' ') << "\n";
            out << generatingString("ВВЕДИТЕ ПУТЬ К ФАЙЛУ:") << "\n";
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


        /* Создает заголовок и отправляет в буфер */
        constexpr void
            showHeader() {
            toBuffer(delimiter());
            toBuffer(generatingString("Группа", "ПБ-21", ' ', FormatingType::JUSTIFY));
            toBuffer(delimiter('-'));
            toBuffer(generatingString("Студент:", "Хакимов А.С.", ' ', FormatingType::JUSTIFY));
            toBuffer(delimiter('-'));
            toBuffer(generatingString("Курсовая работа", "Технологии разработки ПО", ' ', FormatingType::JUSTIFY));
            toBuffer(delimiter('-'));
            toBuffer(generatingString("Вариант №:", "1", ' ', FormatingType::JUSTIFY));
            toBuffer(delimiter('-'));
            toBuffer(generatingString("Тема", "Калькулятор для выражений с постфиксной (польской) формой записи", ' ', FormatingType::JUSTIFY));
            toBuffer(delimiter());

            flush();
        }
    public:


        /* конструктор */
        Interface(std::istream& in, std::ostream& out, std::ostream& err)
        : in(in)
        , out(out)
        , err(err)
        , buffer() {}

                
        /* деструктор */
        ~Interface() {}




        
    };
}
