#pragma once
#include "include_modules.h"

namespace COURSE_WORK {
	/// <summary>
	/// namespace COURSE_WORK
	/// </summary>
	template <typename Type
			, typename = std::enable_if_t<std::is_same_v<Type, std::string>>>
	class Interface
	{
		/*	widthTable	*/
		static constexpr const int widthTable{ 100 };
		std::istream& in;
		std::ostream& out;

		
	private:

		Interface(const Interface&) = delete;
		Interface(const Interface&&) = delete;
		Interface& operator = (const Interface&) = delete;
		Interface& operator = (const Interface&&) = delete;


		/*	*/
		std::string_view generatingString(std::string_view str) {
			return str;
		}


		/**/
		void showHeader() {
			generatingString("Хакимов");
		}


	public:

		/**/
		Interface()
		: in(std::cin)
		, out(std::cout) {}


		/**/
		~Interface() {}


		/**/
		void loop() {
			std::ios::sync_with_stdio(0);
			showHeader();
			//showBody();
			//showFoot();
		}

		
	};
}
