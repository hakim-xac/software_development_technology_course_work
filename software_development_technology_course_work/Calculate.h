#pragma once
#include "include_modules.h"
#include "Interface.h"

namespace COURSE_WORK {
    template <typename Type
        , typename = std::enable_if_t <std::is_same_v<Type, std::string>>>
    class Calculate : public Interface<Type>
    {
        using Interface<Type>::showHeader;
        using Interface<Type>::showBody;
        using Interface<Type>::showError;
        using Interface<Type>::inputRead;
    private:

        

        /* //
        /  Читает файл,
        /  возвращает контейнер с операндами и true, если успешно чтение
        /  возвращает пустой контейнер и false, если чтение не успешное
        /  */
        constexpr std::pair<std::vector<std::string>, ErrorCodes>
            readFile(const Type& str) {
            std::ifstream fs{ str };
            std::vector<std::string> vec;
            if (!fs.is_open()) {
                return { vec, ErrorCodes::FileOpenError };
            }

            vec.reserve(2ull << 10);
            std::string s;
            while (fs >> s) {
                vec.emplace_back(s);
            }
            vec.shrink_to_fit();
            return { vec, ErrorCodes::FileIsOpen };
        }

    public:

        Calculate()
        : Interface<Type>(std::cin, std::cout, std::cerr) {}

        template <typename T>
        Calculate( std::istream& in, std::ostream& out, std::ostream& err)
        : Interface<Type>(in, out, err) {}


        /*  распечатывает контейнер в вывод */
        constexpr void
        print() {
            //std::copy(vec.begin(), vec.end(), std::ostream_iterator<Type>(out, " "));
        }


        /* главная функция-член, запрашивает путь к файлу с выражением и выполняет его, если оно без ошибок */
        constexpr void loop() {

            std::ios::sync_with_stdio(0);
            setlocale(LC_ALL, "Russian");

            showHeader();
            Type filename{ inputRead() };
            auto [vec, isRead] = readFile(filename);
            if (isRead != ErrorCodes::FileIsOpen) showError(isRead);

            showBody(); // TODO
        }

    };
}
