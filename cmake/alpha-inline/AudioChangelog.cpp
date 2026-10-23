#include <Geode/binding/MusicChangelogEntry.hpp>
#include <Geode/binding/MusicChangelogLine.hpp>
#include <Geode/binding/SFXChangelogEntry.hpp>
#include <Geode/cocos/cocoa/CCArray.h>

#ifdef GEODE_IS_WINDOWS
namespace {
    template<class Entry>
    bool initializeEntry(Entry* entry, int value1, int value2, int value3) {
        auto values = cocos2d::CCArray::create();
        if (!values) return false;
        values->retain();
        CC_SAFE_RELEASE(entry->m_entries);
        entry->m_value1 = value1;
        entry->m_value2 = value2;
        entry->m_value3 = value3;
        entry->m_entries = values;
        return true;
    }
}

SFXChangelogEntry::SFXChangelogEntry() : m_value1(0), m_value2(0), m_value3(0), m_entries(nullptr) {}

SFXChangelogEntry::~SFXChangelogEntry() {
    CC_SAFE_RELEASE(m_entries);
}

SFXChangelogEntry* SFXChangelogEntry::create(int value1, int value2, int value3) {
    auto entry = new SFXChangelogEntry();
    if (entry->init(value1, value2, value3)) {
        entry->autorelease();
        return entry;
    }
    delete entry;
    return nullptr;
}

bool SFXChangelogEntry::init(int value1, int value2, int value3) {
    return initializeEntry(this, value1, value2, value3);
}

MusicChangelogEntry::MusicChangelogEntry() : m_value1(0), m_value2(0), m_value3(0), m_entries(nullptr) {}

MusicChangelogEntry::~MusicChangelogEntry() {
    CC_SAFE_RELEASE(m_entries);
}

MusicChangelogEntry* MusicChangelogEntry::create(int value1, int value2, int value3) {
    auto entry = new MusicChangelogEntry();
    if (entry->init(value1, value2, value3)) {
        entry->autorelease();
        return entry;
    }
    delete entry;
    return nullptr;
}

bool MusicChangelogEntry::init(int value1, int value2, int value3) {
    return initializeEntry(this, value1, value2, value3);
}

MusicChangelogLine::MusicChangelogLine() : m_value1(0), m_value2(0) {}

MusicChangelogLine::~MusicChangelogLine() {}

MusicChangelogLine* MusicChangelogLine::create(int value1, std::string value, int value2) {
    auto line = new MusicChangelogLine();
    if (line->init(value1, std::move(value), value2)) {
        line->autorelease();
        return line;
    }
    delete line;
    return nullptr;
}

bool MusicChangelogLine::init(int value1, std::string value, int value2) {
    m_value1 = value1;
    m_value2 = value2;
    m_value = std::move(value);
    return true;
}
#endif
