/******************************************************************************/
#include "stdafx.h"
#include "Client.h" // include the main header, which includes all other headers
/******************************************************************************/
Network network;
/******************************************************************************/
void SetStyle(_List &list) {
	list.cur_color = 0x88a6a6a6; // drop down high light colour
	list.setTextSize(0.09, 0.09);
}
void SetStyle(ComboBox &combobox) {
	combobox.button.color = 0x50c9c9c9; // drop down button colour
	SetStyle(combobox.cmenu.list);
}
void SetStyle(Button &button) {
	button.color = 0x88a5a6b7;
}
void SetStyle(TextLine &textline) {
	textline.back_color = 0x668D8D8D;
	textline.rect_color = 0x77333333;
}
void InitPre() {
	App.name("RakNet Tutorial!");
	App.flag = APP_MEM_LEAKS | APP_KB_EXCLUSIVE | APP_WORK_IN_BACKGROUND | APP_ALLOW_RT_FORWARD | APP_RESIZABLE;
	IOPath("../EsenthelEngineSDK/Data");
	Paks.add("engine.pak");

	// initial settings here if config.txt not found
	D.viewRange(650);

	if (ConfigLoad("config.txt")) {
		// override in here
	}
}
static void InitGui() { // A really basic colour theme for the GUIs
	Text_ds.color					= 0xFFd7d7d7; // title text colour
	Gui.tds_text.color				= 0xFFC7C7C7; // bold left text colourr
	Gui.tds_button.color			= 0xFFd7d7d7; // button/combo text colour
	Gui.color_cmenu					= 0x33a6a6a6; // combo drop-down background colour
	Gui.color_desc					= 0x33a6a6a6; // tooltip background colour
	Gui.tds_desc.color				= 0xFFd7d7d7; // tooltip text colour
	Gui.tds_textline.color			= 0xFFd7d7d7; // input text colour
	Gui.tds_cmenu.color				= 0xFFd7d7d7; // drop down text colour
	Gui.style_window.border_color	= 0x77333333; // options border colour
	Gui.style_window.blur_color		= 0xAA272727; // options background blur

	Text_ds.shade = 180; Gui.tds_button.shade = 180; Gui.tds_textline.shade = 200;

	Gui.tds_button.shadow = 0xFF; Gui.tds_cmenu.shadow = 0xFF; Gui.tds_desc.shadow = 0xFF;
	Gui.tds_list.shadow = 0xFF; Gui.tds_text.shadow = 0xFF; Gui.tds_textline.shadow = 0xFF;
	Gui.tds_text.scale *= 1.2; Gui.kb_lit = 0;
}
Bool Init() {
	InitGui();
	Options.create();
	MsgBox.create();
	StateConnect.set();
	return true;
}
void Shut() {
	network.Logoff();
	ConfigSave("config.txt");
	network.ShutNetworking();
}
/******************************************************************************/
Bool Update() {
	return true;
}
void Draw() {
}
/******************************************************************************/