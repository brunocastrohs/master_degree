<?php

class SubClusterManager{
	
	static $kumeCount = 0;
	
	static $kumeIndex = 1;
	
	static $currentKume = array();
	
	static $kumeDescription = '';
	
	static $kumeColor = 1;
	
	static $n = 0;
	
	static $elementsAdded = 0;
	
	static $notIn = "";
	
	static $objectSQL = "";
	
	static $makeBreak = false;
	
	static $kumeStarter = 0;
	
	static $days = array("Sun"=>'Domingo',"Sat"=>'Sábado',"Mon"=>'Segunda',"Tue"=>'Terça',"Wed"=>'Quarta',"Thu"=>'Quinta',"Fri"=>'Sexta');
	
	static $daysL = array(1=>'Segunda',2=>'Terça',3=>'Quarta',4=>'Quinta',5=>'Sexta');	
	
	static $attendimentCount = 1;
	
	static $dayCount = 0;
	
	static $dayKumeCount = 1;
	
	static $clienteRestricted = array();
	
	static $restrictionActive = false;
	
	static $weekCount = 1;
	
	static function clusterByNumber($subkume,$param){

		self::$restrictionActive = true;
		
		$list = array();
		
		$rs = null;
		
		self::resetPrevious($subkume);
		
		self::countObjects($subkume);
		
		self::retrieveClienteRestricted($subkume);
		
		self::setKumeStarter();
		
		self::$objectSQL = "SELECT gid, '['||st_x(geom)||','||st_y(geom)||']' FROM cliente_ativo WHERE kume like '%.$subkume.%' AND gid not in(select objeto from restricao) ORDER BY st_distance((select geom from objeto where gid = 13970),geom) ASC";
		
		//$rs = DatabaseUtil::execute(self::$objectSQL);
		
		while(self::$elementsAdded!=self::$n){
			
			$rs = DatabaseUtil::execute(self::$objectSQL);
			
			self::checkClienteRestricted($subkume);
			
			while($row = pg_fetch_row($rs)){
	
				self::$kumeCount++;
				
				if(self::$makeBreak){
					self::$kumeCount = 0;
					self::$objectSQL =  "SELECT gid, '['||st_x(geom)||','||st_y(geom)||']' FROM cliente_ativo WHERE kume like '%.$subkume.%' ".self::$notIn." AND gid not in(select objeto from restricao) ORDER BY st_distance((select geom from objeto where gid = ".self::getFarthest($row[0],$subkume)."),geom) ASC";
					self::$dayKumeCount++;
					if(self::$dayKumeCount>5){
						self::$dayKumeCount =1;
						self::$weekCount++;
					}
					break;
				}
			
				else if(self::$kumeCount <= $param){
					
					$list['gid'] 	= 	$row[0] ;
	
					$list['geom'] 	= 	$row[1] ;
					
					self::setObjectColor($list['gid'],self::$kumeColor);
	
					$list['kume'] 	= 	self::addObject($list['geom'],$param,$subkume);
	
					self::setKumeIndex($list['gid'],$list['kume'],$subkume);
	
				}
				
				self::$elementsAdded++;
				
			}
			
			self::$makeBreak = false;
		
			
		}
		
		if(isset(self::$currentKume[0])){
			self::$kumeDescription = " ";
			self::buildKume($subkume);
		}
	}
	//this
	static function clusterByDistance($subkume,$param){
	
		$list = array();
	
		$rs = null;
	
		self::resetPrevious($subkume);
	
		self::countObjects($subkume);
		
		self::setKumeStarter();
	
		self::$objectSQL = "SELECT gid, '['||st_x(geom)||','||st_y(geom)||']' FROM cliente_ativo WHERE kume like '%.$subkume.%' ORDER BY st_distance((select geom from objeto where gid = 13970),geom) ASC";
	
		$rs = DatabaseUtil::execute(self::$objectSQL);
	
		//self::$makeBreak = false;
		
		while(self::$elementsAdded<=self::$n){
				
			$rs = DatabaseUtil::execute(self::$objectSQL);
				
			//echo "<br/>".self::$objectSQL;
			if(self::$elementsAdded>=self::$n)
				break;
			
			while($row = pg_fetch_row($rs)){
	
				self::$kumeCount = self::checkCurrentDistance();
	
				if(self::$makeBreak){
				//	echo self::$kumeCount." km's<br/>";
					self::$kumeCount = 0;
					self::$objectSQL =  "SELECT gid, '['||st_x(geom)||','||st_y(geom)||']' FROM cliente_ativo WHERE kume like '%.$subkume.%' ".self::$notIn." ORDER BY st_distance((select geom from objeto where gid = ".self::getFarthest($row[0],$subkume)."),geom) ASC";
					self::$dayKumeCount++;
					if(self::$dayKumeCount>5){
						self::$dayKumeCount =1;
						self::$weekCount++;
					}
					break;
				}
						
				$list['gid'] 	= 	$row[0] ;
	
				$list['geom'] 	= 	$row[1] ;
						
				self::setObjectColor($list['gid'],self::$kumeColor);
	
				$list['kume'] 	= 	self::addObject($list['geom'],$param,$subkume);
	
				self::setKumeIndex($list['gid'],$list['kume'],$subkume);

				self::$elementsAdded++;
	
			}
				
			self::$makeBreak = false;
			
				
		}
	
		if(isset(self::$currentKume[0])){
			self::$kumeDescription = " ";
			self::buildKume($subkume);
		}
	}
		
