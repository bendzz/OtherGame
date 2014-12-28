// Fill out your copyright notice in the Description page of Project Settings.




#include "OtherGame.h"
#include "TCPconnection.h"


/*
ATCPconnection::ATCPconnection() : AActor()
{
}
*/
/*
ATCPconnection::~ATCPconnection()
{
	//Erase the engine timer, which seems to keep going as long as the UE4 editor remains open (after the game has ended)
	GetWorldTimerManager().ClearAllTimersForObject(this);
}
*/

void ATCPconnection::IsPendingKill(){
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Garbage collecting called"));	//TODO Fix
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

void ATCPconnection::SendMessage()
{
	
	
	FSocket* Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

	FString address = TEXT("127.0.0.1");
	int32 port = 8888;
	//int32 port = 3939;
	FIPv4Address ip;
	FIPv4Address::Parse(address, ip);



	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	addr->SetIp(ip.GetValue());
	addr->SetPort(port);

	bool connected = Socket->Connect(*addr);

	FString serialized = TEXT("loadPlayer|1");
	TCHAR *serializedChar = serialized.GetCharArray().GetData();
	int32 size = FCString::Strlen(serializedChar);
	int32 sent = 0;

	bool successful = Socket->Send((uint8*)TCHAR_TO_UTF8(serializedChar), size, sent);

}

void ATCPconnection::Laaaaaauuuunch()
{

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Laaaaunch called"));	//TODO Fix

	//IP = 127.0.0.1, Port = 8890 for my Python test case
	if (!StartTCPReceiver("RamaSocketListener", "127.0.0.1", 8888))
	{
		//UE_LOG  "TCP Socket Listener Created!"
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Laaaaunch done"));	//TODO Fix

	//UE_LOG  "TCP Socket Listener Created! Yay!"
}

//Rama's Start TCP Receiver
bool ATCPconnection::StartTCPReceiver(
	const FString& YourChosenSocketName,
	const FString& TheIP,
	const int32 ThePort
	){
	//Rama's CreateTCPConnectionListener
	ListenerSocket = CreateTCPConnectionListener(YourChosenSocketName, TheIP, ThePort);

	//Not created?
	if (!ListenerSocket)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("StartTCPReceiver>> Listen socket could not be created! ~> %s %d"), *TheIP, ThePort));
		return false;
	}

	//Start the Listener! //thread this eventually
	GetWorldTimerManager().SetTimer(this,
		&ATCPconnection::TCPConnectionListener, 0.01, true);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("StartTCPReceiver ended"));	//TODO Fix

	return true;
}
//Format IP String as Number Parts
bool ATCPconnection::FormatIP4ToNumber(const FString& TheIP, uint8(&Out)[4])
{
	//IP Formatting
	TheIP.Replace(TEXT(" "), TEXT(""));

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//						   IP 4 Parts

	//String Parts
	TArray<FString> Parts;
	TheIP.ParseIntoArray(&Parts, TEXT("."), true);
	if (Parts.Num() != 4)
		return false;

	//String to Number Parts
	for (int32 i = 0; i < 4; ++i)
	{
		Out[i] = FCString::Atoi(*Parts[i]);
	}

	return true;
}
//Rama's Create TCP Connection Listener
FSocket* ATCPconnection::CreateTCPConnectionListener(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort, const int32 ReceiveBufferSize)
{
	uint8 IP4Nums[4];
	if (!FormatIP4ToNumber(TheIP, IP4Nums))
	{
		//VShow("Invalid IP! Expecting 4 parts separated by .");	//TODO Fix
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("HELLO WORLD"));
		return false;
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//Create Socket
	FIPv4Endpoint Endpoint(FIPv4Address(IP4Nums[0], IP4Nums[1], IP4Nums[2], IP4Nums[3]), ThePort);
	FSocket* ListenSocket = FTcpSocketBuilder(*YourChosenSocketName)
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.Listening(8);

	//Set Buffer Size
	int32 NewSize = 0;
	ListenSocket->SetReceiveBufferSize(ReceiveBufferSize, NewSize);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("CreateTCPConnectionListener ended"));	//TODO Fix

	//Done!
	return ListenSocket;
}
//Rama's TCP Connection Listener
void ATCPconnection::TCPConnectionListener()
{

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("TCPConnectionListener  Started"));	//TODO Fix
	//~~~~~~~~~~~~~
	if (!ListenerSocket) return;
	//~~~~~~~~~~~~~

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("TCPConnectionListener Found socket"));	//TODO Fix

	//Remote address
	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool Pending;

	// handle incoming connections
	if (ListenerSocket->HasPendingConnection(Pending) && Pending)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("TCPConnectionListener connection pending"));	//TODO Fix

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//Already have a Connection? destroy previous
		if (ConnectionSocket)
		{
			ConnectionSocket->Close();
			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectionSocket);
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		//New Connection receive!
		ConnectionSocket = ListenerSocket->Accept(*RemoteAddress, TEXT("RamaTCP Received Socket Connection"));

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("-- RamaTCP Received Socket Connection"));	//TODO Fix

		if (ConnectionSocket != NULL)
		{
			//Global cache of current Remote Address
			RemoteAddressForConnection = FIPv4Endpoint(RemoteAddress);

			//UE_LOG "Accepted Connection! WOOOHOOOO!!!";

			//can thread this too
			GetWorldTimerManager().SetTimer(this,
				&ATCPconnection::TCPSocketListener, 0.01, true);
		}
	}
}

//Rama's String From Binary Array
//This function requires 
//		#include <string>
FString ATCPconnection::StringFromBinaryArray(const TArray<uint8>& BinaryArray)
{
	//Create a string from a byte array!
	std::string cstr(reinterpret_cast<const char*>(BinaryArray.GetData()), BinaryArray.Num());
	return FString(cstr.c_str());
}

//Rama's TCP Socket Listener
void ATCPconnection::TCPSocketListener()
{
	//~~~~~~~~~~~~~
	if (!ConnectionSocket) return;
	//~~~~~~~~~~~~~


	//Binary Array!
	TArray<uint8> ReceivedData;

	uint32 Size;
	while (ConnectionSocket->HasPendingData(Size))
	{
		ReceivedData.Init(FMath::Min(Size, 65507u));

		int32 Read = 0;
		ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Data Read! %d"), ReceivedData.Num()));
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (ReceivedData.Num() <= 0)
	{
		//No Data Received
		return;
	}

	//VShow("Total Data read!", ReceivedData.Num());	//TODO Fix
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Data Bytes Read ~> %d"), ReceivedData.Num()));


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//						Rama's String From Binary Array
	const FString ReceivedUE4String = StringFromBinaryArray(ReceivedData);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


	//VShow("As String!!!!! ~>", ReceivedUE4String);	//TODO Fix
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("As String Data ~> %s"), *ReceivedUE4String));

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT(std::string("-- As String Data ~> ") + *ReceivedUE4String));	//TODO Fix
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT(std::string(*ReceivedUE4String)));	//TODO Fix
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT(*ReceivedUE4String));	//TODO Fix
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("-- Received data. "));	//TODO Fix
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT(*ReceivedUE4String));	//TODO Fix
}


