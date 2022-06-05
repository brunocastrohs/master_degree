<?php
require_once($_SERVER['DOCUMENT_ROOT']."/GeoCod/classes/util/DatabaseUtil.class.inc");

class GenericDAO {

	static function retrieveLatLong($param){

		$sql = "SELECT st_x(st_centroid(geom)),st_y(st_centroid(geom)) FROM phonetic_via  WHERE name = '$param' LIMIT 1";

		$rs = DatabaseUtil::execute($sql);

		$list = array();

		while($row = pg_fetch_row($rs)){
				
			$list[] = $row[0];
			$list[] = $row[1];
				
		}

		return $list;

	}

	static function retrieveGISRecords($param){

		$cols = array('name');

		$params = explode(' ', $param);

		$param = '';

		$isFirst = true;

		foreach ($params as $word)
		if($isFirst){
			$param .= " name like '%$word%' ";
			$isFirst = false;
		}
		else
		$param .= " AND name like '%$word%' ";

		$sql = "SELECT DISTINCT name FROM phonetic_via  WHERE $param ORDER BY name";

		$rs = DatabaseUtil::execute($sql);

		$flag = 0;

		$list = array();

		while($row = pg_fetch_row($rs)){
			for($c = 0; $c != count($cols); $c++){
				$list[$flag][$c]["column"] 	= 	$cols[$c] ;
				$list[$flag][$c]['value'] 	= 	$row[$c];
			}
			$flag ++;
		}

		return $list;

	}

	static function retrievePhoneGISRecords($param){

		$cols = array('name');

		$params = explode(' ', $param);

		$param = '';

		$isFirst = true;

		foreach ($params as $word)
		if($isFirst){
			$param .= " phonetic_key like '%".soundex($word)."%' ";
			$isFirst = false;
		}
		else
		$param .= " AND phonetic_key like '%".soundex($word)."%' ";

		$sql = "SELECT distinct name FROM phonetic_via WHERE $param ORDER BY name ";

		$rs = DatabaseUtil::execute($sql);

		$flag = 0;

		$list = array();

		while($row = pg_fetch_row($rs)){
			for($c = 0; $c != count($cols); $c++){
				$list[$flag][$c]["column"] 	= 	$cols[$c] ;
				$list[$flag][$c]['value'] = $row[$c];
			}
			$flag ++;
		}

		return $list;

	}

	static function retrieveLevenshteinGISRecords($param){

		$cols = array('name');

		$params = explode(' ', $param);

		$param = '';

		$isFirst = true;

		foreach ($params as $word)
		if($isFirst){
			$param .= "beta_levenshtein('$word',name)";
			$isFirst = false;
		}
		else
		$param .= " AND beta_levenshtein('$word',name)";

		$sql = "SELECT distinct name FROM phonetic_via WHERE $param ORDER BY name";

		$rs = DatabaseUtil::execute($sql);

		$flag = 0;

		$list = array();

		while($row = pg_fetch_row($rs)){
			for($c = 0; $c != count($cols); $c++){
				$list[$flag][$c]["column"] 	= 	$cols[$c] ;
				$list[$flag][$c]['value'] = $row[$c];
			}
			$flag ++;
		}

		return $list;

	}


}
?>