#pragma once

#include <QtWaylandClient/QtWaylandClient>
#include <QtQml/QQmlEngine>

class ForeignToplevelManagerV1Private;
class ForeignToplevelHandleV1Private;
class ForeignToplevelHandleV1;

class Q_WAYLANDCLIENT_EXPORT ForeignToplevelManagerV1 
	: public QObject {
	Q_OBJECT
	Q_PROPERTY(QList<ForeignToplevelHandleV1*> toplevels READ toplevels NOTIFY toplevelsChanged);
public:
	ForeignToplevelManagerV1();
	QList<ForeignToplevelHandleV1*> toplevels();
	ForeignToplevelHandleV1 *handleFromObject(void *object);

signals:
	void toplevelsChanged(QList<ForeignToplevelHandleV1*> toplevels);

private:
	ForeignToplevelManagerV1Private *d;
};

class Q_WAYLANDCLIENT_EXPORT ForeignToplevelHandleV1 
	: public QObject {
	Q_OBJECT
	Q_PROPERTY(QString appId READ appId NOTIFY appIdChanged);
	Q_PROPERTY(QString title READ title NOTIFY titleChanged);
	Q_PROPERTY(QSet<State> state READ state WRITE setState NOTIFY stateChanged);
public:
	enum State {
		Maximized = 0, // the toplevel is maximized
		Minimized = 1, // the toplevel is minimized
		Activated = 2, // the toplevel is active
		Fullscreen = 3, // the toplevel is fullscreen
	};
	Q_ENUM(State)

	ForeignToplevelHandleV1(ForeignToplevelManagerV1Private *manager, void *object);
	QString appId();
	QString title();
	QSet<State> state();
	void setState(QSet<State> state);
	Q_INVOKABLE void close();
	void *object();
	ForeignToplevelManagerV1 *manager();

signals:
	void appIdChanged(QString appId);
	void titleChanged(QString title);
	void stateChanged(QSet<State> state);

private:
	ForeignToplevelHandleV1Private *d;
};
