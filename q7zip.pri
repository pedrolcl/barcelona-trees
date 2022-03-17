# set the environment variable Q7Z_ROOT before building
win32 {
    7ZIP_BASE=$$(Q7Z_ROOT)/7zip/win
    INCLUDEPATH += \
            $$(Q7Z_ROOT)/q7z \
            $$7ZIP_BASE/C \
            $$7ZIP_BASE/CPP
} else {
    7ZIP_BASE=$$(Q7Z_ROOT)/7zip/unix
    INCLUDEPATH += \
        $$(Q7Z_ROOT)/q7z \
        $$7ZIP_BASE/C \
        $$7ZIP_BASE/CPP \
        $$7ZIP_BASE/CPP/myWindows \
        $$7ZIP_BASE/CPP/include_windows
}

linux*:!android {
    linux-g++:QMAKE_TARGET.arch = $$QMAKE_HOST.arch
    linux-g++-32:QMAKE_TARGET.arch = x86
    linux-g++-64:QMAKE_TARGET.arch = x86_64

    LIBS += $$PWD/linux_x86_64/libq7z.so
    CONFIG += file_copies
    q7zlbs.path = $$OUT_PWD/../lib
    #message("QMAKE_HOST.arch: " $$QMAKE_HOST.arch)
    contains(QMAKE_TARGET.arch, x86_64) {
        q7zlbs.files = $$PWD/linux_x86_64/libq7z.so.*
    }
    COPIES += q7zlbs
    QMAKE_RPATHDIR += ../lib
}

android {
	LIBS += $$PWD/android/libq7z_$${ANDROID_TARGET_ARCH}.so
	#ANDROID_EXTRA_LIBS += $$PWD/android/libq7z_$${ANDROID_TARGET_ARCH}.so
}

win32 {
    CONFIG += file_copies
    CONFIG(debug, debug|release) {
        q7zlbs.path = $$OUT_PWD/debug
    } else {
        q7zlbs.path = $$OUT_PWD/release
    }
    contains(QMAKE_TARGET.arch, x86_64) {
        LIBS += $$PWD/winx64/q7z.lib
        q7zlbs.files = $$PWD/winx64/q7z.dll
    } else {
        LIBS += $$PWD/winx86/q7z.lib
        q7zlbs.files = $$PWD/winx86/q7z.dll
    }
    COPIES += q7zlbs
}

macx {
    LIBS += $$PWD/mac_x64/libq7z.dylib
}
