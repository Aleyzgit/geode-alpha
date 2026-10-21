# Count declaration placeholders without treating quoted text or comments as
# active bindings. Preserve declarations following quoted URL parameters.
function(alpha_count_windows_placeholders content output)
    string(REGEX REPLACE "\"([^\"\\\r\n]|\\.)*\"" "" content "${content}")
    string(REGEX REPLACE "/\\*([^*]|\\*+[^*/])*\\*+/" "" content "${content}")
    string(REGEX REPLACE "//[^\r\n]*" "" content "${content}")
    string(REGEX MATCHALL "win[ \t]+0x9999999" matches "${content}")
    list(LENGTH matches count)
    set("${output}" "${count}" PARENT_SCOPE)
endfunction()
