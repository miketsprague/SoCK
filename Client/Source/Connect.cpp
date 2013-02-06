/******************************************************************************/
#include "stdafx.h"
#include "Client.h"
/******************************************************************************/
Window connectWin;
/******************************************************************************/
TextLine ipaddr;

struct CONNECTMENU {
	Text iptext, title;
	Button ok; //cancel;

	static void Hide() {
		connectWin.fadeOut();
	}
	static void Login(Ptr) {
		network.Connect((Str8)ipaddr());
		connectWin.fadeOut();
		StateIntro.set(1.0);
	}
	static void Cancel(Ptr) {
		connectWin.fadeOut();
		//titwin.fadeIn();
	}

	void create() {
		Gui += connectWin		    .create(Rect_C(0, 0, 1.0, 0.52))	.barVisible(false)		.visible(true);
		Flt y = -0.07, h = 0.07, s = 0.11; Flt xr = connectWin.crect.w() * 0.72, w = connectWin.crect.w() * 0.40;

		connectWin += title			.create(Vec2(connectWin.crect.w() / 2, y),			"Enter Server IP:");		y -= s;
		connectWin += iptext		.create(Vec2(.35, y),							"IP Address:");
		connectWin += ipaddr    	.create(Rect_C(Vec2(xr, y), w, h)),												y -= s;
		connectWin += ok			.create(Rect_C(Vec2(.5, y), w, h),				"Ok")	.func(Login)		.desc("Connect to this IP");
		ipaddr.set("127.0.0.1");

		connectWin.flag &= ~WIN_MOVABLE;

		SetStyle(ok);
		//SetStyle(cancel);
		SetStyle(ipaddr);
	}
} IPConnect;

Bool InitConnect() {
	IPConnect.create();

	return true;
}
void ShutConnect() {

}
/******************************************************************************/
Bool UpdateConnect() {
	Gui.update();
	//if(Kb.bp(KB_ESC) || Kb.bp(KB_SPACE) || Kb.bp(KB_ENTER) || Ms.bp(0) || Ms.bp(1) )StateIntro.set(0.5f);
	return true;
}
void DrawConnect() {
	D.clear(BLACK);
	Gui.draw();

}
/******************************************************************************/
State StateConnect(UpdateConnect, DrawConnect, InitConnect, ShutConnect);
/******************************************************************************/