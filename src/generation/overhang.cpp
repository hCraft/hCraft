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

#include "generation/overhang.hpp"
#include "utils.hpp"
#include <functional>


namespace hCraft {
	
	/* 
	 * Constructs a new overhang world generator.
	 */
	overhang_world_generator::overhang_world_generator (long seed)
		: gen_birch_trees (5, {BT_TRUNK, 2}, {BT_LEAVES, 2})
	{
		this->gen_seed = seed;
		this->gen_oak_trees.seed (seed);
		this->gen_birch_trees.seed (seed);
		
		// REV #03
		this->pn1.SetSeed (seed & 0x7FFFFFFF);
		this->pn1.SetNoiseQuality (noise::QUALITY_FAST);
		this->pn1.SetFrequency (0.009);
		this->pn1.SetPersistence (0.31);
		this->pn1.SetLacunarity (0.56);
		
		this->co1.SetConstValue (6.33);
		this->mu1.SetSourceModule (0, this->pn1);
		this->mu1.SetSourceModule (1, this->co1);
		
		this->pn2.SetSeed ((seed + 1) & 0x7FFFFFFF);
		this->pn2.SetNoiseQuality (noise::QUALITY_FAST);
		this->pn2.SetFrequency (0.03);
		this->pn2.SetPersistence (0.07);
		this->pn2.SetLacunarity (0.5);
		
		this->pn3.SetSeed ((seed + 2) & 0x7FFFFFFF);
		this->pn3.SetNoiseQuality (noise::QUALITY_FAST);
		this->pn3.SetFrequency (0.05);
		this->pn3.SetPersistence (0.7);
		this->pn3.SetLacunarity (0.55);
		
		/*
		this->pn4.SetSeed ((seed + 3) & 0x7FFFFFFF);
		this->pn4.SetNoiseQuality (noise::QUALITY_FAST);
		this->pn4.SetFrequency (0.0009);
		this->pn4.SetPersistence (0.02);
		this->pn4.SetLacunarity (0.452);
		*/
		this->co2.SetConstValue (1.0);
		
		this->pn5.SetSeed ((seed + 4) & 0x7FFFFFFF);
		this->pn5.SetNoiseQuality (noise::QUALITY_FAST);
		this->pn5.SetFrequency (0.0009);
		this->pn5.SetPersistence (0.02);
		this->pn5.SetLacunarity (0.452);
		
		this->bl1.SetSourceModule (0, this->mu1);
		this->bl1.SetSourceModule (1, this->co2);
		this->bl1.SetControlModule (this->pn5);
		
		this->se1.SetSourceModule (1, this->bl1);
		this->se1.SetSourceModule (0, this->pn2);
		this->se1.SetControlModule (this->pn3);
		this->se1.SetBounds (-1.0, -0.2);
		this->se1.SetEdgeFalloff (0.34);
		
		
		this->pn_sand.SetSeed ((seed + 3) & 0x7FFFFFFF);
		this->pn_sand.SetNoiseQuality (noise::QUALITY_FAST);
		this->pn_sand.SetFrequency (0.006);
		this->pn_sand.SetPersistence (0.2);
		this->pn_sand.SetLacunarity (0.5);
		
		/*
		// REV #02
		this->pn1.SetSeed (seed & 0x7FFFFFFF);
		this->pn1.SetNoiseQuality (noise::QUALITY_FAST);
		this->pn1.SetFrequency (0.009);
		this->pn1.SetPersistence (0.31);
		this->pn1.SetLacunarity (0.56);
		
		this->co1.SetConstValue (5.0);
		this->mu1.SetSourceModule (0, this->pn1);
		this->mu1.SetSourceModule (1, this->co1);
		
		this->pn2.SetSeed ((seed + 1) & 0x7FFFFFFF);
		this->pn2.SetNoiseQuality (noise::QUALITY_FAST);
		this->pn2.SetFrequency (0.03);
		this->pn2.SetPersistence (0.07);
		this->pn2.SetLacunarity (0.5);
		
		this->pn3.SetSeed ((seed + 2) & 0x7FFFFFFF);
		this->pn3.SetNoiseQuality (noise::QUALITY_FAST);
		this->pn3.SetFrequency (0.05);
		this->pn3.SetPersistence (0.7);
		this->pn3.SetLacunarity (0.55);
		
		this->se1.SetSourceModule (1, this->mu1);
		this->se1.SetSourceModule (0, this->pn2);
		this->se1.SetControlModule (this->pn3);
		this->se1.SetBounds (-1.0, -0.6);
		this->se1.SetEdgeFalloff (0.3);
		
		
		this->pn_sand.SetSeed ((seed + 3) & 0x7FFFFFFF);
		this->pn_sand.SetNoiseQuality (noise::QUALITY_FAST);
		this->pn_sand.SetFrequency (0.006);
		this->pn_sand.SetPersistence (0.2);
		this->pn_sand.SetLacunarity (0.5);
		*/
		
		/*
		// REV #01
		
		this->pn1.SetSeed (seed & 0x7FFFFFFF);
		this->pn1.SetNoiseQuality (noise::QUALITY_FAST);
		this->pn1.SetFrequency (0.006);
		this->pn1.SetPersistence (0.35);
		this->pn1.SetLacunarity (0.56);
		
		this->co1.SetConstValue (2.82);
		this->mu1.SetSourceModule (0, this->pn1);
		this->mu1.SetSourceModule (1, this->co1);
		
		this->pn2.SetSeed ((seed + 1) & 0x7FFFFFFF);
		this->pn2.SetNoiseQuality (noise::QUALITY_FAST);
		this->pn2.SetFrequency (0.03);
		this->pn2.SetPersistence (0.07);
		this->pn2.SetLacunarity (0.5);
		
		this->pn3.SetSeed ((seed + 2) & 0x7FFFFFFF);
		this->pn3.SetNoiseQuality (noise::QUALITY_FAST);
		this->pn3.SetFrequency (0.05);
		this->pn3.SetPersistence (0.7);
		this->pn3.SetLacunarity (0.55);
		
		this->se1.SetSourceModule (1, this->mu1);
		this->se1.SetSourceModule (0, this->pn2);
		this->se1.SetControlModule (this->pn3);
		this->se1.SetBounds (-1.0, -0.5);
		this->se1.SetEdgeFalloff (0.15);
		
		
		this->pn_sand.SetSeed ((seed + 3) & 0x7FFFFFFF);
		this->pn_sand.SetNoiseQuality (noise::QUALITY_FAST);
		this->pn_sand.SetFrequency (0.006);
		this->pn_sand.SetPersistence (0.2);
		this->pn_sand.SetLacunarity (0.5);
		*/
	}
	
	
	
#define OFFSET_LEVEL 60
#define WATER_LEVEL  55
#define MAX_HEIGHT  100
	void
	overhang_world_generator::terrain (world& wr, chunk *out, int cx, int cz)
	{
		int x, y, z;
		double v, b;
		for (x = 0; x < 16; ++x)
			for (z = 0; z < 16; ++z)
				{
					for (y = 0; y < 40; ++y)
						out->set_id (x, y, z, BT_STONE);
					for (; y < MAX_HEIGHT; ++y) 
						{
							v = this->se1.GetValue (((cx << 4) | x) * 0.4, y, ((cz << 4) | z) * 0.4);
							
							// bias sampled result with high (offset from waterlevel)
							b = (OFFSET_LEVEL - y) * 0.06; 
							v += b; 
							
							if (v > 0.0)
								out->set_id (x, y, z, BT_STONE);
							else if (y <= WATER_LEVEL)
								out->set_id (x, y, z, BT_WATER);
						}
				}
	}
	
	
	
