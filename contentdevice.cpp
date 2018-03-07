#include "contentdevice.h"
#include <QtAndroidExtras/QtAndroid>

#define TEST_OPEN_VALID(env) do { \
	if(checkJniError(env)) { \
		QIODevice::close(); \
		return false; \
	} \
} while(false)

ContentDevice::ContentDevice(QObject *parent) :
	QIODevice(parent),
	_context(QtAndroid::androidContext()),
	_url()
{
	Q_ASSERT(_context.isValid());
}

ContentDevice::ContentDevice(const QUrl &url, QObject *parent) :
	QIODevice(parent),
	_context(QtAndroid::androidContext()),
	_url(url)
{
	Q_ASSERT(_context.isValid());
}

ContentDevice::ContentDevice(const QAndroidJniObject &context, const QUrl &url, QObject *parent) :
	QIODevice(parent),
	_context(context),
	_url(url)
{
	Q_ASSERT(_context.isValid());
}

void ContentDevice::setContext(const QAndroidJniObject &context)
{
	_context = context;
	Q_ASSERT(_context.isValid());
}

bool ContentDevice::isSequential() const
{
	return true;
}

bool ContentDevice::open(QIODevice::OpenMode mode)
{
	if(!QIODevice::open(mode))
		return false;

	QAndroidJniEnvironment env;
	auto uri = QAndroidJniObject::callStaticObjectMethod("android/net/Uri",
														 "parse",
														 "(Ljava/lang/String;)Landroid/net/Uri;",
														 QAndroidJniObject::fromString(_url.toString(QUrl::FullyEncoded)).object());
	TEST_OPEN_VALID(env);
	if(!uri.isValid()) {
		setErrorString(tr("URL is not valid"));
		QIODevice::close();
		return false;
	}

	auto contentResolver = _context.callObjectMethod("getContentResolver", "()Landroid/content/ContentResolver;");
	TEST_OPEN_VALID(env);
	if(!contentResolver.isValid()) {
		setErrorString(tr("Unable to get content resolver from context"));
		QIODevice::close();
		return false;
	}

	switch(mode) {
	case QIODevice::ReadOnly:
		_stream = contentResolver.callObjectMethod("openInputStream",
												   "(Landroid/net/Uri;)Ljava/io/InputStream;",
												   uri.object());
		break;
	case QIODevice::WriteOnly:
		_stream = contentResolver.callObjectMethod("openOutputStream",
												   "(Landroid/net/Uri;)Ljava/io/OutputStream;",
												   uri.object());
		break;
	default:
		setErrorString(tr("You can only open ContentDevice with QIODevice::ReadOnly OR QIODevice::WriteOnly. Other flags are not supported"));
		QIODevice::close();
		return false;
	}

	TEST_OPEN_VALID(env);
	if(!_stream.isValid()) {
		setErrorString(tr("Unable to get content stream from content resolver"));
		QIODevice::close();
		return false;
	}

	return true;
}

void ContentDevice::close()
{
	if(_stream.isValid()) {
		_stream.callMethod<void>("close");
		_stream = QAndroidJniObject();
	}
	QIODevice::close();
}

qint64 ContentDevice::bytesAvailable() const
{
	if(openMode().testFlag(QIODevice::ReadOnly) && _stream.isValid())
		return (qint64)_stream.callMethod<jint>("available");
	else
		return -1;
}

void ContentDevice::flush()
{
	if(openMode().testFlag(QIODevice::WriteOnly) && _stream.isValid())
		_stream.callMethod<void>("flush");
}

QUrl ContentDevice::url() const
{
	return _url;
}

void ContentDevice::setUrl(QUrl url)
{
	if (_url == url)
		return;

	_url = url;
	emit urlChanged(url);
}

qint64 ContentDevice::readData(char *data, qint64 maxlen)
{
	QAndroidJniEnvironment env;
	auto array = QAndroidJniObject::fromLocalRef(env->NewByteArray((jsize)maxlen));
	auto cnt = _stream.callMethod<jint>("read", "([B)I", array.object());
	if(checkJniError(env))
		return -1;
	if(cnt > 0)
		env->GetByteArrayRegion(array.object<jbyteArray>(), 0, cnt, reinterpret_cast<jbyte*>(data));
	return static_cast<qint64>(cnt);
}

qint64 ContentDevice::writeData(const char *data, qint64 len)
{
	QAndroidJniEnvironment env;
	auto array = QAndroidJniObject::fromLocalRef(env->NewByteArray((jsize)len));
	env->SetByteArrayRegion(array.object<jbyteArray>(), 0, len, reinterpret_cast<const jbyte*>(data));
	_stream.callMethod<void>("write", "([B)V", array.object());
	if(checkJniError(env))
		return -1;
	else
		return len;
}

bool ContentDevice::checkJniError(QAndroidJniEnvironment &env)
{
	if(env->ExceptionCheck()) {
		auto exception = QAndroidJniObject::fromLocalRef(env->ExceptionOccurred());
		setErrorString(exception.callObjectMethod("getLocalizedMessage", "()Ljava/lang/String;").toString());
		env->ExceptionClear();
		return true;
	} else
		return false;
}
