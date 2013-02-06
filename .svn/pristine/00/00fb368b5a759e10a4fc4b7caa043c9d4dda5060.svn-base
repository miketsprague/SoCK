/******************************************************************************/
struct OPTIONS {
	Window   window;
	Text     tfps, tmode, tsync, trender_type, tshd, tshd_size, tshd_soft, tshd_jitter, tbump_mode, tmtn_mode, tmtn_leak, tamb_mode, thdr, thprt;
	ComboBox mode, sync, render_type, shd, shd_size, shd_soft, shd_jitter, bump_mode, mtn_mode, mtn_leak, amb_mode, hdr, hprt;
	Button	 close;

	void create();
	void toGui();
	void toggle();
	void update();
} extern
Options;
/******************************************************************************/
struct Message {
	Str text;
	Flt time;
};
struct MESSAGEBOX {
	Flt timer;
	
	Flt timeout;
	int gizmo;
	Bool freeze;

	Window window;
	Text msgtext;
	Meml<Message> queue;

	void create();
	void update();
	void push(Str, Flt displayTime = 1);
} extern
MsgBox;
/******************************************************************************/