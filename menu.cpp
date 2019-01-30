#include "Menu.hpp"
#define NOMINMAX
#include <Windows.h>
#include <chrono>

#include "valve_sdk/csgostructs.hpp"
#include "helpers/input.hpp"
#include "options.hpp"
#include "ui.hpp"
#include "droid.hpp"
#include "helpers/Config.h"
#include "features/Skinchanger.h"
#include "features/visuals.hpp"
#include "features/misc.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx9.h"

#pragma region shit


std::vector<std::pair<int, const char*>> guns =
{
	{ WEAPON_AK47,			"AK-47" },
{ WEAPON_AUG,			"AUG" },
{ WEAPON_AWP,			"AWP" },
{ WEAPON_CZ75,			"CZ75-A" },
{ WEAPON_DEAGLE,		"Desert Eagle" },
{ WEAPON_DUALS,			"Dual Barettas" },
{ WEAPON_FAMAS,			"Famas" },
{ WEAPON_FIVESEVEN, 	"Five-Seven" },
{ WEAPON_GALILAR,		"Galil-AR" },
{ WEAPON_G3SG1,			"G3SG1" },
{ WEAPON_GLOCK,			"Glock-18" },
{ WEAPON_M249,			"M249" },
{ WEAPON_M4A1S, "M4A1-S" },
{ WEAPON_M4A1,			"M4A4" },
{ WEAPON_MAC10,			"MAC-10" },
{ WEAPON_MAG7,			"Mag-7" },
{ WEAPON_MP7,			"MP7" },
{ WEAPON_MP5_SD,		"MP5-SD" },
{ WEAPON_MP9,			"MP9" },
{ WEAPON_NEGEV,			"Negev" },
{ WEAPON_NOVA,			"Nova" },
{ WEAPON_HKP2000,		"P2000" },
{ WEAPON_P250,			"P250" },
{ WEAPON_P90,			"P90" },
{ WEAPON_BIZON,			"PP-Bizon" },
{ WEAPON_REVOLVER,		"R8 Revolver" },
{ WEAPON_SAWEDOFF,		"Sawed-Off" },
{ WEAPON_SCAR20,		"SCAR-20" },
{ WEAPON_SG556,			"SG-556" },
{ WEAPON_SCOUT,			"Scout" },
{ WEAPON_TEC9,			"Tec-9" },
{ WEAPON_UMP45,			"UMP-45" },
{ WEAPON_USPS,	"USP-s" },
{ WEAPON_XM1014, "XM1014" },
{ WEAPON_KNIFE_BAYONET,					"Bayonet Knife" },
{ WEAPON_KNIFE_FLIP,					"Flip Knife" },
{ WEAPON_KNIFE_GUT,					"Gut Knife" },
{ WEAPON_KNIFE_KARAMBIT,					"Karambit Knife" },
{ WEAPON_KNIFE_M9_BAYONET,					"M9 Bayonet Knife" },
{ WEAPON_KNIFE_TACTICAL,					"Hunstman Knife" },
{ WEAPON_KNIFE_FALCHION,					"Falchion Knife" },
{ WEAPON_KNIFE_SURVIVAL_BOWIE,					"Bowie Knife" },
{ WEAPON_KNIFE_BUTTERFLY,					"Butterfly Knife" },
{ WEAPON_KNIFE_DAGGER, "Shadow Daggers" },
};

std::vector<std::pair<int, const char*>> famas_skins =
{
	{ 0, "NONE"},
	{723, "Eye of Athena"},
	{22, "Contrast Spray"},
{47 , "Colony"},
{92 , "Cyanospatter"},
{429 , "Djinn"},
{154 , "Afterimage"},
{178 , "Doomkitty"},
{194 , "Spitfire"},
{244 , "Teardown"},
{218 , "Hexane"},
{260 , "Pulse"},
{288 , "Sergeant"},
{371 , "Styx"},
{477 ," Neural Net"}
};

std::vector<std::pair<int, const char*>> mag7_skins =
{
	{ 0, "NONE" },
	{ 462 , "Counter Terrace" },
{ 34 , "Metallic DDPAT" },
{ 32 , "Silver"	},
{ 100 , "Storm" },
{ 39 , "Bulldozer" },
{ 431 , "Heat" },
{ 99 ,"Sand Dune" },
{ 171 , "Irradiated Alert" },
{ 177 , "Memento" },
{ 198 , "Hazard" },
{ 291 , "Heaven Guard" },
{ 385 , "Firestarter" },
{ 473 , "Seabird" }
};

std::vector<std::pair<int, const char*>> g3sg1_skins =
{
	{ 0, "NONE" },
	{712, "High Seas"},
	{ 8 , "Desert Storm" },
{ 6 , "Arctic Camo" },
{ 27 , "Bone Mask" },
{ 46 , "Contractor" },
{ 72 , "Safari Mesh" },
{ 74 , "Polar Camo"},
{ 147 , "Jungle Dashed" },
{ 235 , "VariCamo" },
{ 170 , "Predator" },
{ 195 , "Demeter" },
{ 229 , "Azure Zebra" },
{ 294 , "Green Apple" },
{ 465 , "Orange Kimono" },
 {464 , "Neon Kimono"},
{ 382 , "Murky" },
{ 438 , "Chronos" }
};

std::vector<std::pair<int, const char*>> m4a1_skins =
{
	{ 0, "NONE" },
	{ 8 , "Desert Storm" },
{ 101 , "Tornado" },
{ 5 , "Forest DDPAT" },
{ 167 , "Radiation Hazard" },
{ 164 , "Modern Hunter" },
{ 16 , "Jungle Tiger" },
{ 17 , "Urban DDPAT" },
{ 155 , "Bullet Rain" },
{ 170 , "Predator" },
{ 176 , "Faded Zebra" },
{ 187 , "Zirka" },
{ 255 , "Asiimov" },
{ 309 , "Howl" },
{ 215 , "X - Ray" },
{ 336 , "Desert - Strike" },
{ 384 , "Griffin" },
{ 471 , "Daybreak" },
{ 449 , "Poseidon" },
{ 480 , "Evil Daimyo" }
};

