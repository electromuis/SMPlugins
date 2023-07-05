#ifndef ScreenOptionsMemoryCard_H
#define ScreenOptionsMemoryCard_H

#include "ScreenOptions.h"
#include "arch/MemoryCard/MemoryCardDriver.h"

#include <vector>


class ScreenOptionsMemoryCard : public ScreenOptions
{
public:
	virtual void Init();
	void BeginScreen();

	virtual void HandleScreenMessage( const ScreenMessage SM );

protected:
	virtual void AfterChangeRow( PlayerNumber pn );

private:
	void ImportOptions( int iRow, const std::vector<PlayerNumber> &vpns );
	void ExportOptions( int iRow, const std::vector<PlayerNumber> &vpns );

	void HandleMessage( const Message &msg );

	void ProcessMenuStart( const InputEventPlus &input );

	void CreateMenu();
	void SelectRowWithMemoryCard( const RString &sOsMountPoint );

	bool UpdateCurrentUsbStorageDevices();
	std::vector<UsbStorageDevice> m_CurrentUsbStorageDevices;

	BitmapText m_textOsMountDir;
};

#endif

/*
 * (c) 2005 Chris Danford
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */