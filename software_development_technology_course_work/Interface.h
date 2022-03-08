#pragma once
#include "include_modules.h"
//#include "Calculate.h"

namespace COURSE_WORK {   



    template <typename Type
        , typename = std::enable_if_t <std::is_same_v<Type, std::string>>>
        class Interface
    {
                                        /* Класс-Интерфейс */

        /*	максимальная ширина строки	*/
        static constexpr const size_t widthTable{ 100 };
        std::queue<Type> buffer;
        std::queue<Type> calcBuffer;
        Type filename;
    protected:                                               /* PROTECTED SECTIONS */

        std::istream& in;
        std::ostream& out;
        std::ostream& err;
        
    private:                                                /* PRIVATE SECTIONS */

        /* Запрещаем:
            пустой конструктор
            , конструктор копирования
            , конструктор перемещения
            , оператор копирования
            , опретор перемещения 
        */
        Interface() = delete;
        Interface(const Interface&) = delete;
        Interface(const Interface&&) = delete;
        Interface& operator = (const Interface&) = delete;
        Interface& operator = (const Interface&&) = delete;


        /* строка-разделитель */
        constexpr Type 
        delimiter(char del = '=') const {
            Type result{ Type(widthTable, del) };
            result.front() = result.back() = '#';
            return result;
        }

        /*  создает строку по определенному формату */
        //  принимает 1 левый параметр 
        constexpr Type
            generatingString(Type& str
                , char del = ' '
                , FormatingType ft = FormatingType::JUSTIFY) {
            return generatingString(std::move(str), Type(""), del, ft);
        }

        /*  создает строку по определенному формату */
        //  принимает 2 правых параметра 
        constexpr Type
        generatingString(Type&& str
            , Type&& str2 = Type("")
            , char del = ' '
            , FormatingType ft = FormatingType::JUSTIFY) {

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
        insertInString(Type& to, std::size_t pos, const Type& in ) const {
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


    protected:                                              /* PROTECTED SECTIONS */


        /* Печатаем ошибку на вывод */
        constexpr void
            showError(ErrorCodes key) {
            static std::map<ErrorCodes, std::string> base{
                {   ErrorCodes::FileOpenError,      "Ошибка открытия файла"                 }
                , { ErrorCodes::FileReadError,      "Ошибка чтения файла"                   }
                , { ErrorCodes::FileEmpty,          "Файл, не содержит выражений"           }
                , { ErrorCodes::FileParseError,     "Файл содержит не валидное выражение"   }
                , { ErrorCodes::StackOverflow,      "Переполнение типа!"                    }
                , { ErrorCodes::DivideOrModByZero,  "Деление на ноль запрещено!"            }
            };
            if (base.find(key) == base.end()) {
                to_log("Неизвестная ошибка!");
                return;
            } 
            to_log(std::move(base[key]));            
        }


        /* Принимаем данные от пользователя (путь к файлу с выражением постфиксной (польской) формой записи) */
        constexpr Type
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


        /* Печатает в вывод выражение и его результат */
        template <typename VecString, typename Sz, typename = std::enable_if_t<is_container_v<VecString>>>
        constexpr void
        showBody(VecString& con, Sz fileSize, double resultExpression) {
            auto it{ con.begin() };
            toBuffer(delimiter(' '));
            toBuffer(delimiter());
            toBuffer(generatingString("Выражение:"));
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
            toBuffer(generatingString("Результат выражения:"));
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
            toBuffer(generatingString("Размер файла:", std::to_string(fileSize)));
            toBuffer(delimiter());

            flush();
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
