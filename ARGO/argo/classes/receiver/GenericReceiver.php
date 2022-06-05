<?php

include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/bean/GenericBean.class.inc");
include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/util/SecurityManager.class.inc");
include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/util/ClusterManager.class.inc");
include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/util/SubClusterManager.class.inc");

//geomtype
//geomfromgeojson
class GenericReceiver{
	
	static function processRequest(){
		
		if($_POST){
			self::processPOST();
		}
		else if($_GET){
			self::processGET();
		}
		
	}
	
	//geom statics
	static function processPOST(){
		
		if(isset($_POST['clusterList'])){
			ClusterManager::retrieveClusterList();
		}
		else if(isset($_POST['geocodbox'])){
			//select st_geomfromgeojson('{"type":"MultiPolygon","coordinates":[[[ [-4290548.33142274,-429579.790372457] ]]]}')
			GenericBean::retrieveGISRecords('public','cliente_geocod', "st_intersects(geom,st_geomfromewkt('{$_POST['geocodbox']}'))");
		}
		else if(isset($_POST['edgeMerge'])){

			GenericDAO::mergeEdge($_POST['GEOM_1'], $_POST['GEOM_2'], $_POST['ONE_WAY'], $_POST['NODES']);
		
		}
		else if(isset($_POST['routMerge'])){
		
			echo GenericDAO::mergeRout($_POST['routGid']);
		
		}
		else if(isset($_POST['routDrop'])){
		
			echo GenericDAO::dropRout($_POST['routGid']);
		
		}
		else if(isset($_POST['routFinder'])){
		
			echo GenericDAO::nearestEdge($_POST['x'],$_POST['y']);
					
		}
		else if(isset($_POST['reverseGeocod'])){
			//select st_geomfromgeojson('{"type":"MultiPolygon","coordinates":[[[ [-4290548.33142274,-429579.790372457] ]]]}')
			GenericDAO::nearestVia($_POST['lat'], $_POST['long']);
		}
		else if(isset($_POST['subClusterList'])){
			SubClusterManager::retrieveClusterList($_POST['subClusterList']);
		}
		else if(isset($_POST['sequenceList'])){
			ClusterManager::retrieveEuclideanSequenceList($_POST['sequenceList']);
		}
		else if(isset($_POST['clusterNumber'])){
			ClusterManager::getNumber($_POST['clusterNumber']);
		}
		else if(isset($_POST['subClusterNumber'])){
			SubClusterManager::getNumber($_POST['subClusterNumber']);
		}
		else if(isset($_POST['nearestNode'])){
			//SubClusterManager::getNumber($_POST['subClusterNumber']);
			echo GenericDAO::nearestNode($_POST['x'],$_POST['y']);
		}
		else if(isset($_POST['subKume']) && isset($_POST['time'])){
			if($_POST['time'] == '')
				$_POST['time'] = '220';
			if($_POST['x'] == '')
				$_POST['x'] = '15';
			if($_POST['y'] == '')
				$_POST['y'] = '20';
			
			if($_POST['z'] == ''){
				$_POST['z'] = date('Y-m-d');
			}
			else
				$_POST['z'] = DateUtil::reverseDate($_POST['z']);
			
			SubClusterManager::clusterByTime($_POST['subKume'],$_POST['time']);
			sleep(2);
		}
		else if(isset($_POST['subKume']) && isset($_POST['number'])){
			if($_POST['number'] == '')
				$_POST['number'] = '5';
			
			if($_POST['z'] == ''){
				$_POST['z'] = date('Y-m-d');
			}
			else 
				$_POST['z'] = DateUtil::reverseDate($_POST['z']);
			
			SubClusterManager::clusterByNumber($_POST['subKume'],$_POST['number']);
			sleep(2);
		}
		else if(isset($_POST['subKume']) && isset($_POST['distance'])){
			if($_POST['distance'] == '')
				$_POST['distance'] = '5';
			
			if($_POST['z'] == ''){
				$_POST['z'] = date('Y-m-d');
			}
			else
				$_POST['z'] = DateUtil::reverseDate($_POST['z']);
			
			SubClusterManager::clusterByDistance($_POST['subKume'],$_POST['distance']);
			sleep(2);
		}
		else if(isset($_POST['time'])){
			if($_POST['time'] == '')
				$_POST['time'] = '220';
			if($_POST['x'] == '')
				$_POST['x'] = '15';
			if($_POST['y'] == '')
				$_POST['y'] = '20';
			ClusterManager::clusterByTime($_POST['time']);
			sleep(2);
		}
		else if(isset($_POST['number'])){
			if($_POST['number'] == '')
				$_POST['number'] = '5';
			ClusterManager::clusterByNumber($_POST['number']);
			sleep(2);
		}
		else if(isset($_POST['distance'])){
			if($_POST['distance'] == '')
				$_POST['distance'] = '5';
			ClusterManager::clusterByDistance($_POST['distance']);
			sleep(2);
		}
		else if(isset($_POST['view'])){
			ClusterManager::updateViewNumber($_POST['bairro'],$_POST['view']);
		}
		else if(isset($_POST['initCluster'])){
			ClusterManager::initClusterList();
		}
		else if($_POST['action'] == 'geocod'){
			DatabaseUtil::execute("UPDATE CLIENTE_T SET GEOM = '{$_POST['long']} {$_POST['lat']}' WHERE GID = {$_POST['gid']}");
			HTML::reload();
		}
		else if($_POST['action'] == 'query'){
			$_POST = self::decodeRequest($_POST);
			$_POST['values'] = self::checkEncoding($_POST['values']);
			GenericBean::retrieveRecords($_POST['x'],$_POST['y'], $_POST['values'],$_POST['z']);
		}
		else if($_POST['action'] == 'autocomplete'){
			$_POST['values'] = self::checkEncoding($_POST['param']);
			GenericBean::retrieveAutoCompleteRecords($_POST['y'],$_POST['x'],$_POST['k'], $_POST['index'], $_POST['param']);
		}
		else if($_POST['action'] == 'querygis'){
			$_POST['values'] = self::checkEncoding($_POST['values']);
			GenericBean::retrieveGISRecords($_POST['x'],$_POST['y'], $_POST['values']);
		}
		else if($_POST['action'] == 'number'){
			GenericDAO::checkNumber($_POST['x'], $_POST['y']);
		}
		else if($_POST['action'] == 'login')
			SecurityManager::createSession($_POST['login'], $_POST['pass']);
		else if($_POST['action'] == 'logout')
			SecurityManager::unsetSession();
		else{
			
			$_POST = self::decodeRequest($_POST);
			
			$_POST['values'] = str_replace('false', "\"false\"", $_POST['values']);
			
			$_POST['values'] = self::checkNonNumericValues((array)json_decode($_POST['values']));
	
			if($_POST['georg']){
			
				//	$_POST['geom'] = str_replace('{"type"', "st_setsrid(st_geomfromgeojson('".'{"type"', $_POST['geom']);
			
				//	$_POST['geom'] = str_replace("]}", "]}'),{$_POST['epsg']})", $_POST['geom']);
			
				$_POST['geom']  = (array) json_decode($_POST['geom']);
				
				$_POST['geom'] = "ST_GeomFromText('{$_POST['geom']['type']}({$_POST['geom']['coordinates'][0]} {$_POST['geom']['coordinates'][1]})',{$_POST['epsg']})";
				
				$_POST['values'][$_POST['georg']] = $_POST['geom'];
				
			}
			
			if(SecurityManager::checkSession())
				self::checkClass();
			
		}
		
	}
	