	static function checkCurrentDistance(){
	
		if(isset(self::$currentKume[0]) && isset(self::$currentKume[1])){
	
			$json = '';
	
			$isFirst = true;
	
			foreach(self::$currentKume as $geom){
					
				if($isFirst){
					$json .= $geom;
				}else{
					$json .= ",$geom";
				}
					
				$isFirst = false;
					
			}
	
			$line = '{"type":"LineString","coordinates":['.$json.']}';
	
			//echo "SELECT st_length( st_linemerge(st_setsrid(st_geomfromgeojson('$line'),900913) ) ) /1000 <br/>";
	
			$rs = DatabaseUtil::execute("SELECT st_length( st_linemerge(st_setsrid(st_geomfromgeojson('$line'),900913) ) ) /1000 ");
	
			while($row = pg_fetch_row($rs)){
				return $row[0];
			}
	
		}
	
		return 0;
	
	}
	//this
	static function clusterByTime($subkume,$param){
	
		$list = array();
	
		$rs = null;
	
		self::resetPrevious($subkume);
	
		self::countObjects($subkume);
		
		self::setKumeStarter();
	
		self::$objectSQL = "SELECT gid, '['||st_x(geom)||','||st_y(geom)||']' FROM cliente_ativo WHERE kume like '%.$subkume.%' ORDER BY st_distance((select geom from objeto where gid = 13970),geom) ASC";
	
		$rs = DatabaseUtil::execute(self::$objectSQL);
	
		//self::$makeBreak = false;
	
		while(self::$elementsAdded<=self::$n){
	
			$rs = DatabaseUtil::execute(self::$objectSQL);
	
			if(self::$elementsAdded>=self::$n)
				break;
			//echo "<br/>".self::$objectSQL;
				
			while($row = pg_fetch_row($rs)){
	
				self::$attendimentCount++;
				self::$kumeCount = self::checkCurrentTime()+$_POST['x']*self::$attendimentCount;
	
				if(self::$makeBreak){
					//echo self::$kumeCount." min's<br/>";
					self::$kumeCount = 0;
					self::$attendimentCount=1;
					self::$objectSQL =  "SELECT gid, '['||st_x(geom)||','||st_y(geom)||']' FROM cliente_ativo WHERE kume like '%.$subkume.%' ".self::$notIn." ORDER BY st_distance((select geom from objeto where gid = ".self::getFarthest($row[0],$subkume)."),geom) ASC";
					self::$dayKumeCount++;
					if(self::$dayKumeCount>5){
						self::$dayKumeCount =1;
						self::$weekCount++;
					}
					break;
				}
	
				$list['gid'] 	= 	$row[0] ;
	
				$list['geom'] 	= 	$row[1] ;
	
				self::setObjectColor($list['gid'],self::$kumeColor);
	
				$list['kume'] 	= 	self::addObject($list['geom'],$param,$subkume);
	
				self::setKumeIndex($list['gid'],$list['kume'],$subkume);
	
				self::$elementsAdded++;
	
			}
	
			self::$makeBreak = false;
			//break;
		
	
		}
	
		if(isset(self::$currentKume[0])){
			self::$kumeDescription = " ";
			self::buildKume($subkume);
		}
	}
	
