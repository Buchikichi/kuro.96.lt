<?php
require_once('DaoBase.inc');

/**
 * User情報を管理.
 * @author H.Sasai
 */
class UserDao extends DaoBase {
	/**
	 * Userを取得.
	 */
	function getRecord($user, $pass) {
		$param = array('name'=>$user, 'hash'=>$pass);
		$query = <<<EOD
SELECT
 *
FROM
  `user`
WHERE
  name = :name
  AND hash = :hash
EOD;
		return $this->selectOne($query, $param);
	}
}
?>
