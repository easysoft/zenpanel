<?php
$config->installed       = true;
$config->debug           = false;
$config->requestType     = 'PATH_INFO';
$config->db->host        = '127.0.0.1';
$config->db->port        = '%MYSQL_PORT%';
$config->db->user        = '%MYSQL_ACCOUNT%';
$config->db->prefix      = 'zt_';
$config->webRoot         = getWebRoot();

$config->db->name      = 'zentao';
$config->db->password  = '%MYSQL_PASSWORD%';
$config->default->lang = 'zh-cn';
