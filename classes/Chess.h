
#pragma once

#include "Game.h"
#include "Grid.h"
#include "BitBoard.h"
#include "MagicBitBoards.h"


constexpr int pieceSize = 80;
typedef uint64_t BitBoard;
constexpr BitBoard BitZero = 1ULL;
constexpr int negativeInfinity = -1000000;
constexpr int positiveInfinity = 1000000;
constexpr int WHITE = +1;
constexpr int BLACK = -1;

enum AllBitBoards
{
    WhitePawnBitBoard,
    WhiteKnightBitBoard,
    WhiteBishopBitBoard,
    WhiteRookBitBoard,
    WhiteQueenBitBoard,
    WhiteKingBitBoard,

    BlackPawnBitBoard,
    BlackKnightBitBoard,
    BlackBishopBitBoard,
    BlackRookBitBoard,
    BlackQueenBitBoard,
    BlackKingBitBoard,

    OCCUPANCY,
    EMPTY_SQUARES,
    BLACK_ALL_PIECES,
    WHITE_ALL_PIECES,
    e_numBitBoards
};
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
    bool gameHasAI() override { return true; }

    std::string initialStateString() override;
    std::string stateString() override;
    void setStateString(const std::string &s) override;

    Grid* getGrid() override { return _grid; }

    BitBoard generateKnightMoveBitBoard(int square);
    BitBoard generateKingMoveBitBoard(int square);
    void generateKnightMoves(std::vector<BitMove>& moves, BitboardElement knightBoard, uint64_t emptySquares);
    void generateKingMoves(std::vector<BitMove> & moves, BitboardElement kingBoard, uint64_t emptySquares);
    void generatePawnMoves(std::vector<BitMove> &moves,const BitboardElement pawns,const BitboardElement enemyPieces, const BitboardElement occupancy, char color);

    void generateBishopMoves(std::vector<BitMove>& moves, BitboardElement bishopBoard, uint64_t occupancy, uint64_t friendlyPieces);
    void generateQueenMoves(std::vector<BitMove>& moves, BitboardElement queenBoard, uint64_t occupancy, uint64_t friendlyPieces);
    void generateRookMoves(std::vector<BitMove>& moves, BitboardElement rookBoard, uint64_t occupancy, uint64_t friendlyPieces);
    
    void addPawnBitBoardMovesToList(std::vector<BitMove> & moves, BitboardElement bitboard, int  shift);
    std::vector<BitMove> generateAllMoves(const std::string& state, char color);
    void clearBoardHighlights();

private:
    Bit* PieceForPlayer(const int playerNumber, ChessPiece piece);
    Player* ownerAt(int x, int y) const;
    void FENtoBoard(const std::string& fen);
    char pieceNotation(int x, int y) const;
    void updateAI();
    int negamax(std::string& state, int depth, int alpha, int beta, int playerColor);
    int evaluateBoard(const std::string& state);
    

    Grid* _grid;
    int _currentPlayer;
    BitboardElement _knightBitBoards[64];
    BitboardElement _kingBitBoards[64];
    BitboardElement _bishopBitBoards[64];
    int _bitBoardLookup[128];
    int _countMoves = 0;
    BitboardElement _bitBoards[e_numBitBoards];
    std::vector<BitMove> _moves;


    
};