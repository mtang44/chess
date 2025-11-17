
#include "Chess.h"
#include <limits>
#include <cmath>
#include <map>
#include <string>
#include "../Logger.h"



#define WHITE 1
#define BLACK -1

using namespace std;

Chess::Chess()
{
    _grid = new Grid(8, 8);
    
        // printBitboard(_knightBitBoards[0]);
        // printBitboard(_knightBitBoards[1]);

}

Chess::~Chess()
{
    delete _grid;
}

char Chess::pieceNotation(int x, int y) const
{
    const char *wpieces = { "0PNBRQK" };
    const char *bpieces = { "0pnbrqk" };
    Bit *bit = _grid->getSquare(x, y)->bit();
    char notation = '0';
    if (bit) {
        notation = bit->gameTag() < 128 ? wpieces[bit->gameTag()] : bpieces[bit->gameTag()-128];
    }
    return notation;
}

Bit* Chess::PieceForPlayer(const int playerNumber, ChessPiece piece)
{
    const char* pieces[] = { "pawn.png", "knight.png", "bishop.png", "rook.png", "queen.png", "king.png" };

    Bit* bit = new Bit();
    // should possibly be cached from player class?
    const char* pieceName = pieces[piece - 1];
    std::string spritePath = std::string("") + (playerNumber == 0 ? "w_" : "b_") + pieceName;
    bit->LoadTextureFromFile(spritePath.c_str());
    bit->setOwner(getPlayerAt(playerNumber));
    bit->setSize(pieceSize, pieceSize);

    return bit;
}

void Chess::setUpBoard()
{
    setNumberOfPlayers(2);
    _gameOptions.rowX = 8;
    _gameOptions.rowY = 8;

    _grid->initializeChessSquares(pieceSize, "boardsquare.png");
    
    //FENtoBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    FENtoBoard("rnbqkbnr/pppppppp/8/8/8/6p1/PPPPPPPP/RNBQKBNR");

   // FENtoBoard("rn1qkbnr/pp2pp1p/3pb1p1/2p1P3/2B5/2N2N2/PPPP1PPP/R1BQK2R");
    //FENtoBoard("r1bk3r/p2pBpNp/n4n2/1p1NP2P/6P1/3P4/P1P1K3/q5b1");
     startGame();
    for(int i = 0; i < 64; i++)
        {
            _knightBitBoards[i] = generateKnightMoveBitBoard(i);
            
            // _kingBitBoards[i] = generateKingMoveBitBoard(i);
        }
        string state = stateString();
    _moves = generateAllMoves(WHITE); // test pass in state?
 
}

void Chess::FENtoBoard(const std::string& fen) {
    // convert a FEN string to a board
    // FEN is a space delimited string with 6 fields
    // 1: piece placement (from white's perspective)
    // NOT PART OF THIS ASSIGNMENT BUT OTHER THINGS THAT CAN BE IN A FEN STRING
    // ARE BELOW
    // 2: active color (W or B)
    // 3: castling availability (KQkq or -)
    // 4: en passant target square (in algebraic notation, or -)
    // 5: halfmove clock (number of halfmoves since the last capture or pawn advance)

    // will need later.
    // map<char, string> piecesImages;
    // piecesImages['b'] = "b_bishop.png";
    // piecesImages['n'] = "b_knight.png";
    // piecesImages['r'] = "b_rook.png";
    // piecesImages['k'] = "b_king.png";
    // piecesImages['p'] = "b_pawn.png";
    // piecesImages['q'] = "b_queen.png";

    // piecesImages['B'] = "w_bishop.png";
    // piecesImages['N'] = "w_knight.png";
    // piecesImages['R'] = "w_rook.png";
    // piecesImages['K'] = "w_king.png";
    // piecesImages['P'] = "w_pawn.png";
    // piecesImages['Q'] = "w_queen.png";
    
    _grid->forEachSquare([](ChessSquare* square, int x, int y){
        square ->setBit(nullptr);
    });

    int row = 7;
    int col = 0; 
    for(char ch : fen)
    {
        if(ch == '/')
        {
            row--;
            col =0;
        }
        else if(isdigit(ch)){
            col += ch -'0'; // skip empty squares
        }
        else{
            ChessPiece piece = Pawn;
            switch(toupper(ch)){
            case 'P':
                piece = Pawn;
                break;
            case 'N':
                piece = Knight;
                break;
            case 'B':
                piece = Bishop;
                break;
            case 'R':
                piece = Rook;
                break;
            case 'Q':
                piece = Queen;
                break;

            case 'K':
                piece = King;
                break;
            }
            Bit* bit = PieceForPlayer(isupper(ch) ? 0 : 1, piece);//uppercase white player, lower for black
            ChessSquare * square = _grid->getSquare(col, row);
            bit->setPosition(square->getPosition());
            bit->setParent(square);
            bit->setGameTag(isupper(ch)? piece : (piece + 128));
            square->setBit(bit);
            col++; 

        }
    }


    
}