	static function checkCurrentTime(){
	
		if(isset(self::$currentKume[0]) && isset(self::$currentKume[1])){
	
			$json = '';
	
			$isFirst = true;
	
			foreach(self::$currentKume as $geom){
					
				if($isFirst){
					$json .= $geom;
				}else{
					$json .= ",$geom";
				}
					
				$isFirst = false;
					
			}
	
			$line = '{"type":"LineString","coordinates":['.$json.']}';
	
			//echo "SELECT st_length( st_linemerge(st_setsrid(st_geomfromgeojson('$line'),900913) ) ) /1000 <br/>";
	
			$rs = DatabaseUtil::execute("SELECT st_length( st_linemerge(st_setsrid(st_geomfromgeojson('$line'),900913) ) ) /1000 ");
	
			while($row = pg_fetch_row($rs)){
				return ($row[0]*(60/$_POST['y']));
			}
	
		}
	
		return 0;
	
	}
	
	static function addObject($geom,$param,$subkume){
	
		if(self::$kumeCount < $param){
			array_push(self::$currentKume, $geom);
			return self::$kumeIndex;
		}
		else if(self::$kumeCount >= $param){
			self::$makeBreak = true;
			array_push(self::$currentKume, $geom);
			self::$kumeDescription = " ";
			self::buildKume($subkume);
			self::$notIn .= " AND kume not like '%.".(self::$kumeIndex+self::$kumeStarter).".%'";
			return  self::$kumeIndex++;
		}
	
	
	}
	
	static function setKumeIndex($gid,$kume,$subkume){
		
		$kume += self::$kumeStarter;
		
		//$day = self::$dayKumeCount;
		
		DatabaseUtil::execute("UPDATE objeto_kume SET kume = {$kume}, subkume=$subkume WHERE objeto = $gid");
	
	}
	
	static function setObjectColor($gid,$color){
	
		//$color = self::$kumeColor;
	
		if($color == 6)
			$color = 1;
		
		DatabaseUtil::execute("UPDATE objeto SET color = $color WHERE GID = $gid");
	
	}
	
	static function resetPrevious($subkume){
	
	//	DatabaseUtil::execute("DELETE FROM KUME_POLYGON");
		DatabaseUtil::execute("DELETE FROM KUME WHERE SUBKUME = $subkume");
		DatabaseUtil::execute("UPDATE OBJETO SET COLOR = 0 WHERE GID IN(SELECT OBJETO FROM OBJETO_KUME WHERE SUBKUME = $subkume OR KUME = $subkume)");
		DatabaseUtil::execute("UPDATE OBJETO_KUME SET KUME = $subkume,SUBKUME=0 WHERE SUBKUME = $subkume OR KUME = $subkume");
	
	}
	
	static function countObjects($subkume){
	
		if(self::$restrictionActive)
			$rs = DatabaseUtil::execute("SELECT count(OBJETO) FROM OBJETO_KUME WHERE objeto not in(select objeto from restricao) AND KUME = $subkume");
		else
			$rs = DatabaseUtil::execute("SELECT count(OBJETO) FROM OBJETO_KUME WHERE KUME = $subkume");
			
		while($row = pg_fetch_row($rs)){
	
			self::$n = ($row[0]);
	
		}
	
	}
	