std::vector<std::pair<int, const char*>> knife_skins =
{
	{ 0, "NONE" },
{ 1, "Lore" },
{ 2, "Autotronic" },
{ 3, "Black Laminate" },
{582, "Freehand"},
{578, "Bright Water"},
{569, "Gamma Doppler (Phase 1)"},
{570, "Gamma Doppler (Phase 2)"},
{571, "Gamma Doppler (Phase 3)"},
{572, "Gamma Doppler (Phase 4)"},
{568, "Gamma Doppler (Emerald)"},
{418, "Doppler (Phase 1)"},
{618, "Doppler (Phase 2)"},
{420, "Doppler (Phase 3)"},
{421, "Doppler (Phase 4)"},
{415, "Doppler (Ruby)"},
{619, "Doppler (Saphire)"},
{617, "Doppler (Black Pearl)"},
	{ 5 , "Forest DDPAT" },
{ 418 , "Doppler" },
{ 12 , "Crimson Web" },
{ 59 , "Slaughter" },
{ 27 , "Bone Mask" },
{ 38 , "Fade" },
{ 40 , "Night"},
{ 42 , "Blue Steel" },
{ 43 , "Stained "},
{ 44 , "Case Hardened"},
{ 72 , "Safari Mesh" },
{ 77 , "Boreal Forest "},
{ 98 , "Ultraviolet "},
{ 143 , "Urban Masked" },
{ 410 , "Damascus Steel" },
{ 175 , "Scorched" },
{ 409 , "Tiger Tooth" },
{ 414 , "Rust Coat" },
{ 413 , "Marble Fade"},
{ 323 , "Rust Coat" }
};

std::vector<std::pair<int, const char*>> deagle_skins =
{
	{ 0, "NONE" },
	{711, "Code Red"},
	{ 37 , "Blaze" },
{ 347 , "Pilot" },
{ 468 , "Midnight Storm" },
{ 469 , "Sunset Storm" },
{ 5 , "Forest DDPAT" },
{ 12 , "Crimson Web" },
{ 17 , "Urban DDPAT" },
{ 40 , "Night"},
{ 61 , "Hypnotic" },
{ 90 , "Mudder" },
{ 235 , "VariCamo" },
{ 185 , "Golden Koi" },
{ 248 , "Red Quartz" },
{ 231 , "Cobalt Disruption" },
{ 232 , "Crimson Web" },
{ 237 , "Urban Rubble" },
{ 397 , "Naga" },
{ 328 , "Hand Cannon" },
{ 273 , "Heirloom" },
{ 296 , "Meteorite" },
{ 351 , "Conspiracy" },
{ 425 , "Bronze Deco" },
{ 470 , "Sunset Storm" }
};

std::vector<std::pair<int, const char*>> awp_skins =
{
	{ 0, "NONE" },
	{718, "Paw" },
	{ 174 , "BOOM" },
{ 344 , "Dragon Lore" },
{ 5 , "Forest DDPAT" },
{ 84 , "Pink DDPAT" },
{ 30 , "Snake Camo" },
{ 51 , "Lightning Strike" },
{ 72 , "Safari Mesh" },
{ 181 , "Corticera" },
{ 259 , "Redline" },
{ 395 , "Man , o ,war" },
{ 212 , "Graphite" },
{ 227 , "Electric Hive" },
{ 251 , "Pit Viper" },
{ 279 , "Asiimov" },
{ 424 , "Worm God" },
{ 446 , "Medusa" },
{ 451 , "Sun in Leoa" },
{ 475 , "Hyper Beast" }
};

std::vector<std::pair<int, const char*>> m249_skins =
{
	{ 0, "NONE" },
	{ 5 , "Forest DDPAT" },
{ 22 , "Contrast Spray" },
{ 75 , "Blizzard Marbleized" },
{ 202 , "Jungle DDPAT" },
{ 243 , "Gator Mesh" },
{ 266 , "Magma" },
{ 401 , "System Lock" },
{ 452 , "Shipping Forecast" },
{ 472 , "Impact Drill" }
};

std::vector<std::pair<int, const char*>> cz_skins =
{
	{ 0, "NONE" },
	{709, "Eco" },
	{ 435 , "Pole Position" },
{ 12 , "Crimson Web"},
{ 254 , "Nitro" },
{ 218 , "Hexane" },
{ 268 , "Tread Plate" },
{ 269 , "The Fuschia Is Now" },
{ 270 , "Victoria" },
{ 297 , "Tuxedo" },
{ 298 , "Army Sheen" },
{ 315 , "Poison Dart" },
{ 322 , "Nitro" },
{ 325 , "Chalice" },
{ 334 , "Twist" },
{ 350 , "Tigris" },
{ 366 , "Green Plaid" },
{ 453 , "Emerald" },
{ 476 , "Yellow Jacket" }
};

std::vector<std::pair<int, const char*>> aug_skins =
{
	{ 0, "NONE" },
	{ 708, "Amber Slipstream" },
	{ 73 , "Wings" },
{ 10 , "Copperhead" },
{ 9 , "Bengal Tiger" },
{ 28 , "Anodized Navy" },
{ 167 , "Radiation Hazard" },
{ 110 , "Condemned" },
{ 33 , "Hot Rod" },
{ 100 , "Storm" },
{ 46 , "Contractor" },
{ 47 , "Colony" },
{ 197 , "Anodized Navy" },
{ 280 , "Chameleon" },
{ 305 , "Torque" },
{ 375 , "Radiation Hazard" },
{ 442 , "Asterion" },
{ 444 , "Daedalus" },
{ 455 , "Akihabara Accept" }
};

std::vector<std::pair<int, const char*>> ump_skins =
{
	{ 0, "NONE" },
	{ 37 , "Blaze" },
{ 5 , "Forest DDPAT" },
{ 15 , "Gunsmoke" },
{ 17 , "Urban DDPAT" },
{ 436 , "Grand Prix" },
{ 70 , "Carbon Fiber" },
{ 93 , "Caramel" },
{ 169 , "Fallout Warning" },
{ 175 , "Scorched" },
{ 193 , "Bone Pile" },
{ 392 , "Delusion" },
{ 281 , "Corporal" },
{ 333 , "Indigo" },
{ 362 , "Labyrinth" },
{ 488 , "Riot"}
};

std::vector<std::pair<int, const char*>> mac10_skins =
{
	{ 0, "NONE" },
	{ 101 , "Tornado" },
{ 3 , "Candy Apple" },
{ 32 , "Silver" },
{ 5 , "Forest DDPAT" },
{ 17 , "Urban DDPAT" },
{ 38 , "Fade" },
{ 433 , "Neon Rider" },
{ 98 , "Ultraviolet" },
{ 157 , "Palm" },
{ 188 , "Graven" },
{ 337 , "Tatter" },
{ 246 , "Amber Fade" },
{ 284 , "Heat" },
{ 310 , "Curse" },
{ 333 , "Indigo" },
{ 343 , "Commuter" },
{ 372 , "Nuclear Garden"},
{ 402 , "Malachite" }
};

