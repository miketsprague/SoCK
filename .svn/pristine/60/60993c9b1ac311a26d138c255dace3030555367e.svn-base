/******************************************************************************/
#include "stdafx.h"
#include "Client.h"
/******************************************************************************/
Image esenthel, raknet, nvidia, physx;
/******************************************************************************/
Bool InitIntro() {
	esenthel = "../../Client/Logo/esenthel.gfx";
	raknet	 = "../../Client/Logo/raknet.gfx";
	nvidia   = "../../Client/Logo/nvidia.gfx";
	physx    = "../../Client/Logo/physx.gfx";
	return true;
}
void ShutIntro() {
	esenthel.del();
	raknet	.del();
	nvidia  .del();
	physx   .del();
}
/******************************************************************************/
Bool UpdateIntro() {
	if(Kb.bp(KB_ESC) || Kb.bp(KB_SPACE) || Kb.bp(KB_ENTER) || Ms.bp(0) || Ms.bp(1) )StateMenu.set(0.5f);
	return true;
}
void DrawIntro() {
	D.clear(BLACK);

	TextDS tds;
	tds.reset();
	tds.scale=0.13;

	D.text(tds, 0, 0.1,  "RakNet Tutorial!");
	tds.scale=0.09;
	D.text(tds, 0, 0.0,  "Press Enter to Skip.");


	nvidia  .drawFit(ColorAlpha((StateActive->time() - 1.0f)), 0, .63-0.2f,  -D.h() + 0.3f, 0.2f,  0.2f);
	physx   .drawFit(ColorAlpha((StateActive->time() - 0.7f)), 0, .63-0.5f,  -D.h() + 0.3f, 0.2f,  0.2f);
	raknet  .drawFit(ColorAlpha((StateActive->time() - 0.4f)), 0, .63-0.8f,  -D.h() + 0.3f, 0.15f, 0.2f);
	esenthel.drawFit(ColorAlpha((StateActive->time() - 0.1f)), 0, .63-1.27f, -D.h() + 0.3f, 0.35f, 0.2f);
}
/******************************************************************************/
State StateIntro(UpdateIntro, DrawIntro, InitIntro, ShutIntro);
/******************************************************************************/