	static function getNumber($gid){
	
		$rs = DatabaseUtil::execute("SELECT count(OBJETO) FROM OBJETO_KUME WHERE KUME = $gid");
	
		while($row = pg_fetch_row($rs)){
	
			echo  ($row[0]);
	
		}
	
	}
	
	static function setKumeStarter(){
	
		$rs = DatabaseUtil::execute("SELECT max(GID) FROM kume");
	
		while($row = pg_fetch_row($rs)){
	
			self::$kumeStarter += ($row[0]);
	
		}
	
	}
	
	static function buildKume($subkume){
	
		if(self::$kumeColor>5)
			self::$kumeColor=1;
	
		$json = '';
	
		$isFirst = true;
	
		$c = 0;
	
		foreach(self::$currentKume as $geom){
			
			if($isFirst){
				$json .= $geom;
			}else{
				$json .= ",$geom";
			}
	
			$isFirst = false;
	
			$c++;
	
		}
			
		//semana
		//self::$kumeDescription .= "".self::$daysL[self::$kumeColor]." - Semana ". self::$weekCount;
		
		//dia
		self::$kumeDescription .= self::getDay();
			
		$polygon = '{"type":"MultiPolygon","coordinates":[[['.$json.']]]}';
		
		$kumeLarry = self::$kumeStarter+self::$kumeIndex;
			
		DatabaseUtil::execute("INSERT INTO KUME(GID,DESCRICAO,COLORACAO,SUBKUME) VALUES(".$kumeLarry.",'".self::$kumeDescription."',".self::$kumeColor." ,$subkume)");
				
		self::$currentKume = array();
			
		self::$kumeColor++;
			
		echo "<div class='clusterItem'>";
		echo "<input id='subCluster".$kumeLarry."' class='clusterCheck' name='clusterFilter' type='radio' onclick='MapAction.filterCluster(".$kumeLarry.");'/>";
		echo "<div class='clusterDescription'>".self::$kumeDescription."</div>";
		echo "<input id='subClusterNumber".$kumeLarry."' class='clusterNumberButton' onclick='MapAction.sendSubClusterNumberRequest(".'"'.self::$kumeDescription.'"'.",".$kumeLarry.");'	src='/ARGO/images/number.gif' type='image'/>";
		echo "<input id='subClusterSequence".$kumeLarry."' class='clusterClientButton' onclick='MapAction.retrieveSequence(".$kumeLarry.");'	src='/ARGO/images/cliente.gif' type='image'/>";
		echo "<input id='subClusterPath".$kumeLarry."' class='clusterPathButton' onclick='MapAction.buildPath(".$kumeLarry.");'	src='/ARGO/images/byDistance.gif' type='image'/>";
		echo "</div>";
		
	
	}
	
	static function getFarthest($gid,$subkume){
	
		if(self::$restrictionActive)
			$rs = DatabaseUtil::execute("SELECT gid FROM cliente_ativo WHERE kume LIKE '%.$subkume.%' ".self::$notIn." AND gid not in(select objeto from restricao) ORDER BY st_distance((select geom from objeto where gid = {$gid}),geom) DESC");
		else
			$rs = DatabaseUtil::execute("SELECT gid FROM cliente_ativo WHERE kume LIKE '%.$subkume.%' ".self::$notIn." ORDER BY st_distance((select geom from objeto where gid = {$gid}),geom) DESC");
		
		while($row = pg_fetch_row($rs)){
	
			return ($row[0]);
	
		}
	
	}
	
