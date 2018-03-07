# AndroidContentDevice
A QIODevice implementation to be able to open "content://" urls on android

## Features
When handling files on Android and using the modern "file chooser" API (`Intent.createChooser`), you are typically getting a content-URL returned. These are special URLs starting with `content://...` - they can be normal files, but often are references to resources of specific content providers that may not be locally available (A good example is a could service like DropBox or SeaFile).

With this class you can open surch URLs for reading or writing and can thus access such "files" even if they are not locally present. Please note that the class basicly wrapps the Java API, and since it can open Network-Files, too, it may be rather slow for bigger files.

**Note:** If you need a way to get such a content URL from C++ easily, have a look at my [QtMvvm Project](https://github.com/Skycoder42/QtMvvm). It provides such a file picker as part of the Framework. If you only need the file chooser itself, have a look at the [`AndroidFileChooser`](https://github.com/Skycoder42/QtMvvm/blob/master/src/imports/mvvmquick/androidfilechooser.h) class.

## Installation
The package is providet as qpm  package, [`de.skycoder42.android-contentdevice`](https://www.qpm.io/packages/de.skycoder42.android-contentdevice/index.html). You can install it either via qpmx (preferred) or directly via qpm.

### Via qpmx
[qpmx](https://github.com/Skycoder42/qpmx) is a frontend for qpm (and other tools) with additional features, and is the preferred way to install packages. To use it:

1. Install qpmx (See [GitHub - Installation](https://github.com/Skycoder42/qpmx#installation))
2. Install qpm (See [GitHub - Installing](https://github.com/Cutehacks/qpm/blob/master/README.md#installing), for **windows** see below)
3. In your projects root directory, run `qpmx install de.skycoder42.android-contentdevice`

### Via qpm

1. Install qpm (See [GitHub - Installing](https://github.com/Cutehacks/qpm/blob/master/README.md#installing), for **windows** see below)
2. In your projects root directory, run `qpm install de.skycoder42.android-contentdevice`
3. Include qpm to your project by adding `include(vendor/vendor.pri)` to your `.pro` file

Check their [GitHub - Usage for App Developers](https://github.com/Cutehacks/qpm/blob/master/README.md#usage-for-app-developers) to learn more about qpm.

**Important for Windows users:** QPM Version *0.10.0* (the one you can download on the website) is currently broken on windows! It's already fixed in master, but not released yet. Until a newer versions gets released, you can download the latest dev build from here:
- https://storage.googleapis.com/www.qpm.io/download/latest/windows_amd64/qpm.exe
- https://storage.googleapis.com/www.qpm.io/download/latest/windows_386/qpm.exe

## Usage
The ContentDevice can be used like any other QIODevice. Please note that it needs an Android Context in order to function properly. By default, QtAndroid::androidContext() is used. In addition to that, the device is a sequential device, which means you can only access it as "Stream", not as Random access file.

Example:
```cpp
ContentDevice device(QUrl("content://some/content/file"));
if(device.open(QIODevice::ReadOnly)) {
	qDebug() << device.readAll();
	device.close();
} else
	qCritical() << device.errorString();
```