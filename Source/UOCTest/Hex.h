// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
struct UOCTEST_API Hex
{	
	Hex() : Q(0), R(0), S(0) {}

	Hex(const int q, const int r) : Q(q), R(r), S(-q-r) {}
	
	Hex(const int q, const int r, const int s) : Q(q), R(r), S(s) {}
	
	~Hex();

	int Q;
	int R;
	int S;

	void operator=(const Hex Tile)
	{
		Q = Tile.Q;
		R = Tile.R;
		S = Tile.S;
	}

	bool operator==(const Hex Tile) const
	{
		return Q == Tile.Q && R == Tile.R && S == Tile.S;
	}

	bool operator < (const Hex& Tile) const
	{
		return std::tie(Q, R, S) < std::tie(Tile.Q, Tile.R, Tile.S);
	}
};