bool Chess::actionForEmptyHolder(BitHolder &holder)
{
    return false;
}

bool Chess::canBitMoveFrom(Bit &bit, BitHolder &src)
{
    // need to implement friendly/unfriendly in bit so for now this hack
    int currentPlayer = getCurrentPlayer()->playerNumber() * 128;
    int pieceColor = bit.gameTag() & 128;
    if (pieceColor == currentPlayer) {
        bool temp = false;
        ChessSquare * square = (ChessSquare *) &src;
        if(square)
        {
            int squareIndex = square->getSquareIndex();
            for(auto move :_moves){
                if(move.from == squareIndex)
                {
                temp = true;
                auto dest = _grid->getSquareByIndex(move.to);
                dest->setHighlighted(true);
                }
            }
        }
        return temp;
    }
    return false;
}

bool Chess::canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst)
{
    ChessSquare * Sdst = (ChessSquare *) &dst;
    ChessSquare * Ssrc = (ChessSquare *) &src;
    if(Sdst)
    {
        int dstIndex = Sdst->getSquareIndex();
        int srcIndex = Ssrc->getSquareIndex();
        for(auto move: _moves){
            if(move.to == dstIndex && move.from == srcIndex)
            {
                return true;
            }
        }
    }
    return false;
}

void Chess::stopGame()
{
    _grid->forEachSquare([](ChessSquare* square, int x, int y) {
        square->destroyBit();
    });
}

Player* Chess::ownerAt(int x, int y) const
{
    if (x < 0 || x >= 8 || y < 0 || y >= 8) {
        return nullptr;
    }

    auto square = _grid->getSquare(x, y);
    if (!square || !square->bit()) {
        return nullptr;
    }
    return square->bit()->getOwner();
}

Player* Chess::checkForWinner()
{
    return nullptr;
}

bool Chess::checkForDraw()
{
    char currentPlayerColor = getCurrentPlayer() ->playerNumber() == 0? WHITE : BLACK;
    cout << "Generating all moves for player color: " + to_string(currentPlayerColor);
    _moves = generateAllMoves(currentPlayerColor);
    return false;
}

std::string Chess::initialStateString()
{
    return stateString();
}

std::string Chess::stateString()
{
    std::string s;
    s.reserve(64);
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
            s += pieceNotation( x, y );
        }
    );
    return s;}

