<?php
require_once($_SERVER['DOCUMENT_ROOT']."/ARGO/classes/util/DatabaseUtil.class.inc");

class GraphUtil {
	
	
	static function buildVirtualGraph(){
		
		DatabaseUtil::execute("UPDATE NEW_NODE SET NEXT = '' ");
		
		DatabaseUtil::execute("DELETE FROM NEW_NODE WHERE VIA1_NAME IS NULL and VIA2_NAME IS NULL");
		
		DatabaseUtil::execute("DELETE FROM NEW_EDGE");
		
		$place = ", BAIRRO WHERE ST_INTERSECTS(BAIRRO.GEOM,NEW_VIA.GEOM) and (BAIRRO.NOME ILIKE '%ALDEOTA%')";

		$rs = DatabaseUtil::execute("SELECT NEW_VIA.GID,NEW_VIA.ONE_WAY,NEW_VIA.GEOM FROM NEW_VIA $place");
		
		while($row = pg_fetch_row($rs)){
			self::setNode(array('gid'=>$row[0],'one_way'=>$row[1],'geom'=>$row[2]));
		}
		
	}
	
	static function setNode($via){
		
		//SELECT NEW_NODE.* FROM NEW_VIA,NEW_NODE WHERE ST_INTERSECTS(NEW_VIA.GEOM,NEW_NODE.GEOM) AND NEW_VIA.GID = 30 ORDER BY NEW_NODE.GEOM ASC
		$order = $via['one_way'] == 'FT'?'DESC':'ASC';
		
		$rs = DatabaseUtil::execute("SELECT NEW_NODE.GID,NEW_NODE.NEXT FROM NEW_VIA,NEW_NODE WHERE ST_INTERSECTS(NEW_VIA.GEOM,NEW_NODE.GEOM) AND NEW_VIA.GID = {$via['gid']} ORDER BY NEW_NODE.GEOM $order");
		
		$node = array();
		
		$isFirst = true;
		
		while($row = pg_fetch_row($rs)){
			if($isFirst){
				$node['gid'] = $row[0];
				$node['nextHolded'] = $row[1];
				$isFirst = false;
			}else{
				$node['next'] = $row[0];
				$node['nextHoldedDB'] = $row[1];
			}
		}
		
		if(isset($node['next']) && $node['next']!=null && $node['gid']!=null)
			self::setDirection($via, $node);
		
	}
	
	static function setDirection($via,$node){

		if($via['one_way'] == 'DB'){
			DatabaseUtil::execute("UPDATE NEW_NODE SET NEXT = '{$node['next']}'||';'||'{$node['nextHolded']}' WHERE GID = {$node['gid']}");
			DatabaseUtil::execute("UPDATE NEW_NODE SET NEXT = '{$node['gid']}'||';'||'{$node['nextHoldedDB']}' WHERE GID = {$node['next']}");
			if(!self::edgeExists("st_linemerge('{$via['geom']}')"))
				DatabaseUtil::execute("INSERT INTO NEW_EDGE(GID,NODES,ONE_WAY,GEOM) VALUES({$via['gid']},'{$node['gid']}={$node['next']}','{$via['one_way']}',st_linemerge('{$via['geom']}'))");
		}
		else{
			DatabaseUtil::execute("UPDATE NEW_NODE SET NEXT = '{$node['next']}'||';'||'{$node['nextHolded']}' WHERE GID = {$node['gid']}");
			if(!self::edgeExists("st_linemerge('{$via['geom']}')"))
				DatabaseUtil::execute("INSERT INTO NEW_EDGE(GID,NODES,ONE_WAY,GEOM) VALUES({$via['gid']},'{$node['gid']}-{$node['next']}','{$via['one_way']}',st_linemerge('{$via['geom']}'))");
		}
		
	}
		
	static function edgeExists($geom){
	
		$rs = DatabaseUtil::execute("SELECT NEW_EDGE.GID FROM NEW_EDGE WHERE GEOM = $geom");
	
		while($row = pg_fetch_row($rs)){
			return true;
		}
	
		return false;
		
	}
	