std::vector<std::pair<int, const char*>> tec9_skins =
{
	{ 0, "NONE" },
	{722, "Snek-9" },
	{ 101 , "Tornado" },
{ 2 , "Groundwater" },
{ 5 , "Forest DDPAT" },
{ 463 , "Terrace" },
{ 17 , "Urban DDPAT" },
{ 36 , "Ossified" },
{ 439 , "Hades" },
{ 159 , "Brass" },
{ 168 , "Nuclear Threat" },
{ 235 , "VariCamo" },
{ 179 , "Nuclear Threat" },
{ 248 , "Red Quartz" },
{ 206 , "Tornado" },
{ 216 , "Blue Titanium" },
{ 242 , "Army Mesh" },
{ 272 , "Titanium Bit" },
{ 289 , "Sandstorm" },
{ 303 , "Isaac" },
{ 374 , "Toxic" },
{ 459 , "Bamboo Forest" }
};

std::vector<std::pair<int, const char*>> sg556_skins =
{
	{ 0, "NONE" },
	{ 101 , "Tornado" },
{ 28 , "Anodized Navy" },
{ 22 , "Contrast Spray" },
{ 27 , "Bone Mask" },
{ 39 , "Bulldozer" },
{ 98 , "Ultraviolet" },
{ 136 , "Waves Perforated" },
{ 410 , "Damascus Steel" },
{ 169 , "Fallout Warning" },
{ 186 , "Wave Spray" },
{ 243 , "Gator Mesh" },
{ 247 , "Damascus Steel" },
{ 287 , "Pulse" },
{ 298 , "Army Sheen" },
{ 363 , "Traveler" },
{ 378 , "Fallout Warning" },
{ 487 , "Cyrex" }
};

std::vector<std::pair<int, const char*>> p2000_skins =
{
	{ 0, "NONE" },
	{ 104 , "Grassland Leaves" },
{ 32 , "Silver" },
{ 21 , "Granite Marbleized" },
{ 25 , "Forest Leaves" },
{ 36 , "Ossified" },
{ 485 , "Handgun" },
{ 38 , "Fade" },
{ 71 , "Scorpion" },
{ 95 , "Grassland" },
{ 184 , "Corticera" },
{ 211 , "Ocean Foam" },
{ 338 , "Pulse" },
{ 246 , "Amber Fade" },
{ 275 , "Red FragCam" },
{ 327 , "Chainmail" },
{ 346 , "Coach Class" },
{ 357 , "Ivory" },
{ 389 , "Fire Elemental" },
{ 442 , "Asterion" },
{ 443 , "Pathfinder" }
};

std::vector<std::pair<int, const char*>> sawedoff_skins =
{
	{ 0, "NONE" },
	{ 345 , "First Class" },
{ 5 , "Forest DDPAT" },
{ 22 , "Contrast Spray" },
{ 30 , "Snake Camo" },
{ 83 , "Orange DDPAT" },
{ 38 , "Fade" },
{ 41 , "Copper" },
{ 434 , "Origami" },
{ 119 , "Sage Spray" },
{ 235 , "VariCamo" },
{ 171 , "Irradiated Alert" },
{ 204 , "Mosaico" },
{ 405 , "Serenity" },
{ 246 , "Amber Fade" },
{ 250 , "Full Stop" },
{ 390 , "Highwayman" },
{ 256 , "The Kraken" },
{ 323 , "Rust Coat" },
{ 458 , "Bamboo Shadow" },
{ 459 , "Bamboo Forest" }
};

std::vector<std::pair<int, const char*>> glock_skins =
{
	{ 0, "NONE" },
	{713, "Warhawk"},
	{ 2 , "Groundwater" },
{ 3 , "Candy Apple" },
{ 38 , "Fade" },
{ 40 , "Night" },
{ 48 , "Dragon Tattoo" },
{ 437 , "Twilight Galaxy" },
{ 99 , "Sand Dune" },
{ 159 , "Brass" },
{ 399 , "Catacombs" },
{ 208 , "Sand Dune" },
{ 230 , "Steel Disruption" },
{ 278 , "Blue Fissure" },
{ 293 , "Death Rattle" },
{ 353 , "Water Elemental" },
{ 367 , "Reactor" },
{ 381 , "Grinder" },
{ 479 , "Bunsen Burner" }
};

std::vector<std::pair<int, const char*>> galil_skins =
{
	{ 0, "NONE" },
	{ 5 , "Forest DDPAT" },
{ 22 , "Contrast Spray" },
{ 83 , "Orange DDPAT" },
{ 428 , "Eco" },
{ 76 , "Winter Forest" },
{ 119 , "Sage Spray" },
{ 235 , "VariCamo" },
{ 398 , "Chatterbox" },
{ 192 , "Shattered" },
{ 308 , "Kami" },
{ 216 , "Blue Titanium" },
{ 237 , "Urban Rubble" },
{ 241 , "Hunting Blind" },
{ 264 , "Sandstorm" },
{ 297 , "Tuxedo" },
{ 379 , "Cerberus" },
{ 460 , "Aqua Terrace" },
{ 478 , "Rocket Pop" }
};

std::vector<std::pair<int, const char*>> mp7_skins =
{
	{ 0, "NONE" },
	{719, "Powercore"},
	{ 2 , "Groundwater" },
{ 102 , "Whiteout" },
{ 5 , "Forest DDPAT" },
{ 28 , "Anodized Navy" },
{ 11 , "Skulls" },
{ 15 , "Gunsmoke" },
{ 22 , "Contrast Spray" },
{ 27 , "Bone Mask" },
{ 36 , "Ossified" },
{ 141 , "Orange Peel" },
{ 235 , "VariCamo" },
{ 245 , "Army Recon" },
{ 209 , "Groundwater" },
{ 213 , "Ocean Foam" },
{ 250 , "Full Stop" },
{ 354 , "Urban Hazard" },
{ 365 , "Olive Plaid" },
{ 423 , "Armor Core" },
{ 442 , "Asterion" },
{ 481 , "Nemesis" }
};

std::vector<std::pair<int, const char*>> p250_skins =
{
	{ 0, "NONE" },
	{ 102 , "Whiteout" },
{ 34 , "Metallic DDPAT" },
{ 162 , "Splash" },
{ 15 , "Gunsmoke" },
{ 164 , "Modern Hunter" },
{ 27 , "Bone Mask" },
{ 77 , "Boreal Forest" },
{ 99 , "Sand Dune" },
{ 168 , "Nuclear Threat" },
{ 258 , "Mehndi" },
{ 207 , "Facets" },
{ 219 , "Hive" },
{ 404 , "Muertos" },
{ 230 , "Steel Disruption" },
{ 271 , "Undertow" },
{ 295 , "Franklin" },
{ 464 , "Neon Kimono" },
{ 358 , "Supernova" },
{ 373 , "Contamination" },
{ 388 , "Cartel" },
{ 426 , "Valence" },
{ 466 , "Crimson Kimono" },
{ 467 , "Mint Kimono" }
};	

