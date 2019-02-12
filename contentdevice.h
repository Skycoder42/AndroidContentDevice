#ifndef CONTENTDEVICE_H
#define CONTENTDEVICE_H

#include <QtCore/QIODevice>
#include <QtCore/QUrl>

#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QAndroidJniEnvironment>

class Q_ANDROID_CONTENT_DEVICE_EXPORT ContentDevice : public QIODevice
{
	Q_OBJECT

	Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)

public:
	explicit ContentDevice(QObject *parent = nullptr);
	explicit ContentDevice(const QUrl &url, QObject *parent = nullptr);
	explicit ContentDevice(const QAndroidJniObject &context, const QUrl &url, QObject *parent = nullptr);

	void setContext(const QAndroidJniObject &context);

	bool isSequential() const override;
	bool open(OpenMode mode) override;
	void close() override;
	qint64 bytesAvailable() const override;
	void flush();

	QUrl url() const;

public Q_SLOTS:
	void setUrl(QUrl url);

Q_SIGNALS:
	void urlChanged(QUrl url);

protected:
	qint64 readData(char *data, qint64 maxlen) override;
	qint64 writeData(const char *data, qint64 len) override;

private:
	QAndroidJniObject _context;
	QUrl _url;

	QAndroidJniObject _stream;

	bool checkJniError(QAndroidJniEnvironment &env);
};

#endif // CONTENTDEVICE_H
