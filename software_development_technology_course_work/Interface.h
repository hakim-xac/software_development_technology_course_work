#pragma once
#include "include_modules.h"
#include "benchmark.h"

namespace COURSE_WORK {   



    template <typename Type
        , typename = std::enable_if_t <std::is_same_v<Type, std::string>>>
        class Interface
    {
                                        /* Класс-Интерфейс */
        /// класс работает с потоком вывода и отправляет данные в поток, в виде форматированных строк.

        /*	максимальная ширина строки	*/
        int widthTable;
        std::queue<Type> buffer;
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
            , оператор перемещения 
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
            if (str.size() > widthTable) 
                throw std::exception(std::string("Длина строки больше заданного размера!\t" + std::to_string(widthTable)).c_str());
            


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


        /* вставляет в строку to в позицию pos, строку in, замещая собой символы строки to */
        constexpr void 
        insertInString(Type& to, std::size_t pos, const Type& in ) const {
            if (to.empty())						throw std::exception("Строка пуста!");
            if (to.size() < pos)				throw std::exception("Размер строки меньше позиции вставки!");
            if (to.size() < in.size())			throw std::exception("Строка донор меньше вставляемой строки !");
            if ((to.size() - pos) < in.size())	throw std::exception("Строка донор меньше вставляемой строки !");

            to.replace(pos, in.size(), in);
        }


        /* создает  в стеке, строку elem */
        template <typename T>
        constexpr void
        toBuffer(T&& elem) {
            buffer.emplace(std::forward<T>(elem));
        }


        /* "печатает" в поток out - буфер */
        constexpr void 
        flush() {
            while (!buffer.empty())
            {
                out << buffer.front() << "\n";
                buffer.pop();
            }
        }


        /* очищает буфер */
        constexpr void clearBuffer() {
            std::queue<Type> tmp;
            buffer.swap(tmp);
        }


        /* функция логирования */
        template <typename... Args>
        constexpr void
        to_log(Args&&... args) {
            err << delimiter(' ') << "\n";
            err << delimiter('-') << "\n";
            err << generatingString("ОШИБКА!!!") << "\n";
            ((err << generatingString(std::forward<Args>(args), "", ' ') << "\n"), ...);
            err << delimiter('-') << "\n";
        }


    protected:                                              /* PROTECTED SECTIONS */

        /* конструктор */
        Interface(int width, std::istream& in, std::ostream& out, std::ostream& err)
            : widthTable(width)
            , buffer()
            , in(in)
            , out(out)
            , err(err) {}

        /* деструктор */
        ~Interface() {}


        /* Печатаем ошибку на вывод */
        constexpr void
            showError(ErrorCodes key) {
            static std::map<ErrorCodes, std::string> base{
                {   ErrorCodes::FileOpenError,      "Ошибка открытия файла"                     }
                , { ErrorCodes::FileReadError,      "Ошибка чтения файла"                       }
                , { ErrorCodes::FileEmpty,          "Файл, не содержит выражений"               }
                , { ErrorCodes::FileParseError,     "Файл содержит не валидное выражение"       }
                , { ErrorCodes::StackOverflow,      "Переполнение типа!"                        }
                , { ErrorCodes::DivideOrModByZero,  "Деление на ноль запрещено!"                }
                , { ErrorCodes::IsInfinity,         "Результат выражения равен бесконечности!"  }
                , { ErrorCodes::ConversionError,    "Ошибка преобразования типа!"               }
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
        showBody(VecString& con, Sz fileSize, long double resultExpression) {
            auto it{ con.begin() };
            toBuffer(delimiter(' '));
            toBuffer(delimiter());
            toBuffer(generatingString("Выражение:"));
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
            toBuffer(generatingString("Результат выражения:"));
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


        /* Выводит информацию о времени работы программы, через буфер */
        template <typename T>
        constexpr void
            showBenchmark(T&& start, T&& end) noexcept {
            toBuffer(delimiter('-'));
            toBuffer(delimiter(' '));
            auto diff{ end.diffirence(std::forward<T>(start)) };
            toBuffer(generatingString("Время работы программы:", std::to_string(diff.count()) + std::string("ms")));

            toBuffer(delimiter(' '));
            toBuffer(delimiter('-'));
            flush();
        }


        /* преобразовывает тип T в строку */
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