std::vector<std::pair<int, const char*>> five7_skins =
{
	{ 0, "NONE" },
	{ 3 , "Candy Apple" },
{ 27 , "Bone Mask" },
{ 44 , "Case Hardened" },
{ 46 , "Contractor" },
{ 78 , "Forest Night" },
{ 141 , "Orange Peel" },
{ 151 , "Jungle" },
{ 254 , "Nitro" },
{ 248 , "Red Quartz" },
{ 210 , "Anodized Gunmetal" },
{ 223 , "Nightshade" },
{ 252 , "Silver Quartz "},
{ 265 , "Kami" },
{ 274 , "Copper Galaxy" },
{ 464 , "Neon Kimono" },
{ 352 , "Fowl Play" },
{ 377 , "Hot Shot" },
{ 387 , "Urban Hazard" },
{ 427 , "Monkey Business" }
};

std::vector<std::pair<int, const char*>> nova_skins =
{
	{ 0, "NONE" },
	{716, "Toy Soldier"},
	{ 3 , "Candy Apple" },
{ 166 , "Blaze Orange" },
{ 164 , "Modern Hunter" },
{ 25 , "Forest Leaves" },
{ 62 , "Bloomstick" },
{ 99 , "Sand Dune" },
{ 107 , "Polar Mesh" },
{ 158 , "Walnut" },
{ 170 , "Predator" },
{ 191 , "Tempest" },
{ 214 , "Graphite" },
{ 225 , "Ghost Camo" },
{ 263 , "Rising Skull" },
{ 286 , "Antique" },
{ 294 , "Green Apple" },
{ 299 , "Caged Steel" },
{ 356 , "Koi" },
{ 450 , "Moon in Libra" },
{ 484 , "Ranger" }
};

std::vector<std::pair<int, const char*>> negev_skins =
{
	{ 0, "NONE" },
	{ 28 , "Anodized Navy" },
{ 432 ," Man -o- war" },
{ 157 , "Palm" },
{ 235 , "VariCamo" },
{ 201 , "Palm" },
{ 240 , "CaliCamo" },
{ 285 , "Terrain" },
{ 298 , "Army Sheen" },
{ 317 , "Bratatat" },
{ 355 , "Desert - Strike" },
{ 369 , "Nuclear Waste" },
{ 483 , "Loudmouth" }
};

std::vector<std::pair<int, const char*>> p90_skins =
{
	{ 0, "NONE" },
	{717, "Traction"},
	{ 342 , "Leather" },
{ 20 , "Virus" },
{ 22 , "Contrast Spray" },
{ 100 , "Storm" },
{ 67 , "Cold Blooded" },
{ 111 , "Glacier Mesh" },
{ 124 , "Sand Spray" },
{ 156 , "Death by Kitty" },
{ 234 , "Ash Wood" },
{ 169 , "Fallout Warning" },
{ 175 , "Scorched" },
{ 182 , "Emerald Dragon" },
{ 244 , "Teardown" },
{ 228 , "Blind Spot" },
{ 283 , "Trigon" },
{ 311 , "Desert Warfare" },
{ 335 , "Module" },
{ 359 , "Asiimov" },
{ 486 , "Elite Build" }
};

std::vector<std::pair<int, const char*>> scar_skins =
{
	{ 0, "NONE" },
	{ 12 , "Crimson Web" },
{ 165 , "Splash Jam" },
{ 100 , "Storm" },
{ 46 , "Contractor" },
{ 70 , "Carbon Fiber" },
{ 116 , "Sand Mesh" },
{ 157 , "Palm" },
{ 196 , "Emerald" },
{ 232 , "Crimson Web" },
{ 391 , "Cardiac" },
{ 298 , "Army Sheen" },
{ 312 , "Cyrex" },
{ 406 , "Grotto" },
{ 453 , "Emerald" }
};

std::vector<std::pair<int, const char*>> mp9_skins =
{
	{ 0, "NONE" },
	{715, "Capillary"},
	{ 482 , "Ruby Poison Dart" },
{ 27 , "Bone Mask" },
{ 33 , "Hot Rod" },
{ 100 , "Storm" },
{ 39 , "Bulldozer" },
{ 61 , "Hypnotic" },
{ 148 , "Sand Dashed" },
{ 141 , "Orange Peel" },
{ 199 , "Dry Season" },
{ 329 , "Dark Age" },
{ 262 , "Rose Iron" },
{ 366 , "Green Plaid" },
{ 368 , "Setting Sun" },
{ 386 , "Dart" },
{ 403 , "Deadly Poison" },
{ 448 , "Pandora's Box" }
};

std::vector<std::pair<int, const char*>> ak47_skins =
{
	{ 0, "NONE" },
	{707, "Neon Rider"},
	{ 341 , "First Class" },
{ 14 , "Red Laminate" },
{ 22 , "Contrast Spray" },
{ 44 , "Case Hardened" },
{ 72 , "Safari Mesh" },
{ 122 , "Jungle Spray" },
{ 170 , "Predator" },
{ 172 , "Black Laminate" },
{ 180 , "Fire Serpent" },
{ 394 , "Cartel" },
{ 300 , "Emerald Pinstripe" },
{ 226 , "Blue Laminate" },
{ 282 , "Redline"},
{ 302 , "Vulcan" },
{ 316 , "Jaguar" },
{ 340 , "Jet Set" },
{ 380 , "Wasteland Rebel" },
{ 422 , "Elite Build" },
{ 456 , "Hydroponic" },
{ 474 , "Aquamarine Revenge" }
};

std::vector<std::pair<int, const char*>> m4a1s_skins =
{
	{ 0, "NONE" },
	{714, "Nightmare"},
	{ 33 , "Hot Rod" },
{ 60 , "Dark Water" },
{ 430 , "Hyper Beast" },
{ 77 , "Boreal Forest" },
{ 235 , "VariCamo" },
{ 254 , "Nitro" },
{ 189 , "Bright Water" },
{ 301 , "Atomic Alloy" },
{ 217 , "Blood Tiger" },
{ 257 , "Guardian" },
{ 321 , "Master Piece" },
{ 326 , "Knight" },
{ 360 , "Cyrex" },
{ 383 , "Basilisk" },
{ 440 , "Icarus Fell" },
{ 445 , "Hot Rod" }
};