	static function makeCorrectiveViews(){
		
		
		$view = array(
		//update via null one_way
		'vias_from_place'  =>"update via set one_way = 'DB' where one_way = null",
		//materializar essa view Amostra
		'vias_from_place'  =>"SELECT via.gid,via.name,via.type,via.one_way,via.geom FROM via, bairro WHERE st_contains(bairro.geom, via.geom)",
		//make correction 
		'vias_inverse'	   =>"UPDATE new_via SET ONE_WAY = REVERSE(ONE_WAY), GEOM = ST_REVERSE(GEOM) WHERE st_azimuth(st_startpoint(st_linemerge(geom)), st_endpoint(st_linemerge(geom)))::integer < st_azimuth(st_endpoint(st_linemerge(geom)), st_startpoint(st_linemerge(geom)))::integer",
		//db reverse
		'make_db_reverse'  =>"UPDATE VIAS_FROM_PLACE SET ONE_WAY = REVERSE(ONE_WAY) WHERE ONE_WAY = 'BD'",
		//nodes
		'nodes_from_place' => "CREATE OR REPLACE VIEW nodes_from_place AS select DISTINCT ON (st_intersection(via1.geom, via2.geom)) via1.name as via1_name,via2.name as via2_name, via1.one_way as via1_one_way, via2.one_way as via2_one_way,via1.type as via1_type, via2.type as via2_type, ''::text as next, st_intersection(via1.geom,via2.geom) as geom from new_via as via1, new_via as via2 where st_intersects(via1.geom,via2.geom) and via1.gid <> via2.gid and via1.name <> via2.name"
		);

		//execute this sql commands.
		
	}
	
	static function makeBridgeCorrection(){
		
		
		//REFAZER ESSAS CONSULTAS SEM startpoint e endpoint. Para isso, usar o mesmo método de ordenação de setNode
	
		DatabaseUtil::execute("DELETE FROM NEW_NODE WHERE VIA1_NAME IS NULL and VIA2_NAME IS NULL");
		
		$edges = DatabaseUtil::execute("SELECT BRIDGE.GID,BRIDGE.ONE_WAY,NEW_VIA.NAME FROM BRIDGE,NEW_VIA WHERE NEW_VIA.GID = BRIDGE.GID");
		
		$nodes = array();
		
		while($edge = pg_fetch_row($edges)){
			
			$order = $edge[1] == 'FT'?'DESC':'ASC';
			
			$via = $edge[2];
			
			//duplicar o do meio, fazer o primeiro apontar pro novo do meio, e fazer o novo apontar pro terceiro
			
			//dois primeiros nós 0(1)->0(2)
			$rs = DatabaseUtil::execute("select new_node.gid,new_node.next,new_node.geom from new_edge,new_node where new_edge.gid = {$edge[0]} and via1_name is not null and via2_name is not null and st_intersects(new_edge.geom,new_node.geom) order by new_node.geom $order");
			
			while($row = pg_fetch_row($rs)){

				array_push($nodes, array("gid"=>$row[0],"next"=>$row[1],"geom"=>$row[2]));
				
			}
			
			$order = $edge[1] == 'FT'?'ASC':'DESC';
			
			//terceiro nó 0(1)->0(2)->0(3)
			$rs = DatabaseUtil::execute("SELECT NEW_NODE.GID FROM NEW_VIA,NEW_NODE WHERE ST_INTERSECTS(NEW_NODE.GEOM,st_linemerge(new_VIA.geom)) AND via1_name is not null and via2_name is not null AND NEW_VIA.GID = (SELECT NEW_VIA.GID FROM NEW_VIA,NEW_NODE WHERE ST_INTERSECTS(NEW_VIA.GEOM,NEW_NODE.GEOM) AND NEW_NODE.GID = {$nodes[1]['gid']} AND NEW_VIA.NAME = '{$via}' ORDER BY new_via.geom $order LIMIT 1) ORDER BY NEW_NODE.GEOM $order LIMIT 1");

			while($row = pg_fetch_row($rs)){
			
				array_push($nodes, array("gid"=>$row[0]));
			
			}
			
			if(isset($nodes[0]) && isset($nodes[1]) && isset($nodes[2])){
				$newGid = self::getLastNodeGID();

				$newNode = array("gid"=>$newGid,"next"=> $nodes[2]['gid'].';',"geom"=>$nodes[1]['geom']);
			
				DatabaseUtil::execute("UPDATE NEW_NODE SET NEXT = '".str_replace($nodes[1]['gid'], $newGid, $nodes[0]['next'])."' WHERE GID = ".$nodes[0]['gid']);
			
				DatabaseUtil::execute("INSERT INTO NEW_NODE(GID,NEXT,GEOM) VALUES({$newNode['gid']},'{$newNode['next']}','{$newNode['geom']}')");
			}
			
			$nodes = array();
			
		}
		
		
	
	}
	
