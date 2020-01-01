//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// http://www.teamsandpit.com/
//
// Notes:
//
//=============================================================================

#ifndef _TEST_H_
#define _TEST_H_

// CppUnitTestFramework does something strange and breaks the engine's ability to load the .dll so stub it out when building for real
// #define TEST

#ifdef TEST

#undef TEST_METHOD
#define TEST_METHOD(methodName) static const void* test##methodName() { return nullptr; } void methodName()

#endif // TEST

#endif // _TEST_H_