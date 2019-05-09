#pragma once
#ifndef defines_h
#define defines_h

#define ARDUINO

// PIN DEFINES
enum PinConfiguration : char
{
	taster_LINKS_pin = 7,
	taster_RECHTS_pin = 6,
	nfc_RS_pin = 2,
	display_CLK_pin = 0,
	display_DATA_pin = 1,
	nfc_SS_pin = 11,
	sd_CS_pin = 4,
};

// WEB SERVER DEFINES

#define SECRET_SSID "Werners Kaffeestube"
#define SECRET_PASS "Werner"

// STATES
enum MillStates : char
{
	WaitForCard = 0,
	ReadCard = 1,
	WaitForInput = 2,
	Doppelt = 3,
	Einfach = 4,
	WaitForSplitBooking = 5,
	IdentSecondPayer = 6,
	UnknownUserState = 7,
	LastUserState = 8,
	CurrentDrawsState = 9,
	KaffeeKingState = 10
};

#endif