std::vector<std::pair<int, const char*>> elite_skins =
{
	{ 0, "NONE" },
	{730, "Shred"},
	{ 28 , "Anodized Navy" },
{ 36 , "Ossified" },
{ 43 , "Stained" },
{ 46 , "Contractor" },
{ 47 , "Colony" },
{ 153 , "Demolition" },
{ 190 , "Black Limba" },
{ 248 , "Red Quartz" },
{ 249 , "Cobalt Quartz" },
{ 220 , "Hemoglobin" },
{ 396 , "Urban Shock" },
{ 261 , "Marina" },
{ 276 , "Panther" },
{ 307 , "Retribution" },
{ 330 , "Briar" },
{ 447 , "Duelist" },
{ 450 , "Moon in Libra" }
};

std::vector<std::pair<int, const char*>> bizon_skins =
{
	{ 0, "NONE" },
	{ 13 , "Blue Streak" },
{ 164 , "Modern Hunter" },
{ 25 , "Forest Leaves" },
{ 27 , "Bone Mask" },
{ 70 , "Carbon Fiber" },
{ 148 , "Sand Dashed" },
{ 149 , "Urban Dashed" },
{ 159 , "Brass" },
{ 235 , "VariCamo" },
{ 171 , "Irradiated Alert" },
{ 203 , "Rust Coat" },
{ 224 , "Water Sigil" },
{ 236 , "Night Ops" },
{ 267 , "Cobalt Halftone" },
{ 306 , "Antique" },
{ 323 , "Rust Coat" },
{ 349 , "Osiris" },
{ 376 , "Chemical Green" },
{ 457 , "Bamboo Print" },
{ 459 , "Bamboo Forest" }
};

std::vector<std::pair<int, const char*>> xm1014_skins =
{
	{ 0, "NONE" },
	{ 166 , "Blaze Orange" },
{ 238 , "VariCamo Blue" },
{ 27 , "Bone Mask" },
{ 42 , "Blue Steel" },
{ 96 , "Blue Spruce" },
{ 95 , "Grassland" },
{ 135 , "Urban Perforated" },
{ 151 , "Jungle" },
{ 235 , "VariCamo" },
{ 169 , "Fallout Warning" },
{ 205 , "Jungle" },
{ 240 , "CaliCamo" },
{ 251 , "Pit Viper" },
{ 393 , "Tranquility" },
{ 320 , "Red Python" },
{ 314 , "Heaven Guard" },
{ 348 , "Red Leather" },
{ 370 , "Bone Machine" },
{ 407 , "Quicksilver" }
};

std::vector<std::pair<int, const char*>> usp_skins =
{
	{ 0, "NONE" },
	{ 25 , "Forest Leaves" },
{ 60 , "Dark Water" },
{ 235 , "VariCamo" },
{ 183 , "Overgrowth" },
{ 339 , "Caiman" },
{ 217 , "Blood Tiger" },
{ 221 , "Serum" },
{ 236 , "Night Ops" },
{ 277 , "Stainless" },
{ 290 , "Guardian" },
{ 313 , "Orion" },
{ 318 , "Road Rash" },
{ 332 , "Royal Blue" },
{ 364 , "Business Class" },
{ 454 , "Para Green" },
{ 489 , "Torque" }
};

std::vector<std::pair<int, const char*>> scout_skins =
{
	{ 0, "NONE" },
	{ 26 , "Lichen Dashed" },
{ 38 , "Fade" },
{ 60 , "Dark Water" },
{ 96 , "Blue Spruce" },
{ 99 , "Sand Dune" },
{ 157 , "Palm" },
{ 200 , "Mayan Dreams" },
{ 222 , "Blood in the Water" },
{ 233 , "Tropical Storm" },
{ 253 , "Acid Fade" },
{ 304 , "Slashed" },
{ 319 , "Detour" },
{ 361 , "Abyss" }
};

std::vector<std::pair<int, const char*>> null_skins =
{

};

#pragma endregion




float get_sidebar_item_width() {
	
	static int x = 20;

	if ( GetAsyncKeyState( VK_NUMPAD0 ) & 1 ) x++;
	if ( GetAsyncKeyState( VK_NUMPAD1 ) & 1 ) x--;
	if ( GetAsyncKeyState( VK_NUMPAD5 ) & 1 )
		MessageBoxA( 0, std::to_string( x ).c_str( ), 0, 0 );

	return x;
}
constexpr static float get_sidebar_item_height() { return  50.0f; }

namespace ImGuiEx
{
	inline bool ColorEdit4(const char* label, Color* v, bool show_alpha = true)
	{
		auto clr = ImVec4{
			v->r() / 255.0f,
			v->g() / 255.0f,
			v->b() / 255.0f,
			v->a() / 255.0f
		};

		if (ImGui::ColorEdit4(label, &clr.x, show_alpha)) {
			v->SetColor(clr.x, clr.y, clr.z, clr.w);
			return true;
		}
		return false;
	}
	inline bool ColorEdit3(const char* label, Color* v)
	{
		return ColorEdit4(label, v, false);
	}
}

template<size_t N>
void render_tabs(char* (&names)[N], int& activetab, float w, float h, bool sameline)
{
	bool values[N] = { false };

	values[activetab] = true;

	for (auto i = 0; i < N; ++i) {
		if (ImGui::ToggleButton(names[i], &values[i], ImVec2{ w, h })) {
			activetab = i;
		}
		if (sameline && i < N - 1)
			ImGui::SameLine();
	}
}




