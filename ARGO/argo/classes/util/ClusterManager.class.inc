<?php

class ClusterManager{
	
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
	
	static $attendimentCount = 1;
	
	static $hasClusters = false;
		
	static function clusterByNumber($param){

		$list = array();
		
		$rs = null;
		
		self::resetPrevious();
		
		self::countObjects();
		
		self::$objectSQL = "SELECT gid, '['||st_x(geom)||','||st_y(geom)||']' FROM cliente_ativo WHERE kume = '0' ORDER BY st_distance((select geom from objeto where gid = 13970),geom) ASC";
		
		$rs = DatabaseUtil::execute(self::$objectSQL);
		
		while(self::$elementsAdded!=self::$n){
			
			$rs = DatabaseUtil::execute(self::$objectSQL);
			
			while($row = pg_fetch_row($rs)){
	
				self::$kumeCount++;
				
				//echo $row[0]."<br/>";
				
				if(self::$makeBreak){
					self::$kumeCount = 0;
					self::$objectSQL =  "SELECT gid, '['||st_x(geom)||','||st_y(geom)||']' FROM cliente_ativo WHERE kume = '0' ".self::$notIn." ORDER BY st_distance((select geom from objeto where gid = ".self::getFarthest($row[0])."),geom) ASC";
				//	echo self::$objectSQL."<br/>";
				//	self::$initial = $row[0];
					break;
				}
			
				else if(self::$kumeCount <= $param){
					
					$list['gid'] 	= 	$row[0] ;
	
					$list['geom'] 	= 	$row[1] ;
					
					self::setObjectColor($list['gid'],self::$kumeColor);
	
					$list['kume'] 	= 	self::addObject($list['geom'],$param);
	
					self::setKumeIndex($list['gid'],$list['kume']);
	
				}
				
				self::$elementsAdded++;
				
			}
			
			self::$makeBreak = false;
		
			
		}
		
		if(isset(self::$currentKume[0])){
			self::$kumeDescription = "Setor ".self::$kumeIndex;
			self::buildKume();
		}
	}
	
	static function clusterByDistance($param){
	
		$list = array();
	
		$rs = null;
	
		self::resetPrevious();
	
		self::countObjects();
	
		self::$objectSQL = "SELECT gid, '['||st_x(geom)||','||st_y(geom)||']' FROM cliente_ativo WHERE kume = '0' ORDER BY st_distance((select geom from objeto where gid = 13970),geom) ASC";
	
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
					self::$objectSQL =  "SELECT gid, '['||st_x(geom)||','||st_y(geom)||']' FROM cliente_ativo WHERE kume = '0' ".self::$notIn." ORDER BY st_distance((select geom from objeto where gid = ".self::getFarthest($row[0])."),geom) ASC";
					break;
				}
						
				$list['gid'] 	= 	$row[0] ;
	
				$list['geom'] 	= 	$row[1] ;
						
				self::setObjectColor($list['gid'],self::$kumeColor);
	
				$list['kume'] 	= 	self::addObject($list['geom'],$param);
	
				self::setKumeIndex($list['gid'],$list['kume']);

