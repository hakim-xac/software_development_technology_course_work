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
        using Interface<Type>::out;
        using Interface<Type>::toCaclBuffer;
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


        /*  производит проверку и вычисление выражения  */
        template <typename Iter, typename = std::enable_if_t<is_iterable_v<Iter>>>
        constexpr std::pair<long long, ErrorCodes>
        parse(Iter begin, Iter end) { 
            std::stack<long long> stack;
            auto it{ begin };
            std::stringstream ss;
            while (it != end) {
                               
                switch(stack.size()) {
                 case 0: case 1:
                    ss.clear();
                    long long tmp;
                    ss << *it;
                    ss >> tmp;

                    if (ss.fail()) return { 0ll, ErrorCodes::FileParseError };
                    stack.push(tmp);
                   break;
                 case 2:

                    if (*it == std::string("+")) {
                        //auto [a, b] = stack;                      //TO DO
                    }
                    else if (*it == std::string("-")) {
                        std::cout << "minus";
                    }
                    else if (*it == std::string("/")) {
                        std::cout << "divition";
                    }
                    else if (*it == std::string("*")) {
                        std::cout << "multyply";
                    }
                    else { std::cout << "error"; }

                    return { 0ll, ErrorCodes::AllGood };
                    break;
                 default:   return { 0ll, ErrorCodes::FileParseError }; break;
                     
                }
                ++it;
            }

            return {0ll, ErrorCodes::AllGood };
        }

    public:

        Calculate()
        : Interface<Type>(std::cin, std::cout, std::cerr) {}

        template <typename T>
        Calculate( std::istream& in, std::ostream& out, std::ostream& err)
        : Interface<Type>(in, out, err) {}



        /* главная функция-член, запрашивает путь к файлу с выражением и выполняет его, если оно без ошибок */
        constexpr void loop() {

            std::ios::sync_with_stdio(0);
            setlocale(LC_ALL, "Russian");

            showHeader();
            Type filename{ inputRead() };
            auto [vec, isRead] = readFile(filename);
            if (isRead != ErrorCodes::FileIsOpen) showError(isRead);
            auto [result, isParse] = parse(vec.begin(), vec.end());
            if (isParse == ErrorCodes::AllGood) {
                out << "result\t" << result << "\n";
            }
            else {
                out << "not ok";
            }
            showBody(); // TODO
        }

    };
}
