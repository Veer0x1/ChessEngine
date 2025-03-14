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
// nigh attack table [sq]
U64 knight_attacks[64];
// king attack table [sq]
U64 king_attacks[64];

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

// generate knight attacks
U64 mask_knight_attacks(int sq)
{
	U64 attacks = 0ULL;
	U64 bitboard = 0ULL;
	set_bit(bitboard, sq);

	attacks |= (bitboard << 17) & not_a_file;
	attacks |= (bitboard << 15) & not_h_file;
	attacks |= (bitboard << 10) & not_ab_file;
	attacks |= (bitboard << 6) & not_hg_file;
	attacks |= (bitboard >> 17) & not_h_file;
	attacks |= (bitboard >> 15) & not_a_file;
	attacks |= (bitboard >> 10) & not_hg_file;
	attacks |= (bitboard >> 6) & not_ab_file;

	return attacks;
}

// generate king attacks
U64 mask_king_attacks(int sq)
{
	U64 attacks = 0ULL;
	U64 bitboard = 0ULL;
	set_bit(bitboard, sq);

	attacks |= bitboard << 8;
	attacks |= bitboard >> 8;
	attacks |= (bitboard << 1) & not_a_file;
	attacks |= (bitboard >> 1) & not_h_file;
	attacks |= (bitboard << 9) & not_a_file;
	attacks |= (bitboard >> 9) & not_h_file;
	attacks |= (bitboard << 7) & not_h_file;
	attacks |= (bitboard >> 7) & not_a_file;
	return attacks;
}

// mask bishop attacks
U64 mask_bishop_attacks(int sq)
{
	U64 attacks = 0ULL;

	// init ranks and file
	int r,f;

	// int target rank and file
	int tr = sq / 8;
	int tf = sq % 8;

	// mask relevant bishop occupancy bits
	for(r=tr+1,f=tf+1;r<=6 && f<=6;++r,++f)
		attacks |= (1ULL << (r*8+f));
	for(r=tr+1,f=tf-1;r<=6 && f>=1;++r,--f)
		attacks |= (1ULL << (r*8+f));
	for(r=tr-1,f=tf+1;r>=1 && f<=6;--r,++f)
		attacks |= (1ULL << (r*8+f));
	for(r=tr-1,f=tf-1;r>=1 && f>=1;--r,--f)
		attacks |= (1ULL << (r*8+f));

	return attacks;
}

// mask rook attacks
U64 mask_rook_attacks(int sq)
{
	U64 attacks = 0ULL;

	// init ranks and file
	int r,f;

	// int target rank and file
	int tr = sq / 8;
	int tf = sq % 8;

	// mask relevant rook occupancy bits
	for(r=tr+1;r<=6;++r)
		attacks |= (1ULL << (r*8+tf));
	for(r=tr-1;r>=1;--r)
		attacks |= (1ULL << (r*8+tf));
	for(f=tf+1;f<=6;++f)
		attacks |= (1ULL << (tr*8+f));
	for(f=tf-1;f>=1;--f)
		attacks |= (1ULL << (tr*8+f));

	return attacks;
}


void init_leaper_attack()
{
	for(int sq=0;sq<64;++sq)
	{
		// pawn attacks
		pawn_attacks[white][sq]=mask_pawn_attacks(white,sq);
		pawn_attacks[black][sq]=mask_pawn_attacks(black,sq);

		// knight attacks
		knight_attacks[sq]=mask_knight_attacks(sq);

		// king attacks
		king_attacks[sq]=mask_king_attacks(sq);
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
	print_board(mask_rook_attacks(d4));
	return 0;
}