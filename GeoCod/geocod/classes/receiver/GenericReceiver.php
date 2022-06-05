<?php

include_once($_SERVER['DOCUMENT_ROOT']."/GeoCod/classes/bean/GenericBean.class.inc");

class GenericReceiver{

	static function processRequest(){

		if($_POST){
			self::processPOST();
		}
		else if($_GET){
			self::processGET();
		}

	}
	
	static function processGeoCodPOST(){
	
		DatabaseUtil::execute("UPDATE CLIENT SET LAT='{$_POST['lat']}',LONG='{$_POST['long']}' WHERE GID={$_POST['gid']}");
			
	}
	

	static function processPOST(){

		//GenericBean::retrieveYouMean($_POST['param']);

		if(isset($_POST['latLong']))
		GenericBean::retrieveLatLong($_POST['param']);
		else if($_POST['action'] == 'geocod'){
			DatabaseUtil::execute("UPDATE CLIENT SET LAT = '{$_POST['lat']}' , LON = '{$_POST['long']}' WHERE GID = {$_POST['gid']}");
			HTML::reload();
		}
		else if(isset($_POST['levenshtein']))
		GenericBean::retrieveLevenshteinGISRecords($_POST['param']);
		else if(isset($_POST['phone']))
		GenericBean::retrievePhoneGISRecords($_POST['param']);
		else
		GenericBean::retrieveGISRecords($_POST['param']);
			
		//*/
	}
	//statics
	static function processGET(){


	}

}

GenericReceiver::processRequest();


?>