!if ${ALPHA_UNRESOLVED_WINDOWS_BINDINGS} != 0
    !error "Geode Alpha installer is blocked: unresolved Windows native bindings remain."
!endif
!if ${ALPHA_RELEASE_READY} != 1
    !error "Geode Alpha installer is blocked: native layout and compatibility validation is incomplete."
!endif
