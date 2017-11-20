#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <time.h>

#include <QString>

class Log
{
public:
	enum class Targets
	{
		FILE,
		CONSOLE,
		ALL
	};

	static void create(const std::string& path = "log.txt", Targets target = Targets::ALL);

	template<class Arg, class... Args>
	static void write(Arg&& arg, Args&&... args)
	{
		if (m_target == Targets::FILE || m_target == Targets::ALL) {
			writeToStream(m_file, arg, args...);
		}
		if (m_target == Targets::CONSOLE || m_target == Targets::ALL) {
			writeToStream(std::cout, arg, args...);
		}
	}
    
private:
	template<class Arg, class... Args>
	static void writeToStream(std::ostream& out, Arg&& arg, Args&&... args)
	{
		out << getDate() << std::forward<Arg>(arg);
		using dummy = int[];
		(void)dummy{0, (void(out << ' ' << std::forward<Args>(args)), 0)...};

		out << std::endl;
	}

	static const std::string getDate();

	static std::ofstream m_file;
	static Targets m_target;
};

#endif // LOG_H