void RenderEspTab()
{

		ImGui::Columns(3, nullptr, false);
		ImGui::BeginChild(11, ImVec2(-1, 0), true);
		ImGui::Checkbox("Enabled", &g_Options.esp_enabled);
		ImGui::Checkbox("Visible Only", &g_Options.esp_visible_only);
		ImGui::Checkbox("Enemies Only", &g_Options.esp_enemies_only);
		ImGui::Combo("Boxes", &g_Options.esp_player_boxes, "Off\0\rBounding\0\rCornors");
		ImGui::Checkbox("Box outline", &g_Options.esp_player_boxes_outline);
		ImGui::Combo("Names", &g_Options.esp_player_names, "Off\0\rTeam Color\0\rWhite\0\0");
		ImGui::Combo("Health", &g_Options.esp_player_health, "Off\0\rBar\0\rText\0\rBoth\0\0");
		ImGui::Checkbox("Skeleton", &g_Options.esp_skeleton);
		ImGui::Combo("Weapon", &g_Options.esp_player_weapons, "Off\0\rTeam Color\0\rWhite\0\0");
		ImGui::Checkbox("Snaplines", &g_Options.esp_player_snaplines);
		ImGui::Combo("Chams", &g_Options.chams_player_enabled, "Off\0\rNormal\0\rFlat\0\rWireframes\0\rWire + Normal\0\0");
		ImGui::Checkbox("XQZ", &g_Options.chams_player_ignorez);
		ImGui::EndChild();
		ImGui::NextColumn();
		ImGui::BeginChild(23, ImVec2(-1, 0), true);
		ImGui::Checkbox("Dropped Weapons", &g_Options.esp_dropped_weapons);
		ImGui::Checkbox("Defuse Kit", &g_Options.esp_defuse_kit);
		ImGui::Checkbox("Planted C4", &g_Options.esp_planted_c4);
		ImGui::Checkbox("Grenades", &g_Options.esp_nade_esp);
		ImGui::EndChild();
		ImGui::NextColumn();
	
		ImGui::BeginChild(2, ImVec2(-1, 0), true);

		ImGuiEx::ColorEdit3("Enemy ESP", &g_Options.color_esp_enemy_occluded);
		ImGuiEx::ColorEdit3("Enemy ESP (Visible)", &g_Options.color_esp_enemy_visible);
		ImGuiEx::ColorEdit3("Ally ESP", &g_Options.color_esp_ally_occluded);
		ImGuiEx::ColorEdit3("Ally ESP (Visible)", &g_Options.color_esp_ally_visible);
		ImGuiEx::ColorEdit3("Weapons", &g_Options.color_esp_weapons);
		ImGui::NewLine();


		ImGuiEx::ColorEdit3("Chams Ally", &g_Options.color_chams_player_ally_occluded);
		ImGuiEx::ColorEdit3("Chams Ally (Visible)", &g_Options.color_chams_player_ally_visible);
		ImGuiEx::ColorEdit3("Chams Enemy", &g_Options.color_chams_player_enemy_occluded);
		ImGuiEx::ColorEdit3("Chams Enemy (Visible)", &g_Options.color_chams_player_enemy_visible);
		ImGui::EndChild();
		ImGui::Columns(1, nullptr, false);

}



