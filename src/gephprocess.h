#ifndef GEPHPROCESS_H
#define GEPHPROCESS_H

#include <QProcess>
#include <QDateTime>
#include <vector>

struct ConnectArgs
{
	bool excludePrc;
	bool useBridges;
	bool useTcp;

	QString server;
	QString httpListen;
	QString socksListen;

	QString username;
	QString password;

	QString logFilePath;
};

struct SyncArgs
{
	QString username;
	QString password;
};

class ServersInfo : public std::vector<QString>
{
public:
	ServersInfo() {}
};

struct SyncedInfo
{
	QString plan;
	QDateTime expirationTime;
	ServersInfo servers;
};

class GephProcess : public QProcess
{
	Q_OBJECT
public:
	static QString BinaryPath;

	GephProcess();

	/**
	 * no argument validity checking
	 */
	void establishConnection(const ConnectArgs &);
	SyncedInfo sync(const SyncArgs &);
};

#endif // GEPHPROCESS_H
