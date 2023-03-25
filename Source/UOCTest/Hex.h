// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
struct UOCTEST_API Hex
{
	Hex(const int q, const int r, const int s);
	
	~Hex();

	const int Q;
	const int R;
	const int S;
};
