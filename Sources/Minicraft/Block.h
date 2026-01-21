#pragma once

#define BLOCK_TEXSIZE 1.0f / 16.0f

enum ShaderPass {
	SP_OPAQUE,
	SP_TRANSPARENT,

	SP_COUNT
};

enum BlockFlags : uint64_t {
	BF_NONE = 0,
	BF_CUTOUT = 1 << 1,
	BF_NO_PHYSICS = 1 << 2,
	BF_GRAVITY_WATER = 1 << 3,
	BF_NO_RAYCAST = 1 << 4,

	BF_HALF_BLOCK = 1 << 5,
};

#define BLOCKS(F) \
	F( EMPTY,				-1, BF_NO_PHYSICS | BF_NO_RAYCAST ) \
	F( STONE,				1 ) \
	F( DIRT,				2 ) \
	F( GRASS,				3, 0, 2 ) \
	F( WOOD,				4 ) \
	F( HALF_SLAB,		    5, 6, 6, BF_HALF_BLOCK ) \
	F( SLAB,				5, 6, 6 ) \
	F( BRICK,				7 ) \
	F( STONE_BRICK,			54 ) \
	F( BOOKSHELF,			35, 4, 4 ) \
	F( DUNGEON_STONE,		36 ) \
	F( TNT,					8, 9, 10 ) \
	F( COBBLESTONE,			16 ) \
	F( BEDROCK,				17 ) \
	F( SAND,				18 ) \
	F( GRAVEL,				19 ) \
	F( LOG,					20, 21, 21 ) \
	F( SPONGE,				48 ) \
	F( WOOL,				64 ) \
/* ORE */ \
	F( COAL,				34 ) \
	F( IRON_ORE,			33 ) \
	F( IRON_BLOCK,			22 ) \
	F( GOLD_ORE,			32 ) \
	F( GOLD_BLOCK,			23 ) \
	F( DIAMOND_ORE,			50 ) \
	F( DIAMOND_BLOCK,		24 ) \
	F( EMERALD_BLOCK,		25 ) \
	F( REDSTONE_ORE,		51 ) \
	F( OBSIDIAN,			37 ) \
\
/* OBJECTS */ \
	F( CRAFTING_TABLE,		59, 43, 4 ) /* there is a side variation at index 60 */ \
	F( FURNACE,				44, 62, 62 ) /* need an orientation & on/off flag */ \
	F( DISPENSER,			46, 62, 62 ) /* need an orientation flag */ \
/* TRANSPARENT STUFF */ \
	F( GLASS,				49, BF_CUTOUT ) \
	F( WATER,				205, BF_NO_PHYSICS | BF_GRAVITY_WATER | BF_NO_RAYCAST, SP_TRANSPARENT ) \
/* 38, 39 & 40 contains greyscale grass for biome variation */ \
/* as an exercice you can try to implement that by adding back some vertex color informations to the pipeline */ \
/* 52, 53 contains greyscale leaves */ \
	F( HIGHLIGHT, 180) \
	F( COUNT, -1)

#define EXTRACT_BLOCK_ID( v ) v,
enum BlockId: uint8_t {
	BLOCKS(EXTRACT_BLOCK_ID)
};

class BlockData {
public:
	BlockId id;

	int texIdSide;
	int texIdTop;
	int texIdBottom;

	uint64_t flags;
	ShaderPass pass;
public:
	BlockData(BlockId id, int texId, uint64_t flags = BF_NONE, ShaderPass pass = SP_OPAQUE) :
		id(id),
		texIdSide(texId),
		texIdTop(texId),
		texIdBottom(texId),
		flags(flags),
		pass(pass) {}

	BlockData(BlockId id, int texIdSide, int texIdTop, int texIdBottom, uint64_t flags = BF_NONE, ShaderPass pass = SP_OPAQUE) :
		id(id),
		texIdSide(texIdSide),
		texIdTop(texIdTop),
		texIdBottom(texIdBottom),
		flags(flags),
		pass(pass) {}

	static const BlockData& Get(const BlockId id);
};
