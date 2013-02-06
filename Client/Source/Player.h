/******************************************************************************/
struct Player : Game::Chr {
	TextLine chat;
	Vec ctrl_pos;
	Flt updateTime;

	Flt lagSimulator;

	void sendUpdate();

	virtual void create(Game::ObjParams &obj);
	virtual Bool update();
	virtual UInt drawPrepare();
};
/******************************************************************************/
