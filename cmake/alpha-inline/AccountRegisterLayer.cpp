#include <Geode/binding/AccountRegisterLayer.hpp>
#include <Geode/utils/addresser.hpp>

#ifdef GEODE_IS_WINDOWS
bool AccountRegisterLayer::validPassword(std::string password) {
    // The native submit path rejects short, nonprintable and common values.
    // Strength classes 3..6 pass those checks; 0..2 identify the rejection.
    using FunctionType = int(*)(std::string const&);
    return reinterpret_cast<FunctionType>(geode::base::get() + 0x8cb70)(password) >= 3;
}
#endif
