/******************************************************************************/
#include "stdafx.h"
#include "Client.h"
#include "../EsenthelEngineSDK/data/enum/_enums.h"
/******************************************************************************/
Camera desired_camera;
Window game_window;
Bool view, double_run, keyboard_disable, mouse_disable, chat_mode;
/******************************************************************************/
Game::ObjMemx<Game::Static> Statics;
Game::ObjMemx<Game::Item> Items;
Game::ObjMemx<Player> Players;
Game::ObjMemx<Game::ObjParticles> ObjParticles;
Game::ObjMemx<Game::ObjLightPoint> ObjLightPoints;
/******************************************************************************/
struct GAMEMENU {
	Button b_return, b_ctrl, b_options, b_hide;

	static void VisibleToggle() {
		if (game_window.visible()) {
			game_window.fadeOut();
			mouse_disable = keyboard_disable = false;
			Ms.hide().clip(NULL, 1);
		} else {
			game_window.fadeIn();
			mouse_disable = keyboard_disable = true;
			Ms.show().clip();
		}
	}
	static void Return(Ptr) {
		VisibleToggle();
		Ms.show().clip(); // hackish.  It makes it work correctly.
		network.Logoff();
		StateMenu.set(1.0);
	}
	static void Ctrl(Ptr) {}
	static void Opt(Ptr) {
		VisibleToggle();
		Options.toggle();
	}
	static void Hide(Ptr) {
		VisibleToggle();
	}

	void create() {
		Gui += game_window.create(Rect_C(0, 0, 0.5, 0.4)).barVisible(false).visible(false);

		game_window +=	b_hide		.create(Rect_C(game_window.crect.w() / 2, -0.1, 0.4, 0.08), "Hide")		.func(Hide)		.desc("Hide this game menu.");
		game_window +=	b_options	.create(Rect_C(game_window.crect.w() / 2, -0.2, 0.4, 0.08), "Settings")	.func(Opt)		.desc("Find the game balance between performance and quality.");
		game_window +=	b_return	.create(Rect_C(game_window.crect.w() / 2, -0.3, 0.4, 0.08), "Title")	.func(Return)	.desc("Return to the title menu. You can press Alt+F4 to exit directly.");

		game_window.flag &= ~WIN_MOVABLE;
		game_window.level(-1);

		SetStyle(b_hide);
		SetStyle(b_options);
		SetStyle(b_return);
	}
} GameMenu;
Bool InitGame() {
	GameMenu.create();
	Physics.create();
	Sun.image = Images("gfx/sky/sun.gfx");
	Sky.atmospheric();

	Water.images(Images("water/0.gfx"), Images("water/0.n.gfx"), Images("water/reflection.gfx"));
	Water.draw = true;
	Water.wave_scale = 1.8;

	desired_camera.dist = 10;
	keyboard_disable = false;
	mouse_disable = false;
	chat_mode = false;

	Game::World.update_objects_after_physics = false;

	Game::World.init()
	.setObjType(Statics, OBJ_STATIC)
	.setObjType(Players, OBJ_PLAYER)
	.setObjItem(Items, OBJ_ITEM)
	.setObjType(ObjParticles, OBJ_PARTICLES)
	.setObjType(ObjLightPoints, OBJ_LIGHT_POINT)
	.New("world/sample.world");

	Ms.hide().clip(NULL,1);

	return true;
}
/******************************************************************************/
Bool UpdateGame() {
	// Game inputs
	if (Kb.bp(KB_ESC)) {
		if (!Options.window.visible()) GameMenu.VisibleToggle();
		else Options.toggle();

		Players[0].chat.clear();
		chat_mode = false;

	}

	// Chat inputs
	if (Kb.bp(KB_ENTER)) {
		chat_mode = ! chat_mode;
		keyboard_disable = chat_mode;
		if (Players.elms()) {
			/******************************************************************************/
			RakNet::BitStream clientSend;
			StringCompressor strcomp;
			clientSend.Write((unsigned char) CLIENT_CHAT);
			clientSend.Write(network.client->GetInternalID());
			strcomp.EncodeString((Str8)Players[0].chat(), 50, &clientSend);
			network.client->Send(&clientSend, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
			/******************************************************************************/
		}
		Players[0].chat.clear();
	}
	if (chat_mode)
		Players[0].chat.kbSet();
	else {
		Gui.kb();
	}

	Gui.update();
	MsgBox.update();
	Options.update();
	network.CheckMessages();

	Game::World.update(Cam.at);
	Water.update(Vec2(0.01));

	for(std::map<SystemAddress, Character*>::iterator i = network.clients.begin(); i != network.clients.end(); i++) {
		i->second->update();
	}

	if (Players.elms()) 
	{
		OrientP &head = Players[0].cskel.getPoint("head");
		Cam.setPosDir(head.pos, head.dir, head.perp);
		Cam.updateVelocities().set();
	} else { // when no player on the scene
		CamHandle(0.1, 100, CAMH_ZOOM | (Ms.b(1) ? CAMH_MOVE : CAMH_ROT));
	}

	return true;
}
/******************************************************************************/
void Render() {
	switch(Renderer()) {
		case RM_PREPARE: {
			for ( std::map<SystemAddress, Character*>::iterator i = network.clients.begin(); i != network.clients.end(); ++i ) {
				i->second->drawPrepare();
			}
		} break;
	}

	Game::World.draw();
}
/******************************************************************************/
void DrawGame() {
	Renderer(Render);

	Renderer.wire ^= Kb.bp(KB_TILDE);
	if (Kb.b(KB_1))Physics.draw();

	D.text(0,0.8,"Press 'Enter' to Chat");

	Vec2   screen = PosToScreen(Players[0].pos() + Vec(0, 1.15, 0));
	if (chat_mode)
		if (Compare("", Players[0].chat()) == 0)
			D.text(screen, "...");
		else
			D.text(screen, Players[0].chat());

	// Deaw chat messages
	for ( std::map<SystemAddress, Character*>::iterator i = network.clients.begin(); i != network.clients.end(); ++i ) {
		if (i->second->queueChat.elms()) {
			Vec2   screen = PosToScreen(i->second->pos() + Vec(0, 1.15, 0));
			D.text(screen, i->second->queueChat[0].text);
		}
	}


	Gui.draw();
}
void ShutDownGame() {
	Sky.clear();
	Water.draw = false;
	desired_camera.dist = 0;
	keyboard_disable = false;
}
/******************************************************************************/
State StateGame(UpdateGame, DrawGame, InitGame, ShutDownGame);
/******************************************************************************/
