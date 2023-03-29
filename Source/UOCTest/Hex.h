// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexEnum.h"

/**
 * 
 */
struct UOCTEST_API Hex
{	
	Hex() :
        Q(0), R(0), S(0), HexType(EHexTypes::Invalid) {}

    Hex(const int q, const int r) :
        Q(q), R(r), S(-q-r), HexType(EHexTypes::Invalid), Neighbors(6) {}

	Hex(const int q, const int r, const EHexTypes hexType) :
        Q(q), R(r), S(-q-r), HexType(hexType), Neighbors(6) {}

    Hex(const int q, const int r, const int s) :
        Q(q), R(r), S(s), HexType(EHexTypes::Invalid), Neighbors(6) {}
	
	Hex(const int q, const int r, const int s, const EHexTypes hexType) :
        Q(q), R(r), S(s), HexType(hexType), Neighbors(6) {}
	
	bool operator==(const Hex Tile) const
    {
        return Q == Tile.Q && R == Tile.R && S == Tile.S;
    }

    bool operator < (const Hex& Tile) const
    {
        return std::tie(Q, R, S) < std::tie(Tile.Q, Tile.R, Tile.S);
    }
    
	int Q;
	int R;
	int S;

    EHexTypes HexType;

    std::vector<Hex> Neighbors;
};