	static function makeBridgeHVCorrection(){
	
	
		//fUNCIONANDO APENAS PARA VERTICAL - | : pARA FUNCIONAR PARA HORIZONTAL, É NECESSÁRIO REALIZAR A SEGUNDA CONSULTA CERTA(lesserEdges)
		//na segunda consulta, usar nome pode trazer erro
	
		$edges = DatabaseUtil::execute("SELECT NEW_VIA.GID,NEW_VIA.ONE_WAY,NEW_VIA.NAME FROM BRIDGE_HV,NEW_VIA WHERE NEW_VIA.GID = BRIDGE_HV.GID");
	
		$nodes = array();
	
		while($edge = pg_fetch_row($edges)){
				
			$order = $edge[1] == 'FT'?'DESC':'ASC';
				
			$via = $edge[2];
			
				//ok
			$rs = DatabaseUtil::execute("SELECT NEW_NODE.GID,NEW_NODE.NEXT,NEW_NODE.GEOM FROM NEW_VIA,NEW_NODE WHERE ST_INTERSECTS(NEW_VIA.GEOM,NEW_NODE.GEOM) AND NEW_VIA.GID = {$edge[0]} AND VIA1_NAME IS NOT NULL and VIA2_NAME IS NOT NULL ORDER BY NEW_NODE.GEOM $order LIMIT 1");
				
			while($row = pg_fetch_row($rs)){
	
				$nodes[1] = array("gid"=>$row[0],"next"=>$row[1],"geom"=>$row[2]);
	
			}
			
				//ok
			$rs = DatabaseUtil::execute("SELECT NEW_VIA.GID FROM NEW_VIA,NEW_NODE WHERE ST_INTERSECTS(NEW_VIA.GEOM,NEW_NODE.GEOM) AND NEW_NODE.GID = {$nodes[1]['gid']} AND NEW_VIA.NAME <> '$via' ORDER BY NEW_VIA.GEOM $order");
	
			$lesserEdges = array();
			
			while($row = pg_fetch_row($rs)){
					
				array_push($lesserEdges, array("gid"=>$row[0]));
					
			}
			
			//OK
			$rs = DatabaseUtil::execute("SELECT NEW_NODE.GID,NEW_NODE.NEXT FROM NEW_VIA,NEW_NODE WHERE ST_INTERSECTS(NEW_VIA.GEOM,NEW_NODE.GEOM) AND NEW_VIA.GID = {$lesserEdges[0]['gid']} AND VIA1_NAME IS NOT NULL and VIA2_NAME IS NOT NULL ORDER BY NEW_NODE.GEOM $order LIMIT 1");
							
			while($row = pg_fetch_row($rs)){
					
				$nodes[0] = array("gid"=>$row[0],"next"=>$row[1]);
					
			}
			
			//OK
			$rs = DatabaseUtil::execute("SELECT NEW_NODE.GID,NEW_NODE.NEXT FROM NEW_VIA,NEW_NODE WHERE ST_INTERSECTS(NEW_VIA.GEOM,NEW_NODE.GEOM) AND NEW_VIA.GID = {$lesserEdges[0]['gid']} AND VIA1_NAME IS NULL and VIA2_NAME IS NULL ORDER BY NEW_NODE.GEOM $order LIMIT 1");
				
			while($row = pg_fetch_row($rs)){
					
				$nodes[0]['bridge'] = array("gid"=>$row[0],"next"=>$row[1]);
					
			}
			
			$order = $edge[1] == 'FT'?'ASC':'DESC';
			
			//OK
			$rs = DatabaseUtil::execute("SELECT NEW_NODE.GID,NEW_NODE.NEXT FROM NEW_VIA,NEW_NODE WHERE ST_INTERSECTS(NEW_VIA.GEOM,NEW_NODE.GEOM) AND NEW_VIA.GID = {$lesserEdges[1]['gid']} AND VIA1_NAME IS NULL and VIA2_NAME IS NULL ORDER BY NEW_NODE.GEOM $order LIMIT 1");
			
			while($row = pg_fetch_row($rs)){
					
				$nodes[2] = array("gid"=>$row[0],"next"=>$row[1]);
					
			}
			
			if(!isset($nodes[2])){
				//	OK
				$rs = DatabaseUtil::execute("SELECT NEW_NODE.GID,NEW_NODE.NEXT FROM NEW_VIA,NEW_NODE WHERE ST_INTERSECTS(NEW_VIA.GEOM,NEW_NODE.GEOM) AND NEW_VIA.GID = {$lesserEdges[1]['gid']} AND VIA1_NAME IS NOT NULL and VIA2_NAME IS NOT NULL ORDER BY NEW_NODE.GEOM $order LIMIT 1");
			
				while($row = pg_fetch_row($rs)){
					
					$nodes[2] = array("gid"=>$row[0],"next"=>$row[1]);
					
				}
				
			}
			
				
			if(isset($nodes[0]) && isset($nodes[1]) && isset($nodes[2])){
				
				
				$newGid = self::getLastNodeGID();
	
				echo "made $newGid<br/>";
				
				$newNode = array("gid"=>$newGid,"next"=> $nodes[2]['gid'].';',"geom"=>$nodes[1]['geom']);
					
				DatabaseUtil::execute("UPDATE NEW_NODE SET NEXT = '".str_replace($nodes[1]['gid'], $newGid, $nodes[0]['next'])."' WHERE GID = ".$nodes[0]['gid']);
				
				if(isset($nodes[0]['bridge']))
					DatabaseUtil::execute("UPDATE NEW_NODE SET NEXT = '".str_replace($nodes[1]['gid'], $newGid, $nodes[0]['bridge']['next'])."' WHERE GID = ".$nodes[0]['bridge']['gid']);
					
				DatabaseUtil::execute("INSERT INTO NEW_NODE(GID,NEXT,GEOM) VALUES({$newNode['gid']},'{$newNode['next']}','{$newNode['geom']}')");
			}
				
			$nodes = array();
				
		}
	
	
	
	}
	
