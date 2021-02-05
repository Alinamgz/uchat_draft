#include "client.h"

static void activate (GtkApplication *app, gpointer user_data) {
	GtkWidget *window;
	GtkWidget *button;
	GtkWidget *button_box;

	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Window");
	gtk_window_set_default_size (GTK_WINDOW(window), 200, 200);

	gtk_widget_show_all(window);
}

int mx_init_gtk_app(void) {
	GtkApplication *uchat_client;
	int status = 0;

	uchat_client = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (uchat_client, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(uchat_client), 0, NULL);
	g_object_unref(uchat_client);

	return status;
}
