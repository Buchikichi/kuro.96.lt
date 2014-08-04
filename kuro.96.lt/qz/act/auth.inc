<?php
require_once('inc/dba/UserDao.inc');
header('Content-type: text/html; charset=UTF-8');

/**
 * 認証.
 * @author H.Sasai
 */
class Processor extends ProcessorBase {
	function getParamNames() {
		return array('user', 'pass');
	}
	function makeData() {
		$user = $_REQUEST['user'];
		$pass = md5($_REQUEST['pass']);
		$dao = new UserDao();
		$rec = $dao->getRecord($user, $pass);
		if ($rec == null) {
			$this->cause = 'Bad user.';
			return null;
		}
		$now = date('Y-m-d H:i:s');
		return array(
			'ticket'=>$rec['userID'],
			'SERVER_NAME'=>SERVER_NAME,
			'sysDate'=>$now,
		);
	}
	function success($result) {
		return $result;
	}
}
?>
