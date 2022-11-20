//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// http://www.teamsandpit.com/
//
// Notes:
//
//=============================================================================

#include "CppUnitTest.h"

#include "h_export.h"
#include "foolsgoldsource.h"
#include "test.h"
//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: waypointing test code
//
//=============================================================================

#include "wpt.h"
#include "waypoint.h"

namespace tests
{
	TEST_CLASS(waypoint_test)
	{
	public:

		TEST_METHOD(TestWaypointInit)
		{
			WaypointInit();

			for( int i = 0; i < MAX_WAYPOINTS; i++ ) {
				Assert::IsNull( paths[i] );
			}
		}
	};
}
