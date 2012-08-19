/*
 * Domino.h
 *
 *  Created on: 17/08/2012
 *      Author: fjgimon
 */

#ifndef DOMINO_H_
#define DOMINO_H_

class Domino {

public:
	// Values of the sides of the piece
	int x, y;

	// Constructors
	Domino(int x1, int y1);
	Domino();
	// Destroyer
	virtual ~Domino();
};

#endif /* DOMINO_H_ */
