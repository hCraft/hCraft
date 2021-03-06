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

#include "drawc.hpp"
#include "player.hpp"
#include "server.hpp"
#include "stringutils.hpp"
#include "position.hpp"
#include "drawops.hpp"
#include <sstream>
#include <vector>
#include <cmath>


namespace hCraft {
	namespace commands {
		
		namespace {
			struct ellipse_data {
				blocki bl;
				draw_ops::plane pn;
				bool fill;
				int a, b;
			};
		}
		
		
		static bool
		on_blocks_marked (player *pl, block_pos marked[], int markedc)
		{
			ellipse_data *data = static_cast<ellipse_data *> (pl->get_data ("ellipse"));
			if (!data) return true; // shouldn't happen
			
			double a = (data->a == -1) ?
				(vector3 (marked[1]) - vector3 (marked[0])).magnitude () : data->a;
			double b = (data->b == -1) ?
				(vector3 (marked[2]) - vector3 (marked[0])).magnitude () : data->b;
			
			int blocks;
			dense_edit_stage es (pl->get_world ());
			draw_ops draw (es);
			if (data->fill)
				blocks = draw.fill_ellipse (marked[0], a, b, data->bl, data->pn);
			else
				blocks = draw.draw_ellipse (marked[0], a, b, data->bl, data->pn);
			es.commit ();
			
			std::ostringstream ss;
			ss << "§3Ellipse complete §7(§3Modified§7: §b" <<  blocks << " §3blocks§7)";
			pl->message (ss.str ());
			
			pl->delete_data ("ellipse");
			return true;
		}
		
		/* 
		 * /ellipse -
		 * 
		 * Draws a two-dimensional ellipse centered at a point.
		 * 
		 * Permissions:
		 *   - command.draw.ellipse
		 *       Needed to execute the command.
		 */
		void
		c_ellipse::execute (player *pl, command_reader& reader)
		{
			if (!pl->perm (this->get_exec_permission ()))
					return;
		
			reader.add_option ("fill", "f");
			if (!reader.parse (this, pl))
					return;
			if (reader.no_args () || reader.arg_count () > 4)
				{ this->show_summary (pl); return; }
			
			bool do_fill = reader.opt ("fill")->found ();
			
			std::string& str = reader.next ().as_str ();
			if (!sutils::is_block (str))
				{
					pl->message ("§c * §7Invalid block§f: §c" + str);
					return;
				}
			
			blocki bl = sutils::to_block (str);
			if (bl.id == BT_UNKNOWN)
				{
					pl->message ("§c * §7Unknown block§f: §c" + str);
					return;
				}
			
			draw_ops::plane pn = draw_ops::XZ_PLANE;
			if (reader.has_next ())
				{
					std::string& str = reader.next ().as_str ();
					if (sutils::iequals (str, "XZ") || sutils::iequals (str, "ZX"))
						;
					else if (sutils::iequals (str, "YX") || sutils::iequals (str, "XY"))
						pn = draw_ops::YX_PLANE;
					else if (sutils::iequals (str, "YZ") || sutils::iequals (str, "ZY"))
						pn = draw_ops::YZ_PLANE;
					else
						{
							pl->message ("§c * §7The plane must be one of§f: §cXZ, YX, YZ§f.");
							return;
						}
				}
			
			int a = -1, b = -1;
			if (reader.has_next ())
				{
					command_reader::argument a_arg = reader.next ();
					if (!a_arg.is_int ())
						{
							pl->message ("§c * §7Usage§c: §e/ellipe §cblock §8[§cplane§8] §8[§cradx radz§8].");
							return;
						}
					
					a = a_arg.as_int ();
					if (!reader.has_next ())
						b = a;
					else
						{
							command_reader::argument b_arg = reader.next ();
							if (!b_arg.is_int ())
								{
									pl->message ("§c * §7Usage§c: §e/ellipe §cblock §8[§cplane§8] §8[§cradx radz§8].");
									return;
								}
							
							b = b_arg.as_int ();
						}
					
					if (a < 1 || a > 2000 || b < 1 || b > 2000)
						{
							pl->message ("§c * §7Radii must be in the range of 1-2000.");
							return;
						}
				}
			
			ellipse_data *data = new ellipse_data {bl, pn, do_fill, a, b};
			pl->create_data ("ellipse", data,
				[] (void *ptr) { delete static_cast<ellipse_data *> (ptr); });
			pl->get_nth_marking_callback ((a == -1) ? 3 : 1) += on_blocks_marked;
			
			std::ostringstream ss;
			ss << "§8Ellipse §7(§8Plane§7: §b";
			switch (pn)
				{
					case draw_ops::XZ_PLANE: ss << "XZ§7, "; break;
					case draw_ops::YX_PLANE: ss << "YX§7, "; break;
					case draw_ops::YZ_PLANE: ss << "YZ§7, "; break;
				}
			if (a != -1)
				ss << "§8A§7: §b" << a << "§7, ";
			if (b != -1)
				ss << "§8B§7: §b" << b << "§7, ";
			ss << "§8Block§7: §b" << str << "§7):";
			pl->message (ss.str ());
			
			ss.str (std::string ()); ss.clear ();
			ss << "§8 * §7Please mark §b" << ((a == -1) ? 3 : 1) << " §7blocks§7.";
			pl->message (ss.str ());
		}
	}
}

