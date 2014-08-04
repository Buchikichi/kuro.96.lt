<?php
require_once('inc/dba/QuestionDao.inc');
require_once('inc/dba/AnswerDao.inc');
header('Content-type: text/html; charset=UTF-8');

/**
 * 問題更新.
 * @author H.Sasai
 */
class Processor extends ProcessorBase {
	function getParamNames() {
		return array('ticket', 'catID');
	}
	function makeData() {
		$filename = APP_ROOT.'/txt/beer.txt';
		if (!is_file($filename)) {
			$this->cause = $filename;
			return null;
		}
		$catID = $_REQUEST['catID'];
		if (empty($catID)) {
			$this->cause = 'Bad catID.';
			return null;
		}
		$questionDao = new QuestionDao();
		$answerDao = new AnswerDao();
		$lines = file($filename);
		$params = array();
		$content = '';
		$answer = array();
		$phase = 0;
		$questionDao->beginTransaction();
		$questionDao->delete($catID);
		$numOfQuestions = 0;
		foreach ($lines as $line_num => $line) {
			$ch = substr($line, 0, 1);
			if ($ch == '-') {
				if (!empty($content)) {
					$qID = $questionDao->getUUID();
					$params = array(
						'qID'=>$qID,
						'catID'=>$catID,
						'content'=>$content,
					);
					$numOfQuestions++;
					$questionDao->insert($params);
					foreach ($answer as $seq => $answerContent) {
						$params = array(
							'qID'=>$qID,
							'seq'=>$seq,
							'content'=>$answerContent,
						);
						$answerDao->insert($params);
					}
				}
				$content = '';
				$answer = array();
				$phase = 0;
				continue;
			}
			if ($ch == '/') {
				$phase = 1;
				continue;
			}
			if ($phase == 0) {
				$content .= $line;
			} else if ($phase == 1) {
				$answer[] = $line;
			}
		}
		$questionDao->commit();
		$now = date('Y-m-d H:i:s');
		return array(
			'count'=>$numOfQuestions,
			'sysDate'=>$now,
		);
	}
	function success($result) {
		return $result;
	}
}
?>
