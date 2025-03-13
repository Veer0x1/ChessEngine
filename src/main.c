#include <stdio.h>

// bitboard data type
#define U64 unsigned long long

// set, get, pop macros
#define get_bit(bb, sq) (bb & (1ULL << sq))
#define set_bit(bb, sq) (bb |= (1ULL << sq))
#define pop_bit(bb, sq) (bb &= ~(1ULL << sq))

enum { white, black};

enum {
 a8, b8, c8, d8, e8, f8, g8, h8,
 a7, b7, c7, d7, e7, f7, g7, h7,
 a6, b6, c6, d6, e6, f6, g6, h6,
 a5, b5, c5, d5, e5, f5, g5, h5,
 a4, b4, c4, d4, e4, f4, g4, h4,
 a3, b3, c3, d3, e3, f3, g3, h3,
 a2, b2, c2, d2, e2, f2, g2, h2,
 a1, b1, c1, d1, e1, f1, g1, h1
};

/*
  "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
  "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
  "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
  "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
  "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
  "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
  "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
  "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
*/

/*
 not A file constant
 8  0  1  1  1  1  1  1  1 
 7  0  1  1  1  1  1  1  1 
 6  0  1  1  1  1  1  1  1 
 5  0  1  1  1  1  1  1  1 
 4  0  1  1  1  1  1  1  1 
 3  0  1  1  1  1  1  1  1 
 2  0  1  1  1  1  1  1  1 
 1  0  1  1  1  1  1  1  1 
    a  b  c  d  e  f  g  h
 not H file constant
 8  1  1  1  1  1  1  1  0 
 7  1  1  1  1  1  1  1  0 
 6  1  1  1  1  1  1  1  0 
 5  1  1  1  1  1  1  1  0 
 4  1  1  1  1  1  1  1  0 
 3  1  1  1  1  1  1  1  0 
 2  1  1  1  1  1  1  1  0 
 1  1  1  1  1  1  1  1  0 
    a  b  c  d  e  f  g  h
*/
const U64 not_a_file = 18374403900871474942ULL;
const U64 not_h_file = 9187201950435737471ULL;
const U64 not_hg_file = 4557430888798830399ULL;
const U64 not_ab_file = 18229723555195321596ULL;

// pawn attack table [side][sq]
U64 pawn_attacks[2][64];

// generate pawn attacks
U64 mask_pawn_attacks(int side, int sq)
{
	// result attack bitboard
	U64 attacks = 0ULL;

	U64 bitboard = 0ULL;
	set_bit(bitboard, sq);

	// white pawn attacks
	if (!side)
	{
		attacks |= (bitboard >> 7) & not_a_file;
		attacks |= (bitboard >> 9) & not_h_file;
	}
	else
	{
		attacks |= (bitboard << 7) & not_h_file;
		attacks |= (bitboard << 9) & not_a_file;
	}
	return attacks;
}
void init_leaper_attack()
{
	for(int sq=0;sq<64;++sq)
	{
		pawn_attacks[white][sq]=mask_pawn_attacks(white,sq);
		pawn_attacks[black][sq]=mask_pawn_attacks(black,sq);
	}
}

void print_board(U64 bb)
{

	for (int rank = 0; rank < 8; ++rank)
	{
		for (int file = 0; file < 8; ++file)
		{
			int sq = rank * 8 + file;

			// print the rank
			if (file == 0)
				printf(" %d ", 8 - rank);
			printf(" %d ", get_bit(bb, sq) ? 1 : 0);
		}
		printf("\n");
	}

	// print the file
	printf("    a  b  c  d  e  f  g  h\n\n");
	// print bitboard as unsigned decimal
	printf("Bitboard: %llu\n", bb);
}

int main()
{
	init_leaper_attack();
	for(int sq=0;sq<64;++sq)
	{
		printf("Pawn attacks for black at sq %d\n",sq);
		print_board(pawn_attacks[black][sq]);
	}
	return 0;
}