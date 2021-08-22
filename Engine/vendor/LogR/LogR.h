#include <iostream>
#include <sstream>

#define LogR_RED         "\033[31m"
#define LogR_GREEN       "\033[32m"
#define LogR_YELLOW      "\033[33m"
#define LogR_COLOR_RESET "\033[0m "
#define LogR_WHITE       "\033[37m"

#ifdef LOGR_INCLUDE_GLM_TYPES
#include <glm/glm.hpp>
#endif

namespace LogR {
	template<typename T>
	struct Format {
		Format(const std::string& format, const T& value) : fmt(format.c_str()), value(value) {}
	
		const char* fmt;
		const T value;
	};

	class Logger {
	public:
		Logger() : prefix("[LOG]") {
			system("Color");
		}
	
		inline void SetPrefix(const char* prefix) {
			this->prefix = prefix;
		}
	
		template<typename... Args>
		inline void Error(Args&&... args) {
			LogColor(LogR_RED, std::forward<Args>(args)...);
		}
		template<typename... Args>
		inline void Warning(Args&&... args) {
			LogColor(LogR_YELLOW, std::forward<Args>(args)...);
		}
		template<typename... Args>
		inline void Log(Args&&... args) {
			LogColor(LogR_GREEN, std::forward<Args>(args)...);
		}
		template<typename... Args>
		inline void Success(Args&&... args) {
			LogColor(LogR_WHITE, std::forward<Args>(args)...);
		}
	private:
		template<typename... Args>
		void LogColor(const char* color, Args&&... args) {
			std::stringstream stream;
			ParseInput(stream, args...);
	
			std::cout << color << prefix << LogR_COLOR_RESET << stream.str() << '\n';
		}
	private:
		const char* prefix;
	
		//// PARSING ////
	
		template<typename TFirst>
		inline void ParseInput(std::ostream& os, TFirst& first) {
			TypeToString(os, first);
			os << ' ';
		}
		template<typename TFirst, typename... TRest>
		inline void ParseInput(std::ostream& os, TFirst& first, TRest&&... rest) {
			TypeToString(os, first);
			os << ' ';
			ParseInput(os, rest...);
		}
	
	
		//// TYPES TO STIRNGS ////
	
		template<typename T> inline void TypeToString(std::ostream& os, T& type) {
			os << type;
		}

		// Overriding:

		template<typename T>
		inline void TypeToString(std::ostream& os, Format<T>& type) {
			char buf[40];
			snprintf(buf, sizeof(buf), type.fmt, type.value, 40);
			os << buf;
		}

	#ifdef LOGR_INCLUDE_GLM_TYPES
		template <typename T, glm::qualifier Packing>
		inline void TypeToString(std::ostream& os, glm::vec<1, T, Packing>& type) { os << '(' << type.x << ')'; }
		template <typename T, glm::qualifier Packing>
		inline void TypeToString(std::ostream& os, glm::vec<2, T, Packing>& type) { os << '(' << type.x << ", " << type.y << ')'; }
		template <typename T, glm::qualifier Packing>
		inline void TypeToString(std::ostream& os, glm::vec<3, T, Packing>& type) { os << '(' << type.x << ", " << type.y << ", " << type.z << ')'; }
		template <typename T, glm::qualifier Packing>
		inline void TypeToString(std::ostream& os, glm::vec<4, T, Packing>& type) { os << '(' << type.x << ", " << type.y << ", " << type.z << ", " << type.w << ')'; }

		template <typename T, glm::qualifier Packing>
		inline void MatCol(std::ostream& os, glm::vec<1, T, Packing>& type) { os << '[' << type.x << ']'; }
		template <typename T, glm::qualifier Packing>
		inline void MatCol(std::ostream& os, glm::vec<2, T, Packing>& type) { os << '[' << type.x << ", " << type.y << ']'; }
		template <typename T, glm::qualifier Packing>
		inline void MatCol(std::ostream& os, glm::vec<3, T, Packing>& type) { os << '[' << type.x << ", " << type.y << ", " << type.z << ']'; }
		template <typename T, glm::qualifier Packing>
		inline void MatCol(std::ostream& os, glm::vec<4, T, Packing>& type) { os << '[' << type.x << ", " << type.y << ", " << type.z << ", " << type.w << ']'; }

		template <int Cols, typename T, glm::qualifier Packing>
		inline void TypeToString(std::ostream& os, glm::mat<Cols, 2, T, Packing>& type) { 
			os << '['; MatCol(os, type[0]); os << ", "; MatCol(os, type[1]); os << ']'; 
		}
		template <int Cols, typename T, glm::qualifier Packing>
		inline void TypeToString(std::ostream& os, glm::mat<Cols, 3, T, Packing>& type) { 
			os << '['; MatCol(os, type[0]); os << ", "; MatCol(os, type[1]); os << ", "; MatCol(os, type[2]); os << ']';
		}
		template <int Cols, typename T, glm::qualifier Packing>
		inline void TypeToString(std::ostream& os, glm::mat<Cols, 4, T, Packing>& type) { 
			os << '['; MatCol(os, type[0]); os << ", "; MatCol(os, type[1]); os << ", "; MatCol(os, type[2]); os << ", "; MatCol(os, type[3]); os << ']';
		}
	#endif
	};

}