void RenderMiscTab()
{
	ImGui::Columns(2, 0, false);
	ImGui::BeginChild(912, ImVec2(-1, 0), true);
	{
		if ( ImGui::Combo( "Clan Tag", &g_Options.misc_clantag, "Off\0\rStatic\0\rAnimated\0\rCustom" ) )
		{
			C_Misc::Get( ).SetClanTag( "", "" );
		}
		if ( g_Options.misc_clantag == 3 )
		{
			ImGui::InputText( "##CUSTOMTAG", g_Options.tag, 42 );
			ImGui::Spacing( );
		}

		if ( ImGui::Checkbox( "No Name", &g_Options.misc_noname ) )
		{
			C_Misc::Get( ).SetName( "\n\xAD\xAD\xAD" );
		}
		if ( ImGui::Checkbox( "Name stealer", &g_Options.misc_namesteal ) )
		{
			if ( g_EngineClient->IsInGame( ) )
				C_Misc::Get( ).SetName( g_Options.PlayeName );
		}
		ImGui::Combo( "Chat Spam", &g_Options.misc_chatspam, "Off\0\rEuphoria\0\rCustom" );
		if ( g_Options.misc_chatspam == 2 )
		{
			ImGui::InputText( "##CustomSpam", g_Options.spam, 128 );
			ImGui::Spacing( );
		}
		ImGui::Spacing( );
		ImGui::Checkbox("Bhop", &g_Options.misc_bhop );
		ImGui::Checkbox("Memewalk", &g_Options.misc_slidewalk);
		ImGui::Checkbox("Crouch exploit", &g_Options.misc_crouchexploit);
		ImGui::Spacing();
		ImGui::Checkbox("Spoof Radar", &g_Options.misc_radar);
		ImGui::Checkbox("Spectator list", &g_Options.misc_speclist);
		ImGui::Spacing();
		ImGui::Checkbox("Colored Skybox", &g_Options.misc_col_sky); ImGui::SameLine();
		ImGuiEx::ColorEdit3("##COLSKY", &g_Options.color_sky);
		ImGui::Checkbox("Nightmode", &g_Options.misc_nightmode); ImGui::SameLine();
		ImGuiEx::ColorEdit3("##NIGHTCL", &g_Options.color_nightmode);
		if (ImGui::Button("Update World Settings", ImVec2(-1, 25)))
			g_Options.update_world = true;
		ImGui::Spacing();
		ImGui::Checkbox("No hands", &g_Options.misc_no_hands);
		ImGui::SliderFloat("Viewmodel FOV", &g_Options.misc_viewmodel_fov, 0, 120, "%.0f");
		ImGui::SliderFloat("FOV", &g_Options.misc_fov, 0, 140, "%.0f");
		ImGui::PushItemWidth(117);
		ImGui::Combo("Hands Chams", &g_Options.misc_arms_chams, "Off\0\rNormal\0\rFlat\0\rWireframes\0\rWire + Normal\0\0"); ImGui::SameLine();
		ImGuiEx::ColorEdit3("##HANDS", &g_Options.misc_arms_color);
		if (g_Options.misc_arms_chams == 4)
		{
			ImGui::SameLine();
			ImGuiEx::ColorEdit3("##HAND2COL", &g_Options.hand_color2);
		}
		ImGui::Combo("Weapon Chams", &g_Options.misc_weapon_chams, "Off\0\rNormal\0\rFlat\0\rWireframes\0\rWire + Normal\0\0"); ImGui::SameLine();
		ImGuiEx::ColorEdit3("##WEAP", &g_Options.misc_weapon_color);
		if (g_Options.misc_weapon_chams == 4)
		{
			ImGui::SameLine();
			ImGuiEx::ColorEdit3("##HAND1COL", &g_Options.weap_color2);
		}
		ImGui::PopItemWidth();

		ImGui::Checkbox("Rank Revealer", &g_Options.misc_rankreveal);
		ImGui::SliderInt("Backtrack", &g_Options.legit_backtrack, 0, 12);

	}
	ImGui::EndChild();
	ImGui::NextColumn();
	ImGui::BeginChild(672, ImVec2(-1, 0), true, 0);

	static std::vector<std::string> configItems = ConfigSys::Get().GetAllConfigs();
	static int configItemCurrent = -1;
	
	static char fName[32] = "default";

	ImGui::Text("Config List:");
	if (configItems.size())
	{
		ImGui::BeginChild(52, ImVec2(-1, 300), true );
		for (std::vector<int>::size_type i = 0; i != configItems.size(); i++)
		{
			bool item_selected = i == configItemCurrent;

			if (ImGui::Selectable(std::string("  - " + configItems[i]).c_str(), item_selected))
			{
				configItemCurrent = i;
			}
		}
		ImGui::EndChild( );
	}
	else

	ImGui::Spacing();
	ImGui::InputText("Config Name", fName, 32);

	if (ImGui::Button("Save", ImVec2(136, 30))) 
	{
		if (configItems.size() > 0 && (configItemCurrent >= 0 && configItemCurrent < (int)configItems.size()))
		{
			std::string fPath = std::string(ConfigSys::Get().my_documents_folder) + "\\Euphoria\\" + configItems[configItemCurrent] + ".json";
			ConfigSys::Get().SaveConfig(fPath);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Load", ImVec2(136, 30)))
	{
		if (configItems.size() > 0 && (configItemCurrent >= 0 && configItemCurrent < (int)configItems.size()))
		{
			std::string fPath = std::string(ConfigSys::Get().my_documents_folder) + "\\Euphoria\\" + configItems[configItemCurrent] + ".json";
			ConfigSys::Get().LoadConfig(fPath);
		}
		g_Options.update_world = true;
		C_Skins::Get().Fullupdate();
	}

	if (ImGui::Button("Create", ImVec2(-1, 30)))
	{
		std::string fPath = std::string(ConfigSys::Get().my_documents_folder) + "\\Euphoria\\" + fName + ".json";
		ConfigSys::Get().SaveConfig(fPath);

		configItems = ConfigSys::Get().GetAllConfigs();
		configItemCurrent = -1;
	}

	if (ImGui::Button("Remove", ImVec2(-1, 30)))
	{
		if (configItems.size() > 0 && (configItemCurrent >= 0 && configItemCurrent < (int)configItems.size()))
		{
			std::string fPath = std::string(ConfigSys::Get().my_documents_folder) + "\\Euphoria\\" + configItems[configItemCurrent] + ".json";
			std::remove(fPath.c_str());

			configItems = ConfigSys::Get().GetAllConfigs();
			configItemCurrent = -1;
		}
	}


	ImGui::EndChild();
	ImGui::Columns(1);

}

static char* knifes[] = {
	"Off",
	"Bayonet",
	"M9 Bayonet",
	"Karambit",
	"Gut",
	"Flip",
	"Butterfly",
	"Shadow Daggers",
	"Falchion",
	"Huntsman", 
	"Bowei"

};

void RenderSkinsTab()
{

	static int currWeap = g_Options.currentWeapon;
	int pk = g_Options.W[g_Options.currentWeapon].ChangerSkin;
	static int currWeapSkin = g_Options.W[g_Options.currentWeapon].ChangerSkin;
	static char weapName[18];
	static char filterSkins[32];
	static char filterGuns[32];

	static int x = 0;

	if (GetAsyncKeyState(VK_NUMPAD0) & 1) x++;
	if (GetAsyncKeyState(VK_NUMPAD1) & 1) x--;
	if (GetAsyncKeyState(VK_NUMPAD5) & 1) MessageBoxA(0, std::to_string(x).c_str(), 0, 0);

	ImGui::Columns(2, 0, false);

	ImGui::Checkbox("Skinchanger", &g_Options.skins_enable);
	ImGui::Combo("Knife Model", &g_Options.skins_knife_model, knifes, ARRAYSIZE(knifes));
	ImGui::Spacing();
	ImGui::InputText(("Search"), filterGuns, IM_ARRAYSIZE(filterGuns));
	ImGui::BeginChild(5, ImVec2(-1, 336), true, 0);
	for (auto it : guns)
	{
		if (strcmp(it.second, "<-Default->") == 0)
			continue;

		if (!(std::string(it.second).find(std::string(filterGuns)) != std::string::npos))
			continue;

		const bool item_selected = ((int)it.first == (int)g_Options.currentWeapon);

		if (ImGui::Selectable(it.second, item_selected))
		{
			g_Options.currentWeapon = (int)it.first;
		}
	}
	ImGui::EndChild();
	ImGui::NextColumn();

	ImGui::Combo("Glove Model", &g_Options.skins_glove_model, "Default\0\rBloodhound\0\rSport\0\rDriver\0\rHand Wraps\0\rMotorcycle\0\rSpecialist\0\rHydra\0\0");
	const char* gstr;
	if (g_Options.skins_glove_model == 1)
		gstr = "Charred\0\rSnakebite\0\rBronzed\0\rGuerilla\0\0";
	else if (g_Options.skins_glove_model == 2)
		gstr = "Hedge Maze\0\rPandoras Box\0\rSuperconductor\0\rArid\0\rVice\0\rOmega\0\rAmphibious\0\rBronze Morph\0\0";
	else if (g_Options.skins_glove_model == 3)
		gstr = "Lunar Weave\0\rConvoy\0\rCrimson Weave\0\rDiamondback\0\rOvertake\0\rRacing Green\0\rKing Snake\0\rImperial Plaid\0\0";
	else if (g_Options.skins_glove_model == 4)
		gstr = "Leather\0\rSpruce DDPAT\0\rSlaughter\0\rBadlands\0\rCobalt Skulls\0\rOverprint\0\rDuct Tape\0\rArboreal\0\0";
	else if (g_Options.skins_glove_model == 5)
		gstr = "Eclipse\0\rSpearmint\0\rBoom!\0\rCool Mint\0\rTurtle\0\rTransport\0\rPolygon\0\rPOW!\0\0";
	else if (g_Options.skins_glove_model == 6)
		gstr = "Forest DDPAT\0\rCrimson Kimono\0\rEmerald Web\0\rFoundation\0\rCrimson Web\0\rBuckshot\0\rFade\0\rMogul\0\0";
	else if (g_Options.skins_glove_model == 7)
		gstr = "Emerald\0\rMangrove\0\rRattler\0\rCase Hardened\0\0";
	else
		gstr = "";

	ImGui::Combo("Glove Skin", &g_Options.skins_glove_skin, gstr, -1);
	ImGui::Spacing();
	ImGui::InputText(("Search##1"), filterSkins, IM_ARRAYSIZE(filterSkins));
	ImGui::BeginChild(("##SKINS"), ImVec2(-1, 336), true, 0);
	std::string skinStr = "";
	int curItem = -1;
	int s = 0;

	std::vector<std::pair<int, const char*>> weapon_skins;

	switch (g_Options.currentWeapon)
	{
	case WEAPON_AK47: weapon_skins = ak47_skins; break;
	case WEAPON_AUG: weapon_skins = aug_skins; break;
	case WEAPON_AWP: weapon_skins = awp_skins; break;
	case WEAPON_BIZON: weapon_skins = bizon_skins; break;
	case WEAPON_CZ75: weapon_skins = cz_skins; break;
	case WEAPON_DEAGLE: weapon_skins = deagle_skins; break;
	case WEAPON_DUALS: weapon_skins = elite_skins; break;
	case WEAPON_FAMAS: weapon_skins = famas_skins; break;
	case WEAPON_FIVESEVEN: weapon_skins = five7_skins; break;
	case WEAPON_G3SG1: weapon_skins = g3sg1_skins; break;
	case WEAPON_GALILAR: weapon_skins = galil_skins; break;
	case WEAPON_GLOCK: weapon_skins = glock_skins; break;
	case WEAPON_M249: weapon_skins = m249_skins; break;
	case WEAPON_M4A1: weapon_skins = m4a1_skins; break;
	case WEAPON_M4A1S: weapon_skins = m4a1s_skins; break;
	case WEAPON_MAC10: weapon_skins = mac10_skins; break;
	case WEAPON_MAG7: weapon_skins = mag7_skins; break;
	case WEAPON_MP7: weapon_skins = mp7_skins; break;
	case WEAPON_MP9: weapon_skins = mp9_skins; break;
	case WEAPON_NEGEV: weapon_skins = negev_skins; break;
	case WEAPON_NOVA: weapon_skins = nova_skins; break;
	case WEAPON_HKP2000: weapon_skins = p2000_skins; break;
	case WEAPON_P250: weapon_skins = p250_skins; break;
	case WEAPON_P90: weapon_skins = p90_skins; break;
	case WEAPON_SAWEDOFF: weapon_skins = sawedoff_skins; break;
	case WEAPON_SCAR20: weapon_skins = scar_skins; break;
	case WEAPON_SCOUT: weapon_skins = scout_skins; break;
	case WEAPON_SG556: weapon_skins = sg556_skins; break;
	case WEAPON_TEC9: weapon_skins = tec9_skins; break;
	case WEAPON_UMP45: weapon_skins = ump_skins; break;
	case WEAPON_USPS: weapon_skins = usp_skins; break;

	case WEAPON_KNIFE_BAYONET:
	case WEAPON_KNIFE_M9_BAYONET:
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
	case WEAPON_KNIFE_BUTTERFLY:
	case WEAPON_KNIFE_DAGGER:
	case WEAPON_KNIFE_FALCHION:
	case WEAPON_KNIFE_FLIP:
	case WEAPON_KNIFE_GUT:
	case WEAPON_KNIFE_TACTICAL:
	case WEAPON_KNIFE_KARAMBIT:
		weapon_skins = knife_skins; break;

	default: weapon_skins = null_skins; break;
	}


	for (auto it : weapon_skins)
	{
		if (!(std::string(it.second).find(std::string(filterSkins)) != std::string::npos))
			continue;

		bool selected = ((int)it.first == (int)g_Options.W[g_Options.currentWeapon].ChangerSkin);

		if (ImGui::Selectable(it.second, selected))
		{
			g_Options.W[g_Options.currentWeapon].ChangerSkin = (int)it.first;
			break;
		}
	}
	ImGui::EndChild();

	ImGui::Spacing();
	ImGui::Columns(1);
	if (ImGui::Button("Force Update", ImVec2(-1, 46)))
		C_Skins::Get().Fullupdate();
}



static char* sidebar_tabs[] = {
	"Visuals",
	"Misc",
	"Skinchanger"
};



void Menu::Initialize()
{
	_visible = true;

	ImGui::CreateContext();

	ImGui_ImplDX9_Init(InputSys::Get().GetMainWindow(), g_D3DDevice9);

	CreateStyle();
}

void Menu::Shutdown()
{
	ImGui_ImplDX9_Shutdown();
}

void Menu::OnDeviceLost()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void Menu::OnDeviceReset()
{
	ImGui_ImplDX9_CreateDeviceObjects();
}

void Menu::Render()
{

	bool ShouldRender = true;

	if (!ShouldRender)
		return;

	ImGui_ImplDX9_NewFrame();

	ImGui::GetIO().MouseDrawCursor = _visible;

	static int active_sidebar_tab = 0;

	ImGui::GetStyle().FrameBorderSize = 0.f;

	int w, h;
	g_EngineClient->GetScreenSize( w, h );

	ImGui::SetNextWindowPos( ImVec2( 0, 0 ) );
	ImGui::SetNextWindowSize( ImVec2( w, h ) );
	ImGui::SetNextWindowBgAlpha( 0 );
	ImGui::PushStyleColor( ImGuiCol_Border, ImVec4( 0, 0, 0, 0 ) );
	ImGui::Begin( " ", new bool( true ), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus );

	ImGui::GetStyle( ).Alpha = 1;


	ImGui::End( );
	ImGui::PopStyleColor( );


	if (g_Options.misc_speclist)
	{
		Visuals::Misc::SpecList();
	}

	// opacity should reach 1 in 500 milliseconds.
	constexpr float frequency = 1.f / 0.5f;
	float step = frequency * g_GlobalVars->frametime;

	static float alpha = 0;

	_visible ? alpha += step : alpha -= step;

	if ( alpha > 1.f )
		alpha = 1;
	if ( alpha < 0.f )
		alpha = 0;

	ImGui::GetStyle( ).Alpha = alpha;

	if (alpha >= 0.1f) {

		ImGui::GetIO().FontGlobalScale = .9f;
		ImGui::GetIO().FontAllowUserScaling = true;

		ImVec2 menu_pos;
		static int curr_tab = 0;

		ImGui::SetNextWindowSize(ImVec2(650, 30));
		ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Once);
		ImGui::Begin("##MHMENU", &_visible, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		menu_pos = ImGui::GetCurrentWindow()->Pos;

		ImGui::TextColored(ImColor(214, 38, 255, 255), "   Euphoria");

		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(menu_pos.x, menu_pos.y + 40), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(650, curr_tab == 1 ? 565 : 533));
		ImGui::Begin("##TABS", &_visible, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

		render_tabs(sidebar_tabs, curr_tab, 206, 26, true);

		ImGui::Spacing();

		ImGui::BeginGroupBox("##cont");

	    {
			switch ( curr_tab )
			{
			case 0:
				RenderEspTab( );
				break;
			case 1:
				RenderMiscTab( );
				break;
			case 2:
				RenderSkinsTab( );
				break;
			}
		}


		ImGui::EndGroupBox( );
		ImGui::End( );

	}

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void Menu::Show()
{
	_visible = true;
}

void Menu::Hide()
{
	_visible = false;
}

void Menu::Toggle()
{
	_visible = !_visible;
}

void Menu::CreateStyle()
{
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Droid_compressed_data, Droid_compressed_size, 14.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	ImGui::StyleColorsDark();
	ImGui::SetColorEditOptions(ImGuiColorEditFlags_NoInputs);
	_style.FrameRounding = 0.f;
	_style.WindowRounding = 0.f;
	_style.ChildRounding = 0.f;
	ImGui::GetStyle( ) = _style;
}
