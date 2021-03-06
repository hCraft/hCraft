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
#include "utils.hpp"
#include "drawops.hpp"
#include <sstream>


namespace hCraft {
	namespace commands {
		
		namespace {
			struct line_data {
				player *pl;
				sparse_edit_stage es;
				std::vector<vector3> points;
				blocki bl;
				bool cont;
				
				line_data (player *pl, world *wr, blocki bl, bool cont)
					: es (wr)
				{
					this->bl = bl;
					this->cont = cont;
					
					pl->es_add (&this->es);
				}
			};
		}
		
		
		static bool
		on_blocks_marked (player *pl, block_pos marked[], int markedc)
		{
			line_data *data = static_cast<line_data *> (pl->get_data ("line"));
			if (!data) return true; // shouldn't happen
			
			sparse_edit_stage& es = data->es;
			if (es.get_world () != pl->get_world ())
				{
					pl->es_remove (&data->es);
					pl->delete_data ("line");
					pl->message ("§c * §7Worlds changed, drawing cancelled§c.");
					return true;
				}
			
			if (data->cont)
				{
					std::vector<vector3>& points = data->points;
					points.push_back (marked[0]);
					if (points.size () > 1)
						{
							es.restore_to (pl);
							es.clear ();
						}
					
					if (points.size () == 1)
						es.set (marked[0].x, marked[0].y, marked[0].z, data->bl.id, data->bl.meta);
					else
						{
							draw_ops draw (es);
							for (int i = 0; i < ((int)points.size () - 1); ++i)
								draw.draw_line (points[i], points[i + 1], data->bl);
						}
					
					es.preview_to (pl);
					return false;
				}
			
			draw_ops draw (es);
			draw.draw_line (marked[0], marked[1], data->bl);
			es.commit ();
			
			pl->es_remove (&data->es);
			pl->delete_data ("line");
			pl->message ("§3Line complete");
			return true;
		}
		
		
		static void
		draw_line (player *pl)
		{
			line_data *data = static_cast<line_data *> (pl->get_data ("line"));
			if (!data)
				{
					pl->message ("§4 * §cYou are not drawing any lines§4.");
					return;
				}
			
			if (data->es.get_world () != pl->get_world ())
				{
					pl->es_remove (&data->es);
					pl->delete_data ("line");
					pl->message ("§c * §7Worlds changed, drawing cancelled§c.");
					return;
				}
			
			if (!data->cont)
				{
					pl->stop_marking ();
					pl->es_remove (&data->es);
					pl->delete_data ("line");
					return;
				}
			
			sparse_edit_stage& es = data->es;
			std::vector<vector3>& points = data->points;
			if (points.empty ())
				{
					pl->stop_marking ();
					pl->es_remove (&data->es);
					pl->delete_data ("line");
					return;
				}
			
			es.restore_to (pl);
			es.clear ();
			
			draw_ops draw (es);
			for (int i = 0; i < ((int)points.size () - 1); ++i)
				draw.draw_line (points[i], points[i + 1], data->bl);
			es.commit ();
			
			pl->stop_marking ();
			pl->es_remove (&data->es);
			pl->delete_data ("line");
			pl->message ("§3Line complete");
			return;
		}
		
		
		/* 
		 * /line -
		 * 
		 * Draws a line between two selected points.
		 * 
		 * Permissions:
		 *   - command.draw.line
		 *       Needed to execute the command.
		 */
		void
		c_line::execute (player *pl, command_reader& reader)
		{
			if (!pl->perm ("command.draw.line"))
					return;
			
			reader.add_option ("cont", "c");
			if (!reader.parse (this, pl))
					return;
			if (reader.no_args () || reader.arg_count () > 1)
				{ this->show_summary (pl); return; }
			
			bool do_cont = reader.opt ("cont")->found ();
			
			std::string& str = reader.next ().as_str ();
			if (sutils::iequals (str, "stop"))
				{
					draw_line (pl);
					return;
				}
			
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
			
			line_data *data = new line_data (pl, pl->get_world (), bl, do_cont);
			pl->create_data ("line", data,
				[] (void *ptr) { delete static_cast<line_data *> (ptr); });
			pl->get_nth_marking_callback (do_cont ? 1 : 2) += on_blocks_marked;
			
			pl->message ("§8Line §7(§8Block§7: §b" + str + "§7):");
			if (do_cont)
				{
					pl->message ("§8 * §7Please mark the required points§8.");
					pl->message ("§8 * §7Type §c/line stop §7to stop§8."); 
				}
			else
				pl->message ("§8 * §7Please mark §btwo §7blocks§7.");
		}
	}
}

