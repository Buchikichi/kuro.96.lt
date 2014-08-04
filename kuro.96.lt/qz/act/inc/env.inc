<?php
	define('APP_ROOT', dirname(dirname(__FILE__)));

	if (isset($_SERVER['SERVER_NAME'])) {
		define('SERVER_NAME', $_SERVER['SERVER_NAME']);
	} else if (isset($_SERVER['USER'])) {
		define('SERVER_NAME', $_SERVER['USER']);
	} else {
		// windows
		define('SERVER_NAME', strtolower($_SERVER['COMPUTERNAME']));
	}
	if (strpos(SERVER_NAME, 'kuro.96.lt') !== false) {
		define('IS_KURO', true);
	}
	date_default_timezone_set('Asia/Tokyo');
	mb_language('ja');
	mb_internal_encoding('UTF-8');
	error_reporting(E_ALL);
?>