void Chess::setStateString(const std::string &s)
{
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
        int index = y * 8 + x;
        char playerNumber = s[index] - '0';
        if (playerNumber) {
            square->setBit(PieceForPlayer(playerNumber - 1, Pawn));
        } else {
            square->setBit(nullptr);
        }
    });
}
// creates bitboard for knights 
BitBoard Chess::generateKnightMoveBitBoard(int square)
{
    BitBoard bitboard = 0ULL;
    int rank = square/8;
    int file = square % 8;
    pair<int, int> knightOffsets[] ={ 
        {2,1},{2,-1}, {-2,1},{-2,-1},
        {1,2}, {1,-2}, {-1,2}, {-1,-2}};
    constexpr uint64_t oneBit =1;
    for(auto[dr,df] : knightOffsets)
    {
        int r = rank + dr, f = file + df;
        if(r >= 0 && r < 8 && f >= 0 && f < 8)
        {
            bitboard |= oneBit << (r * 8 + f);
        }
    }
    return bitboard;
}
// Generate actual move objects from a bitboard
void Chess::generateKnightMoves(std::vector<BitMove>& moves, BitboardElement knightBoard, uint64_t emptySquares) {
   if(knightBoard.getData() == 0){
        return;
   }
    knightBoard.forEachBit([&](int fromSquare) {
        BitboardElement moveBitboard = BitboardElement(_knightBitBoards[fromSquare].getData() & emptySquares);
        // Efficiently iterate through only the set bits
        moveBitboard.forEachBit([&](int toSquare) {
           moves.emplace_back(fromSquare, toSquare, Knight);
        });
    });
    // while(knightBoard)
    // {
    //     int fromSquare = bitScanForward(knightBoard);
    //     uint64_t moveBitboard = _knightBitBoards[fromSquare];
    //     //printBitboard(moveBitboard);
    //     //efficiently iterates through only the set bits
    //     while(moveBitboard)
    //     {
    //         int toSquare = bitScanForward(moveBitboard);
    //         if(!(occupancy & (1ULL<<toSquare))){
    //             moves.emplace_back(BitMove(fromSquare,toSquare,Knight));
    //         }
    //         //clear the lowest set bit
    //         moveBitboard &=moveBitboard -1;
    //     }
    //     //clear the lowest set bit
    //     knightBoard &= knightBoard -1;
    // }
    
}
void Chess::clearBoardHighlights()
{
    _grid->forEachSquare([](ChessSquare*square, int x, int y){

        square->setHighlighted(false);
    });
}
std::vector<BitMove> Chess:: generateAllMoves(char color)
{
    std::vector<BitMove> moves;
    moves.reserve(32);
    std::string state = stateString();

    uint64_t whiteKnights = 0LL;
    uint64_t whitePawns = 0LL;
    uint64_t whiteKing = 0LL;
    uint64_t whiteRooks = 0LL;
    uint64_t whiteBishops = 0LL;
    uint64_t whiteQueen = 0LL;
    uint64_t blackKnights = 0LL;
    uint64_t blackPawns = 0LL;
    uint64_t blackKing = 0LL;
    uint64_t blackRooks = 0LL;
    uint64_t blackBishops = 0LL;
    uint64_t blackQueen = 0LL;
    const char *whitePieces = "PNBRQK";
    const char *blackPieces = "pnbrqk";

    for(int i = 0; i < 64; i++)
    {
        if(state[i] == whitePieces[0]) whitePawns |= 1ULL << i;
        else if(state[i] == whitePieces[1]) whiteKnights |= 1ULL<<i;
        else if(state[i] == whitePieces[2]) whiteBishops |= 1ULL<<i;
        else if(state[i] == whitePieces[3]) whiteRooks |= 1ULL<<i;
        else if(state[i] == whitePieces[4]) whiteQueen |= 1ULL<<i;
        else if(state[i] == whitePieces[5]) whiteKing |= 1ULL<<i;
        else if(state[i] == blackPieces[0]) blackPawns |= 1ULL << i;
        else if(state[i] == blackPieces[1]) blackKnights |= 1ULL<<i;
        else if(state[i] == blackPieces[2]) blackBishops |= 1ULL<<i;
        else if(state[i] == blackPieces[3]) blackRooks |= 1ULL<<i;
        else if(state[i] == blackPieces[4]) blackQueen |= 1ULL<<i;
        else if(state[i] == blackPieces[5]) blackKing |= 1ULL<<i;
    }

    uint64_t whiteOccupancy = whiteKnights | whitePawns | whiteRooks | whiteBishops | whiteQueen | whitePawns |1ULL<<17 ; 
    uint64_t blackOccupancy = blackKnights | blackPawns | blackRooks | blackBishops | blackQueen | blackPawns |1ULL<<17; 
    if(color == BLACK)
    {
        generateKnightMoves(moves,blackKnights,~blackOccupancy);
        generatePawnMoves(moves,blackPawns,whiteOccupancy,~whiteOccupancy &~blackOccupancy, color);// generatePawnMovesList(moves,blackPawns,whiteOccupancy,blackOccupancy,0, BLACK);
       
        return moves;
    }
    generateKnightMoves(moves,whiteKnights,~whiteOccupancy);
    generatePawnMoves(moves,whitePawns,blackOccupancy,~whiteOccupancy &~blackOccupancy, color);// generatePawnMovesList(moves,whitePawns,blackOccupancy,whiteOccupancy,0, WHITE);
   

    return moves;

}
void Chess::addPawnBitBoardMovesToList(std::vector<BitMove> & moves, BitboardElement bitboard, int  shift)
{
    if(bitboard.getData() == 0)
    {
        return;
    }
    bitboard.forEachBit([&](int toSquare)
    {
        int fromSquare = toSquare - shift;
        moves.emplace_back(fromSquare,toSquare,Pawn);
    });
}


