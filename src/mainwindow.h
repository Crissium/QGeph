#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QLabel>
#include <QTimer>
#include "toolbar.h"
#include "gephprocess.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

protected:
	void closeEvent(QCloseEvent *event) override;

private:
	Ui::MainWindow *ui;
	QLabel connectionInfoLabel;
	QNetworkAccessManager * manager;
	QNetworkReply * reply;
	QTimer timer;

	GephProcess geph;

	void readSettings();
	void writeSettings();

	void createStatusBar();

private slots:
	void toggleConnection(bool toggled);
	void syncInfo();
	void syncStats();
	void syncRegularly(bool connected);
};
#endif // MAINWINDOW_H
