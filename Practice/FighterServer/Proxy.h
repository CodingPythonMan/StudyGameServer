#pragma once

#include "Packet.h"
#include "Message.h"
#include "Session.h"
#include "MyList.h"

void CMDCreateMyCharacter(const MyList<Session*>& SessionList, const Session* session, const int& ID, const unsigned char& Direct, const short& X, const short& Y, const unsigned char& HP);
void CMDCreateOtherCharacter(const MyList<Session*>& SessionList, const Session* session, const int& ID, const unsigned char& Direct, const short& X, const short& Y, const unsigned char& HP);
void CMDDeleteCharacter(const MyList<Session*>& SessionList, const Session* session);
void QRYMoveStart(const MyList<Session*>& SessionList, const Session* session);
void REPMoveStart(const MyList<Session*>& SessionList, const Session* session);
void QRYMoveStop(const MyList<Session*>& SessionList, const Session* session);
void REPMoveStop(const MyList<Session*>& SessionList, const Session* session);
void QRYAttack001(const MyList<Session*>& SessionList, const Session* session);
void REPAttack001(const MyList<Session*>& SessionList, const Session* session);
void SendMessage(const MyList<Session*>& SessionList, const Session* session, const Packet& packet);
