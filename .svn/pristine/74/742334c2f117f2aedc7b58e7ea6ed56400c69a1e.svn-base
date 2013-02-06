/******************************************************************************/
struct Talk {
	Str text;
	Flt time;
};
struct Character : Game::Chr {
	RakNetTime lastTime;

	Vec nextLoc;
	Flt chatTimer;
	Meml<Talk> queueChat;

	void animate();
	void create(Game::ObjParams &obj);
	void push(Netpoint &n);
	void pushChat(Str, Flt displayTime = 1);
	virtual Bool update();
};
/******************************************************************************/