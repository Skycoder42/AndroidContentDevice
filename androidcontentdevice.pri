android {
	QT += androidextras
	
	HEADERS += \
		$$PWD/contentdevice.h
	SOURCES += \
		$$PWD/contentdevice.cpp
	
	QDEP_TRANSLATIONS += \
		$$PWD/contentdevice_de.ts \
		$$PWD/contentdevice_template.ts
		
	!qdep_build: EXTRA_TRANSLATIONS += $$QDEP_TRANSLATIONS
	
	INCLUDEPATH += $$PWD
}

QDEP_PACKAGE_EXPORTS += Q_ANDROID_CONTENT_DEVICE_EXPORT
!qdep_build: DEFINES += "Q_ANDROID_CONTENT_DEVICE_EXPORT="
