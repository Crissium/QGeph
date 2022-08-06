#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QCloseEvent>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <cstdlib>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, connectionInfoLabel(this)
	, manager(this)
	, timer(this)
{
	ui->setupUi(this);

	readSettings();
	createStatusBar();

	connect(ui->toolbar, &Toolbar::connectButtonToggled, this, &MainWindow::toggleConnection);
	connect(ui->toolbar, &Toolbar::connectButtonToggled, this, &MainWindow::syncRegularly);
	connect(ui->toolbar, &Toolbar::syncButtonClicked, this, &MainWindow::syncInfo);
	connect(ui->toolbar, &Toolbar::aboutButtonClicked, this, [this] ()
	{QMessageBox::about(this, "About QGeph", "A simple Qt-based third-party Geph GUI");});
	connect(ui->binarySelectionButton, &QPushButton::clicked, this, [this] ()
	{
		auto fileName = QFileDialog::getOpenFileName(this, "Select Geph Executable", QDir::homePath());
		if (!fileName.isEmpty())
		{
			ui->binaryPathEdit->setText(fileName);
		}
	});
	connect(ui->binaryPathEdit, &QLineEdit::textChanged, this, [this] ()
	{
		GephProcess::BinaryPath = ui->binaryPathEdit->text();
	});
	connect(ui->logFileSelectionButton, &QPushButton::clicked, this, [this] ()
	{
		auto fileName = QFileDialog::getSaveFileName(this, "Save log to", QDir::homePath(), "Logs (*.log)");
		if (!fileName.isEmpty())
		{
			ui->logEdit->setText(fileName);
		}
	});
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::closeEvent(QCloseEvent * event)
{
	writeSettings();
	event->accept();
}

void MainWindow::readSettings()
{
	QSettings settings("Geph", "QGeph");

	settings.beginGroup("mainwindow");
	resize(settings.value("size").toSize());
	if (settings.value("fullscreen").toBool())
		showFullScreen();
	settings.endGroup();

	settings.beginGroup("geph");
	GephProcess::BinaryPath = settings.value("binary").toString();
	ui->binaryPathEdit->setText(settings.value("binary").toString());
	ui->usernameEdit->setText(settings.value("username").toString());
	ui->passwordEdit->setText(settings.value("password").toString());
	ui->subscriptionInfo->setText(settings.value("subscription").toString());
	ui->tcpCheckBox->setChecked(settings.value("tcp").toBool());
	ui->bridgesCheckBox->setChecked(settings.value("bridges").toBool());
	ui->excludeCheckBox->setChecked(settings.value("exclude").toBool());
	ui->httpEdit->setText(settings.value("http").toString());
	ui->socksEdit->setText(settings.value("socks").toString());
	ui->logEdit->setText(settings.value("log").toString());
	settings.endGroup();

	if (!GephProcess::BinaryPath.isEmpty() && !ui->usernameEdit->text().isEmpty() && !ui->passwordEdit->text().isEmpty())
		syncInfo();
}

void MainWindow::writeSettings()
{
	QSettings settings("Geph", "QGeph");

	settings.beginGroup("mainwindow");
	settings.setValue("size", size());
	settings.setValue("fullScreen", isFullScreen());
	settings.endGroup();

	settings.beginGroup("geph");
	settings.setValue("binary", ui->binaryPathEdit->text());
	settings.setValue("username", ui->usernameEdit->text());
	settings.setValue("password", ui->passwordEdit->text());
	settings.setValue("subscription", ui->subscriptionInfo->text());
	settings.setValue("tcp", ui->tcpCheckBox->isChecked());
	settings.setValue("bridges", ui->bridgesCheckBox->isChecked());
	settings.setValue("exclude", ui->excludeCheckBox->isChecked());
	settings.setValue("http", ui->httpEdit->text());
	settings.setValue("socks", ui->socksEdit->text());
	settings.setValue("log", ui->logEdit->text());
	settings.endGroup();
}

void MainWindow::createStatusBar()
{
	connectionInfoLabel.setText("Disconnected");
	statusBar()->addPermanentWidget(&connectionInfoLabel);

	connect(&timer, &QTimer::timeout, this, &MainWindow::syncStats);
}

void MainWindow::toggleConnection(bool toggled)
{
	if (toggled)
	{
		ConnectArgs args;
		args.excludePrc = ui->excludeCheckBox->isChecked();
		args.useTcp = ui->tcpCheckBox->isChecked();
		args.useBridges = ui->bridgesCheckBox->isChecked();
		args.server = ui->serverSelectionBox->currentText();
		args.httpListen = ui->httpEdit->text();
		args.socksListen = ui->socksEdit->text();
		args.username = ui->usernameEdit->text();
		args.password = ui->passwordEdit->text();
		args.logFilePath = ui->logEdit->text();

		geph.establishConnection(args);
	}
	else
		system("killall geph4-client");
}

void MainWindow::syncInfo()
{
	auto const info = geph.sync({ui->usernameEdit->text(), ui->passwordEdit->text()});

	ui->subscriptionInfo->setText("Plan: " + info.plan + "\nExpires at: " + info.expirationTime.toString());
	ui->serverSelectionBox->addItems(info.servers);
}

void MainWindow::syncStats()
{
	manager.get(QNetworkRequest(QUrl("http://127.0.0.1:9809")));
	connect(&manager, &QNetworkAccessManager::finished, this, [this] (QNetworkReply * reply)
	{
		auto const doc = QJsonDocument::fromJson(reply->readAll());

		connectionInfoLabel.setText(QString("%1ms %2%").arg(doc.object().value("latency").toDouble())
									.arg(doc.object().value("loss").toDouble() * 100.));
	});
}

void MainWindow::syncRegularly(bool connected)
{
	if (connected)
	{
		timer.start(3000);
	}
	else
	{
		timer.stop();
		connectionInfoLabel.setText("Disconnected");
	}
}
