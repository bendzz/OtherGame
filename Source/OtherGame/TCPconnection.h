// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Networking.h"
#include <string>

#include "GameFramework/Actor.h"
#include "TCPconnection.generated.h"

/**
 * 
 */
UCLASS()
class OTHERGAME_API ATCPconnection : public AActor
{
	GENERATED_BODY()
	
	public:

		//ATCPconnection();
		//~ATCPconnection();	//Require the deconstructor only		//EDIT: No constructors for UCLASSes

		void IsPendingKill();
	
		//Connect to TCP server
		UFUNCTION(BlueprintCallable, Category = "TCP Chat")
		//UFUNCTION(BlueprintCallable, Category = "Switch Functions")
		virtual void SendMessage();


		FSocket* ListenerSocket;
		FSocket* ConnectionSocket;
		FIPv4Endpoint RemoteAddressForConnection;

		UFUNCTION(BlueprintCallable, Category = "TCP Chat")
		void Laaaaaauuuunch();


		bool StartTCPReceiver(
			const FString& YourChosenSocketName,
			const FString& TheIP,
			const int32 ThePort
			);

		FSocket* CreateTCPConnectionListener(
			const FString& YourChosenSocketName,
			const FString& TheIP,
			const int32 ThePort,
			const int32 ReceiveBufferSize = 2 * 1024 * 1024
			);

		//Timer functions, could be threads
		void TCPConnectionListener(); 	//can thread this eventually
		void TCPSocketListener();		//can thread this eventually


		//Format String IP4 to number array
		bool FormatIP4ToNumber(const FString& TheIP, uint8(&Out)[4]);

		//Rama's StringFromBinaryArray
		FString StringFromBinaryArray(const TArray<uint8>& BinaryArray);



	//private:
	//	FSocket* ListenerSocket;
};