				self::$elementsAdded++;
	
			}
				
			self::$makeBreak = false;
			//break;
		
				
		}
	
		if(isset(self::$currentKume[0])){
			self::$kumeDescription = "Setor ".self::$kumeIndex;
			self::buildKume();
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
	
	static function clusterByTime($param){
	
		$list = array();
	
		$rs = null;
	
		self::resetPrevious();
	
		self::countObjects();
	
		self::$objectSQL = "SELECT gid, '['||st_x(geom)||','||st_y(geom)||']' FROM cliente_ativo WHERE kume = '0' ORDER BY st_distance((select geom from objeto where gid = 13970),geom) ASC";
	
		$rs = DatabaseUtil::execute(self::$objectSQL);
	
		//self::$makeBreak = false;
	
		while(self::$elementsAdded<=self::$n){
	
			$rs = DatabaseUtil::execute(self::$objectSQL);
	
			//echo "<br/>".self::$objectSQL;
			if(self::$elementsAdded>=self::$n)
				break;
				
			while($row = pg_fetch_row($rs)){
	
				self::$attendimentCount++;
				self::$kumeCount = self::checkCurrentTime()+$_POST['x']*self::$attendimentCount;
			//	echo "<br/>".self::$kumeCount;
				
				if(self::$makeBreak){
					//echo self::$kumeCount." min's<br/>";
					self::$kumeCount = 0;
					self::$attendimentCount=1;
					self::$objectSQL =  "SELECT gid, '['||st_x(geom)||','||st_y(geom)||']' FROM cliente_ativo WHERE kume = '0' ".self::$notIn." ORDER BY st_distance((select geom from objeto where gid = ".self::getFarthest($row[0])."),geom) ASC";
					break;
				}
	
				$list['gid'] 	= 	$row[0] ;
	
				$list['geom'] 	= 	$row[1] ;
	
				self::setObjectColor($list['gid'],self::$kumeColor);
	
				$list['kume'] 	= 	self::addObject($list['geom'],$param);
	
				self::setKumeIndex($list['gid'],$list['kume']);
	
				self::$elementsAdded++;
	
			}
	
			self::$makeBreak = false;
			//break;
		
	
		}
	
		if(isset(self::$currentKume[0])){
			self::$kumeDescription = "Setor ".self::$kumeIndex;
			self::buildKume();
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
				return ($row[0] * (60/$_POST['y']));
			}
	
		}
	
		return 0;
	
	}
	
	static function addObject($geom,$param){
	
		if(self::$kumeCount < $param){
			array_push(self::$currentKume, $geom);
			return self::$kumeIndex;
		}
		else if(self::$kumeCount >= $param){
			self::$makeBreak = true;
			array_push(self::$currentKume, $geom);
			self::$kumeDescription = "Setor ".self::$kumeIndex;
			self::buildKume();
			self::$notIn .= " AND kume not like '%.".self::$kumeIndex.".%'";
			return  self::$kumeIndex++;
		}
	
	
	}
	
	static function setKumeIndex($gid,$kume){
		
		//$color = self::$kumeColor;
	
		DatabaseUtil::execute("INSERT INTO objeto_kume values($gid,$kume,0)");
	
	}
	
	static function setObjectColor($gid,$color){
	
		//$color = self::$kumeColor;
	
		if($color == 6)
			$color = 1;
		
		DatabaseUtil::execute("UPDATE objeto SET color = $color WHERE GID = $gid");
	
	}
	
	static function resetPrevious(){
	
	//	DatabaseUtil::execute("DELETE FROM KUME_POLYGON");
		DatabaseUtil::execute("DELETE FROM KUME");
		DatabaseUtil::execute("UPDATE OBJETO SET COLOR = 0");
		DatabaseUtil::execute("DELETE FROM OBJETO_KUME");
	
	}
	
	static function buildKume(){
	
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
			
		$polygon = '{"type":"MultiPolygon","coordinates":[[['.$json.']]]}';
			
		DatabaseUtil::execute("INSERT INTO KUME(GID,DESCRICAO,COLORACAO) VALUES(".self::$kumeIndex.",'".self::$kumeDescription."',".self::$kumeColor.")");
				
		self::$currentKume = array();
			
		self::$kumeColor++;
			
		echo "<div class='clusterItem'>";
		echo "<input id='cluster".self::$kumeIndex."' class='clusterCheck' name='clusterFilter' type='radio' onclick='MapAction.filterCluster(".self::$kumeIndex.");'/>";
		echo "<div class='clusterDescription'>".self::$kumeDescription."</div>";
		echo "<input id='clusterNumber".self::$kumeIndex."' class='clusterNumberButton' onclick='MapAction.sendClusterNumberRequest(".'"'.self::$kumeDescription.'"'.",".self::$kumeIndex.");'	src='/ARGO/images/number.gif' type='image'/>";
	//	echo "<input id='clusterSequence".self::$kumeIndex."' class='clusterClientButton' onclick='MapAction.retrieveSequence(".self::$kumeIndex.");'	src='/ARGO/images/cliente.gif' type='image'/>";
		echo "<input id='subClusterScreen".self::$kumeIndex."' class='subClusterScreenButton' onclick='MapAction.openSubClusterScreen(".self::$kumeIndex.");'	src='/ARGO/images/details.gif' type='image'/>";
		echo "</div>";
		
	
	}

	
	static function countObjects(){
	
		$rs = DatabaseUtil::execute("SELECT count(GID) FROM CLIENTE_ATIVO");
	
		while($row = pg_fetch_row($rs)){
	
			self::$n = ($row[0]);
	
		}
	
	}
	
	static function getNumber($gid){
	
		$rs = DatabaseUtil::execute("SELECT count(objeto) FROM OBJETO_KUME WHERE KUME = $gid or SUBKUME = $gid");
	
		while($row = pg_fetch_row($rs)){
	
			echo ($row[0]);
	
		}
	
	}
	
	static function getFarthest($gid){
	
		$rs = DatabaseUtil::execute("SELECT gid FROM cliente_ativo WHERE kume = '0' ".self::$notIn." ORDER BY st_distance((select geom from objeto where gid = {$gid}),geom) DESC");
	
		while($row = pg_fetch_row($rs)){
	
			return ($row[0]);
	
		}
	
	}
	
	static function updateViewNumber($bairro,$param){
		
		//13940
	
		self::resetPrevious();
		
		$limit = $param!=''?" limit $param ":'';
		
		//coelce::OR (objeto.gid = 66465 and st_contains(bairro.geom, objeto.geom) AND bairro.gid = 85)
		//seplag::AND (rota like '%Rota-01%')
		
		$bairro = $bairro?array("from"=>",bairro","where"=>" AND ((st_contains (bairro.geom,objeto.geom) AND bairro.gid =".$bairro.") ) "):array("from"=>"","where"=>"");
		
		DatabaseUtil::execute("CREATE OR REPLACE VIEW cliente_ativo AS   SELECT objeto.gid, objeto.nome, getclientekume(objeto.gid) AS kume, getclientesubkume(objeto.gid) AS subkume, objeto.ativo, objeto.color,getclientebairro(objeto.gid) as bairro,getclientesetornome(objeto.gid,getclientesubkume(objeto.gid)) as nome_setor,getclientesequencepostion(objeto.gid) AS ordem_visita, objeto.geom FROM objeto {$bairro['from']} WHERE ativo = true {$bairro['where']} ORDER BY st_distance((select geom from objeto where gid = 1),objeto.geom) ASC $limit");
			
	
	}
	
	static function retrieveClusterList(){
	
		$list = array();
	
		$rs = null;
	
		$rs = DatabaseUtil::execute("SELECT gid, descricao FROM kume WHERE SUBKUME = 0 OR SUBKUME IS NULL  ORDER BY gid ASC");
	
		while($row = pg_fetch_row($rs)){
			echo "<div class='clusterItem'>";
			echo "<input id='cluster".$row[0]."' class='clusterCheck' name='clusterFilter' type='radio' onclick='MapAction.filterCluster(".$row[0].");'/>";
			echo "<div class='clusterDescription'>".$row[1]."</div>";
			echo "<input id='clusterNumber".$row[0]."' class='clusterNumberButton' onclick='MapAction.sendClusterNumberRequest(".'"'.$row[1].'"'.",".$row[0].");'	src='/ARGO/images/number.gif' type='image'/>";
		///	echo "<input id='clusterSequence".$row[0]."' class='clusterClientButton' onclick='MapAction.retrieveSequence(".$row[0].");'	src='/ARGO/images/cliente.gif' type='image'/>";
			echo "<input id='subClusterScreen".$row[0]."' class='subClusterScreenButton' onclick='MapAction.openSubClusterScreen(".$row[0].");'	src='/ARGO/images/details.gif' type='image'/>";
			echo "</div>";
		}	
		
	}
	
	static function initClusterList(){
	
		$list = array();
	
		$rs = null;
	
		$rs = DatabaseUtil::execute("SELECT gid, descricao FROM kume WHERE SUBKUME = 0 OR SUBKUME IS NULL ORDER BY gid ASC");
		
		$checked = "checked=checked";
	
		//echo "<div class='clusterHead'><div class='clusterTitleS'>Setor</div><div class='clusterTitle'>Subdividir</div><div class='clusterTitle'>Roteirizar</div><div class='clusterTitle'>Sequência</div></div>";
		
		while($row = pg_fetch_row($rs)){
			echo "<div class='clusterItem'>";
			echo "<input id='cluster".$row[0]."' class='clusterCheck' name='clusterFilter' type='radio' onclick='MapAction.filterCluster(".$row[0].");' $checked/>";
			echo "<div class='clusterDescription'>".$row[1]."</div>";
			echo "<input id='clusterNumber".$row[0]."' class='clusterNumberButton' onclick='MapAction.sendClusterNumberRequest(".'"'.$row[1].'"'.",".$row[0].");'	src='/ARGO/images/number.gif' type='image'/>";
		//	echo "<input id='clusterSequence".$row[0]."' class='clusterClientButton' onclick='MapAction.retrieveSequence(".$row[0].");'	src='/ARGO/images/cliente.gif' type='image'/>";
			echo "<input id='subClusterScreen".$row[0]."' class='subClusterScreenButton' onclick='MapAction.openSubClusterScreen(".$row[0].");'	src='/ARGO/images/details.gif' type='image'/>";
			echo "</div>";
			$checked= '';
		}
	//details.gif
	}
	
	static function retrieveEuclideanSequenceList($id){

		$rs = DatabaseUtil::execute("SELECT objeto_gid,nome,indice,st_x(cliente_ativo.geom)||' '||st_y(cliente_ativo.geom) FROM euclidean_sequence,objeto_kume objeto,cliente_ativo WHERE cliente_ativo.gid = objeto_gid AND objeto.objeto = objeto_gid AND objeto.kume = $id ORDER BY indice");

		$cols = array('indice','codigo','nome','localizador');
		
		$size = 100/6;
		
		$flag = 0;
		
		$htmlTable ="<div id='size' style='overflow:scroll; width:inherit; height:360px;'><TABLE class='gisRecords' cellspacing='0'   ><TR><THEAD>";
		
		foreach($cols as $col){
			$htmlTable .= HTML::createColumn($col,$size,true);
		}
		
		$htmlTable .="</THEAD></TR><TBODY>";
		
		while($row = pg_fetch_row($rs)){
					
			$htmlTable .= '<TR>';
			$htmlTable .= HTML::createColumn($row[2],$size,false);
			$htmlTable .= HTML::createColumn($row[0],$size,false);
			$htmlTable .= HTML::createColumn($row[1],$size,false);
			//$htmlTable .= HTML::createColumn($row[0],$size,false);
			$htmlTable .= HTML::createGeoColumn($row[3],$size);
			$htmlTable .= '</TR>';
			
			$flag++;
		}
		
		$htmlTable .= "</TBODY></TABLE></div><div id='number' style='visibility:hidden;'>$flag</div>";
		
		if($flag>0)
			echo $htmlTable;
		else 
			echo "<div id='size' style='display:none;'>3</div>Nenhum resultado encontrado para este Setor.";
	
	}
	
	
	
}

?>
