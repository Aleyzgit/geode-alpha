#include <Geode/binding/ProfilePage.hpp>

#ifdef GEODE_IS_WINDOWS
ProfilePage::ProfilePage() :
    m_score(nullptr),
    m_accountID(0),
    m_ownProfile(false),
    m_profileKey(),
    m_somethingWentWrong(nullptr),
    m_usernameLabel(nullptr),
    m_list(nullptr),
    m_rightArrow(nullptr),
    m_leftArrow(nullptr),
    m_followBtn(nullptr),
    m_followHint(nullptr),
    m_unk(false),
    m_buttons(nullptr),
    m_arrayWithUsernameLabel(nullptr),
    m_itemCount(0),
    m_pageStartIdx(0),
    m_pageEndIdx(0),
    m_page(0),
    m_circle(nullptr),
    m_popupDelegate(nullptr),
    m_refreshBtn(nullptr),
    m_unk2(0) {}
#endif