	static function getLastNodeGID(){
	
		$rs = DatabaseUtil::execute("SELECT MAX(GID) FROM NEW_NODE");
	
		$nodes = array();
	
		while($row = pg_fetch_row($rs)){
			return $row[0]+1;
		}
		
	}
	
	static function makeMissingCorrection(){
	
		$vias = self::retrieveMissingEdges();
		
		foreach ($vias as $via)
			self::analyseNodesFromMissingEdges($via);
	
	}
	
	static function retrieveMissingEdges(){
		
	
	//	DatabaseUtil::execute("DELETE FROM NEW_EDGE");
		
	//	DatabaseUtil::execute("DELETE FROM NEW_NODE");
		
		$rs = DatabaseUtil::execute("select distinct new_via.gid,new_via.name,new_via.type,one_way,new_via.geom from NEW_via,BAIRRO where st_intersects(bairro.geom,new_via.geom) and new_via.gid in (select NEW_via.gid from NEW_via except select gid from NEW_edge) and (BAIRRO.NOME ILIKE '%ALDEOTA%') order by new_via.name");
		
		$edges = array();
		
		while($row = pg_fetch_row($rs)){
			array_push($edges, array("gid"=>$row[0],"name"=>$row[1],"type"=>$row[2],"one_way"=>$row[3],"geom"=>$row[4]));
		}
	
		return $edges;
		
	}
	