	static function checkClass(){
		
		$class = str_replace(" ", '', ucwords(str_replace("_", " ", ($_POST['x']=='public'?'':$_POST['x'])."_{$_POST['y']}")))."Bean";
		
		if(file_exists($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/bean/{$class}.class.inc")){
			include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/bean/{$class}.class.inc");
			self::makeClassCRUD($class);
		}
		else	
			self::makeGenericCRUD();
		
	}
	
	static function makeGenericCRUD(){
		
			if($_POST['action'] == 'insert'){
				GenericBean::save($_POST['x'],$_POST['y'], $_POST['values']);
			}
			else if($_POST['action'] == 'update'){
				GenericBean::update($_POST['x'],$_POST['y'], $_POST['values'], $_POST['z'], $_POST['values'][$_POST['z']]);
			}
			else if($_POST['action'] == 'remove'){
				GenericBean::remove($_POST['x'],$_POST['y'], $_POST['z'], $_POST['values'][$_POST['z']]);
			}
		
	}
	
	static function makeClassCRUD($class){
		
			//The class must be the schema with ucwords and table with ucwords where _ should be swapped by empty space.
		
			if($_POST['action'] == 'insert'){
				eval($class.'::save($_POST["x"],$_POST["y"], $_POST["values"]);');
			}
			else if($_POST['action'] == 'update'){
				eval($class.'::update($_POST["x"],$_POST["y"], $_POST["values"], $_POST["z"], $_POST["values"][$_POST["z"]]);');
			}
			else if($_POST['action'] == 'remove'){
				eval($class.'::remove($_POST["x"],$_POST["y"], $_POST["z"], $_POST["values"][$_POST["z"]]);');
			}
		
	}
	
	//statics
	static function processGET(){
		
		if(isset($_GET['action']) && $_GET['action'] == 'project')
		{

			include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/pages/GISAPP/index.php");
			
		}
		else if(isset($_GET['action']) && $_GET['action'] == 'gridForm' && isset($_GET['x']) && isset($_GET['y'])){
			
			if(SecurityManager::checkSession()){
				
				$_GET = self::decodeRequest($_GET);
				
				$class = str_replace(" ", '', ucwords(str_replace("_", " ", ($_GET['x']=='public'?'':$_GET['x'])."_{$_GET['y']}")))."Bean";
		
				if(file_exists($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/bean/{$class}.class.inc")){
					include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/bean/{$class}.class.inc");
					include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/pages/Common/Head.php");
					eval($class.'::generateRowGrid($_GET["x"],$_GET["y"]);');
					include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/pages/Common/Foot.php");
				}
				else{	
					include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/pages/Common/Head.php");
					GenericBean::generateRowGrid($_GET['x'],$_GET['y']);
					include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/pages/Common/Foot.php");
				}
				
			}
			else
				GenericBean::generateLoginPage();
		}
		else if(isset($_GET['action']) && $_GET['action'] == 'loginForm'){
			
			GenericBean::generateLoginPage();

		}
		else if(isset($_GET['action']) && $_GET['action'] == 'gisUpdateForm'){
			
			GenericBean::generateForm($_GET['k'],$_GET['y'], $_GET['gid'],'','');				
			
		}
		else if(isset($_GET['action']) && $_GET['action'] == 'gisInsertForm'){
			if(SecurityManager::checkSession()){
				GenericBean::generateForm($_GET['x'],$_GET['y'], '',$_GET['lat'],$_GET['long']);	
			}
			else 
				GenericBean::generateLoginPage();
			
		}		
		else if(isset($_GET['action']) && $_GET['action'] == 'gridForeignForm' && isset($_GET['x']) && isset($_GET['y'])){
			
			if(SecurityManager::checkSession()){
				
				$_GET = self::decodeRequest($_GET);
				
				$class = str_replace(" ", '', ucwords(str_replace("_", " ", ($_GET['x']=='public'?'':$_GET['x'])."_{$_GET['y']}")))."Bean";
		
				if(file_exists($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/bean/{$class}.class.inc")){
					include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/bean/{$class}.class.inc");
					include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/pages/Common/Head.php");
					eval($class.'::generateForeignRowGrid($_GET["x"],$_GET["y"],$_GET["z"],$_GET["id"]);');
					include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/pages/Common/Foot.php");
				}
				else{	
					include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/pages/Common/Head.php");
					GenericBean::generateForeignRowGrid($_GET["x"],$_GET["y"],$_GET["z"],$_GET["id"]);
					include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/pages/Common/Foot.php");
				}
				
				
				
			}
			else
				GenericBean::generateLoginPage();
		}
		else if(isset($_GET['action']) && $_GET['action'] == 'report'){
			
			$_GET = self::decodeRequest($_GET);
			
			if(SecurityManager::checkSession() && isset($_GET['y'])){
				ReportUtil::processReportRequest($_GET['x']," AND {$_GET['y']}={$_GET['z']}");	
			}
			else if(SecurityManager::checkSession()){
				ReportUtil::processReportRequest($_GET['x'],'');
			}
			else 
				GenericBean::generateLoginPage();
			
		}
		else if(isset($_GET['action']) && $_GET['action'] == 'columnScreen'){
				
			//$_GET = self::decodeRequest($_GET);
				
			if(SecurityManager::checkSession() && isset($_GET['z'])){
				include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/pages/Common/Head.php");
				GenericBean::generateColumnScreen($_GET['z'],$_GET['y'], $_GET['x']);
				include_once($_SERVER['DOCUMENT_ROOT']."/ARGO/pages/Common/Foot.php");
			}
			else
				GenericBean::generateLoginPage();
				
		}
		else if(isset($_GET['action']) && $_GET['action'] == 'reportBean'){
		
			$_GET = self::decodeRequest($_GET);
		
			if(SecurityManager::checkSession() && isset($_GET['z']) && $_GET['type'] =='pdf'){
				GenericBean::generatePDFReport($_GET['z'],$_GET['y'], $_GET['k'],$_GET['x']);
			}
			else if(SecurityManager::checkSession() && isset($_GET['z']) && $_GET['type'] =='xls'){
				GenericBean::generateXLSReport($_GET['z'],$_GET['y'], $_GET['k'],$_GET['x']);
			}
			else if(SecurityManager::checkSession() && isset($_GET['z']) && $_GET['type'] =='txt'){
				GenericBean::generateTXTReport($_GET['z'],$_GET['y'], $_GET['k'],$_GET['x']);
			}
		
		}
	}
	
	static function checkNonNumericValues($list){
		
		$keys = array_keys($list);
		
		$values = array_values($list);
		
		for($i = 0; $i != count($keys); $i++){
			if(!is_numeric($values[$i])){
				if($values[$i] == '')
					$list[$keys[$i]]  = "null";
				else
					$list[$keys[$i]]  = "'".self::checkEncoding($values[$i])."'";
			}
		}
		
		return $list;
		
		
	}
	
	static function checkEncoding($values){
		
		if(mb_detect_encoding($values) == 'ISO-8859-1')
			return mb_convert_encoding($values,'UTF-8', 'ISO-8859-1');
		else 	
			return $values;
		
		
	}

	static function decodeRequest($request){
		
		if(isset($request['x']))
			$request['x'] = base64_decode($request['x']);
		if(isset($request['y']))
			$request['y'] = base64_decode($request['y']);
		if(isset($request['z']))
			$request['z'] = base64_decode($request['z']);
		
		return $request;
	}
}

GenericReceiver::processRequest();


?>