#include "Player.h"
#include "Log.h"

#ifndef WIN32
#include <dlfcn.h>
#else
#include <windows.h>
#endif

using namespace AntBattle;

Player::Player(const std::string& libname) : m_libName(libname)
{
	//load lib
	if (loadLibrary()) {
		// get PlayerInfo
		(*m_fnInit)(&m_info);
		m_isInit = true;
	}

	//TODO calc m_libHash as CRC32(file libname)

	m_libHash = 0;
}

Player::~Player()
{
	if (m_fnFinalize) {
		(*m_fnFinalize)();
	}

	if (m_handleLib) {
		#ifndef WIN32
			dlclose(m_handleLib);
		#else
			FreeLibrary((HINSTANCE)m_handleLib);
		#endif
	}
}

bool Player::loadLibrary()
{
	#ifndef WIN32
	   m_handleLib = dlopen(m_libname.c_str(), RTLD_LAZY);
	#else
	   m_handleLib = (void*)LoadLibrary(m_libName.c_str());
	#endif

	if (!m_handleLib) {
		Log::instance().put(format("Can't open library '%s'", m_libName.c_str()));
		return false;
	}

	#ifndef WIN32
		fun = (void (*)(void))dlsym(lib, "run");
	#else
		m_fnInit          = (AntInit)GetProcAddress((HINSTANCE)m_handleLib, "antInit");
		m_fnFinalize      = (AntFinalize)GetProcAddress((HINSTANCE)m_handleLib, "antFinalize");
		m_fnWorkerProcess = (AntProcess)GetProcAddress((HINSTANCE)m_handleLib, "workerProcess");
		m_fnSolderProcess = (AntProcess)GetProcAddress((HINSTANCE)m_handleLib, "solderProcess");
		m_fnQueenProcess  = (AntProcess)GetProcAddress((HINSTANCE) m_handleLib, "queenProcess");
	#endif

	if (!m_fnInit) {
		Log::instance().put(format("Can't load function 'antInit' from '%s'", m_libName.c_str()));

	} else if (!m_fnFinalize) {
		Log::instance().put(format("Can't load function 'antFinalize' from '%s'", m_libName.c_str()));

	} else if (!m_fnWorkerProcess) {
		Log::instance().put(format("Can't load function 'workerProcess' from '%s'", m_libName.c_str()));

	} else if (!m_fnSolderProcess) {
		Log::instance().put(format("Can't load function 'solderProcess' from '%s'", m_libName.c_str()));

	} else if (!m_fnQueenProcess) {
		Log::instance().put(format("Can't load function 'queenProcess' from '%s'", m_libName.c_str()));

	} else {
		return true;
	}

	return false;
}

bool Player::isInit() const
{
	return m_isInit;
}

void Player::antQueen(const std::weak_ptr<Ant>& queen)
{
	m_antQueen = queen;
}

std::weak_ptr<Ant> Player::antQueen()
{
	return m_antQueen;
}