	static function getDay(){
		
		$date=date_create($_POST['z']);
		
		date_add($date,date_interval_create_from_date_string(self::$dayCount." days"));
		
		self::$dayCount++;
		
		if (strstr(date_format($date,"D"), 'Fri')){
			self::$dayCount+=2;
		}
		
		return  self::$days[date_format($date,"D")] ." ".date_format($date,"d/m/Y");
	
	}
	//this		
	static function retrieveClusterList($subkume){
	
		$list = array();
	
		$rs = null;
	
		$rs = DatabaseUtil::execute("SELECT gid, descricao FROM kume WHERE subkume = $subkume ORDER BY gid ASC");
	
		while($row = pg_fetch_row($rs)){
			echo "<div class='clusterItem'>";
			echo "<input id='subCluster".$row[0]."' class='clusterCheck' name='clusterFilter' type='radio' onclick='MapAction.filterCluster(".$row[0].");'/>";
			echo "<div class='clusterDescription'>".$row[1]."</div>";
			echo "<input id='subClusterNumber".$row[0]."' class='clusterNumberButton' onclick='MapAction.sendSubClusterNumberRequest(".'"'.$row[1].'"'.",".$row[0].");'	src='/ARGO/images/number.gif' type='image'/>";
			echo "<input id='subClusterSequence".$row[0]."' class='clusterClientButton' onclick='MapAction.retrieveSequence(".$row[0].");'	src='/ARGO/images/cliente.gif' type='image'/>";
			echo "<input id='subClusterPath".$row[0]."' class='clusterPathButton' onclick='MapAction.buildPath(".$row[0].");'	src='/ARGO/images/byDistance.gif' type='image'/>";
			echo "</div>";
		}	
		
	}
	//this
	static function initClusterList($subkume){
	
		$list = array();
	
		$rs = null;
	
		$rs = DatabaseUtil::execute("SELECT gid, descricao FROM kume WHERE subkume = $subkume ORDER BY gid ASC");
		
		$checked = "checked=checked";
	
		while($row = pg_fetch_row($rs)){
			echo "<div class='clusterItem'>";
			echo "<input id='subCluster".$row[0]."' class='clusterCheck' name='clusterFilter' type='radio' onclick='MapAction.filterCluster(".$row[0].");' $checked/>";
			echo "<div class='clusterDescription'>".$row[1]."</div>";
			echo "<input id='subClusterNumber".$row[0]."' class='clusterNumberButton' onclick='MapAction.sendSubClusterNumberRequest(".'"'.$row[1].'"'.",".$row[0].");'	src='/ARGO/images/number.gif' type='image'/>";
			echo "<input id='subClusterSequence".$row[0]."' class='clusterClientButton' onclick='MapAction.retrieveSequence(".$row[0].");'	src='/ARGO/images/cliente.gif' type='image'/>";
			echo "<input id='subClusterPath".$row[0]."' class='clusterPathButton' onclick='MapAction.buildPath(".$row[0].");'	src='/ARGO/images/byDistance.gif' type='image'/>";
			echo "</div>";
			$checked= '';
		}
	
	}
	
	static function retrieveClienteRestricted($subkume){
	
		$list = array();
		
		$rs = DatabaseUtil::execute("SELECT gid,dia FROM cliente_ativo,restricao WHERE objeto = gid AND kume like '%.$subkume.%'");
	
		while($row = pg_fetch_row($rs)){
	
			self::$clienteRestricted[$row[0].".".$row[1]] = array("gid"=>$row[0],"dia"=>$row[1]);
			
		}
	
	}
	
	static function checkClienteRestricted($subkume){
	
		foreach (self::$clienteRestricted as $cliente){
			if( $cliente["dia"] == self::$dayKumeCount){
	
				self::$kumeCount++;

				$kume = self::$kumeStarter+self::$kumeIndex;
								
				DatabaseUtil::execute("INSERT INTO OBJETO_KUME(OBJETO,KUME,SUBKUME) VALUES({$cliente["gid"]},$kume,$subkume)");
				
				DatabaseUtil::execute("delete from objeto_kume where objeto = {$cliente['gid']} and kume = $subkume and subkume = 0");
				
				unset(self::$clienteRestricted[$cliente["gid"].".".$cliente["dia"]]);
				
			}
		}
		
	
	}
	
}

?>
