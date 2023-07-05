#ifndef ScreenOptionsExportPackage_H
#define ScreenOptionsExportPackage_H

#include "ScreenOptions.h"
#include "ScreenMiniMenu.h"

#include <vector>


class Course;

// Can this be done any better? -aj
namespace ExportPackages
{
	RString m_sPackageType;
	RString m_sFolder; // used for more subpages
}

/** @brief A menu for exporting packages. */
class ScreenOptionsExportPackage : public ScreenOptions
{
public:
	void Init();
	virtual void BeginScreen();

protected:
	virtual void ImportOptions( int iRow, const std::vector<PlayerNumber> &vpns );
	virtual void ExportOptions( int iRow, const std::vector<PlayerNumber> &vpns );

	virtual void ProcessMenuStart( const InputEventPlus &input );

	std::vector<RString> m_vsPackageTypes;
};

class ScreenOptionsExportPackageSubPage : public ScreenOptions
{
public:
	void Init();
	virtual void BeginScreen();

protected:
	virtual void ImportOptions( int iRow, const std::vector<PlayerNumber> &vpns );
	virtual void ExportOptions( int iRow, const std::vector<PlayerNumber> &vpns );

	virtual void ProcessMenuStart( const InputEventPlus &input );

	std::vector<RString> m_vsPossibleDirsToExport;
};

#endif

/*
 * (c) 2003-2004 Chris Danford
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