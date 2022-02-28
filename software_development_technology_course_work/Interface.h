#pragma once
#include "include_modules.h"

namespace COURSE_WORK {
    /// <summary>
    /// namespace COURSE_WORK
    /// </summary>
    template <typename Type
        , typename = std::enable_if_t <std::is_same_v<Type, std::string>>>
        class Interface
    {
        /*	widthTable	*/
        static constexpr const size_t widthTable{ 100 };
        std::istream& in;
        std::ostream& out;
        std::ostream& err;
        std::queue<Type> buffer;

        
    private:

        Interface(const Interface&) = delete;
        Interface(const Interface&&) = delete;
        Interface& operator = (const Interface&) = delete;
        Interface& operator = (const Interface&&) = delete;


        /* разделитель */
        constexpr Type delimiter(char del = '=') {
            Type result{ Type(widthTable, del) };
            result.front() = result.back() = '#';
            return result;
        }


        /*  создает строку по определенному формату */
        constexpr Type generatingString(Type&& str
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


        /* вставляет в строку to в позицию pos, строку in, замещая собой символы строки to */
        constexpr void insertInSitring(Type& to, std::size_t pos, const Type& in ) {
            if (to.empty())						throw;
            if (to.size() < pos)				throw;
            if (to.size() < in.size())			throw;
            if ((to.size() - pos) < in.size())	throw;

            to.replace(pos, in.size(), in);
        }


        /* создает  в стеке, строку elem */
        template <typename T>
        constexpr void toBuffer(T&& elem) {
            buffer.emplace(std::forward<T>(elem));
        }


        /* "печатает" в поток out, буфер */
        constexpr void flush() {
            while (!buffer.empty())
            {
                out << buffer.front() << "\n";
                buffer.pop();
            }
        }


        /* Создает заголовок и отправляет в буфер */
        constexpr void showHeader() {
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


        /* Принимаем данные от пользователя */
        constexpr void showBody() {
            toBuffer(delimiter(' '));
            toBuffer(delimiter('-'));
            toBuffer(generatingString("Введите путь к файлу:", "", ' '));
            toBuffer(delimiter('-'));

            flush();
            Type tmp;
            in >> tmp;
            if (in.fail()) {
                to_log("Ошибка чтения из потока ввода!");
                return;
            }
            
            auto[queue, isRead] = readFile(tmp);
            if (!isRead) {
                to_log("[" + tmp + "]", "Ошибка чтение файла !!");
                return;
            }
            if (queue.empty()) {
                to_log("[" + tmp + " ]", "ФАЙЛ ПУСТ !");
            }

        }


        /* функция логирования */
        template <typename... Args>
        constexpr void to_log(Args&&... args) {
            err << delimiter('-') << "\n";
            ((err << generatingString(std::forward<Args>(args), "", ' ') << "\n"), ...);
            err << delimiter('-') << "\n";
        }


        /* Читает файл и возвращает пару очередь, и логическую операцию успешного чтения  */
        constexpr std::pair<std::queue<std::string>, bool> readFile(const Type& str) {
            std::ifstream fs{ str };
            std::queue<std::string> queue;
            if (!fs.is_open()) {
                return { queue, false };
            }
            std::string s;
            while (fs >> s) {
                queue.push(s);
            }  
            return { queue, true };
        }


    public:


        /* конструктор */
        Interface()
        : in(std::cin)
        , out(std::cout)
        , err(std::cerr)
        , buffer() {}


        /* конструктор */
        Interface(std::istream& in, std::ostream& out, std::ostream& err)
        : in(in)
        , out(out)
        , err(err)
        , buffer() {}


        /* деструктор */
        ~Interface() {}


        /* главная функция-член, запрашивает путь к файлу с выражением и выполняет его, если оно без ошибок */
        constexpr void loop() {
            std::ios::sync_with_stdio(0);
            setlocale(LC_ALL, "Russian");

            showHeader();
            showBody();
        }

        
    };
}
