<?php
require_once('inc/env.inc');
require_once('inc/logger.inc');

function execute() {
	if (!isset($_REQUEST['act'])) {
		return false;
	}
	session_start();
	$act = $_REQUEST['act'];
	$path = __DIR__.'/'.str_replace('.', '/', $act).'.inc';
	if (!is_file($path)) {
		return false;
	}
	unset($_REQUEST['act']);
	require_once('inc/processorBase.inc');
	require_once($path);
Logger::getInstance()->debug($_REQUEST);
	try {
		$proc = new Processor();
		$proc->execute();
	} catch (Exception $e) {
		Logger::getInstance()->debug(array('Exception'=>$e->getMessage()));
		echo json_encode(array('result' => 'NG',
			'cause' => $e->getMessage(),
			'line'=>$e->getFile().':'.$e->getLine()));
	}
	return true;
}
if (!execute()) {
	echo json_encode(array('result' => 'NG', 'cause' => 'Action not found.'));
	exit();
}
?>
