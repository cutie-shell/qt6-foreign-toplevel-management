#pragma once

#include <qt6-foreign-toplevel-management.h>

#include <wayland-util.h>
#include <QtWaylandClient/QWaylandClientExtensionTemplate>
#include <QtWaylandClient/private/qwaylanddisplay_p.h>
#include <QtWaylandClient/private/qwaylandinputdevice_p.h>
#include <QtWaylandClient/private/qwaylandscreen_p.h>
#include "qwayland-wlr-foreign-toplevel-management-unstable-v1.h"

class ForeignToplevelManagerV1Private
	: public QWaylandClientExtensionTemplate<ForeignToplevelManagerV1Private>
	, public QtWayland::zwlr_foreign_toplevel_manager_v1 {
	Q_OBJECT
public:
	ForeignToplevelManagerV1Private(ForeignToplevelManagerV1 *q);
	QList<ForeignToplevelHandleV1*> m_toplevels;

protected:
	void zwlr_foreign_toplevel_manager_v1_toplevel(
		struct ::zwlr_foreign_toplevel_handle_v1 *toplevel) override;

private:
	ForeignToplevelManagerV1 *q;
};

class ForeignToplevelHandleV1Private
	: public QWaylandClientExtensionTemplate<ForeignToplevelHandleV1Private>
	, public QtWayland::zwlr_foreign_toplevel_handle_v1 {
	Q_OBJECT
public:
	ForeignToplevelHandleV1Private(
		ForeignToplevelHandleV1 *q, 
		ForeignToplevelManagerV1Private *manager,
		struct ::zwlr_foreign_toplevel_handle_v1 *object);
	
	QString m_appId;
	QString m_title;
	QSet<ForeignToplevelHandleV1::State> m_state;
	ForeignToplevelManagerV1Private *m_manager;

protected:
	void zwlr_foreign_toplevel_handle_v1_app_id(const QString &appId) override;
	void zwlr_foreign_toplevel_handle_v1_title(const QString &title) override;
	void zwlr_foreign_toplevel_handle_v1_state(wl_array *state) override;
	void zwlr_foreign_toplevel_handle_v1_closed() override;

private:
	ForeignToplevelHandleV1 *q;
};
