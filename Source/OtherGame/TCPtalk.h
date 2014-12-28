// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Networking.h"

/**
 * 
 */
//UCLASS(CPF_BlueprintReadOnly)
class OTHERGAME_API TCPtalk
{
public:
	FSocket* Socket;

	UFUNCTION(BlueprintCallable, Category = "TCP Talk")
	TCPtalk();
	~TCPtalk();
};