	static function analyseNodesFromMissingEdges($via){
	
		$order = $via['one_way'] == 'FT'?'DESC':'ASC';
		
		$nodes = array();
		
		//intersections
		$rs = DatabaseUtil::execute("select DISTINCT ON (st_intersection(via1.geom, via2.geom)) via1.name as via1_name,via2.name as via2_name,via1.one_way as via1_one_way,via2.one_way as via2_one_way, via1.type as via1_type, via2.type as via2_type, ''::text as next, st_intersection(via1.geom,via2.geom) as geom from new_via as via1, new_via as via2,bairro where st_intersects(via1.geom,via2.geom) and via1.gid = {$via['gid']} and via1.name <> via2.name and via1.gid <> via2.gid order by (st_intersection(via1.geom, via2.geom)) $order");
		
		$gid = self::getLastNodeGID();
		
		while($row = pg_fetch_row($rs)){
			array_push($nodes, array("gid"=>$gid,"via1_name"=>str_replace("'", "",$row[0]),"via2_name"=>str_replace("'", "", $row[1]),"via1_one_way"=>$row[2],"via2_one_way"=>$row[3],"via1_type"=>$row[4],"via2_type"=>$row[5],"next"=>$row[6],"geom"=>$row[7]));
			$gid++;
		}
		
		if(count($nodes)<2)
			return false;
		
		$createIntersection = null;
		
		for($i =0; $i != count($nodes);$i++){
			
			$createIntersection = true;
			
			$rs = DatabaseUtil::execute("SELECT NEW_NODE.GID,NEW_NODE.NEXT FROM NEW_VIA,NEW_NODE WHERE ST_INTERSECTS(NEW_VIA.GEOM,NEW_NODE.GEOM) AND ST_INTERSECTS(NEW_NODE.GEOM,'{$nodes[$i]['geom']}'::geometry('Point',900913)) AND NEW_VIA.GID = {$via['gid']} ORDER BY NEW_NODE.GEOM $order");
			
			while($row = pg_fetch_row($rs)){
				$nodes[$i]['gid'] = $row[0];
				$nodes[$i]['next'] = $row[1];
				$createIntersection = false;
			}
			
			if($createIntersection)
				DatabaseUtil::execute("INSERT INTO NEW_NODE(gid,via1_name,via2_name,via1_one_w,via2_one_w, via1_type, via2_type, next, geom) VALUES({$nodes[$i]['gid']},'{$nodes[$i]['via1_name']}','{$nodes[$i]['via2_name']}','{$nodes[$i]['via1_one_way']}','{$nodes[$i]['via2_one_way']}','{$nodes[$i]['via1_type']}','{$nodes[$i]['via2_type']}','{$nodes[$i]['next']}','{$nodes[$i]['geom']}')");
			
		}
		
		if($via['one_way'] == 'DB'){
			DatabaseUtil::execute("UPDATE NEW_NODE SET NEXT = NEXT||'{$nodes[1]['gid']}'||';' WHERE GID = {$nodes[0]['gid']}");
			DatabaseUtil::execute("UPDATE NEW_NODE SET NEXT = NEXT||'{$nodes[0]['gid']}'||';' WHERE GID = {$nodes[1]['gid']}");
			DatabaseUtil::execute("INSERT INTO NEW_EDGE(GID,NODES,ONE_WAY,GEOM) VALUES({$via['gid']},'{$nodes[0]['gid']}={$nodes[1]['gid']}','{$via['one_way']}',st_linemerge('{$via['geom']}'))");
		}
		else{
			DatabaseUtil::execute("UPDATE NEW_NODE SET NEXT = NEXT||'{$nodes[1]['gid']}'||';' WHERE GID = {$nodes[0]['gid']}");
			DatabaseUtil::execute("INSERT INTO NEW_EDGE(GID,NODES,ONE_WAY,GEOM) VALUES({$via['gid']},'{$nodes[0]['gid']}-{$nodes[1]['gid']}','{$via['one_way']}',st_linemerge('{$via['geom']}'))");
		}
		
		return true;
		
	}
	
}

/*
 * Nós em new_node
 * 
INSERT INTO NEW_NODE(via1_name,via2_name,via1_one_w,via2_one_w, via1_type, via2_type, next, geom)

select DISTINCT ON (st_intersection(via1.geom, via2.geom)) via1.name as via1_name,
via2.name as via2_name,
via1.one_way as via1_one_way,
via2.one_way as via2_one_way,
via1.type as via1_type, 
via2.type as via2_type, 
''::text as next, 
st_intersection(via1.geom,via2.geom) as geom 
from new_via as via1, new_via as via2,bairro 
where st_intersects(via1.geom,via2.geom) 
and st_intersects(bairro.geom,via2.geom) 
and st_intersects(bairro.geom,via1.geom) 
and 
(BAIRRO.NOME ILIKE '%ALDEOTA%' 
OR BAIRRO.NOME ILIKE '%VARJOTA%' 
OR BAIRRO.NOME ILIKE '%MEIRELES%' 
OR BAIRRO.NOME ILIKE '%CENTRO%'  
OR BAIRRO.NOME ILIKE '%JOAQUIM TAVORA%'  
OR BAIRRO.NOME ILIKE '%DIONISIO TORRES%'  
OR BAIRRO.NOME ILIKE '%COCO%')
and via1.gid <> via2.gid 
and via1.name <> via2.name
AND GEOMETRYTYPE(st_intersectION(via1.geom,via2.geom)) = 'POINT'
 * 
 * 
 * */

?>

