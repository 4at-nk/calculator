#ifndef CalculatorH
#define CalculatorH

#include <regex>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

double Parse(std::string s)
{
	boost::replace_all(s, ",", ".");

	std::regex spaces_after_minus_expr{"-\\s+"};
	std::smatch what;
	while(std::regex_search(s, what, spaces_after_minus_expr))
		boost::replace_all(s, boost::lexical_cast<std::string> (what[0]), "-");

	//маска для поиска недопустимых символов
	const std::regex wrong_expr{ "[^ */()\\s\\d.+-]+" };
	if (std::regex_search(s, what, wrong_expr))
	{
		setlocale(LC_ALL, "Russian");
		std::cerr << "некорректный ввод, строка содержит недопустимое выражение " << what[0] << '\n';
		exit(1);
	}

	//парсинг выражения без подвыражений в скобках 
	auto ParseInBrace = [](std::string s)
	{
		//маска для поиска чисел
		const std::string NumberMask = "(-?\\d+[.,]?\\d+)|(-?\\d+)";
		std::smatch what;

		//парсинг простейшего выражения: 2 числа и операция между ними
		auto SimpleParse = [&NumberMask](const std::string& s)
		{
			const std::regex expr{ "(" + NumberMask + ")|[*/+-]" };
			std::regex_token_iterator<std::string::const_iterator> it{ s.begin(), s.end(), expr };
			//первый аргумент
			double result = boost::lexical_cast<double> (*it++);
			//операция со вторым аргументом
			switch (static_cast<std::string> (*it)[0])
			{
			case '*':
				result *= boost::lexical_cast<double> (*++it);
				break;
			case '/':
				result /= boost::lexical_cast<double> (*++it);
				break;
			case '+':
				result += boost::lexical_cast<double> (*++it);
				break;
			case '-':
				//когда a-b, то символ '-' не парсится как отдельный элемент
				//(из-за маски NumberMask), а идёт в составе второго слагаемого,
				//поэтому мы искусственно воспринимаем a-b как a+(-b)
				result += boost::lexical_cast<double> (*it);
				break;
			}

			return boost::lexical_cast<std::string> (result);
		};

		//маска для поиска простейших выражений с высоким приоритетом(*/)
		const std::regex hpriority_expr{ "(" + NumberMask + ")\\s*[*/]\\s*(" + NumberMask + ")" };
		//выполняем операции с высоким приоритетом, заменяя выражения их результатами
		while (std::regex_search(s, what, hpriority_expr))
			boost::replace_all(s, boost::lexical_cast<std::string> (what[0]), SimpleParse(what[0]));

		//маска для поиска простейших выражений с низким приоритетом(+-)
		const std::regex lpriority_expr{ "(" + NumberMask + ")\\s*[+-]\\s*(" + NumberMask + ")" };
		//то же для операций с низким приоритетом
		while (std::regex_search(s, what, lpriority_expr))
			boost::replace_all(s, boost::lexical_cast<std::string> (what[0]), SimpleParse(what[0]));

		//избавляемся от нескольких, идущих подряд минусов (минус на минус даёт плюс)
		const std::regex minuses_expr{"-{2,}"};
		while(std::regex_search(s, what, minuses_expr))
			boost::replace_all(s, boost::lexical_cast<std::string> (what[0]), 
				what[0].length() % 2 ? "-" : "");

		//убираем внешние скобки, если они есть
		boost::replace_all(s, "(", "");
		boost::replace_all(s, ")", "");
		boost::trim(s);

		return s;
	};

	//маска для поиска выражений в скобках, которые не содержат в себе выражений в скобках
	const std::regex brace_expr{ "\\([^()]*\\)" };

	//заменяем выажения в скобках результатами этих выражений
	while (std::regex_search(s, what, brace_expr))
		boost::replace_all(s, boost::lexical_cast<std::string> (what[0]), ParseInBrace(what[0]));

	//маска для поиска оставшихся лишних скобок
	const std::regex wrong_brace_expr{ "[()]" };
	if (std::regex_search(s, what, wrong_brace_expr))
	{
		setlocale(LC_ALL, "Russian");
		std::cerr << "некорректный ввод, строка содержит некорректные скобки\n";
		exit(1);
	}

	return std::round(boost::lexical_cast<double> (ParseInBrace(s)) * 100) / 100;
}

#endif