	void
	overhang_world_generator::decorate (world& wr, chunk *out, int cx, int cz)
	{
		enum
			{
				ST_AIR,
				ST_DIRT,
				ST_STONE,
			} state =	ST_AIR;
		
		unsigned int xz_hash = std::hash<long> () (((long)cz << 32) | cx) & 0xFFFFFFFF;
		this->rnd.seed (this->gen_seed + xz_hash);
		std::uniform_int_distribution<> dis (1, 180);
		
		int d;
		int r;
		double v;
		unsigned short id;
		int x, y, z;
		for (x = 0; x < 16; ++x)
			for (z = 0; z < 16; ++z)
				{
					bool first = true;
					int c = 0;
					for (y = MAX_HEIGHT; y >= 0; --y)
						{
							id = out->get_id (x, y, z);
							if (id == BT_STONE)
								{
									if (utils::iabs (WATER_LEVEL - y) <= 3)
										{
											v = pn_sand.GetValue ((cx << 4) | x, 0, (cz << 4) | z);
											if (v > 0.25)
												{ state = ST_DIRT; out->set_id (x, y, z, BT_SAND); continue; }
											else if (v < -0.5)
												{ state = ST_DIRT; out->set_id (x, y, z, BT_GRAVEL); continue; }
										}
									
									if (state == ST_AIR)
										{
											if (first)
												{
													first = false;
													
													// determine biome
													if (y == WATER_LEVEL)
														out->set_biome (x, z, BI_JUNGLE);
													else
														{
															d = y - OFFSET_LEVEL;
															if (d >= 4)
																out->set_biome (x, z, BI_FOREST);
														}
												}
											
											if ((y - WATER_LEVEL) >= 1)
												{
													r = dis (rnd);
													if (r > 90)
														out->set_id_and_meta (x, y + 1, z, BT_TALL_GRASS, 1);
													else if (r < 8)
														out->set_id (x, y + 1, z, (dis (rnd) & 1) ? BT_DANDELION : BT_ROSE);
													else if (r > 10 && r < 15 && ((y - WATER_LEVEL) > 4))
														{
															if (dis (rnd) > 160)
																this->gen_birch_trees.generate (wr, (cx << 4) | x, y + 1, (cz << 4) | z);
															else
																this->gen_oak_trees.generate (wr, (cx << 4) | x, y + 1, (cz << 4) | z);
														}
												}
											
											out->set_id (x, y, z, BT_GRASS);
											state = ST_DIRT;
										}
									else if (state == ST_DIRT)
										{
											if (c++ < 5)
												out->set_id (x, y, z, BT_DIRT);
											else
												state = ST_STONE;
										}
								}
							else
								state = ST_AIR;
						}
				}
	}
	
	/* 
	 * Generates on the specified chunk.
	 */
	void
	overhang_world_generator::generate (world& wr, chunk *out, int cx, int cz)
	{ 
		this->terrain (wr, out, cx, cz);
		this->decorate (wr, out, cx, cz);
	}
}

