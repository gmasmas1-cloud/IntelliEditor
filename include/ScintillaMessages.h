// Scintilla source code edit control
/** @file ScintillaMessages.h
 ** Enumerate the messages that can be sent to Scintilla.
 **/
// Copyright 1998-2019 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef SCINTILLAMESSAGES_H
#define SCINTILLAMESSAGES_H

namespace Scintilla {

// Basic message enumeration for common operations
enum class Message {
	AddText = 2001,
	ClearAll = 2004,
	GetLength = 2006,
	GetCharAt = 2007,
	GetCurrentPos = 2008,
	GetAnchor = 2009,
	GetStyleAt = 2010,
	SelectAll = 2013,
	GotoLine = 2024,
	GotoPos = 2025,
	SetAnchor = 2026,
	EndStyled = 2028,
	GetEOLMode = 2030,
	SetEOLMode = 2031,
	SetStyling = 2033,
	SetTabWidth = 2036,
	GetTabWidth = 2121,
	LineDown = 2300,
	LineUp = 2302,
	LineEnd = 2314,
	Home = 2312,
	DocumentStart = 2316,
	DocumentEnd = 2318,
	PageUp = 2320,
	PageDown = 2322,
	SetCurrentPos = 2141,
	SetSelectionStart = 2142,
	GetSelectionStart = 2143,
	SetSelectionEnd = 2144,
	GetSelectionEnd = 2145,
	GetText = 2182,
	SetText = 2181,
	GetTextLength = 2183,
	GetReadOnly = 2140,
	SetReadOnly = 2171,
	Copy = 2178,
	Cut = 2177,
	Paste = 2179,
	Undo = 2176,
	CanUndo = 2174,
	Redo = 2011,
	CanRedo = 2016,
	EmptyUndoBuffer = 2175,
	BeginUndoAction = 2078,
	EndUndoAction = 2079,
	GetLineCount = 2154,
	GetLine = 2153,
	LineFromPosition = 2166,
	PositionFromLine = 2167,
};

}

#endif
