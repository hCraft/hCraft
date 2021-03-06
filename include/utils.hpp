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

#ifndef _hCraft__UTILS_H_
#define _hCraft__UTILS_H_


namespace hCraft {
	
	/* 
	 * Utility functions and classes.
	 */
	namespace utils {
		
		inline int
		iabs (int x)
		{
			return (x < 0) ? -x : x;
		}
		
		inline double
		abs (double x)
		{
			return (x < 0.0) ? -x : x;
		}
		
		
		inline int
		div (int x, int y)
		{
			if (-13 / 5 == -2 && (x < 0) != (y < 0) && x % y != 0)
	  		return x / y - 1;
			return x / y;
		}
		
		inline int
		mod (int x, int y)
		{
			if (-13 / 5 == -2 && (x < 0) != (y < 0) && x % y != 0)
	  		return x % y + y;
			return x % y;
		}
		
		
		inline int
		zsgn (int x)
		{
			return (x < 0) ? (-1) : ((x > 0) ? 1 : 0);
		}
		
		
		
		inline int
		min (int a, int b)
			{ return (a < b) ? a : b; }
		
		inline int
		max (int a, int b)
			{ return (a > b) ? a : b; }
		
		
		inline int
		floor (double x)
			{ return (x >= 0.0) ? (int)x : ((int)x - 1); }
		
		
		// integer rotation
		inline int
		int_rot (float v)
		{
			v = v - (((int)v / 360) * 360);
			return v * 0.71111111111;
		}
		
		
		
	//----
		
		/* 
		 * The amount of nanoseconds passed since epoch.
		 * Useful to initialize random number generators.
		 */
		unsigned long long ns_since_epoch ();
		
		
		
		//----
		
		/* 
		 * GZIP compression.
		 */
		long gz_compress (unsigned char *src, unsigned long slen,
			unsigned char *dest, int level = 9);
		unsigned char* gz_compress (unsigned char *src, unsigned long slen,
			long& dest_size, int level = 9);
	}
}

#endif

