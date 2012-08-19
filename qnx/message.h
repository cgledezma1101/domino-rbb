/*
 * message.h
 *
 *  Created on: 20/08/2012
 *      Author: C.G Ledezma
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

/*
 * Message indicating that a player has passed
 *
 * Attributes: id -> Identification of the player that passed
 */
struct MsgPass
{
   int id;
};

/*
 * Message that gives the player the initial hand with which he shall play
 *
 * Attributes: id -> Identifier assigned to the player
 *             pieces -> Array with 14 integers, where the values 2*i, 2*i + 1
 *                       for i = 0 .. 6, indicate the values of piece i
 */
struct MsgInitialHand
{
   int id;
   int pieces[14];
};

/*
 * Message notifying a player has placed a piece on the table
 *
 * Attributes: id -> Identifier of the player that placed the piece
 *             v0 -> One side of the dominoe played
 *             v1 -> The other side of the dominoe played
 *             s -> Either v0 or v1, indicating which of them was joined to the
 *                  pieces already on the board
 */
struct MsgPlay
{
   int id;
   int v0;
   int v1;
   int s;
};

/*
 * Message notifying the final scores of the hand after it has finished
 *
 * Attributes: score0 -> Accumulated score for team 0
 *             score1 -> Accumulated score for team 1
 *             pieces -> Array containing the pieces left by all players
 *             delimit -> Delimiter indicating the end of each hand on the
 *                        'pieces' array
 */
struct MsgScore
{
   int score0;
   int score1;
   int *pieces;
   int delimit[4];
};

/*
 * Representation of a message received by the server. The structure contains
 * an union that has a specific type of message and an integer indicating which
 * specific message was received.
 *
 * Attributes: type -> Integer representing which element of the union is valid
 *
 *    These are the possible values for type, and it's meaning:
 *
 *    0 = Player passed. The union's valid value will be mp
 *    1 = Initial hand. The union's valid value will be mi
 *    2 = Turn yield. The union will have no valid value, for it is not necessary
 *    3 = PLayers move. The union's valid value will be mpp
 *    4 = End of hand. The union will have no valid value, for it is not necessary
 *    5 = Partial scores. The union's valid value will be ms
 *
 *             mr -> union containing a specific message structure
 */
struct Message
{
   int type;
   union mr
   {
      struct MsgPass mp;
      struct MsgInitialHand mi;
      struct MsgPlay mpp;
      struct MsgScore ms;
   };
};


#endif /* MESSAGE_H_ */