void Chess:: generatePawnMoves(std::vector<BitMove> &moves,const BitboardElement pawns,const BitboardElement enemyPieces, const BitboardElement emptySquares, char color)
{
    // pawns << 8 shifts all pawns up one rank
    // pawns << 16 shifts all pawns up two ranks

    //singlePush = pawns << 8
    //single & !occupied
    // RANK_3 = 0x0000000000FF0000
    // doublePush = onStartRank << 8

    //left attack = pawns << 7
    //right attacks = pawns << 9
    

    if(pawns.getData() == 0)
    {
        return;
    }
    constexpr uint64_t NotAFile(0xFEFEFEFEFEFEFEFEULL); // A file mask
    constexpr uint64_t NotHFile(0x7F7F7F7F7F7F7FULL); // H file mask 
    constexpr uint64_t Rank3(0x0000000000FF0000ULL); // rank 3 mask
    constexpr uint64_t Rank6(0x0000FF0000000000ULL);    // rank6 mask

    //calculate pawn moes forward
    BitboardElement singleMoves = (color == WHITE) ? (pawns.getData() << 8) & emptySquares.getData() : (pawns.getData() >> 8) & emptySquares.getData();
    BitboardElement doubleMoves = (color == WHITE) ? ((singleMoves.getData() & Rank3) <<8) & emptySquares.getData() : ((singleMoves.getData() & Rank6) >> 8) & emptySquares.getData();
    BitboardElement capturesLeft = (color == WHITE) ?((pawns.getData() & NotAFile) <<7) & enemyPieces.getData() : ((pawns.getData() & NotAFile) >> 9) & enemyPieces.getData();
    BitboardElement capturesRight = (color == WHITE) ?((pawns.getData() & NotHFile) <<9) & enemyPieces.getData() : ((pawns.getData() & NotHFile) >> 7) & enemyPieces.getData();

   int shiftForward = (color == WHITE) ? 8 : -8;
   int doubleShift = (color == WHITE) ? 16 : -16;
   int captureLeftShift = (color == WHITE) ? 7 : -9;
   int captureRightShift = (color == WHITE) ? 9 : -7;

   //add single pawn moves to the List
   addPawnBitBoardMovesToList(moves,singleMoves,shiftForward);
   addPawnBitBoardMovesToList(moves,doubleMoves,doubleShift);
   addPawnBitBoardMovesToList(moves,capturesLeft, captureLeftShift);
   addPawnBitBoardMovesToList(moves,capturesRight, captureRightShift);

}