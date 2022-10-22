//=============================================================================
//
// Sandbot - GoldSource engine multiplayer bot
//
// Based on HPB_Bot by Jeffrey "botman" Broome
//
// http://www.teamsandpit.com/
//
// Notes: test code
//
//=============================================================================

#ifndef __TEST_H__
#define __TEST_H__

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// CppUnitTestFramework does something strange and breaks the engine's ability to load the .dll so stub it out when building normally
// test.ps1 creates a file called run_tests so that during the test script this block is skipped and vstest can run the tests
#if !__has_include("run_tests")

#undef TEST_METHOD
#define TEST_METHOD(methodName) static const void* test##methodName() { return nullptr; } void methodName()

#endif // !__has_include("run_tests")

#endif // __TEST_H__