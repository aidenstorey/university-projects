//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (c) Media Design School
//
// File Name	: program_entry.h
// Description	: Declaration for CProgramEntry
// Author		: Aiden Storey
// Mail			: aiden.storey@mediadesign.school.nz
//

#ifndef GUARD_AIDEN_STOREY_PROGRAM_ENTRY_H_201409151216
#define GUARD_AIDEN_STOREY_PROGRAM_ENTRY_H_201409151216

// Library Includes
#include <vld.h>
#include <Windows.h>

// Prototypes
class CProgramEntry
{
	// Member Functions
public:
	// Constructors
	CProgramEntry();
	~CProgramEntry();

	// Other 
	bool Initialise( HINSTANCE _hInstance );
	int Run();

protected:
private:

	// Member Variables
public:
protected:
private:
};

#endif // GUARD_AIDEN_STOREY_PROGRAM_ENTRY_H_201409151216