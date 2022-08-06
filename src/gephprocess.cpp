#include "gephprocess.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

QString GephProcess::BinaryPath;

GephProcess::GephProcess()
{
	setProgram(BinaryPath);
}

void GephProcess::establishConnection(const ConnectArgs & options)
{
	QStringList args("connect");

	if (options.excludePrc)
		args << "--exclude-prc";
	if (options.useBridges)
		args << "--use-bridges";
	if (options.useTcp)
		args << "--use-tcp";

	args << "--exit-server" << options.server;

	if (!options.httpListen.isEmpty())
		args << "--http-listen" << options.httpListen;

	if (!options.socksListen.isEmpty())
		args << "--socks5-listen" << options.socksListen;

	args << "--username" << options.username
		 << "--password" << options.password;

	if (!options.logFilePath.isEmpty())
		args << "--log-file" << options.logFilePath;

	setArguments(args);
	start();
}

SyncedInfo GephProcess::sync(const SyncArgs & options)
{
	setArguments(QStringList() << "--username" << options.username << "--password" << options.password);
	setReadChannel(QProcess::StandardOutput);

	start();
	waitForFinished();

	auto doc = QJsonDocument::fromJson(readAll());
	SyncedInfo info;

	info.plan = doc.array().at(0).toObject().value("subscription").toObject().value("level").toString();
	info.expirationTime.setSecsSinceEpoch(doc.array().at(0).toObject().value("subscription").toObject().value("expires_unix").toInteger());

	for (auto const & server : doc.array().at(1).toArray())
		info.servers.push_back(server.toObject().value("country_code").toString() + "-" + server.toObject().value("city_code").toString());

	return info;
}
