#include "client.h"

bool mx_get_input_values(t_client *client, t_raw_inputs *inputs, t_scene type){
    char *err = NULL;

    if (type == LOGIN){
        inputs->username = mx_get_n_check_entry(&err,
                               TRUE,
                               client->ui->l_username_entry,
                               client->ui->l_username_status);
        if (err[0])
            return 1;

        inputs->password = mx_get_n_check_entry(&err,
                               TRUE,
                               client->ui->l_pass_entry,
                               client->ui->l_pass_status);
        if (err[0])
            return 1;
    }
    else if (type == REGISTRATION){
        inputs->username = mx_get_n_check_entry(&err,
                               TRUE,
                               client->ui->r_username_entry,
                               client->ui->r_username_status);
        if (err[0])
            return 1;

        inputs->password = mx_get_n_check_entry(&err,
                               TRUE,
                               client->ui->r_pass_entry,
                               client->ui->r_pass_status);
        if (err[0])
            return 1;

        inputs->conf_password = mx_get_n_check_entry(&err,
                               TRUE,
                               client->ui->conf_pass_entry,
                               client->ui->conf_pass_status);
        if (err[0])
            return 1;

        inputs->first_name = mx_get_n_check_entry(&err,
                               FALSE,
                               client->ui->firstname_entry,
                               client->ui->firstname_status);
        if (err[0])
            return 1;

        inputs->last_name = mx_get_n_check_entry(&err,
                               FALSE,
                               client->ui->lastname_entry,
                               client->ui->lastname_status);
        if (err[0])
            return 1;
    }
    return 0;
}


