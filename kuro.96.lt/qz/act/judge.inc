<?php
require_once('inc/dba/AnswerDao.inc');
header('Content-type: text/html; charset=UTF-8');

/**
 * 次の問題.
 * @author H.Sasai
 */
class Processor extends ProcessorBase {
	function getParamNames() {
		return array('ticket', 'qID', 'seq');
	}
	function makeData() {
		$qID = $_REQUEST['qID'];
		$seq = $_REQUEST['seq'];
		$answerDao = new AnswerDao();
		$answer = $answerDao->getAnswer($qID);
		$isOK = $answer['seq'] == $seq;
		$result = $isOK ? '○正解' : '×不正解';
		$content = $answer['content'];
		$content = str_replace("\\n", "<br/>", $content);
		$now = date('Y-m-d H:i:s');
		return array(
			'judge'=>$isOK,
			'result'=>$result,
			'answer'=>$content,
			'sysDate'=>$now,
		);
	}
	function success($result) {
		return $result;
	}
}
?>
