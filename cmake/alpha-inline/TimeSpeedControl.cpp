#include <Geode/binding/TimeSpeedControl.hpp>

#ifdef GEODE_IS_WINDOWS
TimeSpeedControl::TimeSpeedControl() :
    m_gameLayer(nullptr),
    m_slider(nullptr),
    m_label(nullptr),
    m_audioToggle(nullptr) {}
#endif
