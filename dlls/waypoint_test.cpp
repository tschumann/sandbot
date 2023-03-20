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

#include "CppUnitTest.h"

#include "h_export.h"
#include "wpt.h"
#include "waypoint.h"
#include "foolsgoldsource/foolsgoldsource.h"
#include "foolsgoldsource/vscu_test.h"

namespace tests
{
	TEST_CLASS(waypoint_test)
	{
	public:

		TEST_METHOD(TestWaypointInit)
		{
			WaypointInit();

			for( int i = 0; i < MAX_WAYPOINTS; i++ )
			{
				Assert::IsNull( paths[i] );
			}
		}
	};
}
