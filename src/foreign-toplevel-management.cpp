#include "foreign-toplevel-management_p.h"

ForeignToplevelManagerV1::ForeignToplevelManagerV1()
	: d(new ForeignToplevelManagerV1Private(this)) {}

ForeignToplevelManagerV1Private::ForeignToplevelManagerV1Private(
	ForeignToplevelManagerV1 *q
	) : QWaylandClientExtensionTemplate(3),
		q(q) {}

QList<ForeignToplevelHandleV1*> ForeignToplevelManagerV1::toplevels() {
	return d->m_toplevels;
}

void ForeignToplevelManagerV1Private::zwlr_foreign_toplevel_manager_v1_toplevel(
	struct ::zwlr_foreign_toplevel_handle_v1 *toplevel
) {
	m_toplevels.append(new ForeignToplevelHandleV1(this, toplevel));
	emit q->toplevelsChanged(m_toplevels);
}

ForeignToplevelHandleV1::ForeignToplevelHandleV1(ForeignToplevelManagerV1Private *manager, void *object)
	: d(new ForeignToplevelHandleV1Private(this, manager,
	  	(struct ::zwlr_foreign_toplevel_handle_v1 *)object)) {}

ForeignToplevelHandleV1Private::ForeignToplevelHandleV1Private(
	ForeignToplevelHandleV1 *q,
	ForeignToplevelManagerV1Private *manager,
	struct ::zwlr_foreign_toplevel_handle_v1 *object
) : QWaylandClientExtensionTemplate(3),
	QtWayland::zwlr_foreign_toplevel_handle_v1(object),
	q(q), m_manager(manager) {}

void ForeignToplevelHandleV1Private::zwlr_foreign_toplevel_handle_v1_app_id(const QString &appId) {
	m_appId = appId;
	emit q->appIdChanged(m_appId);
}

void ForeignToplevelHandleV1Private::zwlr_foreign_toplevel_handle_v1_title(const QString &title) {
	m_title = title;
	emit q->titleChanged(m_title);
}

void ForeignToplevelHandleV1Private::zwlr_foreign_toplevel_handle_v1_state(wl_array *state) {
	m_state.clear();
	for (ForeignToplevelHandleV1::State *s = (ForeignToplevelHandleV1::State *)state->data; 
		(char *)s < (char *)state->data + state->size;
		s++) {
		m_state += *s;
	}
	emit q->stateChanged(m_state);
}

void ForeignToplevelHandleV1Private::zwlr_foreign_toplevel_handle_v1_closed() {
	m_manager->m_toplevels.removeAll(q);
	delete q;
	delete this;
}

QString ForeignToplevelHandleV1::appId() {
	return d->m_appId;
}

QString ForeignToplevelHandleV1::title() {
	return d->m_title;
}

QSet<ForeignToplevelHandleV1::State> ForeignToplevelHandleV1::state() {
	return d->m_state;
}

void ForeignToplevelHandleV1::setState(QSet<ForeignToplevelHandleV1::State> state) {
	foreach (ForeignToplevelHandleV1::State s, d->m_state - state) {
		switch (s) {
			case ForeignToplevelHandleV1::State::Maximized: {
				d->unset_maximized();
			} break;
			case ForeignToplevelHandleV1::State::Minimized: {
				d->unset_minimized();
			} break;
			case ForeignToplevelHandleV1::State::Fullscreen: {
				d->unset_fullscreen();
			} break;
			default: break;
		}
	}

	foreach (ForeignToplevelHandleV1::State s, state - d->m_state) {
		switch (s) {
			case ForeignToplevelHandleV1::State::Maximized: {
				d->set_maximized();
			} break;
			case ForeignToplevelHandleV1::State::Minimized: {
				d->set_minimized();
			} break;
			case ForeignToplevelHandleV1::State::Activated: {
				d->activate(static_cast<QtWaylandClient::QWaylandScreen*>(
					qApp->screens().first()->handle())->display()->defaultInputDevice()->wl_seat());
			} break;
			case ForeignToplevelHandleV1::State::Fullscreen: {
				d->set_fullscreen(static_cast<QtWaylandClient::QWaylandScreen*>(
					qApp->screens().first()->handle())->output());
			} break;
			default: break;
		}
	}
}

void ForeignToplevelHandleV1::close() {
	d->close();
}
