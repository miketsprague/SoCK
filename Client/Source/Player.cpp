/******************************************************************************/
#include "stdafx.h"
#include "Client.h"
/******************************************************************************/
void Player::create(Game::ObjParams &obj) {
	__super::create(obj);
	Gui += chat.create("").setRect(Rect(-100, -100, -99, -99)); // hide the players chat window
	updateTime = 0;
}
Bool Player::update() {

	if (action) {
		if ((Kb.b(KB_W) || Kb.b(KB_S) || Kb.b(KB_A) || Kb.b(KB_D) || Kb.b(KB_Q) || Kb.b(KB_E)) && !keyboard_disable || mouse_disable || (!Ms.bp(0) && !double_run)) actionBreak();
	}
	if (!action) {
		// turn & move, + jump
		input.anglei.x	= Kb.b(KB_Q) - Kb.b(KB_E);
		input.anglei.y	= Kb.b(KB_T) - Kb.b(KB_G);
		input.diri.x	= Kb.b(KB_D) - Kb.b(KB_A);
		input.diri.z	= Kb.b(KB_W) - Kb.b(KB_S);
		input.diri.y	= Kb.b(KB_SPACE); //- Kb.b(KB_LSHIFT);
		input.jump		= (Kb.bp(KB_SPACE) ? 3.5 : 0);

		// dodge, crouch, walk
		input.dodge		= Kb.bd(KB_D) - Kb.bd(KB_A);
		input.crouch	= Kb.b(KB_LSHIFT);
		input.walk		= Kb.b(KB_LCTRL);

		ready ^= Kb.bp(KB_R); // ready stance change

		if (keyboard_disable) { // Disable Keyboard Events
			input.anglei.x = input.anglei.y = input.diri.x = input.diri.z = input.diri.y = input.jump = 0;
			input.dodge = input.crouch = input.walk = ready = 0;
		}

		// mouse turn
		if (!mouse_disable && !Ms.b(0)) 
		{
			Flt  max=DegToRad(900)*Time.d(),
				  dx =Ms.dir_ds.x*1.7,
				  dy =Ms.dir_ds.y*1.7*Ms.inv();
			 angle.x-=Mid(dx,-max,max);
			 angle.y+=Mid(dy,-max,max);
		}

	}

//	if (Ms.bp(0)) double_run = false;
//	if (Ms.bd(0)) double_run = true;

	if(Game::World.waterUnder(pos())) {
		flying_speed = 2.1;
		ctrl.actor.gravity(false);
		ctrl.flying(true);
	} else {
		ctrl.flying(false);
		flying_speed = 25;
		ctrl.actor.gravity(true);
	}

	// network update -- WARNING 100 TICK atm-- that's a lot.
	updateTime+=Time.d();
	if (updateTime > lagSimulator)
	{
		lagSimulator = 0.01; // sends constant updates every 0.1s, you can change this to RandomF(0.5) e.g. to simulate randomly sending messages with strong jitter
		updateTime = 0;
		sendUpdate();
	}

	return __super::update();
}

UInt Player::drawPrepare() {
	Bool hide_head = (view == VIEW_FPP && mesh);	// disable drawing head when we're in FPP mode
	if(hide_head)mesh->hide("head");				// hide "head" mesh part in 'mesh'
	UInt modes = __super::drawPrepare();			// call default drawing
	if(hide_head)mesh->show("head");				// un-hide "head" mesh part, so other objects which use the same mesh will have the head rendered properly

	return modes;
}
/******************************************************************************/
void Player::sendUpdate() {
	RakNet::BitStream clientSend;

	// Send timestamp
	clientSend.Write((unsigned char) ID_TIMESTAMP);
	clientSend.Write( RakNet::GetTime());
	clientSend.Write((unsigned char) CLIENT_MOVE);
	clientSend.Write(network.client->GetInternalID());

	// Data to be sent
	clientSend.WriteVector(pos().x, pos().y, pos().z);
	clientSend.WriteVector(ctrl.actor.vel().x, ctrl.actor.vel().y, ctrl.actor.vel().z);
	clientSend.WriteVector(angle.x, angle.y, angle.z);

	network.client->Send(&clientSend, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}
/******************************************************************************/