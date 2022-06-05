<?php
class DateUtil{

	#fsfsfsfs
	static function dateToApp($value){

		if($value != ''){

			$date = explode('-', $value);

			return  "'".$date[2]."/".$date[1]."/".$date[0]."'";

		}

		return "null";
	}

	#jjjjj
	static function dateToDataBase($value){}
	
	static function reverseDate($value){
		
		$date = explode('/', $value);
		
		return "{$date[2]}-{$date[1]}-{$date[0]}";
		
	}

	function diffDate($visit)
	{

		$visit = explode('-', $visit);

		$today = explode('-', date("y-m-d"));


		$visit = mktime(0, 0, 0, $visit[1], $visit[2], $visit[0] );
		$today = mktime(0, 0, 0, $today[1], $today[2], $today[0]) ;

		$dif = ($today - $visit) / 2592000;

		if($dif <= 0)
			return 0;

		return floor($dif);

	}

}
?>