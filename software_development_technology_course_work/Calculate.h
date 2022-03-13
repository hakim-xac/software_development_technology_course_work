#pragma once
#include "include_modules.h"
#include "Interface.h"
#include "benchmark.h"


namespace COURSE_WORK {
    template <typename Type
        , typename = std::enable_if_t <std::is_same_v<Type, std::string>>>
    class Calculate : public Interface<Type>
    {
        /// <summary>
        /// Класс читает из файла выражение в постфиксной(польской) форме записи.
        /// Парсит его и обрабатывает ошибки.
        /// 
        /// </summary>
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
        constexpr std::tuple<std::vector<std::string>, long long, ErrorCodes>
            readFile(const Type& str) const {
            std::ifstream fs{ str };
            std::vector<std::string> vec;
            if (!fs.is_open()) {
                fs.close();
                return { vec, 0, ErrorCodes::FileOpenError };
            }
            fs.seekg(0, std::ios_base::end);
            long long fileSize{ fs.tellg() };
            fs.seekg(0, std::ios_base::beg);
            vec.reserve(2ull << 10);
            std::string s;
            while (fs >> s) {
                vec.emplace_back(s);
            }
            fs.close();
            vec.shrink_to_fit();
            if (vec.empty()) return { vec, 0, ErrorCodes::FileEmpty };
            return { vec, fileSize, ErrorCodes::FileIsOpen};
        }


        /*  производит проверку и вычисляет выражение  */
        template <typename Iter, typename = std::enable_if_t<is_iterable_v<Iter>>>
        constexpr std::pair<double, ErrorCodes>
        parse(Iter begin, Iter end) const noexcept {
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
                    double result{ l + r };
                    stack.push(result);

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
                    if(r == 0) return { 0, ErrorCodes::DivideOrModByZero };
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
                    if (ss.fail()) return { 0, ErrorCodes::FileParseError };
                    stack.push(tmp);
                }
                ++it;
            }
            return { stack.top(), ErrorCodes::AllGood };
        }

    public:                                                 /* PUBLIC SECTIONS */

        Calculate(int widthTable)
        : Interface<Type>(widthTable, std::cin, std::cout, std::cerr) {}

        Calculate(int widthTable, std::istream& in, std::ostream& out, std::ostream& err)
        : Interface<Type>(widthTable, in, out, err) {}



        /* Главная функция-член, запрашивает путь к файлу с выражением и выполняет его, если оно без ошибок.
        Результат отправляет на вывод Interface::out */
        constexpr void loop() noexcept {

            std::ios::sync_with_stdio(0);
            setlocale(LC_ALL, "Russian");

            showHeader();

            auto [vec, fileSize, isRead] = readFile( inputRead() );  

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

            showBody(vec, fileSize, result);
            BENCHMARK::BenchMark end{};
            std::cout << "\nВремя работы программы:\t" << end.diffirence(start) << "\n";
        }

    };
}
