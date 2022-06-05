<?php
require_once($_SERVER['DOCUMENT_ROOT']."/ARGO/libs/proj4php/proj4php.php");
class Proj4Util{

	static function googeMercatorToLatLong($long,$lat){
		
		$proj4 = new Proj4php();

		$projL93 = new Proj4phpProj('EPSG:900913');
		
		$projWGS84 = new Proj4phpProj('EPSG:4326');

		$pointSrc = new proj4phpPoint($long,$lat);
		
		$pointDest = (array)$proj4->transform($projL93,$projWGS84,$pointSrc);
		
		return $pointDest;
		
	}

}
?>