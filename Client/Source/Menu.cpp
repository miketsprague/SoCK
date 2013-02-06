/******************************************************************************/
#include "stdafx.h"
#include "Client.h"
#include "../EsenthelEngineSDK/data/enum/_enums.h"
/******************************************************************************/
Image logo;
Flt effects;
Window titwin, logwin, regwin;
TextLine logusername, logpassword, regkeycode, regusername, regpassword;
/******************************************************************************/
struct LOGINMENU {
	Text usertext, passtext, title;
	Button ok, cancel;

	static void Hide() {
		logwin.fadeOut();
	}
	static void Login(Ptr) {
		StringCompressor strcomp;
		MsgBox.push("Information sent!",0.5);
		MsgBox.push("Waiting for a reply.",0.7);
		MsgBox.freeze = true;
		RakNet::BitStream clientSend; 
		clientSend.Write((unsigned char) CLIENT_LOGIN);
		strcomp.EncodeString((Str8)logusername(),	12, &clientSend);
		strcomp.EncodeString((Str8)logpassword(),	12, &clientSend);

		network.client->Send(&clientSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	static void Cancel(Ptr) {
		logwin.fadeOut();
		titwin.fadeIn();
	}

	void create() {
		Gui += logwin			.create(Rect_C(0, 0, 1.0, 0.52))				.barVisible(false)			.visible(false);
		Flt y = -0.07, h = 0.07, s = 0.11; Flt xl = logwin.crect.w() * 0.28, xr = logwin.crect.w() * 0.72, w = logwin.crect.w() * 0.40;

		logwin += title			.create(Vec2(logwin.crect.w() / 2, y),			"Account Details:");												y -= s;
		logwin += usertext		.create(Vec2(xl, y),							"Username:");
		logwin += logusername	.create(Rect_C(Vec2(xr, y), w, h)),																					y -= s;
		logwin += passtext		.create(Vec2(xl, y),							"Password:");
		logwin += logpassword	.create(Rect_C(Vec2(xr, y), w, h))				.password(true);													y -= s;
		logwin += ok			.create(Rect_C(Vec2(xl, y), w, h),				"Ok")	.func(Login)		.desc("Login to your account.");
		logwin += cancel		.create(Rect_C(Vec2(xr, y), w, h),				"Cancel").func(Cancel)		.desc("Return to the title screen.");

		logusername.set("guest");
		logpassword.set("guest");

		logwin.flag &= ~WIN_MOVABLE;

		SetStyle(ok);
		SetStyle(cancel);
		SetStyle(logusername);
		SetStyle(logpassword);
	}
} Login;
struct REGISTERMENU {
	Text usertext, passtext, keytext, title;
	Button ok, cancel;

	static void Hide() {
		regwin.fadeOut();
	}
	static void Register(Ptr) {
		StringCompressor strcomp;
		MsgBox.push("Information sent!",0.5);
		MsgBox.push("Waiting for a reply.",0.7);
		MsgBox.freeze = true;
		RakNet::BitStream clientSend; 
		clientSend.Write((unsigned char) CLIENT_REGISTER);
		strcomp.EncodeString((Str8)regusername(),	12, &clientSend);
		strcomp.EncodeString((Str8)regpassword(),	12, &clientSend);
		strcomp.EncodeString((Str8)regkeycode(),	12, &clientSend);

		network.client->Send(&clientSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);

	}
	static void Cancel(Ptr) {
		regwin.fadeOut();
		titwin.fadeIn();
	}

	void create() {
		Gui += regwin			.create(Rect_C(0,0,1.0,0.51))					.barVisible(false)			.visible(false);
		Flt y = -0.07, h = 0.07, s = 0.11; Flt xl = regwin.crect.w() * 0.28, xr = regwin.crect.w() * 0.72, w = regwin.crect.w() * 0.40;

		regwin += title			.create(Vec2(regwin.crect.w() / 2, y),										"Register New Account:" );															y -= s;
		regwin += usertext		.create(Vec2(xl, y),														"New Username:" );
		regwin += regusername	.create(Rect_C(Vec2(xr, y), w, h))				.desc(						"Create your username.\nDon't use anything which can be guessed." );				y -= s;
		regwin += passtext		.create(Vec2(xl, y),														"New Password:" );
		regwin += regpassword	.create(Rect_C(Vec2(xr, y), w, h))				.desc(						"Create your password.\nDon't use anything which can be guessed." );				y -= s;
		regwin += ok			.create(Rect_C(Vec2(xl, y), w, h), "Ok")		.func(Register).desc(		"Create your account." );
		regwin += cancel		.create(Rect_C(Vec2(xr, y), w, h), "Cancel")	.func(Cancel).desc(			"Return to the title screen." );	regwin.flag &= ~WIN_MOVABLE;

		
		SetStyle(ok);
		SetStyle(cancel);
		SetStyle(regusername);
		SetStyle(regpassword);
	}
} Register;
struct TITLE { // main menu
	Button b_spectate, b_login, b_register, b_options, b_exit;

	static void Hide() {
		titwin.fadeOut();
	}
	static void Login(Ptr) {
		titwin.fadeOut();
		logwin.fadeIn();
	}
	static void Register(Ptr) {
		titwin.fadeOut();
		regwin.fadeIn();
	}
	static void Opt(Ptr) {
		Options.toggle();
	}
	static void Exit(Ptr) {
		StateExit.set();
	}

	void create() {
		Gui += titwin			.create(Rect_C(0,0,0.5,0.5)).barVisible(false);
		titwin += b_login		.create(Rect_C(titwin.crect.w() / 2, -0.1, 0.4, 0.08), "Login")		.func(Login)	.desc("Login to your account.");
		titwin += b_register	.create(Rect_C(titwin.crect.w() / 2, -0.2, 0.4, 0.08), "Register")	.func(Register)	.desc("Register to create a new account.");
		titwin += b_options		.create(Rect_C(titwin.crect.w() / 2, -0.3, 0.4, 0.08), "Settings")	.func(Opt)		.desc("Find the game balance between performance and quality.");
		titwin += b_exit		.create(Rect_C(titwin.crect.w() / 2, -0.4, 0.4, 0.08), "Close")		.func(Exit)		.desc("Exit to windows and save settings to \"config.txt\"");

		titwin.flag &= ~WIN_MOVABLE;
		titwin.level(-1);

		SetStyle(b_spectate);
		SetStyle(b_login);
		SetStyle(b_register);
		SetStyle(b_options);
		SetStyle(b_exit);
	}
} Title;
/******************************************************************************/
Bool InitMenu() {
	effects = 0;

	logo.Import("logo.png", -1, IMAGE_2D, 1);

	Title.create();
	Login.create();
	Register.create();

	return true;
}
void ShutMenu() {
}
/******************************************************************************/
Bool UpdateMenu() {
	if (Kb.bp(KB_ESC))return false;

	effects += Time.d();
	Cam.setSpherical(Vec(0, 0, 0), DegToRad(180), 0, 0, 1);
	Cam.updateVelocities().set();

	if (Kb.b(KB_ENTER)) { logusername.set("chris"); logpassword.set("chris"); }

	Gui.update();
	Options.update();
	MsgBox.update();
	network.CheckMessages();

	return true;
}
/******************************************************************************/
void TitleRender() {
	switch (Renderer()) {
		case RM_PREPARE:
			Meshs("obj/chr/Human/0.mesh")->draw(MatrixIdentity);
			LightPoint(5, Vec(0, 1, 1)).add();
			break;
	}
}
/******************************************************************************/
void DrawMenu() {
	// setup background
	D.clear(BLACK);

	RippleFx ripple;
	ripple.alpha_add  = effects;
	ripple.alpha_scale = -1;

	// draw image with ripple effect
	logo.draw(ripple, Rect_LU(-1, 1, 2, 2));

	Renderer.combine = true ;
	Renderer(TitleRender);
	Renderer.combine = false;

	Gui.draw();
}
/******************************************************************************/
State StateMenu(UpdateMenu, DrawMenu, InitMenu, ShutMenu);
/******************************************************************************/
