#include <utils/split.hpp>


namespace utils
{
	
	template<> void split(std::set<std::string>& out, const std::string& in, const char ch)
	{
		std::string::size_type pos, last_pos = 0;
		while ((pos = in.find(ch, last_pos)) != std::string::npos)
		{
			std::string part = in.substr(last_pos, pos - last_pos);
			if (!part.empty()) out.insert(part);
			last_pos = pos + 1;
		}
		if (last_pos < in.length())
		{
			std::string part = in.substr(last_pos);
			if (!part.empty()) out.insert(part);
		}
	}
	
}
