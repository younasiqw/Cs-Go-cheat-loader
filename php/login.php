<?php
define('IN_MYBB', NULL);
global $mybb, $lang, $query, $db, $cache, $plugins, $displaygroupfields;
require_once 'global.php';
require_once 'class.MyBBIntegrator.php';
$MyBBI = new MyBBIntegrator($mybb, $db, $cache, $plugins, $lang, $config); 

$action = $_GET['action'];

$user = ($_GET['username']);
$password = ($_GET['password']);

$key = 'aes key'; // 32 bytes

$iv  = 'aes iv'; // 16 bytes

$method = 'aes-256-cfb';

date_default_timezone_set("UTC");
$curtime = date('H');

if(!$action)
{
    echo "Please enter an action.";
}
else
{    
	if($action == "login")
	{
		$login_status = $MyBBI->login($user, $password);
		
		if ($login_status == true)
		{	
			$curtime .="csgo";
		}
		else
		{
			echo "error";
		}
	}
	else
	{
		echo "Invalid action.";
	}
}
$encryptedTime = base64_encode( openssl_encrypt ($curtime, $method, $key, true, $iv));
echo $encryptedTime;
?>