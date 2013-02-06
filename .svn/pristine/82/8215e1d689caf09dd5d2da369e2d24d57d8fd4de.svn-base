/******************************************************************************/
#include "stdafx.h"
#include "Client.h"
/******************************************************************************/
void Character::create(Game::ObjParams &obj) {
	__super::create(obj);
}

void Character::animate()
{
	__super::animate();
	if(Dist(nextLoc, pos()) > 0.01)
		cskel.animate(L"../EsenthelEngineSDK/data/anim/run.anim", Time.time(), 0.5);
}

Bool Character::update() {
	// Update the chat messages received
	if(Dist(nextLoc, pos()) > 0.01) // it is close enough
		pos(Lerp(pos(),nextLoc,0.25)); 
	if (queueChat.elms()) {
		chatTimer -= Time.d();
		if (chatTimer < 0) {
			queueChat.remove(queueChat.first);
			if (queueChat.elms()) chatTimer = queueChat[0].time;
		}
	}

	return __super::update();
}
void Character::push(Netpoint &n) {
	
	if (Dist(n.pos,pos()) > 10.0f) pos(n.pos);

	/* EXTRAPOLATE HERE:

	Basically, you need to accelerate the player a little, so that by the time a new packet arrives, 
	approximately in (Flt)(n.packetTime - lastTime)/1000.0f) time, your player is quickly synced
	with the player when they were sent.

	Latency is calculated by (Flt)(RakNet::GetTime() - n.packetTime) / 1000.0f

	Also I think you need to extend the position a bit, so that the player doesn't stop when waiting for new packets
	unfortunately, this means that when you stop, you're going to keep running, then run back.

	I guess you can try something like this:

	n.pos = n.pos+(n.vel*((Flt)(n.packetTime - lastTime)/1000.0f));
	lastTime = n.packetTime; 	*/
	
	//Todo: Predict further, see http://www.mindcontrol.org/~hplus/epic/

	lastTime = n.packetTime;
	// position send times the how long ago it was sent.
	Vec extrapolated = n.pos+(n.vel*((Flt)(n.packetTime - lastTime)/1000.0f));
	pos(Lerp(pos(),extrapolated,0.25)); 

	nextLoc = extrapolated;
	//angle = Lerp(angle,n.ang,0.75); // if we want to get fancy, send the angular velocity so we can extrapolate that, too.
	angle = n.ang;
}

void Character::pushChat(Str s, Flt t) {
	if (!queueChat.elms())
	chatTimer = t;

	Talk &talk = queueChat.New();
	talk.text = s;
	talk.time = t;
}
/******************************************************************************/