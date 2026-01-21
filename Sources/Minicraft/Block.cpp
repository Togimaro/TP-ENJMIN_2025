#include "pch.h"

#include "Block.h"

#define CREATE_BLOCK_DATA( ... ) BlockData(__VA_ARGS__),
const BlockData blocksData[] = {
	BLOCKS(CREATE_BLOCK_DATA)
};

const BlockData& BlockData::Get(const BlockId id) {
	if (id < 0 || id > COUNT) return blocksData[EMPTY];
	return blocksData[id];
}