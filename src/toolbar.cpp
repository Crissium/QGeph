#include "toolbar.h"

ConnectButton::ConnectButton(QWidget * parent): QToolButton(parent), icons{QIcon::fromTheme("media-playback-start"), QIcon::fromTheme("media-playback-stop")}, texts{"Start", "Stop"}
{
	setCheckable(true);
	setText(texts[0]);
	setIcon(icons[0]);
	setToolTip("Start / stop");

	connect(this, &QToolButton::toggled, this, [this] (const bool checked)
	{setText(texts[checked]); setIcon(icons[checked]);});
}

SyncButton::SyncButton(QWidget * parent): QToolButton(parent)
{
	setText("Sync");
	setIcon(QIcon::fromTheme("emblem-synchronizing"));
	setToolTip("Sync");
}

AboutButton::AboutButton(QWidget * parent): QToolButton(parent)
{
	setText("About");
	setIcon(QIcon::fromTheme("help-about"));
	setToolTip("About");
}

Toolbar::Toolbar(QWidget * parent)
	: QToolBar(parent)
	, connectButton(this)
	, syncButton(this)
	, aboutButton(this)
{
	setFloatable(false);
	setMovable(false);
	setAllowedAreas(Qt::TopToolBarArea);
	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	addWidget(&connectButton);
	connect(&connectButton, &QToolButton::toggled, this, [this] (bool toggled) {emit connectButtonToggled(toggled);});

	addWidget(&syncButton);
	connect(&syncButton, &QToolButton::clicked, this, [this] (bool clicked) {emit syncButtonClicked(clicked);});

	addWidget(&aboutButton);
	connect(&aboutButton, &QToolButton::clicked, this, [this] (bool clicked) {emit aboutButtonClicked(clicked);});
}
