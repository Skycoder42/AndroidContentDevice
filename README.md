# AndroidContentDevice
A QIODevice implementation to be able to open "content://" urls on android

## Features
When handling files on Android and using the modern "file chooser" API (`Intent.createChooser`), you are typically getting a content-URL returned. These are special URLs starting with `content://...` - they can be normal files, but often are references to resources of specific content providers that may not be locally available (A good example is a could service like DropBox or SeaFile).

With this class you can open surch URLs for reading or writing and can thus access such "files" even if they are not locally present. Please note that the class basicly wrapps the Java API, and since it can open Network-Files, too, it may be rather slow for bigger files.

**Note:** If you need a way to get such a content URL from C++ easily, have a look at my [QtMvvm Project](https://github.com/Skycoder42/QtMvvm). It provides such a file picker as part of the Framework. If you only need the file chooser itself, have a look at the [`AndroidFileChooser`](https://github.com/Skycoder42/QtMvvm/blob/master/src/imports/mvvmquick/androidfilechooser.h) class.

## Installation
The package is provided via qdep, as `Skycoder42/AndroidContentDevice`. To use it simply:

1. Install and enable qdep (See [qdep - Installing](https://github.com/Skycoder42/qdep#installation))
2. Add the following to your pro file:
```qmake
QDEP_DEPENDS += Skycoder42/AndroidContentDevice
!load(qdep):error("Failed to load qdep feature! Run 'qdep prfgen --qmake $$QMAKE_QMAKE' to create it.")
```

## Usage
The ContentDevice can be used like any other QIODevice. Please note that it needs an Android Context in order to function properly. By default, `QtAndroid::androidContext()` is used. In addition to that, the device is a sequential device, which means you can only access it as "Stream", not as Random access file.

Example:
```cpp
ContentDevice device(QUrl("content://some/content/file"));
if(device.open(QIODevice::ReadOnly)) {
	qDebug() << device.readAll();
	device.close();
} else
	qCritical() << device.errorString();
```
