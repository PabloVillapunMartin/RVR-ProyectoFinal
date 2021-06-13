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

void ConfirmationLoginMessage::to_bin(){
	int messageSize = sizeof(MsgId) + sizeof(int);

	alloc_data(messageSize);

	memset(_data, 0, messageSize);

	char* bufferPointer = _data;

	memcpy(bufferPointer, &id, sizeof(MsgId));
	bufferPointer += sizeof(MsgId);

	memcpy(bufferPointer, &gameObjectID, sizeof(int));
}

int ConfirmationLoginMessage::from_bin(char* obj){
	int messageSize = sizeof(MsgId) + sizeof(int);
	
	alloc_data(messageSize);

	memcpy(static_cast<void*>(_data), obj, messageSize);

	char* bufferPointer = _data;

	memcpy(&id, bufferPointer, sizeof(MsgId));
	bufferPointer += sizeof(MsgId);

	memcpy(&gameObjectID, bufferPointer, sizeof(int));

	return 0;
}

void StartGameMessage::to_bin(){
	int messageSize = sizeof(MsgId) + (8 * sizeof(int));

	alloc_data(messageSize);

	memset(_data, 0, messageSize);

	char* bufferPointer = _data;

	memcpy(bufferPointer, &id, sizeof(MsgId));
	bufferPointer += sizeof(MsgId);

	memcpy(bufferPointer, &x1, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(bufferPointer, &y1, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(bufferPointer, &x2, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(bufferPointer, &y2, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(bufferPointer, &x3, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(bufferPointer, &y3, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(bufferPointer, &x4, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(bufferPointer, &y4, sizeof(int));
}

int StartGameMessage::from_bin(char* obj){
	int messageSize = sizeof(MsgId) + (8 * sizeof(int));
	
	alloc_data(messageSize);

	memcpy(static_cast<void*>(_data), obj, messageSize);

	char* bufferPointer = _data;

	memcpy(&id, bufferPointer, sizeof(MsgId));
	bufferPointer += sizeof(MsgId);

	memcpy(&x1, bufferPointer, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(&y1, bufferPointer, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(&x2, bufferPointer, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(&y2, bufferPointer, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(&x3, bufferPointer, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(&y3, bufferPointer, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(&x4, bufferPointer, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(&y4, bufferPointer, sizeof(int));

	return 0;
}