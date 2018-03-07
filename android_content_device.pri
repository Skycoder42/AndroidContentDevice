INCLUDEPATH += $$PWD

android {
	HEADERS += $$PWD/contentdevice.h
	SOURCES += $$PWD/contentdevice.cpp
	
	TRANSLATIONS += \
		$$PWD/contentdevice_de.ts \
		$$PWD/contentdevice_template.ts
}

include($$PWD/android_content_device.prc)
