#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolButton>
#include <QIcon>
#include <QToolBar>

/**
 * Start/stop, change icon & text upon toggling
 */
class ConnectButton : public QToolButton
{
	Q_OBJECT
private:
	QIcon icons[2];
	QString texts[2];

public:
	ConnectButton(QWidget * parent = nullptr);
};

class SyncButton : public QToolButton
{
	Q_OBJECT
public:
	SyncButton(QWidget * parent = nullptr);
};

class AboutButton : public QToolButton
{
	Q_OBJECT
public:
	AboutButton(QWidget * parent = nullptr);
};

class Toolbar : public QToolBar
{
	Q_OBJECT
private:
	ConnectButton connectButton;
	SyncButton syncButton;
	AboutButton aboutButton;

signals:
	void connectButtonToggled(bool);
	void syncButtonClicked(bool);
	void aboutButtonClicked(bool);

public:
	Toolbar(QWidget * parent = nullptr);
};

#endif // TOOLBAR_H
