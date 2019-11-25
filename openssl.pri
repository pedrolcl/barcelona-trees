android {
    equals(ANDROID_TARGET_ARCH,armeabi-v7a) {
            ANDROID_EXTRA_LIBS += \
                    $$PWD/android_arm/libcrypto_1_1.so \
                    $$PWD/android_arm/libssl_1_1.so
    }

    equals(ANDROID_TARGET_ARCH,arm64-v8a) {
            ANDROID_EXTRA_LIBS += \
                    $$PWD/android_arm64/libcrypto_1_1.so \
                    $$PWD/android_arm64/libssl_1_1.so
    }

    equals(ANDROID_TARGET_ARCH,x86) {
            ANDROID_EXTRA_LIBS += \
                    $$PWD/android_x86/libcrypto_1_1.so \
                    $$PWD/android_x86/libssl_1_1.so
    }

    equals(ANDROID_TARGET_ARCH,x86_64) {
            ANDROID_EXTRA_LIBS += \
                    $$PWD/android_x86_64/libcrypto_1_1.so \
                    $$PWD/android_x86_64/libssl_1_1.so
    }
}

win32 {
    CONFIG += file_copies
    CONFIG(debug, debug|release) {
        tlslibs.path = $$OUT_PWD/debug
    } else {
        tlslibs.path = $$OUT_PWD/release
    }
    contains(QMAKE_TARGET.arch, x86_64) {
        tlslibs.files = $$PWD/winx64/libcrypto-1_1-x64.dll \
                          $$PWD/winx64/libssl-1_1-x64.dll
    } else {
        tlslibs.files = $$PWD/winx86/libcrypto-1_1.dll \
                          $$PWD/winx86/libssl-1_1.dll
    }
    COPIES += tlslibs
}
