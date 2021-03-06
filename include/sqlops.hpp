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

#ifndef _hCraft__SQLOPS_H_
#define _hCraft__SQLOPS_H_

#include "rank.hpp"


namespace hCraft {
	
	// forward decs
	class server;
	namespace sql {
		class connection;
	}
	
	
	/* 
	 * Common SQL queries.
	 */
	struct sqlops
	{
		struct player_info
		{
			int id;
			std::string name;
			bool op;
			rank rnk;
			std::string nick;
	 	};
	 	
	 	
	 //----
	 	
		/* 
		 * Total number of players registered in the database.
		 */
		static int player_count (sql::connection& conn);
		
		/* 
		 * Checks whether the database has a player with the given name registered.
		 */
		static bool player_exists (sql::connection& conn, const char *name);
		
		/* 
		 * Fills the specified player_info structure with information about
		 * the given player. Returns true if the player found, false otherwise.
		 */
		static bool player_data (sql::connection& conn, const char *name,
			server &srv, player_info& out);
		
		
		/* 
		 * Changes the rank of the player that has the specified name.
		 */
		static void modify_player_rank (sql::connection& conn, const char *name,
			const char *rankstr);
	};
}

#endif
