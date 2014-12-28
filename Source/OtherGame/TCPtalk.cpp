// Fill out your copyright notice in the Description page of Project Settings.

#include "OtherGame.h"
#include "TCPtalk.h"


TCPtalk::TCPtalk()
{
	FSocket* Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

	FString address = TEXT("127.0.0.1");
	int32 port = 8888;
	//int32 port = 3939;
	FIPv4Address ip;
	FIPv4Address::Parse(address, ip);



//	TSharedRef addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
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

TCPtalk::~TCPtalk()
{
}
