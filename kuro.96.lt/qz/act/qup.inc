<?php
require_once('inc/dba/CategoryDao.inc');
require_once('inc/dba/QuestionDao.inc');
require_once('inc/dba/AnswerDao.inc');
header('Content-type: text/html; charset=UTF-8');

/**
 * 問題更新.
 * @author H.Sasai
 */
class Processor extends ProcessorBase {
	var $catDao;
	var $questionDao;

	function getParamNames() {
		return array('ticket', 'catID');
	}

	function updateQuestions($filename, $defaultCatID) {
		$catID = $defaultCatID;
		$answerDao = new AnswerDao();
		$lines = file($filename);
		$params = array();
		$content = '';
		$answer = array();
		$phase = 0;
		$numOfQuestions = 0;
		foreach ($lines as $line_num => $line) {
			$ch = substr($line, 0, 1);
			if ($ch == '@') {
				$catName = substr($line, 1);
				$catName = str_replace(array("\r\n","\n","\r"), '', $catName);
				$catID = $this->catDao->getCatID($catName);
			}
			if ($ch == '-') {
				if (!empty($content)) {
					$qID = $this->questionDao->getUUID();
					$params = array(
							'qID'=>$qID,
							'catID'=>$catID,
							'content'=>$content,
					);
					$numOfQuestions++;
					$this->questionDao->insert($params);
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
		return $numOfQuestions;
	}

	function makeData() {
		$catID = $_REQUEST['catID'];
		if (empty($catID)) {
			$this->cause = 'Bad catID.';
			return null;
		}
		$this->catDao = new CategoryDao();
		$this->questionDao = new QuestionDao();
		$this->questionDao->beginTransaction();
		$this->questionDao->delete($catID);
		$fileList = array();
		$dir = APP_ROOT.'/txt/';
		$files = scandir($dir);
		foreach ($files as $filename) {
			if (strpos($filename, '.txt') === false) {
				continue;
			}
			$cnt = $this->updateQuestions($dir.$filename, $catID);
			$fileList[] = array('file'=>$filename, 'count'=>$cnt);
		}
		$this->questionDao->commit();
		$now = date('Y-m-d H:i:s');
		return array(
			'files'=>$fileList,
			'sysDate'=>$now,
		);
	}

	function success($result) {
		return $result;
	}
}
?>
