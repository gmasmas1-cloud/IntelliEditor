// Scintilla source code edit control
/** @file ScintillaTypes.h
 ** Types used to communicate with Scintilla.
 **/
// Copyright 1998-2019 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef SCINTILLATYPES_H
#define SCINTILLATYPES_H

namespace Scintilla {

// Basic type definitions
using Position = intptr_t;
using Line = intptr_t;
using Colour = int;
using ColourAlpha = int;
using uptr_t = uintptr_t;
using sptr_t = intptr_t;

// Common enumerations for basic operations
enum class WhiteSpace {
	Invisible = 0,
	VisibleAlways = 1,
	VisibleAfterIndent = 2,
	VisibleOnlyInIndent = 3,
};

enum class TabDrawMode {
	LongArrow = 0,
	StrikeOut = 1,
	ControlChar = 2,
};

enum class EndOfLine {
	CrLf = 0,
	Cr = 1,
	Lf = 2,
};

enum class MarkerSymbol {
	Circle = 0,
	RoundRect = 1,
	Arrow = 2,
	SmallRect = 3,
	ShortArrow = 4,
	Empty = 5,
	ArrowDown = 6,
	Minus = 7,
	Plus = 8,
	VLine = 9,
	LCorner = 10,
	TCorner = 11,
	BoxPlus = 12,
	BoxPlusConnected = 13,
	BoxMinus = 14,
	BoxMinusConnected = 15,
};

enum class CaretStyle {
	Invisible = 0,
	Line = 1,
	Block = 2,
};

enum class KeyMod {
	Norm = 0,
	Shift = 1,
	Ctrl = 2,
	Alt = 4,
	Super = 8,
	Meta = 16,
};

enum class Notification {
	StyleNeeded = 2000,
	CharAdded = 2001,
	SavePointReached = 2002,
	SavePointLeft = 2003,
	ModifyAttemptRO = 2004,
	Key = 2005,
	DoubleClick = 2006,
	UpdateUI = 2007,
	Modified = 2008,
	MacroRecord = 2009,
	MarginClick = 2010,
	NeedShown = 2011,
	Painted = 2013,
	UserListSelection = 2014,
	DwellStart = 2016,
	DwellEnd = 2017,
	Zoom = 2018,
	HotSpotClick = 2019,
	DoubleClickHotSpot = 2020,
	CallTipClick = 2021,
	AutoCSelection = 2022,
};

}

#endif
