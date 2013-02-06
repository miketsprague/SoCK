/******************************************************************************/
#include "stdafx.h"
#include "Client.h"
/******************************************************************************/
OPTIONS Options;
MESSAGEBOX MsgBox;
/******************************************************************************/
static ListGroup mode_list_group[] = {
	ListGroup(DATA_VECI2, 0, SIZE(VecI2), LGW_PARENT, L"Size"),
};
static CChar8 *sync_t[] = {
	"Off",
	"On",
};
static CChar8 *renderer[] = {
	"Deferred",
	"Forward" ,
	"Simple"  ,
};
static CChar8 *shd_t[] = {
	"Off",
	"On",
};
static CChar8 *shd_size_t[] = {
	"512",
	"768",
	"1024",
	"1536",
	"2048",
};
static CChar8 *shd_soft_t[] = {
	"Off",
	"Medium",
	"High",
};
static CChar8 *shd_jitter_t[] = {
	"Off",
	"On",
};
static CChar8 *bump_mode_t[] = {
	"Flat",
	"Normal",
	"Parallax",
	"Relief",
};
static CChar8 *mtn_mode_t[] = {
	"Off",
	"Medium",
	"High",
};
static CChar8 *mtn_leak_t[] = {
	"None",
	"Medium",
	"High",
};
static CChar8 *ambient[] = {
	"Off",
	"Low",
	"Medium",
	"High",
	"Ultra",
};
static CChar8 *hdr_t[] = {
	"Off",
	"On",
};
static CChar8 *hprt_t[] = {
	"Off",
	"On",
};
/******************************************************************************/
static void Mode(Ptr) {
	Int m = Options.mode();
	if (InRange(m, D.modes())) {
		VecI2 mode = D.modeSize()[m];
		D.mode(mode.x, mode.y);
		Options.toGui();
	}
}
static void ShdSize(Ptr) {
	Int s = Options.shd_size();
	if (InRange(s, shd_size_t)) {
		D.shdMapSize(TextInt(shd_size_t[s]));
		Options.toGui();
	}
}
/******************************************************************************/
static void MotionMode(Ptr) { D.mtnMode		(Options.mtn_mode() ? MOTION_HIGH : MOTION_NONE).mtnSmooth(Options.mtn_mode() == 2);	Options.toGui(); }
static void Shd(Ptr)		{ D.shdMode		(Options.shd() ? SHD_MAP_HW : SHD_NONE);												Options.toGui(); }
static void Sync(Ptr)		{ D.sync		(Options.sync() != 0);																	Options.toGui(); }
static void AmbMode(Ptr)	{ D.ambMode		((AMBIENT_MODE)Options.amb_mode());														Options.toGui(); }
static void ShdSoft(Ptr)	{ D.shdSoft		(Options.shd_soft());																	Options.toGui(); }
static void ShdJitter(Ptr)	{ D.shdJitter	(Options.shd_jitter() != 0);															Options.toGui(); }
static void BumpMode(Ptr)	{ D.bumpMode	((BUMP_MODE)Options.bump_mode());														Options.toGui(); }
static void MotionLeak(Ptr) { D.mtnLeaks	(Options.mtn_leak());																	Options.toGui(); }
static void HdrOn(Ptr)		{ D.hdr			(Options.hdr() != 0);																	Options.toGui(); }
static void HpRt(Ptr)		{ D.hpRt		(Options.hprt() != 0);																	Options.toGui(); }
static void RenderType(Ptr) { Renderer.type	(RENDER_TYPE(Options.render_type()));													Options.toGui(); }

