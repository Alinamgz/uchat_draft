#include "client.h"

void mx_clear_buffer_text(gchar *buff_name, GtkBuilder *builder) {
    GObject *buffer = gtk_builder_get_object(builder, buff_name);
    GtkTextIter start;
    GtkTextIter end;

    if (GTK_IS_TEXT_BUFFER(buffer)) {
        gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(buffer), &start);
        gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(buffer), &end);
        gtk_text_buffer_delete(GTK_TEXT_BUFFER(buffer), &start, &end);
    }
    else
        gtk_entry_buffer_delete_text(GTK_ENTRY_BUFFER(buffer), 0, -1);
}

gchar *mx_get_buffer_text(gchar *buff_name, t_client *client) {
    GObject *buffer = gtk_builder_get_object(client->ui->builder, buff_name);
    GtkTextIter start;
    GtkTextIter end;

    if (GTK_IS_TEXT_BUFFER(buffer)) {
        gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(buffer), &start);
        gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(buffer), &end);
        return gtk_text_buffer_get_text(GTK_TEXT_BUFFER(buffer), &start, &end, FALSE);
    }
    else
        return (gchar*)gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(buffer));
}

gchar *mx_get_text_from_buffer(GtkTextBuffer *buffer) {
    GtkTextIter start;
    GtkTextIter end;

    gtk_text_buffer_get_start_iter(GTK_TEXT_BUFFER(buffer), &start);
    gtk_text_buffer_get_end_iter(GTK_TEXT_BUFFER(buffer), &end);
    return gtk_text_buffer_get_text(GTK_TEXT_BUFFER(buffer),
                                    &start, &end, FALSE);
}
