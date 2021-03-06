/* 
 * hCraft - A custom Minecraft server.
 * Copyright (C) 2012	Jacob Zhitomirsky
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _hCraft__CISTRING_H_
#define _hCraft__CISTRING_H_

#include <string>
#include <cctype>
#include <functional>


namespace hCraft {
	
	struct ci_char_traits: public std::char_traits<char>
	{
		static bool eq (char c1, char c2)
			{ return std::toupper (c1) == std::toupper (c2); }
		static bool ne (char c1, char c2)
			{ return std::toupper (c1) != std::toupper (c2); }
		static bool lt (char c1, char c2)
			{ return std::toupper (c1) < std::toupper (c2); }
		
		static int
		compare (const char *s1, const char *s2, size_t n)
		{
			while (n-- != 0)
				{
					if (std::toupper (*s1) < std::toupper (*s2)) return -1;
					if (std::toupper (*s1) > std::toupper (*s2)) return 1;
					++ s1; ++ s2;
				}
			
			return 0;
		}
		
		static const char*
		find (const char *s, int n, char a)
		{
			while (n-- > 0 && std::toupper (*s) != std::toupper (a))
				++ s;
			return s;
		} 
	};
	
	// case-insensitive string:
	typedef std::basic_string<char, ci_char_traits> cistring;
}

namespace std {
	
	/* 
	 * std::hash specialization for cistrings.
	 */
	template<>
	class hash<hCraft::cistring>
	{
	public:
		size_t operator() (const hCraft::cistring& s) const
		{
			/* 
			 * Bob Jenkins's "one-at-a-time" hash algorithm.
			 */
			
			size_t hash;
			const char *key = s.c_str ();
			
			for (hash = 0; *key; ++key)
				{
					hash += std::toupper (*key);
					hash += (hash << 10);
					hash ^= (hash >> 6);
				}
			hash += (hash << 3);
			hash ^= (hash >> 11);
			hash += (hash << 15);
			
			return hash;
		}
	};
}

#endif

