
#pragma once

#include "Game.h"
#include "Grid.h"
#include "BitBoard.h"


constexpr int pieceSize = 80;
typedef uint64_t BitBoard;
constexpr BitBoard BitZero = 1ULL;
// enum ChessPiece
// {
//     NoPiece,
//     Pawn,
//     Knight,
//     Bishop,
//     Rook,
//     Queen,
//     King
// };


class Chess : public Game
{
public:
    Chess();
    ~Chess();

    void setUpBoard() override;

    bool canBitMoveFrom(Bit &bit, BitHolder &src) override;
    bool canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override;
    bool actionForEmptyHolder(BitHolder &holder) override;

    void stopGame() override;

    Player *checkForWinner() override;
    bool checkForDraw() override;

    std::string initialStateString() override;
    std::string stateString() override;
    void setStateString(const std::string &s) override;

    Grid* getGrid() override { return _grid; }

    BitBoard generateKnightMoveBitBoard(int square);
    void generateKnightMoves(std::vector<BitMove>& moves, BitboardElement knightBoard, uint64_t emptySquares);
    void generatePawnMoves(std::vector<BitMove> &moves,const BitboardElement pawns,const BitboardElement enemyPieces, const BitboardElement occupancy, char color);
    
    void addPawnBitBoardMovesToList(std::vector<BitMove> & moves, BitboardElement bitboard, int  shift);
    std::vector<BitMove> generateAllMoves(char color);
    void clearBoardHighlights();

private:
    Bit* PieceForPlayer(const int playerNumber, ChessPiece piece);
    Player* ownerAt(int x, int y) const;
    void FENtoBoard(const std::string& fen);
    char pieceNotation(int x, int y) const;
    

    Grid* _grid;
    BitboardElement _knightBitBoards[64];
    BitboardElement _pawnBitBoards[64];
    BitboardElement _kingBitBoards[64];
    std::vector<BitMove> _moves;
};