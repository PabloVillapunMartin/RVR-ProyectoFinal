#include "NetworkMessages.h"
#include <memory>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

int NetworkMessage::from_bin(char* obj){
	alloc_data(sizeof(MsgId));

	memcpy(static_cast<void *>(_data), obj, sizeof(MsgId));

	char* bufferPointer = _data;

	memcpy(&id, bufferPointer, sizeof(MsgId));

	return 0;
}
/////////////////////////LOGIN CLIENT//////////////////////////////////
void LoginClientMessage::to_bin(){
	int messageSize = sizeof(MsgId) + 20 * sizeof(char);

	alloc_data(messageSize);
	memset(_data, 0, messageSize);

	char* bufferPointer = _data;
	memcpy(bufferPointer, &id, sizeof(MsgId));

	bufferPointer += sizeof(MsgId);

	memcpy(bufferPointer, name_.c_str(), 20 * sizeof(char));
}

int LoginClientMessage::from_bin(char* obj){
	int messageSize = sizeof(MsgId) + 20 * sizeof(char);

	alloc_data(messageSize);

	memcpy(static_cast<void *>(_data), obj, messageSize);

	char* bufferPointer = _data;
	memcpy(&id, bufferPointer, sizeof(MsgId));

	bufferPointer += sizeof(MsgId);

	char bufferName[20 * sizeof(char)];
	memcpy(&bufferName, bufferPointer, 20 * sizeof(char));

	name_ = bufferName;

	std::cout << "Client " << name_ << '\n';

	return 0;
}
//////////////////CONFIRMATION LOGIN CLIENT//////////////////////////////

void ConfirmationLoginMessage::to_bin(){
	int32_t messageSize = sizeof(MsgId) + sizeof(int);

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
/////////////////////////START GAME//////////////////////////////////

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
//////////////////////////////CLIENT READY ////////////////////////////////
int ClientReadyMessage::from_bin(char* obj){
	alloc_data(sizeof(MsgId));

	memcpy(static_cast<void *>(_data), obj, sizeof(MsgId));

	char* bufferPointer = _data;

	memcpy(&id, bufferPointer, sizeof(MsgId));

	return 0;
}
void ClientReadyMessage::to_bin(){
	int32_t messageSize = sizeof(MsgId);

	alloc_data(messageSize);

	memset(_data, 0, messageSize);

	char* bufferPointer = _data;

	memcpy(bufferPointer, &id, sizeof(MsgId));
}

/////////////////////////// UPDATE CLIENT PLAYER //////////////////////////

void UpdateClientPlayerMessage::to_bin(){
	int32_t messageSize = sizeof(MsgId) + 3 * sizeof(int) + sizeof(float);

	alloc_data(messageSize);

	memset(_data, 0, messageSize);

	char* bufferPointer = _data;

	memcpy(bufferPointer, &id, sizeof(MsgId));
	bufferPointer += sizeof(MsgId);

	memcpy(bufferPointer, &go_id, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(bufferPointer, &x, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(bufferPointer, &y, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(bufferPointer, &rotation, sizeof(float));
}

int UpdateClientPlayerMessage::from_bin(char* obj){
	int32_t messageSize = sizeof(MsgId) + 3 * sizeof(int) + sizeof(float);
	
	alloc_data(messageSize);

	memcpy(static_cast<void *>(_data), obj, messageSize);

	char* bufferPointer = _data;

	memcpy(&id, bufferPointer, sizeof(MsgId));
	bufferPointer += sizeof(MsgId);

	memcpy(&go_id, bufferPointer, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(&x, bufferPointer, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(&y, bufferPointer, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(&rotation, bufferPointer, sizeof(float));

	return 0;
}
//////////////////////UPDATE GAME OBJECT/////////////////////////////////
void UpdateGameObjectMessage::to_bin(){
	int32_t messageSize = sizeof(MsgId) + 3 * sizeof(int) + sizeof(float);

	alloc_data(messageSize);

	memset(_data, 0, messageSize);

	char* bufferPointer = _data;

	memcpy(bufferPointer, &id, sizeof(MsgId));
	bufferPointer += sizeof(MsgId);

	memcpy(bufferPointer, &go_id, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(bufferPointer, &x, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(bufferPointer, &y, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(bufferPointer, &rotation, sizeof(float));
}

int UpdateGameObjectMessage::from_bin(char* obj){
	int32_t messageSize = sizeof(MsgId) + 3 * sizeof(int) + sizeof(float);
	
	alloc_data(messageSize);

	memcpy(static_cast<void *>(_data), obj, messageSize);

	char* bufferPointer = _data;

	memcpy(&id, bufferPointer, sizeof(MsgId));
	bufferPointer += sizeof(MsgId);

	memcpy(&go_id, bufferPointer, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(&x, bufferPointer, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(&y, bufferPointer, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(&rotation, bufferPointer, sizeof(float));

	return 0;
}
//////////////////////SHOOT CLIENT/////////////////////////////
void ShootClientMessage::to_bin(){
	int32_t messageSize = sizeof(MsgId) + 4 * sizeof(int);

	alloc_data(messageSize);

	memset(_data, 0, messageSize);

	char* bufferPointer = _data;

	memcpy(bufferPointer, &id, sizeof(MsgId));
	bufferPointer += sizeof(MsgId);

	memcpy(bufferPointer, &x, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(bufferPointer, &y, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(bufferPointer, &dirX, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(bufferPointer, &dirY, sizeof(int));
}

int ShootClientMessage::from_bin(char* obj){

	int32_t messageSize = sizeof(MsgId) + 4 * sizeof(int);
	
	alloc_data(messageSize);

	memcpy(static_cast<void *>(_data), obj, messageSize);

	char* bufferPointer = _data;

	memcpy(&id, bufferPointer, sizeof(MsgId));
	bufferPointer += sizeof(MsgId);

	memcpy(&x, bufferPointer, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(&y, bufferPointer, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(&dirX, bufferPointer, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(&dirY, bufferPointer, sizeof(int));

	return 0;
}
//////////////////////SHOOT SERVER/////////////////////////////
void ShootServerMessages::to_bin(){
	int32_t messageSize = sizeof(MsgId) + 3 * sizeof(int);

	alloc_data(messageSize);

	memset(_data, 0, messageSize);

	char* bufferPointer = _data;

	memcpy(bufferPointer, &id, sizeof(MsgId));
	bufferPointer += sizeof(MsgId);

	memcpy(bufferPointer, &x, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(bufferPointer, &y, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(bufferPointer, &idGo, sizeof(int));
}

int ShootServerMessages::from_bin(char* obj){

	int32_t messageSize = sizeof(MsgId) + 3 * sizeof(int);
	
	alloc_data(messageSize);

	memcpy(static_cast<void *>(_data), obj, messageSize);

	char* bufferPointer = _data;

	memcpy(&id, bufferPointer, sizeof(MsgId));
	bufferPointer += sizeof(MsgId);

	memcpy(&x, bufferPointer, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(&y, bufferPointer, sizeof(int));
	bufferPointer += sizeof(int);

	memcpy(&idGo, bufferPointer, sizeof(int));

	return 0;
}