<?php
require_once('inc/dba/CategoryDao.inc');
header('Content-type: text/html; charset=UTF-8');

/**
 * 認証.
 * @author H.Sasai
 */
class Processor extends ProcessorBase {
	function getParamNames() {
		return array('ticket');
	}
	function makeData() {
		$dao = new CategoryDao();
		$param = array(
			'from'=>0,
			'to'=>100,
		);
		$list = $dao->getList($param);
		$now = date('Y-m-d H:i:s');
		return array(
			'list'=>$list,
			'sysDate'=>$now,
		);
	}
	function success($result) {
		return $result;
	}
}
?>
