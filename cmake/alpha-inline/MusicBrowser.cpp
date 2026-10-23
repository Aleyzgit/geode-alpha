#include <Geode/binding/MusicBrowser.hpp>

#ifdef GEODE_IS_WINDOWS
MusicBrowser::MusicBrowser() :
    m_songType(static_cast<GJSongType>(-1)),
    m_searchResult(nullptr),
    m_listLayer(nullptr),
    m_infoLabel(nullptr),
    m_pageLabel(nullptr),
    m_prevButton(nullptr),
    m_nextButton(nullptr),
    m_refreshButton(nullptr),
    m_clearSearchButton(nullptr),
    m_searchInput(nullptr),
    m_searchBackground(nullptr),
    m_clearSearchTextButton(nullptr),
    m_searchVisible(false),
    m_brandSprite(nullptr),
    m_tagFilterButton(nullptr),
    m_artistFilterButton(nullptr),
    m_downloadedViewButton(nullptr),
    m_favoriteViewButton(nullptr),
    m_levelViewButton(nullptr),
    m_circleSprite(nullptr),
    m_songID(0),
    m_libraryVersion(0),
    m_gettingURL(false),
    m_selectedCell(false),
    m_delegate(nullptr),
    m_playSlider(nullptr),
    m_playLabel(nullptr),
    m_playButton(nullptr),
    m_autoUpdating(false) {}
#endif
