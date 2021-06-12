#include "NetworkMessages.h"
#include <memory>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int NetworkMessage::from_bin(char* obj){
	alloc_data(sizeof(MsgId));

	memcpy(static_cast<void *>(_data), obj, sizeof(MsgId));

	char* bufferPointer = _data;

	memcpy(bufferPointer, &id, sizeof(MsgId));

	return 0;
}

void TestingMessage::to_bin(){
	int messageSize = sizeof(MsgId) + sizeof(int);

	alloc_data(messageSize);

	memset(_data, 0, messageSize);

	char* bufferPointer = _data;

	memcpy(bufferPointer, &id, sizeof(MsgId));
	bufferPointer += sizeof(MsgId);

	memcpy(bufferPointer, &win, sizeof(int));
}

int TestingMessage::from_bin(char* obj){
	int messageSize = sizeof(MsgId) + sizeof(int);
	
	alloc_data(messageSize);

	memcpy(static_cast<void*>(_data), obj, messageSize);

	char* bufferPointer = _data;

	memcpy(&id, bufferPointer, sizeof(MsgId));
	bufferPointer += sizeof(MsgId);

	memcpy(&win, bufferPointer, sizeof(int));

	return 0;
}