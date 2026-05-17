// Scintilla source code edit control
/** @file ScintillaStructures.h
 ** Structures used to communicate with Scintilla.
 **/
// Copyright 2021 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef SCINTILLASTRUCTURES_H
#define SCINTILLASTRUCTURES_H

namespace Scintilla {

using PositionCR = long;

struct CharacterRange {
	PositionCR cpMin;
	PositionCR cpMax;
};

struct TextRange {
	CharacterRange chrg;
	char *lpstrText;
};

struct TextToFind {
	CharacterRange chrg;
	const char *lpstrText;
	CharacterRange chrgText;
};

using SurfaceID = void *;

struct Rectangle {
	int left;
	int top;
	int right;
	int bottom;
};

struct RangeToFormat {
	SurfaceID hdc;
	SurfaceID hdcTarget;
	Rectangle rc;
	Rectangle rcPage;
	CharacterRange chrg;
};

struct NotifyHeader {
	void *hwndFrom;
	uptr_t idFrom;
	unsigned int code;
};

}

#endif
