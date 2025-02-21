#pragma once

#include <unordered_map>
#include <string>

#undef VK
#define VK(KEYCODENAME, CODE) {#KEYCODENAME, CODE}

const auto VIRTUAL_KEYCODES = std::unordered_map<std::string, int>{
    // https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

    VK(VK_LBUTTON	            , 0x01),    //	Left mouse button
    VK(VK_RBUTTON	            , 0x02),    //	Right mouse button
    VK(VK_CANCEL	            , 0x03),    //	Control-break processing
    VK(VK_MBUTTON	            , 0x04),    //	Middle mouse button
    VK(VK_XBUTTON1	            , 0x05),    //	X1 mouse button
    VK(VK_XBUTTON2	            , 0x06),    //	X2 mouse button
    VK(VK_BACK	                , 0x08),    //	BACKSPACE key
    VK(VK_TAB	                , 0x09),    //	TAB key
    VK(VK_CLEAR	                , 0x0C),    //	CLEAR key
    VK(VK_RETURN	            , 0x0D),    //	ENTER key
    VK(VK_SHIFT	                , 0x10),    //	SHIFT key
    VK(VK_CONTROL	            , 0x11),    //	CTRL key
    VK(VK_MENU	                , 0x12),    //	ALT key
    VK(VK_PAUSE	                , 0x13),    //	PAUSE key
    VK(VK_CAPITAL	            , 0x14),    //	CAPS LOCK key
    VK(VK_KANA	                , 0x15),    //	IME Kana mode
    VK(VK_HANGUL	            , 0x15),    //	IME Hangul mode
    VK(VK_IME_ON	            , 0x16),    //	IME On
    VK(VK_JUNJA	                , 0x17),    //	IME Junja mode
    VK(VK_FINAL	                , 0x18),    //	IME final mode
    VK(VK_HANJA	                , 0x19),    //	IME Hanja mode
    VK(VK_KANJI	                , 0x19),    //	IME Kanji mode
    VK(VK_IME_OFF	            , 0x1A),    //	IME Off
    VK(VK_ESCAPE	            , 0x1B),    //	ESC key
    VK(VK_CONVERT	            , 0x1C),    //	IME convert
    VK(VK_NONCONVERT	        , 0x1D),    //	IME nonconvert
    VK(VK_ACCEPT	            , 0x1E),    //	IME accept
    VK(VK_MODECHANGE	        , 0x1F),    //	IME mode change request
    VK(VK_SPACE	                , 0x20),    //	SPACEBAR
    VK(VK_PRIOR	                , 0x21),    //	PAGE UP key
    VK(VK_NEXT	                , 0x22),    //	PAGE DOWN key
    VK(VK_END	                , 0x23),    //	END key
    VK(VK_HOME	                , 0x24),    //	HOME key
    VK(VK_LEFT	                , 0x25),    //	LEFT ARROW key
    VK(VK_UP	                , 0x26),    //	UP ARROW key
    VK(VK_RIGHT	                , 0x27),    //	RIGHT ARROW key
    VK(VK_DOWN	                , 0x28),    //	DOWN ARROW key
    VK(VK_SELECT	            , 0x29),    //	SELECT key
    VK(VK_PRINT	                , 0x2A),    //	PRINT key
    VK(VK_EXECUTE	            , 0x2B),    //	EXECUTE key
    VK(VK_SNAPSHOT	            , 0x2C),    //	PRINT SCREEN key
    VK(VK_INSERT	            , 0x2D),    //	INS key
    VK(VK_DELETE	            , 0x2E),    //	DEL key
    VK(VK_HELP	                , 0x2F),    //	HELP key
    VK(VK_0                     , 0x30),    //	0 key
    VK(VK_1                     , 0x31),    //	1 key
    VK(VK_2                     , 0x32),    //	2 key
    VK(VK_3                     , 0x33),    //	3 key
    VK(VK_4                     , 0x34),    //	4 key
    VK(VK_5                     , 0x35),    //	5 key
    VK(VK_6                     , 0x36),    //	6 key
    VK(VK_7                     , 0x37),    //	7 key
    VK(VK_8                     , 0x38),    //	8 key
    VK(VK_9                     , 0x39),    //	9 key
    VK(VK_A                     , 0x41),    //	A key
    VK(VK_B                     , 0x42),    //	B key
    VK(VK_C                     , 0x43),    //	C key
    VK(VK_D                     , 0x44),    //	D key
    VK(VK_E                     , 0x45),    //	E key
    VK(VK_F                     , 0x46),    //	F key
    VK(VK_G                     , 0x47),    //	G key
    VK(VK_H                     , 0x48),    //	H key
    VK(VK_I                     , 0x49),    //	I key
    VK(VK_J                     , 0x4A),    //	J key
    VK(VK_K                     , 0x4B),    //	K key
    VK(VK_L                     , 0x4C),    //	L key
    VK(VK_M                     , 0x4D),    //	M key
    VK(VK_N                     , 0x4E),    //	N key
    VK(VK_O                     , 0x4F),    //	O key
    VK(VK_P                     , 0x50),    //	P key
    VK(VK_Q                     , 0x51),    //	Q key
    VK(VK_R                     , 0x52),    //	R key
    VK(VK_S                     , 0x53),    //	S key
    VK(VK_T                     , 0x54),    //	T key
    VK(VK_U                     , 0x55),    //	U key
    VK(VK_V                     , 0x56),    //	V key
    VK(VK_W                     , 0x57),    //	W key
    VK(VK_X                     , 0x58),    //	X key
    VK(VK_Y                     , 0x59),    //	Y key
    VK(VK_Z                     , 0x5A),    //	Z key
    VK(VK_LWIN	                , 0x5B),    //	Left Windows key
    VK(VK_RWIN	                , 0x5C),    //	Right Windows key
    VK(VK_APPS	                , 0x5D),    //	Applications key
    VK(VK_SLEEP	                , 0x5F),    //	Computer Sleep key
    VK(VK_NUMPAD0	            , 0x60),    //	Numeric keypad 0 key
    VK(VK_NUMPAD1	            , 0x61),    //	Numeric keypad 1 key
    VK(VK_NUMPAD2	            , 0x62),    //	Numeric keypad 2 key
    VK(VK_NUMPAD3	            , 0x63),    //	Numeric keypad 3 key
    VK(VK_NUMPAD4	            , 0x64),    //	Numeric keypad 4 key
    VK(VK_NUMPAD5	            , 0x65),    //	Numeric keypad 5 key
    VK(VK_NUMPAD6	            , 0x66),    //	Numeric keypad 6 key
    VK(VK_NUMPAD7	            , 0x67),    //	Numeric keypad 7 key
    VK(VK_NUMPAD8	            , 0x68),    //	Numeric keypad 8 key
    VK(VK_NUMPAD9	            , 0x69),    //	Numeric keypad 9 key
    VK(VK_MULTIPLY	            , 0x6A),    //	Multiply key
    VK(VK_ADD	                , 0x6B),    //	Add key
    VK(VK_SEPARATOR	            , 0x6C),    //	Separator key
    VK(VK_SUBTRACT	            , 0x6D),    //	Subtract key
    VK(VK_DECIMAL	            , 0x6E),    //	Decimal key
    VK(VK_DIVIDE	            , 0x6F),    //	Divide key
    VK(VK_F1	                , 0x70),    //	F1 key
    VK(VK_F2	                , 0x71),    //	F2 key
    VK(VK_F3	                , 0x72),    //	F3 key
    VK(VK_F4	                , 0x73),    //	F4 key
    VK(VK_F5	                , 0x74),    //	F5 key
    VK(VK_F6	                , 0x75),    //	F6 key
    VK(VK_F7	                , 0x76),    //	F7 key
    VK(VK_F8	                , 0x77),    //	F8 key
    VK(VK_F9	                , 0x78),    //	F9 key
    VK(VK_F10	                , 0x79),    //	F10 key
    VK(VK_F11	                , 0x7A),    //	F11 key
    VK(VK_F12	                , 0x7B),    //	F12 key
    VK(VK_F13	                , 0x7C),    //	F13 key
    VK(VK_F14	                , 0x7D),    //	F14 key
    VK(VK_F15	                , 0x7E),    //	F15 key
    VK(VK_F16	                , 0x7F),    //	F16 key
    VK(VK_F17	                , 0x80),    //	F17 key
    VK(VK_F18	                , 0x81),    //	F18 key
    VK(VK_F19	                , 0x82),    //	F19 key
    VK(VK_F20	                , 0x83),    //	F20 key
    VK(VK_F21	                , 0x84),    //	F21 key
    VK(VK_F22	                , 0x85),    //	F22 key
    VK(VK_F23	                , 0x86),    //	F23 key
    VK(VK_F24	                , 0x87),    //	F24 key
    VK(VK_NUMLOCK	            , 0x90),    //	NUM LOCK key
    VK(VK_SCROLL	            , 0x91),    //	SCROLL LOCK key
    VK(VK_LSHIFT	            , 0xA0),    //	Left SHIFT key
    VK(VK_RSHIFT	            , 0xA1),    //	Right SHIFT key
    VK(VK_LCONTROL	            , 0xA2),    //	Left CONTROL key
    VK(VK_RCONTROL	            , 0xA3),    //	Right CONTROL key
    VK(VK_LMENU	                , 0xA4),    //	Left ALT key
    VK(VK_RMENU	                , 0xA5),    //	Right ALT key
    VK(VK_BROWSER_BACK	        , 0xA6),    //	Browser Back key
    VK(VK_BROWSER_FORWARD	    , 0xA7),    //	Browser Forward key
    VK(VK_BROWSER_REFRESH	    , 0xA8),    //	Browser Refresh key
    VK(VK_BROWSER_STOP	        , 0xA9),    //	Browser Stop key
    VK(VK_BROWSER_SEARCH	    , 0xAA),    //	Browser Search key
    VK(VK_BROWSER_FAVORITES	    , 0xAB),    //	Browser Favorites key
    VK(VK_BROWSER_HOME	        , 0xAC),    //	Browser Start and Home key
    VK(VK_VOLUME_MUTE	        , 0xAD),    //	Volume Mute key
    VK(VK_VOLUME_DOWN	        , 0xAE),    //	Volume Down key
    VK(VK_VOLUME_UP	            , 0xAF),    //	Volume Up key
    VK(VK_MEDIA_NEXT_TRACK	    , 0xB0),    //	Next Track key
    VK(VK_MEDIA_PREV_TRACK	    , 0xB1),    //	Previous Track key
    VK(VK_MEDIA_STOP	        , 0xB2),    //	Stop Media key
    VK(VK_MEDIA_PLAY_PAUSE	    , 0xB3),    //	Play/Pause Media key
    VK(VK_LAUNCH_MAIL	        , 0xB4),    //	Start Mail key
    VK(VK_LAUNCH_MEDIA_SELECT	, 0xB5),    //	Select Media key
    VK(VK_LAUNCH_APP1	        , 0xB6),    //	Start Application 1 key
    VK(VK_LAUNCH_APP2	        , 0xB7),    //	Start Application 2 key
    VK(VK_OEM_1	                , 0xBA),    //	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ;: key
    VK(VK_OEM_PLUS	            , 0xBB),    //	For any country/region, the + key
    VK(VK_OEM_COMMA	            , 0xBC),    //	For any country/region, the , key
    VK(VK_OEM_MINUS	            , 0xBD),    //	For any country/region, the - key
    VK(VK_OEM_PERIOD	        , 0xBE),    //	For any country/region, the . key
    VK(VK_OEM_2	                , 0xBF),    //	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the /? key
    VK(VK_OEM_3	                , 0xC0),    //	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the `~ key
    VK(VK_OEM_4	                , 0xDB),    //	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the [{ key
    VK(VK_OEM_5	                , 0xDC),    //	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \\| key
    VK(VK_OEM_6	                , 0xDD),    //	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ]} key
    VK(VK_OEM_7	                , 0xDE),    //	Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '" key
    VK(VK_OEM_8	                , 0xDF),    //	Used for miscellaneous characters; it can vary by keyboard.
    VK(VK_OEM_102	            , 0xE2),    //	The <> keys on the US standard keyboard, or the \\| key on the non-US 102-key keyboard
    VK(VK_PROCESSKEY	        , 0xE5),    //	IME PROCESS key
    VK(VK_PACKET	            , 0xE7),    //	Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
    VK(VK_ATTN	                , 0xF6),    //	Attn key
    VK(VK_CRSEL	                , 0xF7),    //	CrSel key
    VK(VK_EXSEL	                , 0xF8),    //	ExSel key
    VK(VK_EREOF	                , 0xF9),    //	Erase EOF key
    VK(VK_PLAY	                , 0xFA),    //	Play key
    VK(VK_ZOOM	                , 0xFB),    //	Zoom key
    VK(VK_NONAME	            , 0xFC),    //	Reserved
    VK(VK_PA1	                , 0xFD),    //	PA1 key
    VK(VK_OEM_CLEAR	            , 0xFE),    //	Clear key
};

#undef VK

namespace GameEngine {

class Input {
    struct AxisDescriptor {
        float val = 0.0;
        float sencetivity = 10.0;
        int minKey = -1, maxKey = -1;
    };

    std::unordered_map<std::string, AxisDescriptor> aces;
public:
    void Init (const char* configPath);

    void Update (float dt);

    float GetAxis (const std::string& axisName) const;
};

}