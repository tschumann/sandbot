
#ifdef _WIN32
#define EXPORT	_declspec( dllexport )
#else
#define EXPORT	/* */
#endif

typedef void (*testfunc)();

extern "C" EXPORT void RunTests()
{
	// TODO: get all the tests and load this function from an external executable to run them
}