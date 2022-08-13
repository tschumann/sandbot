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
