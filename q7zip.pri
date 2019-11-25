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
    extralibs.path = $$OUT_PWD/lib
    message("QMAKE_HOST.arch: " $$QMAKE_HOST.arch)
    contains(QMAKE_TARGET.arch, x86_64) {
        extralibs.files = $$PWD/linux_x86_64/libq7z.so.*
    }
    COPIES += extralibs
    QMAKE_RPATHDIR += lib
}

android {
    equals(ANDROID_TARGET_ARCH,armeabi-v7a) {
        LIBS += $$PWD/android_arm/libq7z.so
        ANDROID_EXTRA_LIBS += $$PWD/android_arm/libq7z.so
    }
    equals(ANDROID_TARGET_ARCH,arm64-v8a) {
        LIBS += $$PWD/android_arm64/libq7z.so
        ANDROID_EXTRA_LIBS += $$PWD/android_arm64/libq7z.so
    }
    equals(ANDROID_TARGET_ARCH,x86) {
        LIBS += $$PWD/android_x86/libq7z.so
        ANDROID_EXTRA_LIBS += $$PWD/android_x86/libq7z.so
    }
    equals(ANDROID_TARGET_ARCH,x86_64) {
        LIBS += $$PWD/android_x86_64/libq7z.so
        ANDROID_EXTRA_LIBS += $$PWD/android_x86_64/libq7z.so
    }
}

win32 {
    CONFIG += file_copies
    CONFIG(debug, debug|release) {
        extralibs.path = $$OUT_PWD/debug
    } else {
        extralibs.path = $$OUT_PWD/release
    }
    contains(QMAKE_TARGET.arch, x86_64) {
        LIBS += $$PWD/winx64/q7z.lib
        extralibs.files = $$PWD/winx64/q7z.dll
    } else {
        LIBS += $$PWD/winx86/q7z.lib
        extralibs.files = $$PWD/winx86/q7z.dll
    }
    COPIES += extralibs
}