static void Close (Ptr)		{ Options.toggle();}
/******************************************************************************/
void OPTIONS::create() {
	Gui += window.create(Rect_C(0, 0, 1.1, 1.67), "Options").barVisible(false).level(1).hide();
	window.flag &= ~WIN_MOVABLE;

	Flt y = -0.07, // starting position
	    h = 0.07, // element height
	    s = 0.11; // spacing between elements

	Flt xl = window.crect.w() * 0.25, // left  x position
	    xr = window.crect.w() * 0.75, // right x position
	    w = window.crect.w() * 0.40; // element width

	window += tfps			.create(Vec2(window.crect.w() / 2, y));																																																										y -= s;
	window += tmode			.create(Vec2(xl, y),	"Resolution");			window += mode			.create(Rect_C(xr, y, w, h))									.func(Mode)			.setGroups(mode_list_group, ELMS(mode_list_group)).setData(D.modeSize(), D.modes());										y -= s;
	window += tsync			.create(Vec2(xl, y),	"Synchronization");		window += sync			.create(Rect_C(xr, y, w, h), sync_t,		ELMS(sync_t))		.func(Sync)			.desc("Enable Screen Synchronization\nLimits framerate to Screen refresh ratio to increase smoothness");				y -= s;
	window += trender_type	.create(Vec2(xl, y),	"Renderer");			window += render_type	.create(Rect_C(xr, y, w, h), renderer,		ELMS(renderer))		.func(RenderType)	.desc("Renderer type\nSimple and SinglePass renderers may work faster, but have limited number of special effects");	y -= s;
	window += tshd			.create(Vec2(xl, y),	"Shadows");				window += shd			.create(Rect_C(xr, y, w, h), shd_t,			ELMS(shd_t))		.func(Shd)			.desc("Enable Shadows");																								y -= s;
	window += tshd_size		.create(Vec2(xl, y),	"ShadowMap Size");		window += shd_size		.create(Rect_C(xr, y, w, h), shd_size_t,	ELMS(shd_size_t))	.func(ShdSize)		.desc("Shadow map resolution\nhigher resolutions reduce blockiness of shadows");										y -= s;
	window += tshd_soft		.create(Vec2(xl, y),	"Shadow Softing");		window += shd_soft		.create(Rect_C(xr, y, w, h), shd_soft_t,	ELMS(shd_soft_t))	.func(ShdSoft)		.desc("Enable shadows softing");																						y -= s;
	window += tshd_jitter	.create(Vec2(xl, y),	"Shadow Jittering");	window += shd_jitter	.create(Rect_C(xr, y, w, h), shd_jitter_t,	ELMS(shd_jitter_t))	.func(ShdJitter)	.desc("Enable jittering on shadows,\nworks best when enabled with shadow softing");										y -= s;
	window += tbump_mode	.create(Vec2(xl, y),	"Bump Mapping");		window += bump_mode		.create(Rect_C(xr, y, w, h), bump_mode_t,	ELMS(bump_mode_t))	.func(BumpMode)		.desc("Simulate bumpy surfaces");																						y -= s;
	window += tmtn_mode		.create(Vec2(xl, y),	"Motion Blur");			window += mtn_mode		.create(Rect_C(xr, y, w, h), mtn_mode_t,	ELMS(mtn_mode_t))	.func(MotionMode)	.desc("Blur fast moving objects");																						y -= s;
	window += tmtn_leak		.create(Vec2(xl, y),	"Blur Accuracy");		window += mtn_leak		.create(Rect_C(xr, y, w, h), mtn_leak_t,	ELMS(mtn_leak_t))	.func(MotionLeak)	.desc("Remove leaking of blurred background objects\ninto not blurred foreground objects");								y -= s;
	window += tamb_mode		.create(Vec2(xl, y),	"Ambient Occlusion");	window += amb_mode		.create(Rect_C(xr, y, w, h), ambient,		ELMS(ambient))		.func(AmbMode)		.desc("Darkens occluded areas");																						y -= s;
	window += thdr			.create(Vec2(xl, y),	"Hdr");					window += hdr			.create(Rect_C(xr, y, w, h), hdr_t,			ELMS(hdr_t))		.func(HdrOn)		.desc("Enables automatic screen brightness adjustment (Tone Mapping)");													y -= s;
	window += thprt			.create(Vec2(xl, y),	"High Precision RT's");	window += hprt			.create(Rect_C(xr, y, w, h), hprt_t,		ELMS(hprt_t))		.func(HpRt)			.desc("Enable 64bit render target rendering\nWhich increaseses number of visible colors");								y -= s;
	window += close			.create(Rect_C(Vec2(window.crect.w() / 2, y), 0.3, h), "Close")																			.func(Close)		.desc("Settings will be saved to \"config.txt\" on Shutdown.");															y -= s;

	SetStyle(mode);
	SetStyle(sync);
	SetStyle(render_type);
	SetStyle(shd);
	SetStyle(shd_size);
	SetStyle(shd_soft);
	SetStyle(shd_jitter);
	SetStyle(bump_mode);
	SetStyle(mtn_mode);
	SetStyle(mtn_leak);
	SetStyle(amb_mode);
	SetStyle(hdr);
	SetStyle(hprt);
	SetStyle(close);

	toGui();
}
/******************************************************************************/
void OPTIONS::toGui() {
	// hide unavailable options
	{
		shd       .visible(Renderer.type() != RT_SIMPLE);
		shd_size  .visible(Renderer.type() != RT_SIMPLE && D.shdMode() != SHD_NONE);
//		shd_soft  .visible(Renderer.type()==RT_MULTI_PASS && D.shdMode()!=SHD_NONE);
		shd_jitter.visible(Renderer.type() != RT_SIMPLE && D.shdMode() != SHD_NONE);
		bump_mode .visible(Renderer.type() != RT_SIMPLE);
		mtn_mode  .visible(Renderer.type() != RT_SIMPLE);
		mtn_leak  .visible(Renderer.type() != RT_SIMPLE);
//		amb_mode  .visible(Renderer.type()==RT_MULTI_PASS);
		hdr       .visible(Renderer.type() != RT_SIMPLE);
		hprt      .visible(Renderer.type() != RT_SIMPLE);
//		bump_mode .setData(bump_mode_t,Min(ELMS(bump_mode_t),(Renderer.type()==RT_SINGLE_PASS) ? 2 : 0xFF));
	}

	// set gui settings from current display options
	{
		Int set;

		set = -1; REP(D.modes())if (D.x() == D.modeSize()[i].x && D.y() == D.modeSize()[i].y) {
			set = i; break;
		} if (set != -1)mode.set(set, true); else mode.setText(S + VecI2(D.x(), D.y()), true, true);

		set = -1; REPA(shd_size_t)if (TextInt(shd_size_t[i]) == D.shdMapSize()) {
			set = i; break;
		} shd_size.set(set, true);

		if (!D.mtnMode())mtn_mode.set(0, true); else if (D.mtnSmooth())mtn_mode.set(2, true); else
			mtn_mode.set(1, true);

		sync       .set(D.sync(), true);
		shd        .set(D.shdMode() != SHD_NONE, true);

		render_type.set(Mid(Renderer.type(), 0,	ELMS(renderer)		- 1), true);
		shd_soft   .set(Mid(D.shdSoft(),	0,	ELMS(shd_soft_t)	- 1), true);
		shd_jitter .set(Mid(D.shdJitter(),	0,	ELMS(shd_jitter_t)	- 1), true);
		bump_mode  .set(Mid(D.bumpMode(),	0,	ELMS(bump_mode_t)	- 1), true);
		mtn_leak   .set(Mid(D.mtnLeaks(),	0,	ELMS(mtn_leak_t)	- 1), true);
		amb_mode   .set(Mid(D.ambMode(),	0,	ELMS(ambient)		- 1), true);
		hdr        .set(Mid(D.hdr(),		0,	ELMS(hdr_t)			- 1), true);
		hprt       .set(Mid(D.hpRt(),		0,	ELMS(hprt_t)		- 1), true);
	}
}
/******************************************************************************/
void OPTIONS::toggle() {
	// toggle options window visibility
	if (window.visible()) {
		window.fadeOut();
		mouse_disable = keyboard_disable = false;
	} else {
		window.fadeIn();
		mouse_disable = keyboard_disable = true;
	}

}
/******************************************************************************/
void OPTIONS::update() {
	tfps.set(S + "Settings at " + Time.fps() + " FPS");
}
/******************************************************************************/
void MESSAGEBOX::create() {
	Gui		+= window	.create(Rect_C(0, 0, 0.5, 0.2)).barVisible(false).hide();
	window	+= msgtext	.create(Vec2(window.crect.w() / 2, -0.1), "");
	window				.flag &= ~WIN_MOVABLE;
	timer = 1; timeout = gizmo = 0;
}
void MESSAGEBOX::update() {

	if (queue.elms()) {
		window.level(1);
		if (!window.visible())
			window.fadeIn();

		window	.setRect(Rect_C(0, 0, msgtext.tds->textWidth(queue[0].text) + 0.1, 0.2));
		window	+= msgtext.create(Vec2(window.crect.w() / 2, -0.1), queue[0].text);

		timer -= Time.d();
		if (timer < 0) {
			queue.remove(queue.first);
			if (queue.elms()) timer = queue[0].time;
		}
	} else if (window.visible() && !freeze)
		window.fadeOut();

	if (freeze) {
		timeout += Time.d();
		if (timeout > 5) {
			timeout = 0;
			freeze = false;
			switch (gizmo) {
				case 0: push("For some reason, you can't connect to our server.", 3);		gizmo++; break;
				case 1: push("We're doing our best to ressolve this problem.", 3);			gizmo++; break;
				default : push("You should go and have some tea, while we fix this.", 3);	gizmo++; break;
			}
		}
	}
}
void MESSAGEBOX::push(Str m, Flt t) {
	if (!queue.elms())
		timer = t;

	Message &message = queue.New();
	message.text = m;
	message.time = t;
}
/******************************************************************************/