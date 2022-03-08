#pragma once
#include "include_modules.h"
#include "Interface.h"
#include "benchmark.h"


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
    private:                                            /* PRIVATE SECTIONS */

        

        /* //
        /  Читает файл,
        /  возвращает контейнер с операндами и true, если успешно чтение
        /  возвращает пустой контейнер и false, если чтение не успешное
        /  */
        constexpr std::pair<std::vector<std::string>, ErrorCodes>
            readFile(const Type& str) const {
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
            if (vec.empty()) return { vec, ErrorCodes::FileEmpty };
            return { vec, ErrorCodes::FileIsOpen };
        }


        /*  производит проверку и вычисление выражения  */
        template <typename Iter, typename = std::enable_if_t<is_iterable_v<Iter>>>
        constexpr std::pair<double, ErrorCodes>
        parse(Iter begin, Iter end) const {
            if(end - begin < 2) return { 0, ErrorCodes::FileParseError }; 
            std::stack<double> stack;
            auto it{ begin };
            std::stringstream ss;
            while (it != end) {
                               
                if (*it == "+") {
                    if(stack.size() < 2) return { 0, ErrorCodes::FileParseError };
                    auto r{ stack.top() };
                    stack.pop();
                    auto l{ stack.top() };
                    stack.pop();
                    stack.push(l + r);
                }
                else if (*it == "-") {
                    if (stack.size() < 2) return { 0, ErrorCodes::FileParseError };
                    auto r{ stack.top() };
                    stack.pop();
                    auto l{ stack.top() };
                    stack.pop();
                    stack.push(l - r);
                }
                else if (*it == "/") {
                    if (stack.size() < 2) return { 0, ErrorCodes::FileParseError };
                    auto r{ stack.top() };
                    stack.pop();
                    auto l{ stack.top() };
                    stack.pop();
                    stack.push(l / r);
                }
                else if (*it == "*") {
                    if (stack.size() < 2) return { 0, ErrorCodes::FileParseError };
                    auto r{ stack.top() };
                    stack.pop();
                    auto l{ stack.top() };
                    stack.pop();
                    stack.push(l * r);
                }
                else if (*it == "sin") {
                    if (stack.size() < 1) return { 0, ErrorCodes::FileParseError };
                    auto l{ stack.top() };
                    stack.pop();
                    stack.push(sin(l));
                }
                else if (*it == "cos") {
                    if (stack.size() < 1) return { 0, ErrorCodes::FileParseError };
                    auto l{ stack.top() };
                    stack.pop();
                    stack.push(cos(l));
                }
                else {
                    ss.clear();
                    ss << *it;
                    double tmp;
                    ss >> tmp;
                    if( ss.fail() ) return { 0, ErrorCodes::FileParseError };
                    stack.push(tmp);
                }
                ++it;
            }
            return { stack.top(), ErrorCodes::AllGood };
        }

    public:                                                 /* PUBLIC SECTIONS */

        Calculate()
        : Interface<Type>(std::cin, std::cout, std::cerr) {}

        Calculate( std::istream& in, std::ostream& out, std::ostream& err)
        : Interface<Type>(in, out, err) {}



        /* главная функция-член, запрашивает путь к файлу с выражением и выполняет его, если оно без ошибок */
        constexpr void loop() {

            //std::ios::sync_with_stdio(0);
            setlocale(LC_ALL, "Russian");

            showHeader();
            auto [vec, isRead] = readFile( inputRead() );

            BENCHMARK::BenchMark start{};                   

            if (isRead != ErrorCodes::FileIsOpen) {
                showError(isRead);
                return;
            }
            auto [result, isParse] = parse(vec.begin(), vec.end());
            if (isParse != ErrorCodes::AllGood) {
                showError(isParse);
                return;
            }

            showBody(vec, result);
            BENCHMARK::BenchMark end{};
            std::cout << "\nВремя работы программы:\t" << end.diffirence(start) << "\n";
        }

    };
}
