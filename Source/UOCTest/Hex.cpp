// Fill out your copyright notice in the Description page of Project Settings.


#include "Hex.h"


Hex::Hex(const int q, const int r, const int s): Q(q), R(r), S(s)
{
	if (q + r + s != 0)
	{
		throw "q + r + s must be 0!";
	}
}

Hex::~Hex()
{
}
